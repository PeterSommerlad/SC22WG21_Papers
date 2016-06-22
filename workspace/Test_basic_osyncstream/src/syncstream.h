#ifndef BASIC_OSYNCSTREAM_H_
#define BASIC_OSYNCSTREAM_H_
#include <sstream>
#include <cassert>

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
	std::basic_ostream<charT,traits> &out;
	detail__::spmx mxptr;

public:
	void emit() {
		this->seekp(0, std::ios::end);
		{
			std::lock_guard<std::mutex> lk{*mxptr};
			out.write(this->pbase(), this->pptr() - this->pbase());
			if (this->rdbuf()->flushed()) {
				out.flush();
				this->rdbuf()->flushed() = false;
			}
		}// UNLOCK
		assert(!(this->rdbuf()->flushed()));
		this->seekpos(std::ios::beg);
		this->str().clear(); //efficient?
	}

	explicit basic_osyncstream(std::basic_ostream<charT,traits> &os)
		:mybuf{std::ios_base::out}
		,std::basic_ostream<charT, traits>{this}
	 	,out(os)
	 	,mxptr(detail__::thelocks.get_lock(out.rdbuf())){	}
	~basic_osyncstream() noexcept {
		this->emit();
		detail__::thelocks.release_lock(mxptr,out.rdbuf());
	}
    mybuf* rdbuf() const{
        return dynamic_cast<mybuf*>(std::basic_ostream<charT,traits>::rdbuf());
    }
    std::basic_ostream<charT,traits>& get() const {
    	return out;
    }
};
using osyncstream = basic_osyncstream<char>;
using wosyncstream = basic_osyncstream<wchar_t>;


#endif /* BASIC_OSYNCSTREAM_H_ */
