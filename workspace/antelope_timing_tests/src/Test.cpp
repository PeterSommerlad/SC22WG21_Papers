#include "string_functions.h"
#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"
#include <chrono>

#include <string>

using namespace std::chrono_literals;
static size_t volatile count{};
template <typename SFunc>
std::chrono::microseconds time_func(SFunc f, size_t times=1){
	using namespace std::chrono;
	high_resolution_clock clock { };
	auto start=clock.now();
	size_t n{};
	while(times--){
		auto res=f();
		n+=res.size();
	}
	auto end = clock.now();
	count += n; // trick optimizer to take result
	return duration_cast<microseconds>(end - start);
}
void testSameResult(){
    std::string s("\07Now is the time\07 for all good men\r\n to come to the aid of their country. \07");
    std::string test("Now is the time for all good men to come to the aid of their country. ");
    s = s + s + s;
    test = test + test + test;
    ASSERT_EQUAL(test,remove_ctrl(s));
    ASSERT_EQUAL(test,remove_ctrl_erase_alg(s));
}
template<typename SF>
std::chrono::microseconds timeStringFunc(SF func,size_t times=1){
    std::string s("\07Now is the time\07 for all good men\r\n"
    		          "to come to the aid of their country. \07");
    s = s + s + s;
    return time_func([&]{return func(s);},times);
}

void timeSomeStringFunctions(){
	size_t const times=1000;
	auto slow=timeStringFunc(remove_ctrl,times);
	auto fast=timeStringFunc(remove_ctrl_erase_alg,times);
	ASSERT_EQUAL(1.0,double(slow.count())/fast.count());
}

void timeSomeFasterStringFunctions(){
	size_t const times=100000;
	auto slow=timeStringFunc(remove_ctrl_block_append_cache_it,times);
	auto fast=timeStringFunc(remove_ctrl_erase_alg,times);
	ASSERT_EQUAL(1.0,double(slow.count())/fast.count());
}
void timeSomeFasterIndexingStringFunctions(){
	size_t const times=100000;
	auto slow=timeStringFunc(remove_ctrl_block_append_cache_it,times);
	auto fast=timeStringFunc(remove_ctrl_erase_alg,times);
	ASSERT_EQUAL(1.0,double(slow.count())/fast.count());
}


void timeSomeFasterEraseStringFunctions(){
	size_t const times=100000;
	auto slow=timeStringFunc(remove_ctrl_erase_it,times);
	auto fast=timeStringFunc(remove_ctrl_erase_alg,times);
	ASSERT_EQUAL(1.0,double(slow.count())/fast.count());
}
void timeRemCtrlBlocksStringFunctions(){
	size_t const times=100000;
	auto slow=timeStringFunc(remove_ctrl_blocks,times);
	auto fast=timeStringFunc(remove_ctrl_erase_alg,times);
	ASSERT_EQUAL(1.0,double(slow.count())/fast.count());
}




void thisIsATest() {
	auto t = time_func([]{return std::string{};},10000);
	ASSERT_LESS(t.count(),100);
}

bool runAllTests(int argc, char const *argv[]) {
	cute::suite s { };
	//TODO add your test here
	s.push_back(CUTE(thisIsATest));
	s.push_back(CUTE(testSameResult));
	s.push_back(CUTE(timeSomeStringFunctions));
	s.push_back(CUTE(timeSomeFasterStringFunctions));
	s.push_back(CUTE(timeSomeFasterEraseStringFunctions));
	s.push_back(CUTE(timeRemCtrlBlocksStringFunctions));
	s.push_back(CUTE(timeSomeFasterIndexingStringFunctions));
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
	auto runner = cute::makeRunner(lis, argc, argv);
	bool success = runner(s, "AllTests");
	return success;
}

int main(int argc, char const *argv[]) {
    return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
