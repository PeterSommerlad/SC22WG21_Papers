#ifndef SRC_REVERSE_H_
#define SRC_REVERSE_H_

#include <type_traits>
#include <utility>
#include <iterator>
template<typename Cont>
struct reversed{
	constexpr reversed(Cont &&c,std::enable_if_t<!std::is_reference_v<Cont>>* = nullptr):container{std::move(c)}{}
	constexpr reversed(Cont c,std::enable_if_t<std::is_reference_v<Cont>>* = nullptr):container{c}{}
	Cont container;
	constexpr auto begin() { return std::rbegin(container);}
	constexpr auto begin() const { return std::rbegin(container);}
	constexpr auto end() { return std::rend(container);}
	constexpr auto end() const { return std::rend(container);}
};
template<typename Cont>
reversed(Cont &) -> reversed<Cont &>;
template<typename Cont>
reversed(Cont  &&) -> reversed<Cont>;
template<typename T>
reversed(std::initializer_list<T>)-> reversed<std::initializer_list<T>const&>;


#endif /* SRC_REVERSE_H_ */
