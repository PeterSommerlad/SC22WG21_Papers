#ifndef UNIQUE_RESOURCE_H_
#define UNIQUE_RESOURCE_H_
#include <type_traits>
namespace std{
namespace experimental{
// contribution by Eric Niebler, adapted by Peter Sommerlad
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
struct _resource_traits
{
    using value_type = T;
};

template<typename T>
struct _resource_traits<std::reference_wrapper<T>>
{
    using value_type = std::remove_cv_t<T>;
};

template<typename T>
using _is_nothrow_movable =
    std::integral_constant<bool,
        std::is_nothrow_move_constructible<T>::value &&
        std::is_nothrow_move_assignable<T>::value>;

}


template<typename R, typename D>
class unique_resource
{
    using value_type = typename _resource_traits<R>::value_type;

    struct resource_box : private _immovable
    {
        resource_box(R const &r, D const &d)
        try: value(r)
        {}
        catch(...)
        {
            d(r);//[&](value_type const &rr) { d(rr); }(r);
            throw;
        }
        resource_box(R const &r, D &d)
        try: value(r)
        {}
        catch(...)
        {
            d(r);//[&](value_type const &rr) { d(rr); }(r);
            throw;
        }
        resource_box(R &&r, D const &d)
        try: value(std::move_if_noexcept(r))
        {}
        catch(...)
        {
            d(r); //[&](value_type const &rr) { d(rr); }(r);
            throw;
        }
        resource_box(R &&r, D  &d)
        try: value(std::move_if_noexcept(r))
        {}
        catch(...)
        {
            d(r);//[&](value_type const &rr) { d(rr); }(r);
            throw;
        }

        R value;
    };
    struct deleter_box : private _immovable
    {
        deleter_box(value_type const &r, D const &d)
        try: value(d)
        {}
        catch(...)
        {
            d(r); // problem here if D::operator()(value_type) is non-const
            throw;
        }
        deleter_box(value_type const &r, D  &d)
        try: value(d)
        {}
        catch(...)
        {
            d(r); // problem here if D::operator()(value_type) is non-const
            throw;
        }
        deleter_box(value_type const &r, D &&d)
        try: value(std::move_if_noexcept(d))
        {}
        catch(...)
        {
            d(r);
            throw;
        }
        deleter_box(value_type  &r, D const &d)
        try: value(d)
        {}
        catch(...)
        {
            d(r); // problem here if D::operator()(value_type) is non-const
            throw;
        }
        deleter_box(value_type  &r, D  &d)
        try: value(d)
        {}
        catch(...)
        {
            d(r); // problem here if D::operator()(value_type) is non-const
            throw;
        }
        deleter_box(value_type  &r, D &&d)
        try: value(std::move_if_noexcept(d))
        {}
        catch(...)
        {
            d(r);
            throw;
        }

        D value;
    };

    void _delete(value_type const &r)
    {
        get_deleter()(r);
    }
    void _delete(value_type  &r)
    {
        get_deleter()(r);
    }

    using _is_nothrow_swappable =
        std::integral_constant<bool,
            _is_nothrow_movable<R>::value && _is_nothrow_movable<D>::value>;
public:
    unique_resource(R const &r, D const &d)
        noexcept(std::is_nothrow_copy_constructible<R>::value &&
                 std::is_nothrow_copy_constructible<D>::value)
      : resource(r, d), deleter(resource.value, d)
    {}
    unique_resource(R &&r, D const &d)
        noexcept(std::is_nothrow_move_constructible<R>::value &&
                 std::is_nothrow_copy_constructible<D>::value)
      : resource(std::move(r), d), deleter(resource.value, d)
    {}
    unique_resource(R const &r, D &&d)
        noexcept(std::is_nothrow_copy_constructible<R>::value &&
                 std::is_nothrow_move_constructible<D>::value)
      : resource(r, d), deleter(resource.value, std::move(d))
    {}
    unique_resource(R &&r, D &&d)
        noexcept(std::is_nothrow_move_constructible<R>::value &&
                 std::is_nothrow_move_constructible<D>::value)
      : resource(std::move(r), d), deleter(resource.value, std::move(d))
    {}
    unique_resource(unique_resource &&that)
        noexcept(std::is_nothrow_move_constructible<R>::value &&
                 std::is_nothrow_move_constructible<D>::value)
    try: resource(std::move(that.resource.value), that.deleter.value),
         deleter(resource.value, std::move(that.deleter.value)),
         execute_on_destruction(that.execute_on_destruction)
    {
        that.execute_on_destruction = false;
    }
    catch(...)
    {
        that.execute_on_destruction = false;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wterminate"
        throw;
#pragma GCC diagnostic pop
    }

    unique_resource &operator=(unique_resource &&that)
        noexcept(std::is_nothrow_move_assignable<R>::value &&
                 std::is_nothrow_move_assignable<D>::value)
    {
        if(&that != this)
        {
            reset();
            execute_on_destruction = that.execute_on_destruction;
            that.execute_on_destruction = false;
            {
                auto &&guard = make_scope_fail([&]{ that._delete(that.get()); });
                resource.value = std::move_if_noexcept(that.resource.value);
            }
            {
                auto &&guard = make_scope_fail([&,this]{ that._delete(get()); });
                deleter.value = std::move_if_noexcept(that.deleter.value);
            }
        }
        return *this;
    }
    unique_resource& operator=(unique_resource const &) = delete;
    unique_resource(unique_resource const &) = delete;
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
            swap(execute_on_destruction, that.execute_on_destruction);
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
    void reset() // PS: allow throwing: noexcept
    {
        if(execute_on_destruction)
        {
            execute_on_destruction = false;
            _delete(get());
        }
    }
    void reset(R const &r)
      //PS: allow throwing:  noexcept(std::is_nothrow_copy_assignable<R>::value)
    {
        reset();
        auto &&guard = make_scope_fail([&,this]{ _delete(r); });
        resource.value = r;
        execute_on_destruction = true;
    }
    void reset(R &&r)
        // PS: allow throwing: noexcept(std::is_nothrow_move_assignable<R>::value)
    {
        reset();
        auto &&guard = make_scope_fail([&,this]{ _delete(r); });
        resource.value = std::move_if_noexcept(r);
        execute_on_destruction = true;
    }
    //// I would probably change release() to return void, but this is what it should
    //// look like if you wanted to return the value "safely".
    R release()
       // PS: allow throwing:  noexcept(std::is_nothrow_move_constructible<R>::value)
    {
        execute_on_destruction = false;
        auto &&guard = make_scope_fail([this]{ _delete(get()); });
        return std::move_if_noexcept(resource.value);
    }
//    void release() noexcept
//    {
//        execute_on_destruction = false;
//    }
    R const &get() const noexcept
    {
        return resource.value;
    }
    D const &get_deleter() const noexcept
    {
        return deleter.value;
    }
    explicit operator R const &() const noexcept // Not sure if this is still desirable.
    {
        return get();
    }
    decltype(auto) operator*() const noexcept
    {
        return *get();
    }
    R operator->() const noexcept(std::is_nothrow_copy_constructible<R>::value)
    {
        return get();
    }
private:
    D &get_deleter() noexcept
    {
        return deleter.value;
    }
    resource_box resource;
    deleter_box deleter;
    bool execute_on_destruction = true;
};

// Specialization for references
template<typename R, typename D>
class unique_resource<R &, D>
{
    unique_resource<std::reference_wrapper<R>, D> resource;
    // PS: I wouldn't allow changing the deleter
    D &get_deleter() noexcept
    {
        return resource.get_deleter();
    }

public:
    unique_resource(R &r, D const &d)
        noexcept(std::is_nothrow_copy_constructible<D>::value)
      : resource(std::ref(r), d)
    {}
    unique_resource(R &r, D &&d)
        noexcept(std::is_nothrow_move_constructible<D>::value)
      : resource(std::ref(r), std::move(d))
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
    //std::remove_cv_t<R> release()
    //    noexcept(std::is_nothrow_move_constructible<std::remove_cv_t<R>>::value)
    //{
    //    auto rref = resource.release();
    //    auto &&guard = make_scope_fail([rref,this]{ resource.get_deleter()(rref.get()); });
    //    return std::move_if_noexcept(rref.get());
    //}
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
unique_resource<std::decay_t<R>, std::decay_t<D>>
make_unique_resource(R &&r, D &&d)
    noexcept(std::is_nothrow_constructible<std::decay_t<R>, R>::value &&
             std::is_nothrow_constructible<std::decay_t<D>, D>::value)
{
    return {std::forward<R>(r), std::forward<D>(d)};
}

template<typename R, typename D>
unique_resource<R &, std::decay_t<D>>
make_unique_resource(std::reference_wrapper<R> r, D &&d)
    noexcept(std::is_nothrow_constructible<std::decay_t<D>, D>::value)
{
    return {r.get(), std::forward<D>(d)};
}

template<typename R,typename D, typename S>
unique_resource<std::decay_t<R>, std::decay_t<D>>
make_unique_resource_checked(R &&r, S const &invalid, D &&d)
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
