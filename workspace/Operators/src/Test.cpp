#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"
#include <utility>
using std::declval;
template <typename T>
class Ops {
//	template <typename U=T>
//	constexpr void operator +=(U const &r) noexcept = delete;
	template <typename TT=T, typename U=TT>
	constexpr friend auto operator+(TT l, U const& r) noexcept
	-> decltype(declval<TT&>()+=declval<U>())
	{
		return l += r;
	}

//	template <typename TT=Ops, typename U=TT>
//	constexpr friend void operator+(TT l, U const& r)
//	{
//		static_assert(false,"you need to define operator+= in your type");
//	}

};


struct addable: Ops<addable>{
	constexpr addable(int i):val{i}{
	}
	constexpr addable& operator+=(addable const &r){
		val += r.val;
		return *this;
	}
	explicit constexpr operator int() const { return val;}
private:
	int val;
};

struct nonaddable: Ops<nonaddable>{

};

void InsaneBool() {
	addable one{1};
	addable two = one+one;
	ASSERT_EQUAL(2,two.operator int());
}
void nonaddableTest_compile_error(){
	nonaddable na{};
	na+na;
	ASSERTM("start writing tests", false);
}



bool runAllTests(int argc, char const *argv[]) {
	cute::suite s { };
	//TODO add your test here
	s.push_back(CUTE(InsaneBool));
	s.push_back(CUTE(nonaddableTest_compile_error));
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
	auto runner = cute::makeRunner(lis, argc, argv);
	bool success = runner(s, "AllTests");
	return success;
}

int main(int argc, char const *argv[]) {
    return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
