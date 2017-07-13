// -*- mode: c++; c-basic-offset: 2; tab-width: 2 -*-

#include "globalstreambuflocks.h"
#include "syncstream.h"
#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"
#include <thread>

using namespace std::experimental;

template <class charT, class traits>
std::basic_string<charT, traits>
str(const basic_osyncstream<charT, traits>& oss)
{
	return oss.str();
}

void testlockmapsimple(){
	auto const sz=detail__::thelocks.size();
	std::ostringstream out { };
	{
		auto mx=detail__::thelocks.get_lock(out.rdbuf());
		{
			auto my=detail__::thelocks.get_lock(out.rdbuf());
			ASSERT_EQUAL(mx,my);
			detail__::thelocks.release_lock(my,out.rdbuf());
			ASSERT_EQUAL(1+sz,detail__::thelocks.size());
		}
		ASSERT(mx.get() != nullptr);
		detail__::thelocks.release_lock(mx,out.rdbuf());
	}
	ASSERT_EQUAL(0+sz,detail__::thelocks.size());
}

void testlockmapwithnullreturnsnull(){
	auto const sz=detail__::thelocks.size();
	{
		auto mx=detail__::thelocks.get_lock(nullptr);
		ASSERT(mx == nullptr);
		detail__::thelocks.release_lock(mx,nullptr); // should be no-op
	}
	ASSERT_EQUAL(0+sz,detail__::thelocks.size());

}

void thisIsATest() {
	std::ostringstream outer { };
	{
		osyncstream inner { outer };
		inner << "hello world";
		inner << std::endl;
		ASSERT_EQUAL("", outer.str());
	}
	outer << "hello lawrence\n";
	ASSERT_EQUAL("hello world\nhello lawrence\n", outer.str());
}

void testBSIclearForReuseFeature() {
	std::ostringstream outer { };
	{
		osyncstream inner { outer };
		inner << "hello world\n";
		ASSERT_EQUAL("", outer.str());
		inner.emit();
		ASSERT_EQUAL("hello world\n", outer.str());
		inner << "hello roger\n" << std::flush; // did flush\nneedsflush\did flush\n
	}
	outer << "hello lawrence\n";
	ASSERT_EQUAL("hello world\nhello roger\nhello lawrence\n", outer.str());

}
void testNestedBufferedStream() {
	std::ostringstream outs { };
	{
		osyncstream outer { outs };
		outer << "hello ";
		{
			osyncstream inner { outer.get_wrapped() };
			inner << "hello world\n";
			ASSERT_EQUAL("", outs.str());
			inner.emit();
			inner << "hello roger\n" << std::flush;
		}
		outer << "lawrence\n";
	}
	ASSERT_EQUAL("hello world\nhello roger\nhello lawrence\n", outs.str());

}

void testSeekingBufferedStream(){
	std::ostringstream outer { };
	{
		osyncstream inner { outer };
		inner << "hello world\n";
		ASSERT_EQUAL("", outer.str());
		inner.seekp(-4,std::ios::end);
		inner << "lfi\n" << std::flush;
	}
	outer << "hello lawrence\n";
	ASSERT_EQUAL("hello wolfi\nhello lawrence\n", outer.str());
}
void testSeekingWithPotentialMissingOutput(){
	std::ostringstream outer { };
	{
		osyncstream inner { outer };
		inner << "hello world\n";
		ASSERT_EQUAL("hello world\n",inner.str());
		ASSERT_EQUAL("", outer.str());
		inner.seekp(0,std::ios::beg);
		inner << "hi." << std::flush;
		ASSERT_EQUAL("hi.lo world\n",inner.str());
	}
	outer << "hello lawrence\n";
	ASSERT_EQUAL("hi.lo world\nhello lawrence\n", outer.str());

}
void ostreamsWithSharingStreambuf(){
	std::ostringstream out{};
	std::ostream out2{out.rdbuf()};

	{
		osyncstream inner{out};
		osyncstream inner2{out2};
		inner << "inner"<<std::endl;
		inner2 << "inner2\n";
	}
	ASSERT_EQUAL("inner2\ninner\n",out.str());
}


void testMoveConstruction(){
	auto const sz=detail__::thelocks.size();
	std::ostringstream out{};
	{
		osyncstream os{out};
		os << "hallo\n";
		{
			osyncstream os1{std::move(os)};
			os1 << "welt\n";
		}
		ASSERT_EQUAL("hallo\nwelt\n",out.str());
	}
	ASSERT_EQUAL(sz,detail__::thelocks.size());
}
void testMoveAssignment(){
	auto const sz=detail__::thelocks.size();
	std::ostringstream out{};
	std::ostringstream dummy{};
	{
		osyncstream os{out};
		os << "hallo\n";
		{
			osyncstream os1{dummy};
			os1 << "welt\n";
			ASSERT_EQUAL("welt\n",str(os1));
			os1 = std::move(os);
			ASSERT_EQUAL("hallo\n",str(os1));
			os1 << "world\n";
		}
		ASSERT_EQUAL("welt\n",dummy.str());
		ASSERT_EQUAL("hallo\nworld\n",out.str());
	}
	ASSERT_EQUAL(sz,detail__::thelocks.size());
}

void testMemberSwap(){
	auto const sz=detail__::thelocks.size();
	std::ostringstream out{};
	std::ostringstream dummy{};
	{
		osyncstream os{out};
		os << "hallo\n";
		{
			osyncstream os1{dummy};
			os1 << "welt\n";
			ASSERT_EQUAL("welt\n",str(os1));
			os1.swap(os);
			ASSERT_EQUAL("hallo\n",str(os1));
			os1 << "world\n";
		}
		os.emit();
		ASSERT_EQUAL("welt\n",dummy.str());
		ASSERT_EQUAL("hallo\nworld\n",out.str());
	}
	ASSERT_EQUAL(sz,detail__::thelocks.size());
}

void manyThreadsOnSingleStream(){
	auto const sz=detail__::thelocks.size();
	std::ostringstream out{};
	std::ostringstream out2{};
	{
		osyncstream outer2{out2};
		std::vector<std::thread> v;
		for (int i=0; i < 10; ++i){
			outer2 << "2 main "<< i << " sz= "<<detail__::thelocks.size()<< '\n';
			v.push_back(std::thread([i,&out,&out2]{
				osyncstream o2{out2};
        std::string hello{"hello"};
				osyncstream{out} << hello << ' ' << i << std::endl;
				o2 << "2 hello "<< i << '\n';
			}));
			osyncstream{out} << "main " << i << " sz= "<<detail__::thelocks.size()<< '\n';
		}
		for (auto &t:v){
			t.join();
		}
	}
	ASSERT_EQUAL(sz,detail__::thelocks.size());
	ASSERT_EQUAL("",out.str()+out2.str()); // intentionally fails
}
void trigger_assert_in_ctor(){
	std::ostream noout{nullptr};
	osyncstream sync(noout); // should abort
}

void testmanipulators(){
	std::ostringstream outer { };
	{
		osyncstream inner { outer };
		inner << std::experimental::emit_on_flush;
		inner << "hello world";
		inner << std::endl;
		outer << "within ";
		inner << std::experimental::no_emit_on_flush;
		inner << "hello world 2"<< std::endl;
		ASSERT_EQUAL("hello world\nwithin ", outer.str());
		inner << "something else\n"<< std::experimental::flush_emit ;
		ASSERT_EQUAL("hello world\nwithin hello world 2\nsomething else\n",outer.str());
	}
	outer << "hello lawrence\n";
	ASSERT_EQUAL("hello world\nwithin hello world 2\nsomething else\nhello lawrence\n", outer.str());
}

void testmanipulatorsforregularostream(){
	std::ostringstream outer { };
	outer << "hello" << std::experimental::emit_on_flush << std::endl;
	ASSERT_EQUAL("hello\n",outer.str());
	outer << "world" << std::experimental::no_emit_on_flush << std::endl;
	ASSERT_EQUAL("hello\nworld\n",outer.str());
	outer << "can not test flush" << std::experimental::flush_emit ;
	ASSERT_EQUAL("hello\nworld\ncan not test flush",outer.str());
}




void runAllTests(int argc, char const *argv[]) {
	cute::suite s;
	//TODO add your test here
	s.push_back(CUTE(testlockmapsimple));
	s.push_back(CUTE(testlockmapwithnullreturnsnull));
	s.push_back(CUTE(thisIsATest));
	s.push_back(CUTE(testBSIclearForReuseFeature));
	s.push_back(CUTE(testNestedBufferedStream));
	s.push_back(CUTE(testSeekingBufferedStream));
	s.push_back(CUTE(testSeekingWithPotentialMissingOutput));
	s.push_back(CUTE(ostreamsWithSharingStreambuf));
	s.push_back(CUTE(trigger_assert_in_ctor));
	s.push_back(CUTE(testMoveConstruction));
	s.push_back(CUTE(testMoveAssignment));
	s.push_back(CUTE(testMemberSwap));
	s.push_back(CUTE(testmanipulators));
	s.push_back(CUTE(testmanipulatorsforregularostream));
	s.push_back(CUTE(manyThreadsOnSingleStream));
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<> > lis(xmlfile.out);
	cute::makeRunner(lis, argc, argv)(s, "AllTests");
}

int main(int argc, char const *argv[]) {
	runAllTests(argc, argv);
	osyncstream outer { std::cout };
	{
		osyncstream inner { outer.get_wrapped() };
		inner << "hello world";
		inner << std::endl;
	}
	outer << "hello lawrence\n";

	return 0;
}

