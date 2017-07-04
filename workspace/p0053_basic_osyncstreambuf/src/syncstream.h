// -*- mode: c++; c-basic-offset: 2; tab-width: 2 -*-

#ifndef BASIC_OSYNCSTREAM_H_
#define BASIC_OSYNCSTREAM_H_
#include <ostream>
#include <cassert>
#include <utility>
#include "globalstreambuflocks.h"
#include <iostream>
namespace std {
namespace experimental {
inline namespace concurrency_v2 {

// concrete base class is an implementation detail, just a subclass of basic_stringbuf
template <class charT1, class traits1,class Allocator1>
struct basic_osyncstream;

template <class charT, class traits = char_traits<charT>>
class __basic_syncbuf : public basic_streambuf<charT, traits> {
	using base=basic_streambuf<charT, traits>;

	template <class charT1, class traits1 >
	friend
		basic_ostream<charT1,traits1>&
		emit_on_flush(basic_ostream<charT1,traits1>&out);
	template <class charT1, class traits1>
	friend
		basic_ostream<charT1,traits1>&
		no_emit_on_flush(basic_ostream<charT1,traits1>&out);
	template <class charT1, class traits1 >
	friend
		basic_ostream<charT1,traits1>&
		flush_emit(basic_ostream<charT1,traits1>&out);

protected:
	bool needs_flush=false; // exposition only
	bool flush_immediate=false; // exposition only
	virtual bool do_emit()=0; // implementation detail

public:
	using basic_streambuf<charT,traits>::basic_streambuf;
    __basic_syncbuf()=default;
    __basic_syncbuf(__basic_syncbuf&& other)
    :base{static_cast<base const &>(other)}
	 , needs_flush{other.needs_flush}
	 , flush_immediate{other.flush_immediate}
	 {}
	 void swap(__basic_syncbuf &other){
		 using std::swap;
		 base::swap(other);
		 swap(needs_flush,other.needs_flush);
		 swap(flush_immediate,other.flush_immediate);
	 }
	 __basic_syncbuf & operator=(__basic_syncbuf&& other){
		 base::operator=(static_cast<base const &>(other));
		 needs_flush= other.needs_flush;
		 flush_immediate=other.flush_immediate;
	 }

};
template <class charT, class traits = char_traits<charT>,
class Allocator=allocator<charT>>
class basic_syncbuf : public __basic_syncbuf<charT, traits> {
public:
		using char_type = charT;
		using int_type = typename traits::int_type;
		using pos_type = typename traits::pos_type;
		using off_type = typename traits::off_type;
		using traits_type = traits;
		using allocator_type = Allocator;

		using streambuf_type = basic_streambuf<charT,traits>;
private:
	friend class basic_osyncstream<charT,traits,Allocator>;
	using base = __basic_syncbuf<charT, traits>;
	using base::flush_immediate;
	using base::needs_flush;

	using __string_type=std::basic_string<charT,traits,Allocator>;
	using __size_type = typename __string_type::size_type;
	__string_type _M_string;
	basic_streambuf<charT,traits> *wrapped;
	detail__::spmx                 mxptr;
    mutable __size_type      _M_hack_for_end{};

    void _M_sync(){
    	__size_type const __oldlen = _M_end();
    	assert(__oldlen <= _M_string.capacity());
    	_M_pbump(__oldlen);
    }
	void
	_M_pbump(off_type __off)
    {
		char_type* __pbeg=this->_M_string.data();
		char_type* __pend= __pbeg+this->_M_string.capacity();
      this->setp(__pbeg, __pend);
      while (__off > __gnu_cxx::__numeric_traits<int>::__max)
	{
	  this->pbump(__gnu_cxx::__numeric_traits<int>::__max);
	  __off -= __gnu_cxx::__numeric_traits<int>::__max;
	}
      this->pbump(__off);
    }
	__size_type _M_end() const {
		__size_type len=this->pptr()-this->pbase();
		if (_M_string.size()>len) len =_M_string.size();
		if (len < _M_hack_for_end) len=_M_hack_for_end;
		else {
			_M_hack_for_end=len;
			std::cerr << _M_string.size() <<":"<<_M_hack_for_end <<" of "<<_M_string.capacity()<< std::endl;
		}
		assert(len <= _M_string.capacity());
		return len;
	}
	bool do_emit() override final {
		return this->emit();
	}
public:


	explicit
	basic_syncbuf(streambuf_type* obuf = nullptr,
								Allocator const &allocator = Allocator())
		: base{}
		, _M_string{allocator}
		, wrapped{obuf}
		, mxptr{detail__::thelocks.get_lock(wrapped)}
		{
			//_M_string.reserve(256);
			_M_sync();
		}
	
	basic_syncbuf(basic_syncbuf&& other) noexcept
		: base{std::move(*static_cast<base *>(&other))}
		, _M_string{std::move(other._M_string)}
		, wrapped{other.wrapped}
		, mxptr{std::move(other.mxptr)}
	{
		__size_type o_off = this->pptr()-this->pbase(); // pointers where transfered
		_M_pbump(o_off);

		other._M_string.clear(); // at least gcc's string move ctor is destructive
		other._M_pbump(0);
		assert(other.pbase()==other.pptr()); // bummer
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
				base::operator=(std::move(other));
				wrapped = nullptr;
				mxptr=nullptr;
				//this->swap(other);
				__size_type o_off = other.pptr()-other.pbase();
				_M_string.reserve(other._M_string.capacity());
				_M_string.assign(other.pbase(),other._M_end());
				this->_M_pbump(o_off);
				using std::swap;
				swap(this->wrapped,other.wrapped);
				swap(this->mxptr,other.mxptr);
				other._M_string.clear();
				other._M_hack_for_end=0;
				other._M_pbump(0);
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
		bool result = wrapped != nullptr && mxptr!=nullptr ;
		streamsize len= _M_end();
std::cout << len << std::endl;
		if (result && (len>0||this->needs_flush) )
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
		this->_M_string.clear();
		this->_M_hack_for_end=0;
		this->_M_pbump(0);
		return result;
	}

	void swap(basic_syncbuf &other){ // not thread safe! not noexcept
		// might need to do better, but .... this is a proof of concept, not a perfect implementation.
		basic_syncbuf tmp{std::move(*this)};
		*this = std::move(other);
		other = std::move(tmp);
	}
	// for testing:
	__string_type str() const {
		return this->pbase()?__string_type{this->pbase(),_M_end()}:__string_type{};
	}

protected:
		int_type
		overflow(int_type __c) override
		{
			std::cerr << "overflow:"<<_M_end()<<std::endl;
			if (nullptr == wrapped)	return traits_type::eof();
		    const bool __testeof = traits_type::eq_int_type(__c, traits_type::eof());
		    if (__testeof) return traits_type::not_eof(__c);

			const __size_type __capacity = _M_string.capacity();
			const __size_type __max_size = _M_string.max_size();
			if ( __capacity == __max_size)
			return traits_type::eof();

			const char_type __conv = traits_type::to_char_type(__c);
			const bool __testput = this->pptr() < this->epptr();

			if (!__testput)
			{
				// NB: Start osyncstream buffers at 512 chars.  This is an
				// experimental value (pronounced "arbitrary" in some of the
				// hipper English-speaking countries), and can be changed to
				// suit particular needs.
				const __size_type __opt_len = std::max(__size_type(2 * __capacity),
						__size_type(512));
				const __size_type __len = std::min(__opt_len, __max_size);
				__string_type __tmp {_M_string.get_allocator()};
				__tmp.reserve(__len);
				if (this->pbase() && this->epptr()!=this->pbase())
					__tmp.assign(this->pbase(), this->epptr() - this->pbase());
				__tmp.push_back(__conv);
				_M_string.swap(__tmp);
				_M_sync(); // will bump base on _M_string.size() here!
			}
			else{ // hack needed to adjust maximum string length in _M_end()
				*this->pptr() = __conv;
				this->pbump(1);
			}
			return __c;
		}
	int_type sync() override {
		this->needs_flush = true;
		if(this->flush_immediate) {
			if (this->emit()){
				return 0;
			}
			return traits::eof();
		}
		return 0;
	}
	pos_type seekoff(off_type __off, ios_base::seekdir __way, ios_base::openmode __mode) override {
	      pos_type __ret =  pos_type(off_type(-1));
		  bool __testout {(ios_base::out & __mode) != 0};
	      const char_type * const __beg=this->pbase();
	      auto __end=_M_end(); // hack to save old end
	      if ((__beg|| !__off)&& __testout){
	    	  off_type newoff=__off;
	    	  if (__way == ios_base::cur){
	    		  newoff += this->pptr()-__beg;
	    	  } else if (__way ==ios_base::end){
	    		  // figure out max size, either pptr or _M_string.size()
	    		  newoff += __end;
	    	  }
	    	  if (newoff >=0 && __size_type(newoff)<=_M_end()){
	    		  _M_pbump(newoff);
	    		  __ret=pos_type(newoff);
	    	  }
	      }
	      return __ret;
	}
	pos_type seekpos(pos_type __sp, ios_base::openmode __mode) override {
	      pos_type __ret =  pos_type(off_type(-1));
			bool __testout {(ios_base::out & __mode) != 0};
	      const char_type * const __beg=this->pbase();
	      if ((__beg|| !off_type(__sp))&& __testout){
	    	  const off_type __pos(__sp);
	    	  if (0<= __pos && __size_type(__pos) <= _M_end()){
	    		  _M_pbump(__pos);
	    		  __ret=__sp;
	    	  }
	      }
	      return __ret;
	}
    streamsize
    xsputn(const char_type* __s, streamsize __n) override{
        streamsize __ret = base::xsputn(__s,__n);
        _M_end(); // hack _M_hack_for_end update
        return __ret;

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
struct basic_osyncstream :
		 private basic_syncbuf<charT,traits,Allocator>
		, public basic_ostream<charT,traits>
{
 		using mybuf_t = basic_syncbuf<charT,traits,Allocator>;
 		using base = basic_ostream<charT,traits>;
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
		: mybuf_t{os.rdbuf(),a}
	    , base{} {
			base::init(static_cast<mybuf_t*>(this));
		}

	explicit basic_osyncstream(streambuf_type *outbuf=nullptr,
														 Allocator const &a=Allocator())
		: mybuf_t{outbuf,a}
		, base{}{
			base::init(static_cast<mybuf_t*>(this));
		}

		basic_osyncstream(basic_osyncstream&& other) noexcept
		: mybuf_t{std::move(*static_cast<mybuf_t*>(&other))}
		, base{std::move(*static_cast<base *>(&other))}
	{
		assert(this->rdbuf()==nullptr); // sanity check. base move ctor doesn't xfer streambuf ptr
		base::init(static_cast<mybuf_t*>(this));
	}

	basic_osyncstream & operator=(basic_osyncstream &&other) noexcept { // cannot be noexcept
		if (this != &other){ // not really required!
			mybuf_t::operator=(std::move(*static_cast<mybuf_t*>(&other)));
			base::operator=(std::move(*static_cast<base*>(&other)));
		}
		return *this;
	}

	~basic_osyncstream() noexcept {	}

	void swap(basic_osyncstream & other) noexcept{
		if (this != &other){
			mybuf_t::swap(other);
			basic_ostream<charT,traits>::swap(other);
		}
	}

	streambuf_type* get_wrapped() const noexcept {
		return mybuf_t::get_wrapped();
	}

	void emit(){
		using sentry=typename basic_ostream<charT,traits>::sentry;
		sentry s{*this};
		if (not s || not mybuf_t::emit()){
			this->setstate(ios_base::badbit);
		}
	}
	//for testing
	basic_string<charT,traits,Allocator>
	str() const {
		return mybuf_t::str();
	}
};


template <class charT, class traits = char_traits<charT>>
	basic_ostream<charT,traits>&
	emit_on_flush(basic_ostream<charT,traits>&out){
		auto syncbuf=dynamic_cast<__basic_syncbuf<charT,traits>*>(out.rdbuf());
		if (syncbuf){
			syncbuf->flush_immediate=true;
		}
		return out;
	}
template <class charT, class traits = char_traits<charT>>
	basic_ostream<charT,traits>&
	no_emit_on_flush(basic_ostream<charT,traits>&out){
		auto syncbuf=dynamic_cast<__basic_syncbuf<charT,traits>*>(out.rdbuf());
		if (syncbuf){
			syncbuf->flush_immediate=false;
		}
		return out;
	}

template <class charT, class traits = char_traits<charT>>
	basic_ostream<charT,traits>&
	flush_emit(basic_ostream<charT,traits>&out){
		out.flush();
		auto syncbuf=dynamic_cast<__basic_syncbuf<charT,traits>*>(out.rdbuf());
		if (syncbuf){
			if (not syncbuf->do_emit())
				out.setstate(ios_base::badbit);
		}
		return out;
	}


using osyncstream = basic_osyncstream<char>;
using wosyncstream = basic_osyncstream<wchar_t>;

}}} // Close namespaces

#endif /* BASIC_OSYNCSTREAM_H_ */
