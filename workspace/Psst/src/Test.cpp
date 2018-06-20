#include "psst.h"
#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"

#include <string>

using namespace Psst;
struct Int: strong<int,Int>,ops<Int,EqWith<int>::apply,Cmp,Inc,Add,Eq,Out>{
};

struct S:strong<std::string,S>,ops<S,Out,Eq>{};
struct E:ops<E,Eq>{
	int x{};
};

void thisIsATest() {
	Int x{42};
	++x;
	ASSERT_EQUAL(43,x);
}

void testWithStringBase(){
	S s{"hello"};
	ASSERT_EQUAL(S{"hello"},s);
}

void testEmpty(){
	E e{};
	ASSERT_EQUAL(e,e);
}


bool runAllTests(int argc, char const *argv[]) {
	cute::suite s { };
	//TODO add your test here
	s.push_back(CUTE(thisIsATest));
	s.push_back(CUTE(testWithStringBase));
	s.push_back(CUTE(testEmpty));
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
	auto runner = cute::makeRunner(lis, argc, argv);
	bool success = runner(s, "AllTests");
	return success;
}

int main(int argc, char const *argv[]) {
    return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
