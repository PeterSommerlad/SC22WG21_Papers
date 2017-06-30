// -*- mode: c++; c-basic-offset: 2; tab-width: 2 -*-

#ifndef BASIC_OSYNCSTREAM_H_
#define BASIC_OSYNCSTREAM_H_
#include <sstream>
#include <cassert>
#include <utility>

#include "globalstreambuflocks.h"

namespace std {
namespace experimental {
inline namespace concurrency_v2 {

// concrete base class is an implementation detail, just a subclass of basic_stringbuf
template <class charT1, class traits1,class Allocator1>
struct basic_osyncstream;
template <class charT, class traits = char_traits<charT>,
class Allocator=allocator<charT>>
class basic_syncbuf : public basic_stringbuf<charT, traits, Allocator> {

	using base = basic_stringbuf<charT, traits, Allocator>;

	bool                           needs_flush=false; // exposition only
	bool                           flush_immediate=false; // exposition only
	basic_streambuf<charT,traits> *wrapped;
	detail__::spmx                 mxptr;

	friend class basic_osyncstream<charT,traits,Allocator>;

public:
		using char_type = charT;
		using int_type = typename traits::int_type;
		using pos_type = typename traits::pos_type;
		using off_type = typename traits::off_type;
		using traits_type = traits;
		using allocator_type = Allocator;

		using streambuf_type = basic_streambuf<charT,traits>;



	explicit
	basic_syncbuf(streambuf_type* obuf = nullptr,
								Allocator const &allocator = Allocator())
		: base(ios_base::out) // ,allocator) should pass allocator
		, wrapped{obuf}
		, mxptr{detail__::thelocks.get_lock(wrapped)}
		{}
	
	basic_syncbuf(basic_syncbuf&& other) noexcept
		: base{std::move(other)}
		, wrapped{other.wrapped}
		, mxptr{std::move(other.mxptr)}
	{
		this->needs_flush = std::exchange(other.needs_flush, false);
		other.str({});
		assert(other.pbase()==other.pptr());

	}

		basic_syncbuf&operator =(basic_syncbuf&& other) noexcept
		{
			if (this != &other) {
				try {
					this->emit(); // might throw
				}catch(...){}
				try{
					detail__::thelocks.release_lock(mxptr, wrapped);// might throw
				}catch(...){}
				wrapped = nullptr;
				this->swap(other);
			}
			return *this;
		}

	~basic_syncbuf() noexcept {
		try{
			emit();
		}catch(...){}
		try{
			detail__::thelocks.release_lock(mxptr,wrapped);
		}catch(...){}
	}

	streambuf_type* get_wrapped() const noexcept {
		return wrapped;
	}

	bool emit() {
		bool result = wrapped != nullptr;
		this->pubseekoff(0, ios::end, ios_base::out);
		auto const len=this->pptr() - this->pbase();
		if (wrapped!=nullptr && mxptr && (len>0||this->needs_flush) )
		{
			lock_guard<mutex> lk{*mxptr};
			if (len>0 && len != wrapped->sputn(this->pbase(), len)){
				result=false;//this->setstate(ios_base::badbit);
			}
			if (this->needs_flush) {
				if (wrapped->pubsync() == -1)
					result=false;//this->setstate(ios_base::badbit);
			}
		}// UNLOCK
		this->needs_flush = false;
		this->str({});
		return result;
	}

	void swap(basic_syncbuf &other){ // not thread safe! not noexcept
		using std::swap;
		base::swap(other); // this is not noexcept! :-(
		swap(needs_flush,other.needs_flush);
		swap(flush_immediate,other.flush_immediate);
		swap(wrapped,other.wrapped);
		swap(mxptr,other.mxptr);
	}

protected:
	int_type sync() override {
		if(flush_immediate) {
			if (emit()){
				return 0;
			}
			return traits::eof();
		}
		needs_flush = true;
		return 0;
	}
  int_type overflow(int_type c = traits::eof()) override {
    if (nullptr == wrapped)
      return traits::eof();
    else
      return base::overflow(c); 
  }
};




template <class charT, class traits,class Allocator>
inline void swap(basic_syncbuf<charT,traits,Allocator>& a,
								 basic_syncbuf<charT,traits,Allocator>& b)
{
	a.swap(b);
}

using syncbuf  = basic_syncbuf<char>;
using wsyncbuf = basic_syncbuf<wchar_t>;

template <class charT, class traits = char_traits<charT>,class Allocator=allocator<charT>>
struct basic_osyncstream : public basic_ostream<charT,traits>
,private basic_syncbuf<charT,traits,Allocator>{
 		using mybuf_t = basic_syncbuf<charT,traits,Allocator>;
	//mybuf_t                                         *mybuf;
public:
		using char_type = charT;
		using int_type = typename traits::int_type;
		using pos_type = typename traits::pos_type;
		using off_type = typename traits::off_type;
		using traits_type = traits;
		using allocator_type = Allocator;

		using streambuf_type = basic_streambuf<charT,traits>;

public:

	explicit basic_osyncstream(basic_ostream<charT,traits> &os, 
														 Allocator const &a=Allocator())
		: mybuf_t{os.rdbuf(),a} { this->rdbuf(&mybuf); }

	explicit basic_osyncstream(streambuf_type *outbuf=nullptr,
														 Allocator const &a=Allocator())
		: mybuf_t{outbuf,a} { this->rdbuf(&mybuf); }

		basic_osyncstream(basic_osyncstream&& other) noexcept
		: basic_ostream<charT, traits>{std::move(other)}
		, mybuf{std::move(other.mybuf)}
	{
		assert(this->rdbuf()==nullptr); // sanity check. base move ctor doesn't xfer streambuf ptr
		this->rdbuf(&mybuf);
	}

	basic_osyncstream & operator=(basic_osyncstream &&other) noexcept { // cannot be noexcept
		if (this != &other){ // not really required!
			basic_ostream<charT,traits>::operator=(std::move(other));
			mybuf.operator=(std::move(other.mybuf));
			assert(&mybuf == this->rdbuf());
		}
		return *this;
	}

	~basic_osyncstream() noexcept {	}

	void swap(basic_osyncstream & other) noexcept{
		if (this != &other){
			mybuf.swap(other.mybuf);
			basic_ostream<charT,traits>::swap(other);
		}
	}

	streambuf_type* get_wrapped() const noexcept {
		return mybuf.get_wrapped();
	}

	void emit(){
		using sentry=typename basic_ostream<charT,traits>::sentry;
		sentry s{*this};
		if (not s || not mybuf.emit()){
			this->setstate(ios_base::badbit);
		}
	}
	template <class charT1=charT,class traits1=traits,class Allocator1=Allocator>
	friend
	basic_ostream<charT1,traits1>&
		immediateflush(basic_ostream<charT1,traits1>&out){ // doesn't work along with Allocator awareness!!!
			auto syncbuf=dynamic_cast<basic_syncbuf<charT1,traits1,Allocator1>*>(out.rdbuf());
			if (syncbuf){
				syncbuf->flush_immediate=true;
			}
			return out;
		}
};


template <class charT, class traits = char_traits<charT>, class Allocator=allocator<charT>>
	basic_ostream<charT,traits>&
	noimmediateflush(basic_ostream<charT,traits>&out){ // doesn't work along with Allocator awareness!!!
		auto syncbuf=dynamic_cast<basic_syncbuf<charT,traits,Allocator>*>(out.rdbuf());
		if (syncbuf){
			syncbuf->flush_immediate=false;
		}
		return out;
	}



using osyncstream = basic_osyncstream<char>;
using wosyncstream = basic_osyncstream<wchar_t>;

}}} // Close namespaces

#endif /* BASIC_OSYNCSTREAM_H_ */
