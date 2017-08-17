#ifndef UNIQUE_RESOURCE_H_
#define UNIQUE_RESOURCE_H_
/*
 * MIT License

Copyright (c) 2016/2017 Eric Niebler, slightly adapted by Peter Sommerlad

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
#include <type_traits>
#include "_scope_guard_common.h"
namespace std{
namespace experimental{
namespace detail {
template<typename T>
using _is_nothrow_movable =
    _bool<std::is_nothrow_move_constructible<T>::value &&
          std::is_nothrow_move_assignable<T>::value>;

template<typename T>
constexpr bool _is_nothrow_movable_v = _is_nothrow_movable<T>::value;

//template<typename T>
//using _is_nothrow_movable =
//    std::integral_constant<bool,
//        std::is_nothrow_move_constructible<T>::value &&
//        std::is_nothrow_move_assignable<T>::value>;

// the following is never ODR used. it is only relevant for obtaining a noexcept value
template<typename T>
T for_noexcept_on_copy_construction(const T &t) noexcept(noexcept(T(t)))
{
    return t;
}



} // detail
//namespace std{ // should come from there, but gcc has it not yet implemented today:
}
//template<class T>
//constexpr auto is_reference_v=std::is_reference<T>::value;
//template<class T>
//constexpr auto is_nothrow_move_constructible_v=std::is_nothrow_move_constructible<T>::value;
//template<class T>
//constexpr auto is_copy_constructible_v=std::is_copy_constructible<T>::value;
//template<class T, class TT>
//constexpr auto is_nothrow_constructible_v=std::is_nothrow_constructible<T, TT>::value;
//}
namespace experimental {


template<typename R, typename D>
class unique_resource
{
    static_assert(std::is_nothrow_move_constructible_v<R> ||
                  std::is_copy_constructible_v<R>,
				  "resource must be nothrow_move_constructible or copy_constructible");
    static_assert(std::is_nothrow_move_constructible_v<D> ||
                  std::is_copy_constructible_v<D>,
				  "deleter must be nothrow_move_constructible or copy_constructible");

	static const unique_resource &this_; // never ODR used! Just for getting no_except() expr

    detail::_box<R> resource;
    detail::_box<D> deleter;
    bool execute_on_destruction = true;

    static constexpr auto is_nothrow_delete_v=detail::_bool<noexcept(std::declval<D &>()(std::declval<R &>()))>::value;

    static constexpr auto is_nothrow_swappable_v=detail::_bool<is_nothrow_delete_v &&
            detail::_is_nothrow_movable_v<R> &&
            detail::_is_nothrow_movable_v<D>>::value;

public:
    template<typename RR, typename DD,
        typename = std::enable_if_t<std::is_constructible<detail::_box<R>, RR, detail::_empty_scope_exit>::value &&
                                    std::is_constructible<detail::_box<D>, DD, detail::_empty_scope_exit>::value>>
    explicit unique_resource(RR &&r, DD &&d)
        noexcept(noexcept(detail::_box<R>((RR &&) r, detail::_empty_scope_exit{})) &&
                 noexcept(detail::_box<D>((DD &&) d, detail::_empty_scope_exit{})))
      : resource((RR &&) r, make_scope_exit([&]{ d(r); }))
      , deleter((DD &&) d, make_scope_exit([&, this]{ d(get()); }))
    {}
    unique_resource(unique_resource &&that)
        noexcept(noexcept(detail::_box<R>(that.resource.move(), detail::_empty_scope_exit{})) &&
                 noexcept(detail::_box<D>(that.deleter.move(), detail::_empty_scope_exit{})))
      : resource(that.resource.move(), detail::_empty_scope_exit{})
      , deleter(that.deleter.move(),
                make_scope_exit([&, this]{ that.get_deleter()(get()); that.release(); }))
      , execute_on_destruction(std::exchange(that.execute_on_destruction, false))
    {}

    unique_resource &operator=(unique_resource &&that)
        noexcept(is_nothrow_delete_v &&
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
        if(std::is_nothrow_move_assignable<detail::_box<R>>::value)
        {
            deleter = _move_assign_if_noexcept(that.deleter);
            resource = _move_assign_if_noexcept(that.resource);
        }
        else if(std::is_nothrow_move_assignable<detail::_box<D>>::value)
        {
            resource = _move_assign_if_noexcept(that.resource);
            deleter = _move_assign_if_noexcept(that.deleter);
        }
        else
        {
            resource = _as_const(that.resource);
            deleter = _as_const(that.deleter);
        }
        execute_on_destruction = std::exchange(that.execute_on_destruction, false);
        return *this;
    }
    ~unique_resource() //noexcept(is_nowthrow_delete_v)
    {
        reset();
    }
    void swap(unique_resource &that) noexcept(is_nothrow_swappable_v)
    {
        if(is_nothrow_swappable_v)
        {
            using std::swap;
            swap(execute_on_destruction, that.execute_on_destruction);
            swap(resource.get(), that.resource.get());
            swap(deleter.get(), that.deleter.get());
        }
        else
        {
            auto tmp = std::move(*this);
            *this = std::move(that);
            that = std::move(tmp);
        }
    }
    void reset()
        noexcept(is_nothrow_delete_v)
    {
        if(execute_on_destruction)
        {
            execute_on_destruction = false;
            get_deleter()(get());
        }
    }
    template<typename RR>
    auto reset(RR &&r)
        noexcept(is_nothrow_delete_v && noexcept(resource.reset((RR &&) r)))
        -> decltype(resource.reset((RR &&) r), void())
    {
        auto &&guard = make_scope_exit([&, this]{ get_deleter()(r); });
        reset();
        resource.reset((RR &&) r);
        execute_on_destruction = true;
        guard.release();
    }
    void release() noexcept
    {
        execute_on_destruction = false;
    }
    decltype(auto) get() const noexcept
    {
        return resource.get();
    }
    decltype(auto) get_deleter() noexcept
    {
        return deleter.get();
    }
    decltype(auto) get_deleter() const noexcept
    {
        return deleter.get();
    }
    auto operator->() const noexcept(noexcept(detail::for_noexcept_on_copy_construction(this_.get())))
    {
        return get();
    }
    decltype(auto) operator*() const noexcept
    {
        return *get();
    }

	unique_resource& operator=(const unique_resource &) = delete;
	unique_resource(const unique_resource &) = delete;

};


template<typename R, typename D>
void swap(unique_resource<R, D> &lhs, unique_resource<R, D> &rhs)
    noexcept(detail::_is_nothrow_movable_v<R> && detail::_is_nothrow_movable_v<D>)
{
    lhs.swap(rhs);
}
template<typename R, typename D>
unique_resource(R &&r, D &&d)
-> unique_resource<std::decay_t<R>, std::decay_t<D>>;

template<typename R, typename D>
auto make_unique_resource(R &&r, D &&d)
    noexcept(noexcept(unique_resource<std::decay_t<R>, std::decay_t<D>>{
        std::forward<R>(r), std::forward<D>(d)}))
{
    return unique_resource<std::decay_t<R>, std::decay_t<D>>{
        std::forward<R>(r), std::forward<D>(d)};
}
template<typename R, typename D>
unique_resource(std::reference_wrapper<R> r, D &&d)
-> unique_resource<R &, std::decay_t<D>>; // should need to unwrap, but how?

template<typename R, typename D>
auto make_unique_resource(std::reference_wrapper<R> r, D &&d)
noexcept(noexcept(unique_resource<R &, std::decay_t<D>>{r.get(), std::forward<D>(d)}))
{
    return unique_resource<R &, std::decay_t<D>>{r.get(), std::forward<D>(d)};
}

template<typename R, typename D, typename S>
auto make_unique_resource_checked(R &&r, const S &invalid, D &&d)
noexcept(noexcept(make_unique_resource(std::forward<R>(r), std::forward<D>(d))))
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
