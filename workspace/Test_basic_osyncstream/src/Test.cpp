#include "globalstreambuflocks.h"
#include "syncstream.h"
#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"
#include <thread>

void testlockmapsimple(){
	auto const sz=detail__::thelocks.size();
	std::ostringstream out;
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
	// cerr: needsflush\ndid flush
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
			osyncstream inner { outer.get() };
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
		ASSERT_EQUAL("", outer.str());
		inner.seekp(0,std::ios::beg);
		inner << "hi" << std::flush;
	}
	outer << "hello lawrence\n";
	ASSERT_EQUAL("hillo world\nhello lawrence\n", outer.str());

}
void manyThreadsOnSingleStream(){
	auto sz=detail__::thelocks.size();
	std::ostringstream out{};
	std::ostringstream out2{};
	{
		osyncstream outer2{out2};
		std::vector<std::thread> v;
		for (int i=0; i < 10; ++i){
			outer2 << "2 main "<< i << " sz= "<<detail__::thelocks.size()<< '\n';
			v.push_back(std::thread([i,&out,&out2]{
				osyncstream o2{out2};
				osyncstream{out} << "hello "<< i << std::endl;
				o2 << "2 hello "<< i << '\n';
			}));
			osyncstream{out} << "main " << i << " sz= "<<detail__::thelocks.size()<< '\n';
		}
		for (auto &t:v){
			t.join();
		}
	}
	ASSERT_EQUAL(sz,detail__::thelocks.size());
	ASSERT_EQUAL("",out.str()+out2.str());
}



void runAllTests(int argc, char const *argv[]) {
	cute::suite s;
	//TODO add your test here
	s.push_back(CUTE(testlockmapsimple));
	s.push_back(CUTE(thisIsATest));
	s.push_back(CUTE(testBSIclearForReuseFeature));
	s.push_back(CUTE(testNestedBufferedStream));
	s.push_back(CUTE(testSeekingBufferedStream));
	s.push_back(CUTE(testSeekingWithPotentialMissingOutput));
	s.push_back(CUTE(manyThreadsOnSingleStream));
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<> > lis(xmlfile.out);
	cute::makeRunner(lis, argc, argv)(s, "AllTests");
}

int main(int argc, char const *argv[]) {
	runAllTests(argc, argv);
	osyncstream outer { std::cout };
	{
		osyncstream inner { outer.get() };
		inner << "hello world";
		inner << std::endl;
	}
	outer << "hello lawrence\n";

	return 0;
}

