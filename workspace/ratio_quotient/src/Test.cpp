#include "ratio.h"
#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"
#include <type_traits>
#include <typeinfo>


namespace std//_peter
{
namespace literals{

namespace detail{

template<char ...c>
constexpr size_t len{0};
template<char d, char ...c>
constexpr size_t len<d,c...>{1+len<c...>};
template< char ...c>
constexpr size_t len<'\'',c...>{len<c...>};
template<>constexpr size_t len<>{0};

template<intmax_t n,size_t exp>
constexpr intmax_t pow{ n*pow<n,exp-1>};
template<intmax_t n>
constexpr intmax_t pow<n,0>{ 1 };

static_assert(100==pow<10,2>,"");

template<int rep,char ...c>
constexpr int do_parse_int{}; //when matched fail.
template<int n>
constexpr intmax_t do_parse_int<n>{0};
template<int n,char d,char ...c>
constexpr intmax_t  do_parse_int<n,d,c...>{
	(d-'0')*pow<n,len<c...>>+do_parse_int<n,c...>
};
template<int n,char...c>
constexpr intmax_t  do_parse_int<n,'\'',c...>{ // ignore digit separators
	do_parse_int<n,c...>
};
// better match for hex digits, concepts could simplify
template<char...c>
constexpr intmax_t  do_parse_int<16,'a',c...>{
	10*pow<16,len<c...>>+do_parse_int<16,c...>
};
template<char...c>
constexpr intmax_t  do_parse_int<16,'b',c...>{
	11*pow<16,len<c...>>+do_parse_int<16,c...>
};
template<char...c>
constexpr intmax_t  do_parse_int<16,'c',c...>{
	12*pow<16,len<c...>>+do_parse_int<16,c...>
};
template<char...c>
constexpr intmax_t  do_parse_int<16,'d',c...>{
	13*pow<16,len<c...>>+do_parse_int<16,c...>
};
template<char...c>
constexpr intmax_t  do_parse_int<16,'e',c...>{
	14*pow<16,len<c...>>+do_parse_int<16,c...>
};
template<char...c>
constexpr intmax_t  do_parse_int<16,'f',c...>{
	15*pow<16,len<c...>>+do_parse_int<16,c...>
};
template<char...c>
constexpr intmax_t  do_parse_int<16,'A',c...>{
	10*pow<16,len<c...>>+do_parse_int<16,c...>
};
template<char...c>
constexpr intmax_t  do_parse_int<16,'B',c...>{
	11*pow<16,len<c...>>+do_parse_int<16,c...>
};
template<char...c>
constexpr intmax_t  do_parse_int<16,'C',c...>{
	12*pow<16,len<c...>>+do_parse_int<16,c...>
};
template<char...c>
constexpr intmax_t  do_parse_int<16,'D',c...>{
	13*pow<16,len<c...>>+do_parse_int<16,c...>
};
template<char...c>
constexpr intmax_t  do_parse_int<16,'E',c...>{
	14*pow<16,len<c...>>+do_parse_int<16,c...>
};
template<char...c>
constexpr intmax_t  do_parse_int<16,'F',c...>{
	15*pow<16,len<c...>>+do_parse_int<16,c...>
};

template<char ...c>
constexpr intmax_t  do_parse{
	do_parse_int<10,c...>
};

template<char ...c>
constexpr intmax_t do_parse<'0','x',c...>{
	 do_parse_int<16,c...>
};
template<char ...c>
constexpr intmax_t  do_parse<'0',c...>{
	 do_parse_int<8,c...>
};



}
template<char ...c>
constexpr auto operator"" _to_c(){
	return std::integral_constant<intmax_t,detail::do_parse<c...>>{};
	// parse value and return integral_constant<intmax_t,value>{}
}
template<typename INT,INT val>
constexpr auto
operator-(std::integral_constant<INT,val> v){return std::integral_constant<INT,-val>{};}

constexpr auto x=0x1'8A_to_c;
static_assert(0x18A==x,"");
constexpr auto y=0'17_to_c;
static_assert(017==y,"");

}

} // namespace


void simpleminimum() {
	using namespace std_peter::literals;
	using std_peter::ratio;
	constexpr ratio r{2_to_c,4_to_c};
	constexpr auto fourth=r*r;
	static_assert(std::is_same_v<ratio<1,2>,decltype(r)>,"argument deduction wrong");
	static_assert(ratio<1>{}==ratio<1,2>{}+r,"add failed");
	static_assert(ratio<1,4>{}==fourth,"mul failed");
	ASSERT_EQUAL((ratio<1,4>{}),fourth);
}
void test_ratio_scale(){
	ASSERT_EQUAL(3e12,std_peter::tera::scale(3.));
	ASSERT_EQUAL(10,std_peter::deci::scale(100));
}

void test_integral_constant_negate(){
	using namespace std_peter::literals;
	constexpr auto minus10 = operator-(10_to_c);
	ASSERT_EQUAL(typeid(std::integral_constant<intmax_t,-10>).name(),typeid(decltype(minus10)).name());
	static_assert(std::is_same_v<std::integral_constant<intmax_t,-10>,decltype(-10_to_c)>,"operator - works");
	static_assert(std::is_same_v<std::integral_constant<intmax_t,-10>,std::remove_const_t<decltype(minus10)>>,"operator - works");
}




template <class T>
struct A{};

//template <intmax_t N, intmax_t D>
//struct A<std::__ratio_impl<N,D>>{};

// ill formed! gcc no diagnostics
//template <intmax_t N, intmax_t D>
//struct A<std::ratio_t<N,D>>{
//	static constexpr int val=2;
//};

//works
template <intmax_t N, intmax_t D>
struct A<std_peter::ratio<N,D>>{
	static constexpr int valu=N;
};


static_assert(A<std_peter::ratio<2,4>>::valu,""); //==1 fails...
//template<>
//struct A<std::ratio<1,2>>{};
static_assert(A<std_peter::ratio_t<1,2>>::valu,"");

A<std_peter::ratio_t<2,4>> a{};
A<std_peter::ratio_t<1,3>> b{};

static_assert(std::is_same_v<std_peter::ratio_t<1,2>,std_peter::ratio_t<2,4>>,"");

bool runAllTests(int argc, char const *argv[]) {
	cute::suite s { };
	//TODO add your test here
	s.push_back(CUTE(simpleminimum));
	s.push_back(CUTE(test_ratio_scale));
	s.push_back(CUTE(test_integral_constant_negate));
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
	auto runner = cute::makeRunner(lis, argc, argv);
	bool success = runner(s, "AllTests");
	return success;
}

int main(int argc, char const *argv[]) {
    return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
