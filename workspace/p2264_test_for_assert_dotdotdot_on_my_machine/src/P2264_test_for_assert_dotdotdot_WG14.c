#include "system_assert_h_BSD.h"
#include "system_assert_h_BSD.h"
// NDEBUG not set

struct intpair{ int i,j;};

void checkThatNoArgumentDoesntCompile(){

	//assert();
	// error: scalar initializer cannot be empty
}


void checkThatMultipleArgsDontCompile(){
	//assert(1,2,3); // should fail to compile: error: excess elements in scalar initializer [-Werror,-Wexcess-initializers]
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
	assert((1,2,3));
#pragma GCC diagnostic pop
}

void checkThatBracesConstructorWithCommasCompiles(){
	assert((int[2]){1,2}[0] == 1);
	//assert((int[2]){1,2}[0] , 1); // should fail to compile
	assert((struct intpair){1,2}.j == 1); // false
}
void checkThatPointerAssertWorks(){
	int a=0;
	int const *pi = &a;
	assert(pi);
	a = *pi + 1;
}

#define NDEBUG
#include "system_assert_h_BSD.h"
// check double inclusion is possible without problems
#include "system_assert_h_BSD.h"

void checkThatNoArgumentDoesntCompileNDEBUG(){

	assert(); // cannot prevent...
}


void checkThatMultipleArgsDontCompileNDEBUG(){
	assert(1,2,3); // will compile with NDEBUG set
	assert((1,2,3));
}

void checkThatBracesConstructorWithCommasCompilesNDEBUG(){
	assert((int[2]){1,2}[0] == 1);
	assert((int[2]){1,2}[0] , 1);
	assert((struct intpair){1,2}.j == 1); // false
}

void checkThatPointerAssertWorksNDEBUG(){
	int a=0;
	int const *pi = &a;
	assert(pi);
	a = *pi+1;
}

void runCasserts(){
	checkThatMultipleArgsDontCompileNDEBUG();
	checkThatBracesConstructorWithCommasCompilesNDEBUG();
	checkThatMultipleArgsDontCompile();
	checkThatPointerAssertWorks();
	//checkThatBracesConstructorWithCommasCompiles();
}
