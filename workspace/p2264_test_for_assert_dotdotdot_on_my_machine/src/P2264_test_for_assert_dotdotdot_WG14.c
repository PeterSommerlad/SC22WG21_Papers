#include "system_assert_h_BSD.h"
#include "system_assert_h_BSD.h"
// NDEBUG not set

struct intpair{ int i,j;};

void checkThatMultipleArgsDontCompile(){
	//assert(1,2,3);
	assert((1,2,3));
}

void checkThatBracesConstructorWithCommasCompiles(){
	assert((int[2]){1,2}[0] == 1);
	//assert((int[2]){1,2}[0] , 1);
	assert((struct intpair){1,2}.j == 1); // false
}

#define NDEBUG
#include "system_assert_h_BSD.h"
// check double inclusion is possible without problems
#include "system_assert_h_BSD.h"
void checkThatMultipleArgsDontCompileNDEBUG(){
	assert(1,2,3); // will compile with NDEBUG set
	assert((1,2,3));
}

void checkThatBracesConstructorWithCommasCompilesNDEBUG(){
	assert((int[2]){1,2}[0] == 1);
	assert((int[2]){1,2}[0] , 1);
	assert((struct intpair){1,2}.j == 1); // false
}


void runCasserts(){
	checkThatMultipleArgsDontCompileNDEBUG();
	checkThatBracesConstructorWithCommasCompilesNDEBUG();
	checkThatMultipleArgsDontCompile();
	//checkThatBracesConstructorWithCommasCompiles();
}
