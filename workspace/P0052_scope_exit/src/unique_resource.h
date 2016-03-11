#ifndef UNIQUE_RESOURCE_H_
#define UNIQUE_RESOURCE_H_
#include <type_traits>
namespace std{
namespace experimental{
// contribution by (c) Eric Niebler, slightly adapted by Peter Sommerlad
inline namespace _detail {

struct _immovable
{
    _immovable() = default;
    _immovable(_immovable const &) = delete;
    _immovable(_immovable &&) = delete;
    _immovable &operator=(_immovable const &) = delete;
    _immovable &operator=(_immovable &&) = delete;
};
template<typename T>
struct _box : _immovable
{
private:
    template<typename TT>
    _box(TT &&t, auto &&on_error, int)
    try:
        value(std::forward<TT>(t))
    {}
    catch(...)
    {
        on_error(t);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wterminate"
        throw;
#pragma GCC diagnostic pop
    }
public:
    _box(T const &t, auto &&on_error)
      : _box(t, on_error, 0)
    {}
    _box(T &t, auto &&on_error)
      : _box(t, on_error, 0)
    {}
    _box(T &&t, auto &&on_error)
      : _box(std::move_if_noexcept(t), on_error, 0)
    {}

    T value;
};



template<typename T>
struct _resource_traits
{
    using reference = T &;
};

template<typename T>
struct _resource_traits<std::reference_wrapper<T>>
{
	using reference = T &;
};
template<typename T>
constexpr std::conditional_t<
    (!std::is_nothrow_move_assignable<T>::value &&
      std::is_copy_assignable<T>::value),
    T const &,
    T &&>
_move_assign_if_noexcept(T &x) noexcept
{
    return std::move(x);
}

template<typename T>
using _is_nothrow_movable =
    std::integral_constant<bool,
        std::is_nothrow_move_constructible<T>::value &&
        std::is_nothrow_move_assignable<T>::value>;

template<typename T>
struct _id
{
    using type = T;
};

template<typename T>
T _implicit_cast(typename _id<T>::type t)
{
    return static_cast<T &&>(t);
}

//namespace std{ // should come from there, but gcc has it not yet implemented today:
template<class T>
constexpr auto is_reference_v=std::is_reference<T>::value;
template<class T>
constexpr auto is_nothrow_move_constructible_v=std::is_nothrow_move_constructible<T>::value;
template<class T, class TT>
constexpr auto is_nothrow_constructible_v=std::is_nothrow_constructible<T, TT>::value;
//}


template<class T, class TT>
using _is_constructible =
    conditional_t<
        is_reference_v<TT> || !is_nothrow_move_constructible_v<TT>,
        std::is_constructible<T, TT const &>,
        std::is_constructible<T, TT>>;
template<class T, class TT>
constexpr auto  is_copy_or_nothrow_move_constructible_from_v=
		_is_constructible<T,TT>::value;


}


template<typename R, typename D>
class unique_resource
{
    static_assert(std::is_nothrow_move_constructible<R>::value ||
                  std::is_copy_constructible<R>::value, "");
    static_assert(std::is_nothrow_move_constructible<D>::value ||
                  std::is_copy_constructible<D>::value, "");

    using reference = typename _resource_traits<R>::reference;

    // PS: what does that help?
    template<typename, typename>
    friend class unique_resource;
    _box<R> resource;
    _box<D> deleter;
    bool is_owned = true;



    template<typename RR>
    void _reset(RR &&r)
    {
        reset();
        auto &&guard = make_scope_exit([&,this]{ get_deleter()(r); });
        resource.value = std::forward<RR>(r);
        is_owned = true;
        guard.release();
    }

    using _is_nothrow_delete =
        std::integral_constant<bool,
            noexcept(std::declval<D&>()(std::declval<R const &>()))>;
    using _is_nothrow_swappable =
        std::integral_constant<bool,
            _is_nothrow_delete::value &&
            _is_nothrow_movable<R>::value &&
            _is_nothrow_movable<D>::value>;

public:
    template<typename RR, typename DD,
   typename = std::enable_if_t<is_copy_or_nothrow_move_constructible_from_v<R, RR>>,
    typename = std::enable_if_t<is_copy_or_nothrow_move_constructible_from_v<D, DD>>>
 //   typename = std::enable_if_t<_is_constructible<R, RR>::value>,
 //   typename = std::enable_if_t<_is_constructible<D, DD>::value>>
    explicit unique_resource(RR &&r, DD &&d)
        noexcept((is_nothrow_constructible_v<R, RR> || is_nothrow_constructible_v<R, const R &>)&&
        		(is_nothrow_constructible_v<D, DD> || is_nothrow_constructible_v<D, const D &>))
      : resource(std::forward<RR>(r), [&](auto &&rr) mutable {
            d(_implicit_cast<reference>(rr)); })
      , deleter(std::forward<DD>(d), [&,this](auto &&dd) mutable {
            dd(_implicit_cast<reference>(resource.value)); })
    {}
    unique_resource(unique_resource &&that)
        noexcept(std::is_nothrow_move_constructible<R>::value &&
                 std::is_nothrow_move_constructible<D>::value)
      : resource(std::move(that.resource.value), [&](auto &&rr) mutable {
            that.deleter.value(_implicit_cast<reference>(rr)); }),
        deleter(std::move(that.deleter.value), [&,this](auto &&dd) mutable {
            dd(_implicit_cast<reference>(resource.value)); }),
        is_owned(std::exchange(that.is_owned, false))
    {}

    unique_resource &operator=(unique_resource &&that)
        noexcept(_is_nothrow_delete::value &&
                 std::is_nothrow_move_assignable<R>::value &&
                 std::is_nothrow_move_assignable<D>::value)
    {
        static_assert(std::is_nothrow_move_assignable<R>::value ||
                      std::is_copy_assignable<R>::value,
            "The resource must be nothrow-move assignable, or copy assignable");
        static_assert(std::is_nothrow_move_assignable<D>::value ||
                      std::is_copy_assignable<D>::value,
            "The deleter must be nothrow-move assignable, or copy assignable");
        if(&that == this)
            return *this;
        reset();
        if(std::is_nothrow_move_assignable<R>::value)
        {
            deleter.value = _move_assign_if_noexcept(that.deleter.value);
            resource.value = _move_assign_if_noexcept(that.resource.value);
        }
        else if(std::is_nothrow_move_assignable<D>::value)
        {
            resource.value = _move_assign_if_noexcept(that.resource.value);
            deleter.value = _move_assign_if_noexcept(that.deleter.value);
        }
        else
        {
            deleter.value = that.deleter.value;
            resource.value = that.resource.value;
        }
        std::swap(is_owned, that.is_owned);
        return *this;
    }
    ~unique_resource()
    {
        reset();
    }
    void swap(unique_resource &that)
        noexcept(_is_nothrow_swappable::value)
    {
        if(_is_nothrow_swappable::value)
        {
            using std::swap;
            swap(is_owned, that.is_owned);
            swap(resource.value, that.resource.value);
            swap(deleter.value, that.deleter.value);
        }
        else
        {
            auto tmp = std::move(*this);
            *this = std::move(that);
            that = std::move(tmp);
        }
    }
    void reset()
        noexcept(_is_nothrow_delete::value)
    {
        if(is_owned)
        {
            is_owned = false;
            get_deleter()(resource.value);
        }
    }
    void reset(R const &r)
        noexcept(_is_nothrow_delete::value && std::is_nothrow_copy_assignable<R>::value)
    {
        _reset(r);
    }
    void reset(R &r)
        noexcept(_is_nothrow_delete::value && std::is_nothrow_copy_assignable<R>::value)
    {
        _reset(r);
    }
    void reset(R &&r)
        noexcept(_is_nothrow_delete::value && std::is_nothrow_move_assignable<R>::value)
    {
        _reset(std::move_if_noexcept(r));
    }
    void release() noexcept
    {
        is_owned = false;
    }
    R const &get() const noexcept
    {
        return resource.value;
    }
    D const &get_deleter() const noexcept
    {
        return deleter.value;
    }
    D &get_deleter() noexcept
    {
        return deleter.value;
    }
    decltype(auto) operator*() const noexcept
    {
        return *get();
    }
    R operator->() const noexcept(std::is_nothrow_copy_constructible<R>::value)
    {
        return get();
    }
    unique_resource& operator=(unique_resource const &) = delete;
    unique_resource(unique_resource const &) = delete;

};

// Specialization for references
template<typename R, typename D>
class unique_resource<R &, D>
{
    unique_resource<std::reference_wrapper<R>, D> resource;
public:
    template<typename DD,
        typename = std::enable_if_t<_is_constructible<D, DD>::value>>
    unique_resource(R &r, DD &&d)
        noexcept(std::is_nothrow_constructible<D, DD>::value)
      : resource(std::ref(r), std::forward<DD>(d))
    {}
    void swap(unique_resource &that)
        noexcept(_is_nothrow_movable<D>::value)
    {
        resource.swap(that.resource);
    }
    void reset() noexcept
    {
        resource.reset();
    }
    void reset(R &r) noexcept
    {
        resource.reset(std::ref(r));
    }
    void release() noexcept
    {
        resource.release();
    }
    R &get() const noexcept
    {
        return resource.get();
    }
    D const &get_deleter() const noexcept
    {
        return resource.get_deleter();
    }
    D &get_deleter()  noexcept
    {
        return resource.get_deleter();
    }
    explicit operator R &() const noexcept // Not sure if this is still desirable.
    {
        return resource.get();
    }
    decltype(auto) operator*() const noexcept
    {
        return *get();
    }
    std::remove_cv_t<R> operator->() const
        noexcept(std::is_nothrow_copy_constructible<std::remove_cv_t<R>>::value)
    {
        return get();
    }
};

template<typename R, typename D>
void swap(unique_resource<R, D> &lhs, unique_resource<R, D> &rhs)
    noexcept(_is_nothrow_movable<R>::value && _is_nothrow_movable<D>::value)
{
    lhs.swap(rhs);
}

template<typename R, typename D>
auto make_unique_resource(R &&r, D &&d)
    noexcept(is_nothrow_constructible_v<std::decay_t<R>, R> &&
             is_nothrow_constructible_v<std::decay_t<D>, D>)
{
    return unique_resource<std::decay_t<R>, std::decay_t<D>>{
        std::forward<R>(r), std::forward<D>(d)};
}

template<typename R, typename D>
auto make_unique_resource(std::reference_wrapper<R> r, D &&d)
    noexcept(std::is_nothrow_constructible<std::decay_t<D>, D>::value)
{
    return unique_resource<R &, std::decay_t<D>>{r.get(), std::forward<D>(d)};
}

template<typename R, typename D, typename S>
auto make_unique_resource_checked(R &&r, S const &invalid, D &&d)
    noexcept(std::is_nothrow_constructible<std::decay_t<R>, R>::value &&
             std::is_nothrow_constructible<std::decay_t<D>, D>::value /*&&
             std::is_nothrow_move_constructible<std::decay_t<R>>::value*/)
{
    bool must_release = bool(r == invalid);
    auto ur = make_unique_resource(std::forward<R>(r), std::forward<D>(d));
    if(must_release)
        ur.release();
    return ur;
}


// end of (c) Eric Niebler part


#if 0
namespace __detail {
template <typename D, typename R,typename=void>
struct provide_operator_arrow_for_pointer_to_class_types{}; // R is non-pointer or pointer-to-non-class-type

template <typename DERIVED, typename R>
struct provide_operator_arrow_for_pointer_to_class_types<DERIVED, R,
	typename std::enable_if<std::is_pointer<R>::value
		&& (
				std::is_class<std::remove_pointer_t<R>>::value ||
				std::is_union<std::remove_pointer_t<R>>::value )
		>::type >

{
	R operator->() const {
		return static_cast<const DERIVED*>(this)->get();
	}
};
template <typename D, typename R,typename=void>
struct provide_operator_star_for_pointer_types{}; // R is non-pointer or pointer-to-non-class-type

template <typename DERIVED, typename R>
struct provide_operator_star_for_pointer_types<DERIVED, R,
	 std::enable_if_t<std::is_pointer<R>::value,void*> >

{
	std::add_lvalue_reference_t<std::remove_pointer_t<R>>
	operator*() const noexcept {
		return * (static_cast<const DERIVED*>(this)->get());
	}
};

}


template<class R,class D>
class unique_resource
		:public
		 __detail::provide_operator_arrow_for_pointer_to_class_types<
		 	 unique_resource<R,D>,R>
		, __detail::provide_operator_star_for_pointer_types<unique_resource<R,D>,R>{
	static_assert(std::is_nothrow_copy_constructible<R>{},"RESOURCE type must not throw on copy");
	static_assert(std::is_nothrow_copy_constructible<D>{},"DELETER type must not throw on copy");
public:
	// construction
	explicit
	unique_resource(R r, D d) noexcept
		:  resource{r}
		,  deleter{d}
	{
	}

	unique_resource(unique_resource &&other) noexcept
	:resource(std::forward<R>(other.resource))
	,deleter(std::move(other.deleter))
	,execute_on_destruction{other.execute_on_destruction}
	{
		other.release();
	}
	unique_resource(unique_resource const &)=delete; // no copies!
	unique_resource& operator=(unique_resource  &&other)
		noexcept(noexcept(unique_resource::reset()))
	{
		reset();
		deleter=std::move(other.deleter);
		resource=std::move(other.resource);
		execute_on_destruction=other.execute_on_destruction;
		other.release();
		return *this;
	}
	unique_resource& operator=(unique_resource const &)=delete;
	~unique_resource()
	{
		reset();
	}
	void reset()
	{
		if (execute_on_destruction) {
			execute_on_destruction = false;
			get_deleter()(resource);
		}
	}
	void reset(R r)
	{
		reset();
		resource = r;
		execute_on_destruction = true;
	}
	R release() noexcept{
		execute_on_destruction = false;
		return get();
	}

	R const & get() const noexcept {
		return resource;
	}
	operator R const &() const noexcept {
		return resource;
	}

	const D &
	get_deleter() const noexcept {
		return deleter;
	}
private:
	R resource; // exposition only
	D deleter;  // exposition only
	bool execute_on_destruction = true; // exposition only
};

//factories
template<class R,class D>
auto
make_unique_resource(R resource, D deleter) noexcept {
	return unique_resource<R, D>(resource,deleter);
}
template<class R,class D>
auto
make_unique_resource(std::reference_wrapper<R> resource, D deleter) noexcept {
	return unique_resource<R&, D>(resource.get(),deleter);
}

template<class R,class D, class S=R>
auto
make_unique_resource_checked(R r, S invalid, D d ) noexcept {
	bool mustrelease =  bool(r == invalid);
	auto ur= unique_resource<R,D>(r, d);
	if(mustrelease) ur.release();
	return ur;
}
#endif
}}

// the following is explicitly discouraged by LWG members
//#include <functional>
//template <typename R>
//auto
//make_unique_resource_type_erased(R r, std::function<void(std::remove_reference_t<R>)> d){
//	return std::experimental::make_unique_resource(std::move(r),std::move(d));
//}

#endif /* UNIQUE_RESOURCE_H_ */
