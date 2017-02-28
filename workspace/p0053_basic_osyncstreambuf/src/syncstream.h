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

template <class charT, class traits = char_traits<charT>,
					class Allocator = allocator<charT> >
class basic_syncbuf : public basic_stringbuf<charT, traits, Allocator> {

	using base = basic_stringbuf<charT, traits, Allocator>;

	bool                           needs_flush=false; // exposition only
	basic_streambuf<charT,traits> *wrapped;
	detail__::spmx                 mxptr;
	Allocator                      alloc;

public:
	typedef charT                     char_type;
	typedef typename traits::int_type int_type;
	typedef typename traits::pos_type pos_type;
	typedef typename traits::off_type off_type;
	typedef traits                    traits_type;
	typedef Allocator                 allocator_type;

  typedef basic_streambuf<charT,traits> streambuf_type;

	explicit
	basic_syncbuf(streambuf_type* obuf = nullptr,
								Allocator const &allocator = Allocator())
		: base(ios_base::out) // should pass allocator
		, wrapped{obuf}
		, mxptr{detail__::thelocks.get_lock(wrapped)}
    , alloc(allocator)
  {
	}
	
	basic_syncbuf(basic_syncbuf&& other)
		: base{std::move(other)}
		, wrapped{other.wrapped}
		, mxptr{std::move(other.mxptr)}
    , alloc(other.alloc)
	{
		this->needs_flush = std::exchange(other.needs_flush, false);
		other.str({});
		assert(other.pbase()==other.pptr());

	}

	basic_syncbuf& operator=(basic_syncbuf&& other){
		if (this != &other){
			this->emit(); // might throw
			detail__::thelocks.release_lock(mxptr,wrapped); // might throw
			wrapped = nullptr;
			this->swap(other);
		}
		return *this;
	}

	~basic_syncbuf(){
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

  Allocator get_allocator() const noexcept { return alloc; }

	bool emit() {
		bool result = wrapped != nullptr;
		this->pubseekoff(0, ios::end, ios_base::out);
		auto const len=this->pptr() - this->pbase();
		if (wrapped && mxptr && (len>0||this->needs_flush) )
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

	void swap(basic_syncbuf &other){ // not thread safe!
		using std::swap;
		base::swap(other); // this is not noexcept!
		swap(needs_flush,other.needs_flush);
		swap(wrapped,other.wrapped);
		swap(mxptr,other.mxptr);
	}

protected:
	int sync() override { needs_flush = true; return 0; }
  int_type overflow(int_type c = traits::eof()) override {
    if (nullptr == wrapped)
      return traits::eof();
    else
      return base::overflow(c); 
  }
};

template <class charT, class traits, class Allocator>
inline void swap(basic_syncbuf<charT,traits,Allocator>& a,
								 basic_syncbuf<charT,traits,Allocator>& b)
{
	a.swap(b);
}

using syncbuf  = basic_syncbuf<char>;
using wsyncbuf = basic_syncbuf<wchar_t>;

template <class charT, class traits = char_traits<charT>,
					class Allocator = allocator<charT> >
struct basic_osyncstream : public basic_ostream<charT,traits> {

	typedef basic_syncbuf<charT, traits, Allocator> mybuf_t;
	mybuf_t                                         mybuf;
	
public:
	typedef charT                     char_type;
	typedef typename traits::int_type int_type;
	typedef typename traits::pos_type pos_type;
	typedef typename traits::off_type off_type;
	typedef traits                    traits_type;
	typedef Allocator                 allocator_type;

  typedef basic_streambuf<charT,traits> streambuf_type;

public:

	explicit basic_osyncstream(basic_ostream<charT,traits> &os, 
														 Allocator const &a=Allocator())
		: mybuf{os.rdbuf(),a} { this->rdbuf(&mybuf); }

	explicit basic_osyncstream(streambuf_type *outbuf=nullptr,
														 Allocator const &a=Allocator())
		: mybuf{outbuf,a} { this->rdbuf(&mybuf); }

	basic_osyncstream(basic_osyncstream&& other) // can not be noexcept
		: mybuf{std::move(other.mybuf)}
		, basic_ostream<charT, traits>{std::move(other)}
	{
		assert(this->rdbuf()==nullptr); // sanity check. base move ctor doesn't xfer streambuf ptr
		this->rdbuf(&mybuf);
	}

	basic_osyncstream & operator=(basic_osyncstream &&other) { // cannot be noexcept
		if (this != &other){ // not really required!
			basic_ostream<charT,traits>::operator=(std::move(other));
			mybuf.operator=(std::move(other.mybuf));
			assert(&mybuf == this->rdbuf());
		}
		return *this;
	}

	~basic_osyncstream() noexcept {	}

	void swap(basic_osyncstream & other){
		if (this != &other){
			mybuf.swap(other.mybuf);
			basic_ostream<charT,traits>::swap(other);
		}
	}

	// mybuf_t* rdbuf() const noexcept {
	//	return dynamic_cast<mybuf_t*>(basic_ostream<charT,traits>::rdbuf());
	// }

	streambuf_type* get_wrapped() const noexcept {
		return mybuf.get_wrapped();
	}

  Allocator get_allocator() const noexcept { return mybuf.get_allocator(); }

	void emit(){
		using sentry=typename basic_ostream<charT,traits>::sentry;
		sentry s{*this};
		if (not s || not mybuf.emit()){
			this->setstate(ios_base::badbit);
		}
	}
};

using osyncstream = basic_osyncstream<char>;
using wosyncstream = basic_osyncstream<wchar_t>;

}}} // Close namespaces

#endif /* BASIC_OSYNCSTREAM_H_ */
