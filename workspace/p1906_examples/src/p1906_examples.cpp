#include <complex>
#include <type_traits>
template <typename INT>
struct check{
	INT x{};
	INT side_effect(INT &lv){
		lv = INT{42};
		return lv;
	}
	void demo(){
		auto &dangle = (INT{} = INT{42});
		auto side = side_effect(dangle);
		side = side_effect(INT{} = INT{42});
	}
	static_assert(std::is_trivially_copy_assignable_v<INT>);
	static_assert(std::is_trivially_move_assignable_v<INT>);
	static_assert(std::is_trivially_copy_constructible_v<INT>);
	static_assert(std::is_trivially_move_constructible_v<INT>);
	static_assert(std::is_trivially_destructible_v<INT>);
};

struct Int {
	int val{};
};

struct SafeInt {
	int val{};
	constexpr SafeInt& operator=(SafeInt const &other) & = default;
	constexpr SafeInt& operator=(SafeInt &&other) & = default;
	// need to provide copy move ctor and default ctor as well
	constexpr SafeInt() = default;
	constexpr SafeInt(SafeInt const &other) = default;
	constexpr SafeInt(SafeInt &&other) = default;
};

int main() {
	check<Int> c{};
	c.demo(); // compiles
	check<int> fail{};
	fail.demo(); // compile error
	check<std::complex<double>> cc{};
	cc.demo(); // compiles
	check<SafeInt> failstoo{};
	failstoo.demo(); // compile error
}
