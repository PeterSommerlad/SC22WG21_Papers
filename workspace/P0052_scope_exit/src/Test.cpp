#include "scope_exit.h"
#include "unique_resource.h"
#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"
#include <fstream>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <memory>
#include <exception>

//#define CHECK_COMPILE_ERRORS

using std::experimental::make_unique_resource;
using std::experimental::make_unique_resource_checked;
using std::experimental::make_scope_exit;
using std::experimental::make_scope_success;
using std::experimental::make_scope_fail;
void thisIsATest() {
	std::ostringstream out { };
	{
		auto guard = make_scope_exit([&] {out << "done\n";});
	}
	ASSERT_EQUAL("done\n", out.str());
}

void testDismissedGuard() {
	std::ostringstream out { };
	{
		auto guard = make_scope_exit([&] {out << "done1\n";});
		auto guard2dismiss = make_scope_exit([&] {out << "done2\n";});
		guard2dismiss.release();
	}
	ASSERT_EQUAL("done1\n", out.str());
}
void testThrowDoesntCrashIt() { // LEWG wants it to crash!
	std::ostringstream out { };
	{
		auto guard = make_scope_exit([&] {out << "done\n";});
		auto guard1 = make_scope_exit([] {throw 42;});
		guard1.release(); // we no longer want throwing scope guards
	}
	ASSERT_EQUAL("done\n", out.str());

}
void testScopeExitWithReferenceWrapper(){
	std::ostringstream out { };
	auto const &lambda=[&]{ out << "lambda done.\n";};
	{
		auto guard=make_scope_exit(std::cref(lambda));
	}
	ASSERT_EQUAL("lambda done.\n",out.str());
}


// by Eric Niebler, adapted for unit testing
struct throwing_copy
{
    throwing_copy(char const* sz, std::ostream &os) : sz_{sz},out{os} {}
    throwing_copy(throwing_copy const &other):out{other.out}
    {
        throw 42;
    }
    void operator()() const
    {
        out << sz_ << std::endl;
    }
private:
    char const *sz_{""};
    std::ostream &out;
};


void testsFromEricNiebler_scope_exit_with_throwing_function_object(){
	std::ostringstream out { };
	try
	    {
	        throwing_copy c{"called anyway",out};
	        auto &&x = make_scope_exit(c);
	        out << "whoops" << std::endl;
	    }
	    catch(...)
	    {}
	ASSERT_EQUAL("called anyway\n", out.str());
}

void testsFromEricNiebler_scope_success_with_throwing_function_object(){
	std::ostringstream out { };
	try
	    {
        throwing_copy c{"Oh noes!!!",out};
        auto &&x = make_scope_success(c);
	        out << "whoops" << std::endl;
	    }
	    catch(...)
	    {
	    	out << "just exception\n";
	    }
	ASSERT_EQUAL("just exception\n", out.str());
}

void testsFromEricNiebler_scope_fail_with_throwing_function_object(){
	std::ostringstream out { };
	try
	    {
        throwing_copy c{"called because of exception!!!",out};
        auto &&x = make_scope_fail(c);
	        out << "whoops" << std::endl;
	    }
	    catch(...)
	    { }
	ASSERT_EQUAL("called because of exception!!!\n", out.str());
}


#include <stdexcept>
void testThrowOnUniqueResourceDoesntCrashIt() {
	std::ostringstream out { };
	{
		auto guard = make_unique_resource(1, [&] (auto) {out << "done\n";});
		try {
			{
				auto guard1 = make_unique_resource(2, [] (auto) noexcept(false) {throw 42;});
				guard1.reset();
			}
			FAILM("didn't throw");
		} catch (int) {
		} catch (...) {
			FAILM("threw unknown error");
		}
	}
	ASSERT_EQUAL("done\n", out.str());

}
void testUniqueResourceSimple() {
	using namespace std;
	std::ostringstream out { };

	std::string const msg { " deleted resource\n" };
	{
		auto res = make_unique_resource(std::ref(msg), [&out](string msg) {out << msg<< flush;});
	}
	ASSERT_EQUAL(msg, out.str());
}

void test_unique_resource_semantics_reset() {
	std::ostringstream out { };
	{
		auto cleanup = make_unique_resource(1, [&out](int i) {out << "cleaned " << i;});
		cleanup.reset(2);
	}
	ASSERT_EQUAL("cleaned 1cleaned 2", out.str());
}

void demonstrate_unique_resource_with_stdio() {
	const std::string filename = "hello.txt";
	{
		auto file = make_unique_resource(::fopen(filename.c_str(), "w"), &::fclose);
		::fputs("Hello World!\n", file.get());
		ASSERT(file.get()!= NULL);
	}
	{
		std::ifstream input { filename };
		std::string line { };
		getline(input, line);
		ASSERT_EQUAL("Hello World!", line);
		getline(input, line);
		ASSERT(input.eof());
	}
	::unlink(filename.c_str());
	{
		auto file = make_unique_resource_checked(::fopen("nonexistingfile.txt", "r"), (FILE*) NULL, &::fclose);
		ASSERT_EQUAL((FILE*)NULL, file.get());
	}

}

void demontrate_unique_resource_with_POSIX_IO() {
	const std::string filename = "./hello1.txt";
	{
		auto file = make_unique_resource(::open(filename.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0666), &::close);

		::write(file.get(), "Hello World!\n", 12u);
		ASSERT(file.get() != -1);
	}
	{
		std::ifstream input { filename };
		std::string line { };
		getline(input, line);
		ASSERT_EQUAL("Hello World!", line);
		getline(input, line);
		ASSERT(input.eof());
	}
	::unlink(filename.c_str());
	{
		auto file = make_unique_resource_checked(::open("nonexistingfile.txt", O_RDONLY), -1, &::close);
		ASSERT_EQUAL(-1, file.get());
	}

}

void test_make_unique_resource_checked(){
	std::ostringstream out{};

	{
		auto bar=make_unique_resource_checked(0.0,0,[&out](auto i){out << i << "not called";});
		auto foo=make_unique_resource_checked(0.0,-1,[&out](auto i){out << "called";});
	}
	ASSERT_EQUAL("called",out.str());

}

void testReferenceWrapper(){
	std::ostringstream out{};
	int i{42};
	{
		auto uref=make_unique_resource(std::ref(i),[&out](int &j){out << "reference to "<<j;});
	static_assert(std::is_same<int &,decltype(uref.release())>::value," should be reference");
	}
	ASSERT_EQUAL("reference to 42",out.str());
}




void TalkToTheWorld(std::ostream& out, std::string const farewell = "Uff Wiederluege...") {
	// Always say goodbye before returning,
	// but if given a non-empty farewell message use it...
	auto goodbye = make_scope_exit([&out]()
	{
		out << "Goodbye world..." << std::endl;
	});
	// must pass farewell by reference, otherwise it is not non-throw-moveconstructible
	auto altgoodbye = make_scope_exit([&out,&farewell]()
	{
		out << farewell << std::endl;
	});

	if (farewell.empty()) {
		altgoodbye.release();		// Don't use farewell!
	} else {
		goodbye.release();	// Don't use the alternate
	}
}

void testTalkToTheWorld() {
	std::ostringstream out { };
	TalkToTheWorld(out, "");
	ASSERT_EQUAL("Goodbye world...\n", out.str());
	out.str("");
	TalkToTheWorld(out);
	ASSERT_EQUAL("Uff Wiederluege...\n", out.str());
}

struct X {
	void foo() const {
	}
};
void testCompilabilityGuardForNonPointerUniqueResource() {
	auto x = make_unique_resource(X { }, [](X i) {});
#if defined(CHECK_COMPILE_ERRORS)
	x->foo();// compile error!
	*x; // compile error!
#endif

}
void testCompilabilityGuardForPointerTypes() {
	auto x = make_unique_resource(new int { 42 }, [](int * ptr) {delete ptr;});
	ASSERT_EQUAL(42, *x);
	auto y = make_unique_resource(new X { }, [](X * ptr) {delete ptr;});
	y->foo(); // compiles, SFINAE works
	(void)*y; // compiles, through SFINAE (again)
	ASSERT_EQUAL(42,*(int*)(void*)x.get());
}

// type erasure no longer works, because std::function's copy ctor can throw
// void testmakeUniqueResourcetypeerased() {
//	 auto x = ::make_unique_resource_type_erased(new int { 43 }, [](int * p) {delete p;});
//	 ASSERT_EQUAL(43, *x);
// }

struct functor_copy_throws{
	functor_copy_throws()=default;
	functor_copy_throws(functor_copy_throws const &)noexcept(false){ throw 42;}
	functor_copy_throws(functor_copy_throws &&)=default;
	void operator()(){}
};
struct functor_move_throws{ // bad idea anyway.
	functor_move_throws()=default;
	functor_move_throws(functor_move_throws &&)noexcept(false){ throw 42;}
	functor_move_throws(functor_move_throws const &)=default;
	void operator()()const{}
};

struct functor_move_copy_throws{
	functor_move_copy_throws()=default;
	functor_move_copy_throws(functor_move_copy_throws &&)noexcept(false){ throw 42;}
	functor_move_copy_throws(functor_move_copy_throws const &)noexcept(false){throw 42;}
	void operator()()const{}
};


void test_scope_exit_with_throwing_fun_copy(){
	functor_copy_throws fun;
#if defined(CHECK_COMPILE_ERRORS)
	auto x = make_scope_exit(std::move(fun)); // doesn't compile due to static_assert
#endif
	auto y = make_scope_exit(std::ref(fun)); // hold by reference
	auto ff=functor_copy_throws{};
	auto z = make_scope_exit(std::ref(ff)); // hold by const reference
	ASSERTM("should just work",true);
}

void test_scope_exit_with_throwing_fun_move(){
	functor_move_throws fun;
	functor_move_throws const &funref{fun};
//#if defined(CHECK_COMPILE_ERRORS)
	auto x = make_scope_exit(std::move(fun)); // no longer a compile error, because it is copied.
//#endif
	auto y = make_scope_exit(fun); // hold by copy
	auto z = make_scope_exit(funref); // hold by copy?, no const ref
	ASSERTM("should just work",true);
}
void test_scope_exit_with_throwing_fun_move_and_copy(){
	functor_move_copy_throws fun;
#if defined(CHECK_COMPILE_ERRORS)
	auto x = make_scope_exit(std::move(fun)); // compile error, because non-copyable
#endif
	auto y = make_scope_exit(std::ref(fun)); // hold by reference works
	ASSERTM("should not work",true);
}

void test_scope_success_with_side_effect(){
	std::ostringstream out{};
	auto lam=[&]{out << "not called";};
	try{
		auto x=make_scope_success(lam); // lam not called
		throw 42;
	}catch(...){
		auto y=make_scope_success([&]{out << "handled";});
	}
	ASSERT_EQUAL("handled",out.str());
}
void test_scope_success_might_throw(){
	std::ostringstream out{};
	auto lam=[&]{out << "called"; /* throw 42;*/}; // doesn't work.
	try{{
		auto x=make_scope_success(lam);
		}
		//out << " never ";
	} catch (int) {
		// OK
	} catch (...) {
		out << "bla";
	}
	ASSERT_EQUAL("called",out.str());
}



void demo_scope_exit_fail_success(){
	std::ostringstream out{};
	auto lam=[&]{out << "called ";};
	try{
		auto v=make_scope_exit([&]{out << "always ";});
		auto w=make_scope_success([&]{out << "not ";}); // not called
		auto x=make_scope_fail(lam); // called
		throw 42;
	}catch(...){
		auto y=make_scope_fail([&]{out << "not ";}); // not called
		auto z=make_scope_success([&]{out << "handled";}); // called
	}
	ASSERT_EQUAL("called always handled",out.str());
}

void test_scope_exit_lvalue_ref_passing_rvalue_fails_to_compile(){
	typedef void (*fun)();
	auto y=make_scope_exit<fun const &>(fun(nullptr)); // no static assert needed. fails to match.
	y.release(); // avoid crash from calling nullptr
#if defined(CHECK_COMPILE_ERRORS)
	auto x=make_scope_exit<fun  &>(fun(nullptr)); // no static assert needed. fails to match
	std::experimental::scope_exit< fun const& > se{fun(nullptr)}; // static assert needed
#endif


}




void runAllTests(int argc, char const *argv[]){
	cute::suite s;
	//TODO add your test here
	s.push_back(CUTE(thisIsATest));
//	s.push_back(CUTE(testmakeUniqueResourcetypeerased));
	s.push_back(CUTE(testCompilabilityGuardForPointerTypes));
	s.push_back(CUTE(testTalkToTheWorld));
	s.push_back(CUTE(demontrate_unique_resource_with_POSIX_IO));
	s.push_back(CUTE(demonstrate_unique_resource_with_stdio));
	s.push_back(CUTE(test_unique_resource_semantics_reset));
	s.push_back(CUTE(testUniqueResourceSimple));
	s.push_back(CUTE(testThrowOnUniqueResourceDoesntCrashIt));
	s.push_back(CUTE(testThrowDoesntCrashIt));
	s.push_back(CUTE(testDismissedGuard));
	s.push_back(CUTE(test_scope_exit_with_throwing_fun_copy));
	s.push_back(CUTE(test_scope_success_with_side_effect));
	s.push_back(CUTE(demo_scope_exit_fail_success));
	s.push_back(CUTE(test_scope_exit_lvalue_ref_passing_rvalue_fails_to_compile));
	s.push_back(CUTE(test_scope_success_might_throw));
	s.push_back(CUTE(test_make_unique_resource_checked));
	s.push_back(CUTE(testReferenceWrapper));
	s.push_back(CUTE(testScopeExitWithReferenceWrapper));
	s.push_back(CUTE(testsFromEricNiebler_scope_exit_with_throwing_function_object));
	s.push_back(CUTE(testsFromEricNiebler_scope_success_with_throwing_function_object));
	s.push_back(CUTE(testsFromEricNiebler_scope_fail_with_throwing_function_object));
	cute::xml_file_opener xmlfile(argc,argv);
	cute::xml_listener<cute::ide_listener<> >  lis(xmlfile.out);
	cute::makeRunner(lis,argc,argv)(s, "AllTests");
}

int main(int argc, char const *argv[]){
    runAllTests(argc,argv);
    return 0;
}



