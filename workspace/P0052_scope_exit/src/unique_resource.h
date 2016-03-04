#ifndef UNIQUE_RESOURCE_H_
#define UNIQUE_RESOURCE_H_
#include <type_traits>
namespace std{
namespace experimental{
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

}}

// the following is explicitly discouraged by LWG members
//#include <functional>
//template <typename R>
//auto
//make_unique_resource_type_erased(R r, std::function<void(std::remove_reference_t<R>)> d){
//	return std::experimental::make_unique_resource(std::move(r),std::move(d));
//}

#endif /* UNIQUE_RESOURCE_H_ */
