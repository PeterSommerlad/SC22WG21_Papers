#include "reverse.h"
#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"
#include <array>

using std::array;
void testReverseArray(){
	array a{1,2,3,4,5,6};
	std::ostringstream out{};
	for(auto const &i:reversed(a)){
		out << i;
	}
	ASSERT_EQUAL("654321",out.str());
}
void testReverseConstArray(){
	array const a{1,2,3,4,5,6};
	std::ostringstream out{};
	for(auto const &i:reversed(a)){
		out << i;
	}
	ASSERT_EQUAL("654321",out.str());
}
void testReverseLRefArray(){
	array a{0,1,2,3,4,5};
	std::ostringstream out{};
	for(auto  &i:reversed(a)){
		++i;
	}
	for(auto const &i:reversed(a)){
		out << i;
	}
	ASSERT_EQUAL("654321",out.str());
}
void testReverseTempArray(){
	std::ostringstream out{};
	for(auto  &i:reversed(array{1,2,3,4,5,6})){
		out << i;
	}
	ASSERT_EQUAL("654321",out.str());
}
void testReverseInitializerList(){
	std::ostringstream out{};
	for(auto const &i:reversed({1,2,3,4,5,6})){
		out << i;
	}
	ASSERT_EQUAL("654321",out.str());
}
void testReverseCArray(){
	int a[]={1,2,3,4,5,6};
	std::ostringstream out{};
	for(auto const &i:reversed(a)){
		out << i;
	}
	ASSERT_EQUAL("654321",out.str());

}
void runAllTests(int argc, char const *argv[]) {
	cute::suite s;
	s.push_back(CUTE(testReverseCArray));
	s.push_back(CUTE(testReverseInitializerList));
	s.push_back(CUTE(testReverseTempArray));
	s.push_back(CUTE(testReverseLRefArray));
	s.push_back(CUTE(testReverseConstArray));
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<> > lis(xmlfile.out);
	cute::makeRunner(lis, argc, argv)(s, "AllTests");
}

int main(int argc, char const *argv[]) {
    runAllTests(argc, argv);
    return 0;
}
