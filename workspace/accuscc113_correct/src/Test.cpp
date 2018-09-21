#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"
#include <iterator>
#include <initializer_list>

template <typename T, typename ... Ts>
constexpr auto least(T i, Ts...js) {
	static_assert((std::is_same_v<T,Ts>&&...));
	std::initializer_list const il{i, js...};
	return *std::min_element(begin(il),end(il));
}

void simpleminimum() {
	ASSERT_EQUAL(1,least(1));
}
void multipleminimum(){
	ASSERT_EQUAL(1,least(2,1,3));
}
void testfromacc113(){
	ASSERT_EQUAL(1,least(10,9,8,7,6,5,4,3,2,1));
}



bool runAllTests(int argc, char const *argv[]) {
	cute::suite s { };
	//TODO add your test here
	s.push_back(CUTE(simpleminimum));
	s.push_back(CUTE(multipleminimum));
	s.push_back(CUTE(testfromacc113));
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
	auto runner = cute::makeRunner(lis, argc, argv);
	bool success = runner(s, "AllTests");
	return success;
}

int main(int argc, char const *argv[]) {
    return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
