#include <vector>
#include <memory>


#include "cassert_gcc"
#include "cassert_gcc"
// NDEBUG not set

void checkThatNoArgumentDoesntCompile(){

	//assert(); // no longer compiles for C++...
}

void checkThatMultipleArgsDontCompile(){
	//assert(1,2,3); //should not compile: error: expression list treated as compound expression in functional cast
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
	assert((1,2,3));
#pragma GCC diagnostic pop
}

void checkThatBracesConstructorWithCommasCompiles(){
	assert(std::vector<int>{1,2,3,4}.size()==4u);
	//assert(std::vector{1,2,3,4}.size(),4u); // should not compile, error: expression list treated as compound expression in functional cast
}
void checkThatContextualConversionToBoolWorks(){
	using vpi = std::vector<std::unique_ptr<int>>;
	using upvpi = std::unique_ptr<std::vector<std::unique_ptr<int>>>;
	upvpi pi = std::make_unique<vpi>();
	assert(pi);
	pi = nullptr;
	assert(!pi);
}

#define NDEBUG
#include "cassert_gcc"
#include "cassert_gcc"

void checkThatNoArgumentDoesntCompileNDEBUG(){
	assert(); // will compile with NDEBUG set
}


void checkThatMultipleArgsDontCompileNDEBUG(){
	assert(1,2,3); // will compile with NDEBUG set
	assert((1,2,3));
}

void checkThatBracesConstructorWithCommasCompilesNDEBUG(){
	assert(std::vector{1,2,3,4}.size()==3u); // false
	assert(std::vector{1,2,3,4}.size(),4u); // will compile with NDEBUG set
}

void checkThatPointerAssertWorks(){
	int a=0;
	int const *pi = &a;
	assert(pi);
	a = *pi;
}

enum class ConfVals
{
    DefaultConfig = 1,
    ExpertConfigCustomer1 = 2,
    ExpertConfigCustomer2 = 4
}; // from Ville
void checkThatScopedEnumsDontWork(){

	assert(ConfVals::DefaultConfig); // shouldn't compile
}
void runCppasserts(){
	checkThatMultipleArgsDontCompileNDEBUG();
	checkThatBracesConstructorWithCommasCompilesNDEBUG();
	checkThatMultipleArgsDontCompile();
	checkThatBracesConstructorWithCommasCompiles();
	checkThatContextualConversionToBoolWorks();
	checkThatPointerAssertWorks();

}

extern "C" void runCasserts();
#undef NDEBUG
#include <iostream>

int main() {
	runCasserts();
	std::cout << "!!!Hello World!!!\n" ;
	runCppasserts();
	checkThatNoArgumentDoesntCompile(); // compiles for C++ but fails assertion
}
