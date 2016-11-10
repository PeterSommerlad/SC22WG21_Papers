#include "string"
#include <experimental/string_view>
#include "sstream"
#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"
#include <chrono>

void thisIsATest() {
	std::string s{"input"};
	auto x = [](auto i){return ++i;}(41);
	std::istringstream in{std::move(s)};
	in >> s;
	ASSERT_EQUAL("input",s);
}

void time_many_in_conversions(){
	using namespace std::chrono_literals;
	auto now =std::chrono::system_clock::now();

	for(int i=0;i < 10000; ++i){
		std::string s{"here is a very long input."};
		std::istringstream in{std::move(s)}; // move about 20% faster than copy for this loop
		in >> s;
	}
	std::chrono::nanoseconds dur=std::chrono::system_clock::now()-now;
	//ASSERT_EQUAL(1, dur.count()/1000); // forces failure and output
}

void test_to_stringview_from_stringbuf(){
	std::ostringstream out{};
	out << "hello, world!\n";
	auto sv=out.rdbuf()->str_view();
	ASSERT_EQUAL("hello, world!\n",sv);
}

void test_to_stringview_from_ostream(){
	std::ostringstream out{};
	out << "hello, world!\n";
	auto sv=out.str_view();
	ASSERT_EQUAL("hello, world!\n",sv);
}
void test_to_stringview_from_partially_read_written_stringstream(){
	std::stringstream inout{"hello, "};
	ASSERT_EQUAL("hello, ",inout.str_view());
	std::string s{};
	inout >> s;
	ASSERT_EQUAL("hello, ",inout.str_view());
	inout.seekp(0,std::ios::end);
	inout << "world and more\n";
	ASSERT_EQUAL("hello, world and more\n",inout.str_view());
}
void test_to_stringview_from_istream(){
	std::istringstream in{"42 43"};
	ASSERT_EQUAL("42 43",in.str_view());
}
void test_str_from_rvalue_moved_out(){
	char const * const msg="xxxxx, world\n";
	std::stringstream out{msg};
	out << "hello";
	std::string s{std::move(out).str()};
	ASSERT_EQUAL("hello, world\n",s);
	ASSERT_EQUAL(0,out.str_view().size());
}
void test_str_from_rvalue_moved_out_output_only(){
	char const * const msg="hello, world\n";
	std::stringstream out{};
	out << msg;
	std::string s{std::move(out).str()};
	ASSERT_EQUAL(msg,s);
	ASSERT_EQUAL(0,out.str().size());
}
void test_str_from_rvalue_moved_out_frombuf(){
	char const * const msg="hello, world\n";
	std::ostringstream out{};
	out << msg;
	std::string s{std::move(*out.rdbuf()).str()};
	ASSERT_EQUAL(msg,s);
	ASSERT_EQUAL(0,out.str().size());
}

void test_str_from_rvalue_moved_out_with_seek(){
	char const * const msg="xxxxx, world\n";
	std::stringstream out{};
	out << msg;
	out.seekp(0,std::ios::beg);
	out << "hello";
	std::string s{std::move(out).str()};
	ASSERT_EQUAL("hello, world\n",s);
	ASSERT_EQUAL(0,out.str_view().size());
}





void runAllTests(int argc, char const *argv[]){
	cute::suite s;
	//TODO add your test here
	s.push_back(CUTE(thisIsATest));
	s.push_back(CUTE(time_many_in_conversions));
	s.push_back(CUTE(test_to_stringview_from_stringbuf));
	s.push_back(CUTE(test_to_stringview_from_ostream));
	s.push_back(CUTE(test_to_stringview_from_partially_read_written_stringstream));
	s.push_back(CUTE(test_to_stringview_from_istream));
	s.push_back(CUTE(test_str_from_rvalue_moved_out));
	s.push_back(CUTE(test_str_from_rvalue_moved_out_output_only));
	s.push_back(CUTE(test_str_from_rvalue_moved_out_frombuf));
	s.push_back(CUTE(test_str_from_rvalue_moved_out_with_seek));
	cute::xml_file_opener xmlfile(argc,argv);
	cute::xml_listener<cute::ide_listener<> >  lis(xmlfile.out);
	cute::makeRunner(lis,argc,argv)(s, "AllTests");
}

int main(int argc, char const *argv[]){
    runAllTests(argc,argv);
    return 0;
}



