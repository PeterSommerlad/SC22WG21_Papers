 /* Copyright (c) 2016-2018 Peter Sommerlad, IFS
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *********************************************************************************/

#include "sstream"
#include "FixedAllocator.h"
#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"
#include <chrono>
#include <type_traits>
#include <string>

#include <utility>

using imystream=std::basic_istringstream<char,std::char_traits<char>,fixed_allocator<char>>;
using omystream=std::basic_ostringstream<char,std::char_traits<char>,fixed_allocator<char>>;
using mystring=std::basic_string<char,std::char_traits<char>,fixed_allocator<char>>;

static_assert(std::is_same_v<typename imystream::__string_type,mystring>,"template forward wrong");

void testInputWithFixedAllocator(){
	fixed_allocator<char> a;
	char const * const start=a.loc();
	mystring ms{"hibutlongenoughtobeatsmallstringoptimization"};
	ASSERT_EQUAL((unsigned long long)ms.data(),(unsigned long long)start);

	imystream in{"Hellobutlongenoughtobeatsmallstringoptimization world",a};
	std::string s{};
	in >> s;
	auto rest=std::move(in).str();
	ASSERT_EQUAL("Hellobutlongenoughtobeatsmallstringoptimization",s);
	ASSERT_EQUAL("Hellobutlongenoughtobeatsmallstringoptimization world",rest);
	ASSERT_EQUAL(rest.get_allocator(),in.rdbuf()->get_allocator());
	ASSERT_EQUAL(a,rest.get_allocator());
	ASSERT(rest.data() >= a.loc() && rest.data() < a.loc()+decltype(a)::SIZE);
	static_assert(std::is_same_v<decltype(rest),mystring>,"forwardng of type murks");
	decltype(a)::reset();
}

void testInputWithFixedAllocatorButStdString(){
	fixed_allocator<char> a;
	char const * const start=a.loc();
	mystring ms{"hibutlongenoughtobeatsmallstringoptimization world",a};
	ASSERT_EQUAL((unsigned long long)ms.data(),(unsigned long long)start);

	std::istringstream in{ms};
	mystring s{};
	in >> s;
	auto rest=in.str(a);
	ASSERT_EQUAL("hibutlongenoughtobeatsmallstringoptimization",s.c_str());
	ASSERT_EQUAL("hibutlongenoughtobeatsmallstringoptimization world",rest.c_str());
	ASSERT_EQUAL(a,rest.get_allocator());
	ASSERT(rest.data() >= a.loc() && rest.data() < a.loc()+decltype(a)::SIZE);
	static_assert(std::is_same_v<decltype(rest),mystring>,"forwardng of type murks");
	decltype(a)::reset();
}
void testOutputWithFixedAllocator(){
	fixed_allocator<char> a;
	char const * const start=a.loc();
	omystream out{a};
	out << 42 << " hibutlongenoughtobeatsmallstringoptimization";
	mystring read{std::move(out).str()};
	ASSERT_EQUAL("42 hibutlongenoughtobeatsmallstringoptimization",read.c_str());
	ASSERT_EQUAL(start,read.c_str());
	ASSERT_EQUAL(0,out.str().size());
	decltype(a)::reset();
}



void thisIsATest() {
	std::string s{"input"};
	std::istringstream in{std::move(s)};
	in >> s;
	ASSERT_EQUAL("input",s);
}

void testFromLiteral(){
	std::string s{};
	std::istringstream in{"input "};
	in >> s;
	ASSERT_EQUAL("input",s);
}

void testIfReallyMovedFromLargeString(){
	std::string s{"A long string that should not be optimized"};
	void * ptrs=s.data();
	std::istringstream in{std::move(s)};
	ASSERT(ptrs != s.data());
	auto s2=std::move(in).str();
	ASSERT_EQUAL(ptrs,(void*)s2.data()); // enforce pointer comparison
}

void testIfstringbufCtorOverloadsWork(){
	std::stringbuf buf("hello world",std::ios_base::in);

}



template <typename F>
std::chrono::microseconds time_n_calls(size_t n, F&& fun){
	std::chrono::high_resolution_clock clock{};
	auto start=clock.now();
	while(n-->0){
		fun(n);
	}
	return std::chrono::duration_cast<std::chrono::microseconds>(clock.now()-start);
}

void time_many_in_conversions(){
	const std::chrono::microseconds tcopy = time_n_calls(1000, [] (int n){
			std::string s{"here is a very long input."+std::to_string(n)};
			std::istringstream in{s};
			in >> s;
	});
	const std::chrono::microseconds tmove = time_n_calls(1000, [] (int n) {
			std::string s{"here is a very long input."+std::to_string(n)};
			std::istringstream in{std::move(s)};
			in >> s;
	});
	ASSERT_GREATER(tcopy.count()*0.9,tmove.count()*1.0); // at least 10% gain assumed
	//ASSERT_EQUAL(tcopy.count(),tmove.count()); // uncomment to see the real values
}
void time_many_out_conversions(){
	const std::chrono::microseconds tcopy = time_n_calls(1000, [] (int n) {
			std::ostringstream out{};
			out << "hello world a bit longer to prevent sso." << n;
			std::string s{out.str()};
			s.front() = s.back();;
	});
	const std::chrono::microseconds tmove = time_n_calls(1000, [] (int n){
			std::ostringstream out{};
			out << "hello world a bit longer to prevent sso." << n;
			std::string s{std::move(out).str()};
			s.front()=s.back();
	});
	ASSERT_GREATER(tcopy.count()*0.9,tmove.count()*1.0); // at least 10% gain assumed
	//ASSERT_EQUAL(tcopy.count(),tmove.count()); // uncomment to see the real values

}


void test_to_stringview_from_stringbuf(){
	std::ostringstream out{};
	out << "hello, world!\n";
	auto sv=out.rdbuf()->view();
	ASSERT_EQUAL("hello, world!\n",sv);
}

void test_to_stringview_from_ostream(){
	std::ostringstream out{};
	out << "hello, world!\n";
	auto sv=out.view();
	ASSERT_EQUAL("hello, world!\n",sv);
}
void test_to_stringview_from_partially_read_written_stringstream(){
	std::stringstream inout{"hello, "};
	ASSERT_EQUAL("hello, ",inout.view());
	std::string s{};
	inout >> s;
	ASSERT_EQUAL("hello, ",inout.view());
	inout.seekp(0,std::ios::end);
	inout << "world and more\n";
	ASSERT_EQUAL("hello, world and more\n",inout.view());
}
void test_to_stringview_from_istream(){
	std::istringstream in{"42 43"};
	ASSERT_EQUAL("42 43",in.view());
}
void test_str_from_rvalue_moved_out(){
	char const * const msg="xxxxx, world\n";
	std::stringstream out{msg};
	out << "hello";
	std::string s{std::move(out).str()};
	ASSERT_EQUAL("hello, world\n",s);
	ASSERT_EQUAL(0,out.view().size());
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
	ASSERT_EQUAL(0,out.view().size());
}





void runAllTests(int argc, char const *argv[]){
	cute::suite s;
	//TODO add your test here
	s.push_back(CUTE(thisIsATest));
	s.push_back(CUTE(test_to_stringview_from_stringbuf));
	s.push_back(CUTE(test_to_stringview_from_ostream));
	s.push_back(CUTE(test_to_stringview_from_partially_read_written_stringstream));
	s.push_back(CUTE(test_to_stringview_from_istream));
	s.push_back(CUTE(test_str_from_rvalue_moved_out));
	s.push_back(CUTE(test_str_from_rvalue_moved_out_output_only));
	s.push_back(CUTE(test_str_from_rvalue_moved_out_frombuf));
	s.push_back(CUTE(test_str_from_rvalue_moved_out_with_seek));
	s.push_back(CUTE(time_many_in_conversions));
	s.push_back(CUTE(time_many_out_conversions));
	s.push_back(CUTE(testFromLiteral));
	s.push_back(CUTE(testIfReallyMovedFromLargeString));
	s.push_back(CUTE(testIfstringbufCtorOverloadsWork));
	s.push_back(CUTE(testInputWithFixedAllocator));
	s.push_back(CUTE(testInputWithFixedAllocatorButStdString));
	s.push_back(CUTE(testOutputWithFixedAllocator));
	cute::xml_file_opener xmlfile(argc,argv);
	cute::xml_listener<cute::ide_listener<> >  lis(xmlfile.out);
	cute::makeRunner(lis,argc,argv)(s, "AllTests");
}

int main(int argc, char const *argv[]){
    runAllTests(argc,argv);
    return 0;
}



