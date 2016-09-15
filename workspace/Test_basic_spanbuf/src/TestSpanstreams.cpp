#include "span.h"
#include "spanstream.h"

#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"

using namespace std::experimental;
void thisIsATest() {
	char input[] = "10 20 30";
	ispanstream is{span<char>{input}};
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




bool runAllTests(int argc, char const *argv[]) {
	cute::suite s { };
	//TODO add your test here
	s.push_back(CUTE(thisIsATest));
	s.push_back(CUTE(testOspanstreamOutputToCharArray));
	s.push_back(CUTE(testOspanstreamWithShortOutput));
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
	auto runner { cute::makeRunner(lis, argc, argv) };
	bool success = runner(s, "AllTests");
	return success;
}

int main(int argc, char const *argv[]) {
    return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
