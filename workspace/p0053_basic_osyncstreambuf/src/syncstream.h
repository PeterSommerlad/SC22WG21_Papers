#ifndef BASIC_OSYNCSTREAM_H_
#define BASIC_OSYNCSTREAM_H_
#include <sstream>
#include <cassert>
#include <utility>

#include "globalstreambuflocks.h"

template <class charT, class traits , class Allocator >
struct basic_osyncstream;

namespace detail__{
template <class charT, class traits, class Allocator>
class noteflush_streambuf:public std::basic_stringbuf<charT, traits, Allocator>{
	using base=std::basic_stringbuf<charT, traits, Allocator>;
	bool needs_flush_=false; // exposition only
	//friend struct basic_osyncstream<charT, traits, Allocator>;
	std::basic_streambuf<charT,traits> *out;
	detail__::spmx mxptr;
public:
	explicit noteflush_streambuf(std::basic_streambuf<charT,traits>* wrapped=nullptr,Allocator const &a=Allocator())
	:base::basic_stringbuf(std::ios_base::out) // should pass a
	,out{wrapped}
	,mxptr{detail__::thelocks.get_lock(out)}{
	}
	noteflush_streambuf(noteflush_streambuf&& other)
	:base::basic_stringbuf{std::move(other)}
	,out{other.out}
	,mxptr{std::move(other.mxptr)}
	{
		std::exchange(this->flushed(),other.flushed());
		other.pubseekoff(0,std::ios::beg,std::ios_base::out);
		other.str().clear();
		assert(other.pbase()==other.pptr());

	}
	noteflush_streambuf& operator=(noteflush_streambuf&& other){
		if (this != &other){
			this->emit(); // might throw
			detail__::thelocks.release_lock(mxptr,out); // might throw
			out = nullptr;
			this->swap(other);
		}
		return *this;
	}
	~noteflush_streambuf(){
		try{
			detail__::thelocks.release_lock(mxptr,out);
		}catch(...){}
	}

	std::basic_streambuf<charT,traits>* rdbuf_wrapped() const noexcept {
		return out;
	}
	bool emit() {
		//using sentry=typename std::basic_ostream<charT,traits>::sentry;
		bool result{true};
		this->pubseekoff(0, std::ios::end,std::ios_base::out);
		auto const len=this->pptr() - this->pbase();
		if (out && mxptr && (len>0||this->flushed()) )
		{
			std::lock_guard<std::mutex> lk{*mxptr};
				if (len>0 && len != out->sputn(this->pbase(), len)){
					result=false;//this->setstate(std::ios_base::badbit);
				}
				if (this->flushed()) {
					if (out->pubsync() == -1)
						result=false;//this->setstate(std::ios_base::badbit);
				}
		}// UNLOCK
		this->flushed() = false;
		this->pubseekpos(0,std::ios_base::out);
		this->_M_string.clear(); //efficient? should retain allocated memory, only gcc
		return result;
	}

protected:
	bool& flushed() noexcept{return needs_flush_;} // exposition only
	bool flushed()const noexcept{return needs_flush_;} // exposition only
	virtual int sync(){ needs_flush_=true; return 0;}
	void swap(noteflush_streambuf &other){ // not thread safe!
		using std::swap;
		std::basic_stringbuf<charT, traits, Allocator>::swap(other); // this is not noexcept!
		swap(needs_flush_,other.needs_flush_);
		swap(out,other.out);
		swap(mxptr,other.mxptr);
	}
};
}

template <class charT, class traits = std::char_traits<charT>, class Allocator = std::allocator<charT> >
struct basic_osyncstream
		: private detail__::noteflush_streambuf<charT, traits, Allocator> // implementation detail
, public std::basic_ostream<charT,traits>  {
public:
	typedef charT                         char_type;
	typedef traits                        traits_type;
	typedef typename traits_type::int_type int_type;
	typedef typename traits_type::pos_type pos_type;
	typedef typename traits_type::off_type off_type;
	typedef Allocator                     allocator_type;

	typedef detail__::noteflush_streambuf<charT, traits, Allocator> mybuf; // implementation detail
public:

	explicit basic_osyncstream(std::basic_ostream<charT,traits> &os, Allocator const &a=Allocator())
	:basic_osyncstream{os.rdbuf(),a}{}

	explicit basic_osyncstream(std::basic_streambuf<charT,traits> *outbuf=0, Allocator const &a=Allocator())
	:mybuf{outbuf,a}
	,std::basic_ostream<charT, traits>{this}
	{}
	basic_osyncstream(basic_osyncstream&& other) // can not be noexcept
	:mybuf{std::move(other)}
	,std::basic_ostream<charT, traits>{std::move(other)} // doesn't transfer streambuf
	{
		assert(this->rdbuf()==nullptr); // sanity check. base' move ctor doesn't
		this->set_rdbuf(this);
	}
	basic_osyncstream & operator=(basic_osyncstream &&other){ // can not be noexcept
		if (this != &other){ // not really required!
			mybuf::operator=(std::move(other));
			std::basic_ostream<charT,traits>::operator=(std::move(other));
			assert(this==rdbuf());
		}
		return *this;
	}

	~basic_osyncstream() noexcept {
		try{
			this->emit();
		} catch(...){}
	}
	void swap(basic_osyncstream & other){
		if (this != &other){
			mybuf::swap(other);
			std::basic_ostream<charT,traits>::swap(other);
		}
	}

	mybuf* rdbuf() const noexcept {
		return dynamic_cast<mybuf*>(std::basic_ostream<charT,traits>::rdbuf());
	}
	std::basic_streambuf<charT,traits>* rdbuf_wrapped() const noexcept {
		return mybuf::rdbuf_wrapped();
	}
	void emit(){
		using sentry=typename std::basic_ostream<charT,traits>::sentry;
		sentry s{*this};
		if (not s || not mybuf::emit()){
			this->setstate(std::ios_base::badbit);
		}
	}
};
using osyncstream = basic_osyncstream<char>;
using wosyncstream = basic_osyncstream<wchar_t>;


#endif /* BASIC_OSYNCSTREAM_H_ */
