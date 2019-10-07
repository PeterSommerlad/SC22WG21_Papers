typedef struct Int {
	int val;
} Int;

struct Int foo();

void demo(){
	Int i = {42};

	Int j = {};
	j = i;
//	foo() = j;
}
