
#include <iostream>
#include <string>
#include <numeric>
#include <chrono>
#include <vector>
#include <experimental/numeric>

namespace experimental {
using namespace std;
template<typename _InputIterator, typename _Tp>
  inline _Tp
  accumulate(_InputIterator __first, _InputIterator __last, _Tp __init)
  {
    for (; __first != __last; ++__first)
	__init = std::move(__init) + *__first;
    return __init;
  }

template<typename _InputIterator, typename _Tp, typename _BinaryOperation>
  inline _Tp
  accumulate(_InputIterator __first, _InputIterator __last, _Tp __init,
	       _BinaryOperation __binary_op)
  {
    for (; __first != __last; ++__first)
	__init = __binary_op(std::move(__init), *__first);
    return __init;
  }
}

template <typename F>
std::chrono::microseconds time_n_calls(size_t n, F&& fun){
	std::chrono::high_resolution_clock clock{};
	auto start=clock.now();
	while(n-->0){
		fun();
	}
	return std::chrono::duration_cast<std::chrono::microseconds>(clock.now()-start);
}

using namespace std;

int main() {
	std::vector<std::string> v(10000,"hello"s);

	auto tcopy=time_n_calls(10,[&v]{
		std::string s{"start"};
		//s.reserve(s.size()+v.size()*v.at(0).size()); //useless
		return accumulate(begin(v),end(v),s);});
	cout << "microseconds:"<<tcopy.count() << endl;
	auto tmove=time_n_calls(10,[&v]{
		std::string s{"start"};
		s.reserve(s.size()+v.size()*v.at(0).size()); // pre-allocation saves more
		return ::experimental::accumulate(begin(v),end(v),std::move(s));});
	cout << "microseconds:"<<tmove.count() << endl;
	cout << "speedup:" << tcopy.count()/double(tmove.count())<<endl;
	// not available yet:
//	auto tpar=time_n_calls(10,[&v]{return std::reduce(begin(v),end(v),"start"s);});
//	cout << "microseconds:"<<tmove.count() << endl;
}
