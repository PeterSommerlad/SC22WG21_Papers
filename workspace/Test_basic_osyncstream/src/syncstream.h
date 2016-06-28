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
	bool needs_flush_=false; // exposition only
	friend struct basic_osyncstream<charT, traits, Allocator>;
public:
	using std::basic_stringbuf<charT, traits, Allocator>::basic_stringbuf;
protected:
	bool& flushed() noexcept{return needs_flush_;} // exposition only
	bool flushed()const noexcept{return needs_flush_;} // exposition only
    virtual int sync(){ needs_flush_=true; return 0;}
    void swap(noteflush_streambuf &other){
    	std::basic_stringbuf<charT, traits, Allocator>::swap(other); // this is not noexcept!
    	std::swap(needs_flush_,other.needs_flush_);
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
private:
	std::basic_streambuf<charT,traits> *out;
	detail__::spmx mxptr;

public:
	void emit() {
		using sentry=typename std::basic_ostream<charT,traits>::sentry;
		this->seekp(0, std::ios::end);
		auto const len=this->pptr() - this->pbase();
		if (out && mxptr && (len>0||this->flushed()) )
		{
			std::lock_guard<std::mutex> lk{*mxptr};
			sentry __s(*this);
			if (__s ) // should do some exception handling here like in ostream
			{
				if (len>0 && len != out->sputn(this->pbase(), len)){
					this->setstate(std::ios_base::badbit);
				}
				if (this->flushed()) {
					if (out->pubsync() == -1)
						this->setstate(std::ios_base::badbit);
				}
			}
		}// UNLOCK
		this->flushed() = false;
		this->seekpos(std::ios::beg);
		this->str().clear(); //efficient? should retain allocated memory
	}

	explicit basic_osyncstream(std::basic_ostream<charT,traits> &os, Allocator const &a=Allocator())
	:basic_osyncstream{os.rdbuf(),a}{}

	explicit basic_osyncstream(std::basic_streambuf<charT,traits> *outbuf=0, Allocator const &a=Allocator())
	:mybuf{std::ios_base::out} // should pass a here, but basic_stringbuf doesn't take it (yet)
	,std::basic_ostream<charT, traits>{this}
	,out{outbuf}
	,mxptr{detail__::thelocks.get_lock(out)}{
	}
	 basic_osyncstream(basic_osyncstream&& other) // can not be noexcept
	 :mybuf{std::move(other)}
	 ,std::basic_ostream<charT, traits>{std::move(other)} // doesn't transfer streambuf
	 ,out{other.out}
	 ,mxptr{std::move(other.mxptr)}
	 {
		 assert(this->rdbuf()==nullptr); // sanity check. base' move ctor doesn't
		this->set_rdbuf(this);
		std::exchange(this->flushed(),other.flushed());
		other.seekpos(std::ios::beg);
		other.str().clear();
		assert(other.rdbuf()->pbase()==other.rdbuf()->pptr());
	 }
	 basic_osyncstream & operator=(basic_osyncstream &&other){ // can not be noexcept
		 if (this != &other){ // not really required!
			 this->emit(); // might throw
			 detail__::thelocks.release_lock(mxptr,out); // might throw
			 out = nullptr;
             std::basic_ios<charT,traits>::swap(static_cast<std::basic_ios<charT,traits>&>(other));
             assert(this==rdbuf());
             // retains streambuf mybuf
             std::swap(mxptr,other.mxptr);
			 std::exchange(this->flushed(),other.flushed());
			 std::swap(out,other.out);
			 this->str(other.rdbuf()->str()); // might throw, copies
			 other.seekoff(0,std::ios::end);
			 this->seekpos(other.tellp());
			 //mxptr = detail__::thelocks.get_lock(out);
		 }
		 return *this;
	 }

	 ~basic_osyncstream() noexcept {
		 try{
			 this->emit();
		 } catch(...){}
		 try{
			 detail__::thelocks.release_lock(mxptr,out);
		 }catch(...){}
	}
	 void swap(basic_osyncstream & other){
		 if (this != &other){
			 mybuf::swap(other);
			 std::basic_ostream<charT,traits>::swap(other);
             std::swap(mxptr,other.mxptr);
			 std::exchange(this->flushed(),other.flushed());
			 std::swap(out,other.out);
		 }
	 }

    mybuf* rdbuf() const noexcept {
        return dynamic_cast<mybuf*>(std::basic_ostream<charT,traits>::rdbuf());
    }
    std::basic_streambuf<charT,traits>* rdbuf_wrapped() const noexcept {
    	return out;
    }
};
using osyncstream = basic_osyncstream<char>;
using wosyncstream = basic_osyncstream<wchar_t>;


#endif /* BASIC_OSYNCSTREAM_H_ */
