#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"

#include <string>

#include <sstream>

#include <vector>

#include <set>
void NicoVectorCTADEx(){
	std::set s{1,2,3,4,5,6};
	std::vector v(begin(s),end(s));
	std::vector v{begin(s),end(s)};
	ASSERT_EQUAL(1,v.front());
}



void InsaneBool() {
	using namespace std::string_literals;
	auto const i { 41 };
	bool const throdd = i % 3;
	auto const theanswer= (throdd & (i+1) ) ? "yes"s : "no"s;
	ASSERT_EQUAL("",theanswer);
}

void InterestingSetDouble(){
	std::vector v{0.0,0.01,0.2,3.0};
	std::set<double> s{};
	for (auto x:v){
		for (auto y:v)
			s.insert(x/y);
	}
	ASSERT_EQUAL(v.size()*v.size(),s.size());
//	ASSERT_EQUAL((std::set<double>{}),s);
}

void printBackwards(std::ostream &out,
		std::vector<int> const &v){
	for( auto i=v.size()-1; i >= 0; --i)
		out << v[i] << " ";
}


void InterstingLoop(){
	std::vector<int> const v { };
	std::ostringstream out{};
	printBackwards(out,v);
	ASSERT_EQUAL("",out.str());
}



void runAllTests(int argc, char const *argv[]) {
	cute::suite s;
	s.push_back(CUTE(InsaneBool));
	s.push_back(CUTE(InterestingSetDouble));
//	s.push_back(CUTE(InterstingLoop));
	s.push_back(CUTE(NicoVectorCTADEx));
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<> > lis(xmlfile.out);
	cute::makeRunner(lis, argc, argv)(s, "AllTests");
}

int main(int argc, char const *argv[]) {
    runAllTests(argc, argv);
    return 0;
}
