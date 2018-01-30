# include "test_driver.h"

void test_driver(testfunc flist[], int argc, char** argv){
	while(*flist){
		test_driver(*flist,argc,argv);
		++flist;
	}
}

void test_driver(testfunc f, int argc, char** argv){

}
