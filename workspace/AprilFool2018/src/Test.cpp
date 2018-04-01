/* This is free to take by whomever wants it
 * WITHOUT ANY WARRANTY FOR ANYTHING!
 *
 * (c) Peter Sommerlad April 1st 2018
 */

#include "april_fool.h"
#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"
#include <utility>
#include <numeric>
#include <functional>
#include <set>

using namespace std_ex;

void thisIsATest() {
	constexpr auto x = Plus(35,7);
	ASSERT_EQUAL(42,x);
	ASSERT_EQUAL(-42,Negate(x));
}
void multiply_vector(){
	std::vector v{1,2,3,4,5,6};
	auto res=accumulate(begin(v),end(v),1,std::multiplies<>{});
	ASSERT_EQUAL(720,res);
}
void multiply_vector_new(){
	std::vector v{1,2,3,4,5,6};
	auto res=accumulate(begin(v),end(v),1,Times);
	ASSERT_EQUAL(720,res);
}

void WtfTest(){
	auto x=Wtf(8>5,42,0);
	ASSERT_EQUAL(42,x);
}

void AssignTest(){
	int a,b,c;
	ASSERT_EQUAL(42,Assign(a,b,c,42));
	static_assert(std::is_same_v<int&,decltype(Assign(a,b,c,42))>);
	static_assert(std::is_same_v<int&,decltype(c=42)>);
	ASSERT_EQUAL(a*b,42*42);
}
void AddAssignTest(){
	int i{21};
	int j{};
	ASSERT_EQUAL(42,PlusAssign(i,j,21));
	ASSERT_EQUAL(21,j);
	ASSERT_EQUAL(42,i);
	static_assert(std::is_same_v<decltype(i+=j+=21),decltype(PlusAssign(i,j,42))>);

}
void ostreamouttest(){
	std::ostringstream out{};
	Lshift(out,"hello ",1," world");
	ASSERT_EQUAL("hello 1 world",out.str());
}



void setLessTest(){
	std::set<int,lessTea> s({3,1,4,1,5,9,2,6},Smaller); // no C++20 compiler available yet
	ASSERT(Smaller(1,2));
	// ASSERT(lessTea{}(1,2)); not compiling yet.
	ASSERT_EQUAL(7,s.size());
	ASSERT_EQUAL(1,*begin(s));
	ASSERT_EQUAL(9,*rbegin(s));
}



bool runAllTests(int argc, char const *argv[]) {
	cute::suite s { };
	//TODO add your test here
	s.push_back(CUTE(thisIsATest));
	s.push_back(CUTE(multiply_vector));
	s.push_back(CUTE(multiply_vector_new));
	s.push_back(CUTE(WtfTest));
	s.push_back(CUTE(AssignTest));
	s.push_back(CUTE(AddAssignTest));
	s.push_back(CUTE(ostreamouttest));
	s.push_back(CUTE(setLessTest));
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
	auto runner = cute::makeRunner(lis, argc, argv);
	bool success = runner(s, "AllTests");
	return success;
}

int main(int argc, char const *argv[]) {
    return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
