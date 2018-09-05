#include "psst.h"
#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"
#include <string>

#include <stdexcept>

#include <algorithm>

#include <iterator>

#include <sstream>

#include <utility>

#include <cstddef>

using namespace Psst;
struct Int: strong<int,Int>,ops<Int,Cmp,Inc,Add,Eq,Out>{
};


struct Size: strong<unsigned,Size>,ops<Size,Eq,Cmp,Inc,Add,Out> {
};
static_assert(sizeof(Size)==sizeof(unsigned),"no overhead");

void testSizeworks(){
	Size sz{42};
	//ASSERT_EQUAL(42,sz);// doesn't compile
	//ASSERT_EQUAL(42u,sz);//doesn't compile
	ASSERT_EQUAL(Size{21}+Size{21},sz);
}


void testBoolConverts(){
	//bool b=Bool{};
	Bool b42{42}; // unfortunately, only a SFINAE ctor  could diallow that, not considered narrowing
	//int i{b42}; // no automatic conversion
	ASSERTM("start writing tests", b42);
}




namespace test {
struct Size: strong<size_t,Size>,ops<Size,Eq,Cmp,Additive,Out> {
};
struct Diff: strong<std::ptrdiff_t,Diff>,ops<Diff,Eq,Cmp,Additive,Out>{};

template <typename T, size_t N>
struct array{
	T a[N];
	using size_type=Size;
	using value_type=T;
	using difference_type=Diff;
	using reference=value_type&;
	using const_reference= value_type const&;
	using pointer=value_type*;
	using const_pointer=const value_type*;
	using iterator=pointer;
	using const_iterator=const_pointer;
	using reverse_iterator=std::reverse_iterator<iterator>;
	using const_reverse_iterator=std::reverse_iterator<const_iterator>;
	constexpr pointer data() { return a;}
	constexpr const_pointer data() const { return a;}
	constexpr reference at( size_type pos ) { if (pos < size()) return a[pos]; throw std::out_of_range{"array::at"};}
	constexpr const_reference at( size_type pos ) const { if (pos < size()) return a[pos]; throw std::out_of_range{"array::at"};}
	constexpr reference operator[]( size_type pos ) { return a[pos];}
	constexpr const_reference operator[]( size_type pos ) const { return a[pos]; }
	constexpr reference front() { return *a;}
	constexpr const_reference front() const{ return *a;}
	constexpr reference back() { return a[N-1];}
	constexpr const_reference back() const{ return a[N-1];}
	constexpr iterator begin() noexcept{return a;}
	constexpr const_iterator begin() const noexcept {return a;}
	constexpr const_iterator cbegin() const noexcept {return a;}
	constexpr iterator end() noexcept {return a+N;}
	constexpr const_iterator end() const noexcept {return a+N;}
	constexpr const_iterator cend() const noexcept {return a+N;}
	constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(end());}
	constexpr const_reverse_iterator  rbegin() const noexcept { return const_reverse_iterator(end());}
	constexpr const_reverse_iterator  crbegin() const noexcept { return const_reverse_iterator(end());}
	constexpr reverse_iterator rend() noexcept { return reverse_iterator(begin());}
	constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin());}
	constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin());}
	constexpr bool empty() const noexcept {return N !=0;}
	constexpr size_type size() const noexcept { return size_type{N};}
	constexpr size_type max_size() const noexcept { return size();}
	constexpr void fill( const T& value ) { std::fill(begin(),end(),value);}
	constexpr void swap( array& other ) noexcept(std::is_nothrow_swappable<T>::value){ std::swap_ranges(begin(), end(), other.begin());}
};
template <class T, class... U>
array(T, U...) -> array<T, 1 + sizeof...(U)>;


}

void InsaneBool() {
	Int x{42};
	++x;
	ASSERT_EQUAL(43,x.get());
}
void testUPlus(){
	struct uptest:strong<int,uptest>,ops<uptest,UPlus>{};
	uptest one{1};
	ASSERT_EQUAL(one.val,(+one).val);
}
void testUMinus(){
	struct umtest:strong<int,umtest>,ops<umtest,UMinus>{};
	umtest one{1};
	ASSERT_EQUAL(-(one.val),(-one).val);
}
void testUInc(){
	struct uinctest:strong<int,uinctest>,ops<uinctest,Inc>{};
	uinctest var{1};
	ASSERT_EQUAL(2,(++var).val);
	ASSERT_EQUAL(2,(var++).val);
	ASSERT_EQUAL(3,var.val);
}
void testUDec(){
	struct udtest:strong<int,udtest>,ops<udtest,Dec>{};
	udtest var{2};
	ASSERT_EQUAL(1,(--var).val);
	ASSERT_EQUAL(1,(var--).val);
	ASSERT_EQUAL(0,var.val);
}


void testWithStringBase(){
	struct S:strong<std::string,S>,ops<S,Out,Eq>{};
	S s{"hello"};
	ASSERT_EQUAL(S{"hello"},s);
}
namespace testRelativeOps{
struct WidthD : Relative<WidthD,double>{};

void testWidthDConstructionEq(){
	WidthD w{3.14};
	ASSERT_EQUAL(WidthD{3.14},w);
}

void testWidthDMultiplyWith(){
	WidthD w{42.0};
	w /= 2.0;
	w *= 2.0;
	w += w;
	w /= 2;
	ASSERT_EQUAL(WidthD{42},w);
}
void testWidthDAddable(){
	WidthD w{42.0};
	w += w;
	w++;
	ASSERT_EQUAL(WidthD{86},++w);
}
void testWidthDSubtractable(){
	WidthD w{42.0};
	w -= WidthD{-2};
	w--;
	ASSERT_EQUAL(WidthD{42},--w);
}
void testWidthDivides(){
	WidthD w{42.0};
	auto d= w / WidthD{21};
	static_assert(std::is_same_v<decltype(d),underlying_value_type<WidthD>>,"should be double");
	ASSERT_EQUAL(2,d);
}
//struct Diff : strong<std::ptrdiff_t,Diff>,ops<Diff,RelArithmetic<std::ptrdiff_t>::apply,Eq,Cmp,Out>{};
struct Diff:Relative<Diff,std::ptrdiff_t>{};
void testDiffCtorEq(){
	Diff d{42};
	ASSERT_EQUAL(Diff{42},d);
}

}
struct WaitC:strong<unsigned,WaitC>
            ,ops<WaitC,Eq,Inc,Out>{};
static_assert(sizeof(unsigned)==sizeof(WaitC));
void testWaitCounter(){
	WaitC c{};
	WaitC const one{1};
	ASSERT_EQUAL(WaitC{0},c);
	ASSERT_EQUAL(one,++c);
	ASSERT_EQUAL(one,c++);
	ASSERT_EQUAL(3,c.val);
}



bool runAllTests(int argc, char const *argv[]) {
	cute::suite s { };
	//TODO add your test here
	s.push_back(CUTE(InsaneBool));
	s.push_back(CUTE(testWithStringBase));
	s.push_back(CUTE(testSizeworks));
	s.push_back(CUTE(testRelativeOps::testWidthDConstructionEq));
	s.push_back(CUTE(testRelativeOps::testWidthDMultiplyWith));
	s.push_back(CUTE(testRelativeOps::testWidthDAddable));
	s.push_back(CUTE(testRelativeOps::testWidthDSubtractable));
	s.push_back(CUTE(testRelativeOps::testWidthDivides));
	s.push_back(CUTE(testRelativeOps::testDiffCtorEq));
	s.push_back(CUTE(testUPlus));
	s.push_back(CUTE(testUMinus));
	s.push_back(CUTE(testUInc));
	s.push_back(CUTE(testUDec));
	s.push_back(CUTE(testBoolConverts));
	s.push_back(CUTE(testWaitCounter));
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
	auto runner = cute::makeRunner(lis, argc, argv);
	bool success = runner(s, "AllTests");
	return success;
}

int main(int argc, char const *argv[]) {
    return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
