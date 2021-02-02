
#include "spanstream.h"

#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"

using namespace std::experimental;
using std::span;

void thisIsATest() {
	char const input[] = "10 20 30";
	ispanstream is{span{input}};
	int i;
	is >> i;
	ASSERT_EQUAL(10,i);
	is >> i ;
	ASSERT_EQUAL(20,i);
	is >> i;
	ASSERT_EQUAL(30,i);
	ASSERT(is.good());
	//ASSERT(is.eof());
	is >>i;
	ASSERT(is.fail());
	ASSERT(!is.bad());
}

void testOspanstreamOutputToCharArray(){
	char  output[30]{};
	ospanstream os{span<char >{output}};
	os << 10 << 20 << 30 << "hello world and a long string with more than 30 chars";
	ASSERT_EQUAL(sizeof(output),os.span().size());
	std::string s{os.span().data(),size_t(os.span().size())};
	ASSERT_EQUAL("102030hello world and a long s",s);
	//ASSERT(os.eof());
}

void testOspanstreamWithShortOutput(){
char  output[30]{};
ospanstream os{span<char >{output}};
os << 10 << 20 << 30 ;
auto const sp = os.span();
ASSERT_EQUAL(6,sp.size());
ASSERT_EQUAL("102030",std::string(sp.data(),sp.size()));
ASSERT_EQUAL(static_cast<void*>(output),sp.data()); // no copying of underlying data!
ASSERT_EQUAL("102030",output); // initialization guarantees NUL termination
}

void testSpanstreambufCopy(){
	char  output[30]{};
	span<char > theSpan{output};
	ospanstream os{theSpan};
	auto buf2=*os.rdbuf(); // not sure if we should allow this...
	std::ostream os2{&buf2};
	os << "bla" << std::flush;
	os2 << 10 << 20 << 30 << std::flush;
	std::string s{theSpan.data()};
	ASSERT_EQUAL("102030",s);
}


void testIspanstreamFromStringview(){
	using namespace std::literals;
	ispanstream is{"start writing tests"sv};
	ASSERT(! ! is);
	std::string s{};
	is >> s;
	ASSERT_EQUAL("start"s,s);
	is >> s;
	ASSERT_EQUAL("writing",s);
	is >> s;
	ASSERT_EQUAL("tests"sv,s);
	is >> s;
	ASSERT(! is);

}
void testIspanStreamFromVector(){
	std::vector<char> vc{};
	ispanstream s1(vc);
	ispanstream s2(std::as_const(vc));
	ispanstream s3(vc, s1.out);
//	ispanstream s4(std::move(vc)); // fails to compile
}
void testIspanStreamFromConstString(){
	using namespace std::literals;
	std::string const si{"this is a test"};
	ispanstream is{si};
	ASSERT(! ! is);
	std::string s{};
	is >> s;
	ASSERT_EQUAL("this"sv,s);
	is >> s;
	ASSERT_EQUAL("is"sv,s);
	is >> s;
	ASSERT_EQUAL("a"sv,s);
	is >> s;
	ASSERT_EQUAL("test"sv,s);
	is >> s;
	ASSERT(! is);
}
void testIspanStreamSpanRebind(){
	using namespace std::literals;
	std::string const si{"this is a test"};
	ispanstream is{si};
	ASSERT(! ! is);
	std::string s{};
	is >> s;
	ASSERT_EQUAL("this",s);
	is >> s;
	ASSERT_EQUAL("is",s);
	is.span("not");
	is >> s;
	auto sp=is.span();
	ASSERT_EQUAL("not\0"s,s);
	ASSERT( is.eof());
}



void testISpanStreamFromTemporaries(){
	using namespace std::literals;
	ispanstream s1("Hello world"); // error, char array literal
	ispanstream s2("Hello world"sv); // ok?, string view of char array literal
	//ispanstream s3("Hello workd"s); // fails to compile

}









bool runAllTests(int argc, char const *argv[]) {
	cute::suite s { };
	//TODO add your test here
	s.push_back(CUTE(thisIsATest));
	s.push_back(CUTE(testOspanstreamOutputToCharArray));
	s.push_back(CUTE(testOspanstreamWithShortOutput));
	s.push_back(CUTE(testIspanstreamFromStringview));
	s.push_back(CUTE(testIspanStreamFromVector));
	s.push_back(CUTE(testIspanStreamFromConstString));
	s.push_back(CUTE(testSpanstreambufCopy));
	s.push_back(CUTE(testISpanStreamFromTemporaries));
	s.push_back(CUTE(testIspanStreamSpanRebind));
	//s.push_back(CUTE(testSpanstreambufCopy));
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
	auto runner { cute::makeRunner(lis, argc, argv) };
	bool success = runner(s, "AllTests");
	return success;
}

int main(int argc, char const *argv[]) {
    return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
