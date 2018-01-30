#include "cute_demangle.h"
#include <ratio>

template<class D, class E>
struct dim {
    typedef D dimension;
    typedef E exponent;
};

template<class... D>
struct dimension;

template<>
struct dimension<> {
    static const constexpr bool empty = true;
};

template<class D0, class... D>
struct dimension<D0, D...> {
    static const constexpr bool empty = false;
    using front = D0;
    using pop_front = dimension<D...>;
};

template<class T, class U>
using combine_dims = dim<typename T::dimension, std::ratio_add<typename T::exponent, typename U::exponent> >;

template<int Test>
struct merge_dimensions_impl;

constexpr int const_strcmp(const char * lhs, const char * rhs)
{
    return (*lhs && *rhs) ?
        (*lhs == *rhs ? const_strcmp(lhs+1, rhs+1) : (*lhs < *rhs ? -1 : 1)) :
        ((!*lhs && !*rhs) ? 0 : (!*lhs ? -1 : 1));
}

template<bool HasT, bool HasU>
struct merge_dimensions_recurse_impl;

template<>
struct merge_dimensions_recurse_impl<true, true> {
    template<class T, class U, class... R>
    using apply = typename merge_dimensions_impl<const_strcmp(T::front::dimension::name, U::front::dimension::name)>::template apply<T, U, R...>;
};

template<class T, class U>
struct append;

template<class...T, class...U>
struct append<dimension<T...>, dimension<U...> > {
    using type = dimension<T..., U...>;
};

template<>
struct merge_dimensions_recurse_impl<true, false>
{
    template<class T, class U, class... R>
    using apply = typename append<dimension<R...>, T>::type;
};

template<>
struct merge_dimensions_recurse_impl<false, true>
{
    template<class T, class U, class... R>
    using apply = typename append<dimension<R...>, U>::type;
};

template<>
struct merge_dimensions_recurse_impl<false, false>
{
    template<class T, class U, class... R>
    using apply = dimension<R...>;
};

template<class T, class U, class... R>
using merge_dimensions_recurse = typename merge_dimensions_recurse_impl<!T::empty, !U::empty>::template apply<T, U, R...>;

template<>
struct merge_dimensions_impl<1> {
    template<class T, class U, class... R>
    using apply = merge_dimensions_recurse<T, typename U::pop_front, R..., typename U::front>;
};

template<>
struct merge_dimensions_impl<-1> {
    template<class T, class U, class... R>
    using apply = merge_dimensions_recurse<typename T::pop_front, U, R..., typename T::front>;
};

template<bool Cancels>
struct merge_dimensions_combine_impl;

template<>
struct merge_dimensions_combine_impl<true>
{
    template<class T, class U, class X, class... R>
    using apply = merge_dimensions_recurse<T, U, R...>;
};

template<>
struct merge_dimensions_combine_impl<false>
{
    template<class T, class U, class X, class... R>
    using apply = merge_dimensions_recurse<T, U, R..., X>;
};

template<>
struct merge_dimensions_impl<0> {
    template<class T, class U, class... R>
    using apply = typename merge_dimensions_combine_impl<
        std::ratio_add<typename T::front::exponent, typename U::front::exponent>::num == 0
    >::template apply<typename T::pop_front, typename U::pop_front,
        dim<typename T::front::dimension,
            std::ratio_add<typename T::front::exponent, typename U::front::exponent> >, R...>;
};

template<class T, class U>
using merge_dimensions = merge_dimensions_recurse<T, U>;

template<class T, class E>
struct dimension_pow_impl;

template<class... T, class... E, class R>
struct dimension_pow_impl<dimension<dim<T, E>...>, R> {
    using type = dimension<dim<T, std::ratio_multiply<E, R> >...>;
};

template<class T, class E>
using dimension_pow = typename dimension_pow_impl<T, E>::type;

template<class T, class E>
using dimension_root = dimension_pow<T, std::ratio_divide<std::ratio<1>, E> >;

template<class T, class U>
using dimension_multiply = merge_dimensions<T, U>;

template<class T, class U>
using dimension_divide = merge_dimensions<T, dimension_pow<U, std::ratio<-1> > >;

template<class T0 = void, class N0 = std::ratio<1>, class ... Rest>
struct make_dimension_list {
    using type = dimension_multiply<dimension<dim<T0, N0> >, typename make_dimension_list<Rest...>::type>;
};

template<class... T, class N0, class ... Rest>
struct make_dimension_list<dimension<T...>, N0, Rest...> {
    using type = dimension_multiply<dimension_pow<dimension<T...>, N0>, typename make_dimension_list<Rest...>::type>;
};

template<>
struct make_dimension_list<>
{
    using type = dimension<>;
};

template<class... T>
using make_dimension = typename make_dimension_list<T...>::type;

// ============================================================================

struct length_tag {
    static constexpr const char * const name = "length";
};

struct mass_tag {
    static constexpr const char * const name = "mass";
};

struct time_tag {
    static constexpr const char * const name = "time";
};
namespace dims{
using length = make_dimension<length_tag>;
using mass = make_dimension<mass_tag>;
using time = make_dimension<time_tag>;
using velocity = dimension_divide<length, time>;
using dimensionless = dimension_divide<velocity, velocity>;
using force = dimension_divide<dimension_multiply<velocity, mass>, time>;
using energy = make_dimension<mass, std::ratio<1>, length, std::ratio<2>, time, std::ratio<-2> >;
}
#include <iostream>

int main() {
	using namespace dims;
    std::cout << cute::demangle(typeid(length).name()) << std::endl;
    std::cout << cute::demangle(typeid(mass).name()) << std::endl;
    std::cout << cute::demangle(typeid(dims::time).name()) << std::endl;
    std::cout << cute::demangle(typeid(velocity).name()) << std::endl;
    std::cout << cute::demangle(typeid(dimensionless).name()) << std::endl;
    std::cout << cute::demangle(typeid(force).name()) << std::endl;
    std::cout << cute::demangle(typeid(energy).name()) << std::endl;
}
