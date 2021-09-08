#include <vector>

#include "cassert_gcc"
#include "cassert_gcc"
// NDEBUG not set

void checkThatMultipleArgsDontCompile(){
	//assert(1,2,3); should not compile
	assert((1,2,3));
}

void checkThatBracesConstructorWithCommasCompiles(){
	assert(std::vector{1,2,3,4}.size()==4u);
	//assert(std::vector{1,2,3,4}.size(),4u); // should not compile, comma operator
}

#define NDEBUG
#include "cassert_gcc"
#include "cassert_gcc"
void checkThatMultipleArgsDontCompileNDEBUG(){
	assert(1,2,3); // will compile with NDEBUG set
	assert((1,2,3));
}

void checkThatBracesConstructorWithCommasCompilesNDEBUG(){
	assert(std::vector{1,2,3,4}.size()==3u); // false
	assert(std::vector{1,2,3,4}.size(),4u); // will compile with NDEBUG set
}


void runCppasserts(){
	checkThatMultipleArgsDontCompileNDEBUG();
	checkThatBracesConstructorWithCommasCompilesNDEBUG();
	checkThatMultipleArgsDontCompile();
	checkThatBracesConstructorWithCommasCompiles();

}

extern "C" void runCasserts();

#include <iostream>

int main() {
	runCasserts();
	std::cout << "!!!Hello World!!!" << std::endl;
	runCppasserts();
}
