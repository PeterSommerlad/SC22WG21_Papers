#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"

struct TForwardMove{
	int i;
	int &ri;
	TForwardMove(int j, int &rj):i{j},ri{rj}{}
	TForwardMove(TForwardMove&&rhs)
	:i{std::move(rhs.i)},
	 //ri{std::move(rhs.ri)}{} // does not compile
	 ri{std::forward<decltype(ri)>(rhs.ri)}{}
};


void InsaneBool() {
	int i{42};
	TForwardMove x{i,i};
	TForwardMove y{std::move(x)};
	ASSERT_EQUAL(i,y.ri);
	ASSERT_EQUAL(&i,&y.ri);
}

bool runAllTests(int argc, char const *argv[]) {
	cute::suite s { };
	//TODO add your test here
	s.push_back(CUTE(InsaneBool));
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
	auto runner = cute::makeRunner(lis, argc, argv);
	bool success = runner(s, "AllTests");
	return success;
}

int main(int argc, char const *argv[]) {
    return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
