typedef int (*testfunc)(int, unsigned long);
void test_driver(testfunc flist[], int argc=0, char** argv=0);
void test_driver(testfunc f, int argc=0, char** argv=0);
