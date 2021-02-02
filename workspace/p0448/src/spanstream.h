#ifndef SRC_SPANSTREAM_H_
#define SRC_SPANSTREAM_H_
// Span based streams -*- C++ -*-

// Copyright (C) 2016 Peter Sommerlad and Free Software Foundation, Inc.
//

/** @file include/spanstream
 *  This is a proposed Standard C++ Library header.
 */

//
// ISO C++ 14882(20): 27.X  Span-based streams
//

//#pragma GCC system_header

#include <string_view>

// should go into iosfwd
namespace std {
namespace experimental {

template<typename _CharT, typename _Traits = std::char_traits<_CharT> >
class basic_spanbuf;

template<typename _CharT, typename _Traits = std::char_traits<_CharT>>
class basic_ispanstream;

template<typename _CharT, typename _Traits = std::char_traits<_CharT>>
class basic_ospanstream;

template<typename _CharT, typename _Traits = std::char_traits<_CharT> >
class basic_spanstream;

/// Class for @c char memory buffers.
using spanbuf = basic_spanbuf<char>;

/// Class for @c char input memory streams.
using ispanstream = basic_ispanstream<char>;

/// Class for @c char output memory streams.
using ospanstream = basic_ospanstream<char>;

/// Class for @c char mixed input and output memory streams.
using spanstream = basic_spanstream<char>;

/// Class for @c wchar_t memory buffers.
using wspanbuf = basic_spanbuf<wchar_t>;

/// Class for @c wchar_t input memory streams.
using wispanstream = basic_ispanstream<wchar_t>;

/// Class for @c wchar_t output memory streams.
using wospanstream = basic_ospanstream<wchar_t>;

/// Class for @c wchar_t mixed input and output memory streams.
using wspanstream = basic_spanstream<wchar_t>;
}
}

#include <span>

#include <istream>
#include <ostream>

namespace std {
namespace experimental {
// [27.X.1] template class basic_spanbuf
/**
 *  @brief  The actual work of input and output (for std::span).
 *  @ingroup io
 *
 *  @tparam _CharT  Type of character stream.
 *  @tparam _Traits  Traits for character type, defaults to
 *                   char_traits<_CharT>.
 *
 *  This class associates either or both of its input and output sequences
 *  with a sequence of characters, which can be initialized from, or made
 *  available as, a @c std::span<_CharT>.  (Paraphrased from [27.X.1]/1.)
 *  The underlying sequence is never managed and must remain valid while used
 *
 *  For this class, open modes (of type @c ios_base::openmode) have
 *  @c in set if the input sequence can be read, and @c out set if the
 *  output sequence can be written.
 */
template<typename _CharT, typename _Traits>
class basic_spanbuf: public basic_streambuf<_CharT, _Traits> {
	struct __xfer_bufptrs;
public:
	// Types:
	typedef _CharT char_type;
	typedef _Traits traits_type;
	typedef typename traits_type::int_type int_type;
	typedef typename traits_type::pos_type pos_type;
	typedef typename traits_type::off_type off_type;

	typedef basic_streambuf<char_type, traits_type> __streambuf_type;
	typedef std::/*experimental::*/span<char_type> __span_type;
	typedef typename __span_type::size_type __size_type;

protected:
	/// Place to stash in || out || in | out settings for current spanbuf.
	ios_base::openmode _M_mode;

	// Data Members:
	__span_type _M_string; // always dynamic Extent

public:
	// Constructors:
	/**
	 *  @brief  Starts with an existing span buffer.
	 *  @param  __str  A span to use as buffer (reference semantics! we do not own the buffer)
	 *  @param  __mode  Whether the buffer can read, or write, or both.
	 *
	 *  This constructor initializes the parent class using its
	 *  own default ctor.
	 */
	explicit basic_spanbuf(__span_type __str, ios_base::openmode __mode = ios_base::in | ios_base::out) :
			__streambuf_type { }, _M_mode { }, _M_string { __str.data(), __str.size() } // TODO: could use templated copy-ctor
	{
		_M_spanbuf_init(__mode);
	}

	basic_spanbuf(basic_spanbuf const &__rhs)
	:__streambuf_type{}
	, _M_mode{__rhs._M_mode}
	,_M_string{__rhs._M_string}{
		_M_spanbuf_init(_M_mode);
	}


	// TODO: could move-enable the spanbuf but it is dangerous, because of the 
	// non-managed memory area.
	basic_spanbuf(basic_spanbuf&& __rhs) noexcept 
	:__streambuf_type { std::move(__rhs) } // might actually copy
	, _M_mode{std::move(__rhs._M_mode)}
	,_M_string{std::move(__rhs._M_string)}
	{
		// since we do not own any resource and destroying or re-assigning does not have a side-effect all is fine
	}
	
	// 27.x.2.2 Assign and swap:

	basic_spanbuf&
	operator=(basic_spanbuf const &) = delete;

	basic_spanbuf&
	operator=(basic_spanbuf&& __rhs) noexcept {
		this->swap(__rhs);
		return *this;
	}
    // do we need that elaborated swap?
	void swap(basic_spanbuf& __rhs) noexcept {
		__xfer_bufptrs const __l_st {*this, std::__addressof(__rhs)};
		__xfer_bufptrs const __r_st {__rhs, this};
		__streambuf_type& __base = __rhs;
		__streambuf_type::swap(__base);
		__rhs.pubimbue(this->pubimbue(__rhs.getloc()));
		std::swap(_M_mode, __rhs._M_mode);
		std::swap(_M_string, __rhs._M_string);
	}

	// Get and set:
	/**
	 *  @brief  Access to the underlying sequence.
	 *  @return  A span of one of the underlying sequences.
	 *
	 *  <em>If the buffer is only created in input mode, the underlying
	 *  character sequence is equal to the input sequence; otherwise, it
	 *  is equal to the output sequence.</em> [27.X.1.2]/1
	 */

	__span_type span() const {
		if (this->pptr() && (this->pptr() >= this->egptr()) ) { // output only
				return __span_type(this->pbase(), this->pptr() - this->pbase());
		}
		return _M_string; // return the whole span
	}
	/**
	 *  @brief  Setting a new buffer.
	 *  @param  __s  The span to use as a new sequence.
	 *
	 *  copies @a s to
	 *  use as a new one.
	 */
	void span(__span_type __s) {
		_M_string = __span_type{ __s.data(), __s.size() };
		_M_spanbuf_init(_M_mode);
	}
protected:
	// Common initialization code goes here.
	void _M_spanbuf_init(ios_base::openmode __mode) {
		_M_mode = __mode;
		__size_type  __len = 0;
		if (_M_mode & (ios_base::ate | ios_base::app)) // doesn't make much sense, we can not grow span!
			__len = _M_string.size();
		_M_sync(_M_string.data(), 0, __len);
	}

	virtual streamsize showmanyc() {
		streamsize __ret = -1;
		if (_M_mode & ios_base::in) {
			_M_update_egptr();
			__ret = this->egptr() - this->gptr();
		}
		return __ret;
	}

#if 0 // defaults should be ok
	virtual int_type underflow() {
		int_type __ret = traits_type::eof();
//		bool const __testin = this->_M_mode & ios_base::in;
//		if (__testin) {
//			// Update egptr() to match the actual span end.
//			_M_update_egptr();
//
//			if (this->gptr() < this->egptr())
//				__ret = traits_type::to_int_type(*this->gptr());
//		}
		return __ret;
	}

	virtual int_type pbackfail(int_type __c = traits_type::eof()) {
		int_type __ret = traits_type::eof();
		if (this->eback() < this->gptr()) {
			// Try to put back __c into input sequence in one of three ways.
			// Order these tests done in is unspecified by the standard.
			bool const __testeof = traits_type::eq_int_type(__c, __ret);
			if (!__testeof) {
				bool const __testeq = traits_type::eq(traits_type::to_char_type(__c), this->gptr()[-1]);
				bool const __testout = this->_M_mode & ios_base::out;
				if (__testeq || __testout) {
					this->gbump(-1);
					if (!__testeq)
						*this->gptr() = traits_type::to_char_type(__c);
					__ret = __c;
				}
			} else {
				this->gbump(-1);
				__ret = traits_type::not_eof(__c);
			}
		}
		return __ret;
	}

	virtual int_type overflow(int_type __c = traits_type::eof()) {
		bool const __testout = this->_M_mode & ios_base::out;
		if (!__testout)
			return traits_type::eof();

		bool const __testeof = traits_type::eq_int_type(__c, traits_type::eof());
		if (__testeof)
			return traits_type::not_eof(__c);

		bool const __testput = this->pptr() < this->epptr();
		if (!__testput)
			return traits_type::eof();
		// we might never get here, because usually overflow will only be called when !__testput
		// Try to append __c into output sequence in one of two ways.
		// Order these tests done in is unspecified by the standard.
		char_type const __conv = traits_type::to_char_type(__c);

		*this->pptr() = __conv;
		this->pbump(1);
		return __c;
	}
#endif
	/**
	 *  @brief  Manipulates the buffer.
	 *  @param  __s  Pointer to a buffer area.
	 *  @param  __n  Size of @a __s.
	 *  @return  @c this
	 *
	 *  This is similar to 
	 */
	virtual __streambuf_type* 
	setbuf(char_type* __s, streamsize __n)
	{
		if (__s && __n >= 0)
		{
			this->span( __span_type{__s,static_cast<__span_type::size_type>(__n)});
		}
		return this;
	}

	virtual pos_type
	seekoff(off_type __off, ios_base::seekdir __way,
			ios_base::openmode __mode = ios_base::in | ios_base::out) {

		pos_type __ret = pos_type(off_type(-1));
		bool __testin = (ios_base::in & this->_M_mode & __mode) != 0;
		bool __testout = (ios_base::out & this->_M_mode & __mode) != 0;
		bool const __testboth {__testin && __testout && __way != ios_base::cur};
		__testin &= !(__mode & ios_base::out);
		__testout &= !(__mode & ios_base::in);

		char_type const *const __beg = __testin ? this->eback() : this->pbase();
		if ((__beg || !__off) && (__testin || __testout || __testboth))
		{
			_M_update_egptr();

			off_type __newoffi = __off;
			off_type __newoffo = __newoffi;
			if (__way == ios_base::cur)
			{
				__newoffi += this->gptr() - __beg;
				__newoffo += this->pptr() - __beg;
			}
			else if (__way == ios_base::end)
			__newoffo = __newoffi += this->egptr() - __beg;

			if ((__testin || __testboth)
					&& __newoffi >= 0
					&& this->egptr() - __beg >= __newoffi)
			{
				this->setg(this->eback(), this->eback() + __newoffi,
						this->egptr());
				__ret = pos_type(__newoffi);
			}
			if ((__testout || __testboth)
					&& __newoffo >= 0
					&& this->egptr() - __beg >= __newoffo)
			{
				_M_pbump(this->pbase(), this->epptr(), __newoffo);
				__ret = pos_type(__newoffo);
			}
		}
		return __ret;

	}

	virtual pos_type
	seekpos(pos_type __sp,
			ios_base::openmode __mode = ios_base::in | ios_base::out) {

		pos_type __ret = pos_type(off_type(-1));
		bool const __testin = (ios_base::in & this->_M_mode & __mode) != 0;
		bool const __testout = (ios_base::out & this->_M_mode & __mode) != 0;

		char_type const *const __beg = __testin ? this->eback() : this->pbase();
		if ((__beg || !off_type(__sp)) && (__testin || __testout))
		{
			_M_update_egptr();

			off_type const __pos(__sp);
			bool const __testpos = (0 <= __pos
					&& __pos <= this->egptr() - __beg);
			if (__testpos)
			{
				if (__testin)
				this->setg(this->eback(), this->eback() + __pos,
						this->egptr());
				if (__testout)
				_M_pbump(this->pbase(), this->epptr(), __pos);
				__ret = __sp;
			}
		}
		return __ret;

	}

	// Internal function for correctly updating the internal buffer
	// for a particular _M_string, due to initialization or re-sizing
	// of an existing _M_string.
	void
	_M_sync(char_type* __base, __size_type __i, __size_type __o) {

		bool const __testin = _M_mode & ios_base::in;
		bool const __testout = _M_mode & ios_base::out;
		char_type* __endg = __base + _M_string.size();
		char_type* __endp = __base + _M_string.size();

		if (__base != _M_string.data())
		{
			// setbuf: __i == size of buffer area (_M_string.size() == 0).
			__endg += __i;
			__i = 0;
			__endp = __endg;
		}

		if (__testin)
		this->setg(__base, __base + __i, __endg);
		if (__testout)
		{
			_M_pbump(__base, __endp, __o);
			if (!__testin)
				this->setg(__base, __base, __base); // nothing to read
		}

	}

	// Internal function for correctly updating egptr() to the actual
	// span end.
	void
	_M_update_egptr()
	{
		bool const __testin = _M_mode & ios_base::in;
		if (this->pptr() && this->pptr() > this->egptr())
		{
			if (__testin)
			this->setg(this->eback(), this->gptr(), this->pptr());
			else
			this->setg(this->pptr(), this->pptr(), this->pptr());
		}
	}

	// Works around the issue with pbump, part of the protected
	// interface of basic_streambuf, taking just an int.
	void
	_M_pbump(char_type* __pbeg, char_type* __pend, off_type __off) {

		this->setp(__pbeg, __pend);
		while (__off > __gnu_cxx::__numeric_traits<int>::__max)
		{
			this->pbump(__gnu_cxx::__numeric_traits<int>::__max);
			__off -= __gnu_cxx::__numeric_traits<int>::__max;
		}
		this->pbump(__off);

	}

private:
	// This type captures the state of the gptr / pptr pointers as offsets
	// so they can be restored in another object after moving the span.
	struct __xfer_bufptrs
	{
		__xfer_bufptrs(basic_spanbuf const & __from, basic_spanbuf* __to)
		: _M_to {__to}, _M_goff {-1, -1, -1}, _M_poff {-1, -1, -1}
		{
			_CharT const * __str = __from._M_string.data();
			if (__from.eback())
			{
				_M_goff[0] = __from.eback() - __str;
				_M_goff[1] = __from.gptr() - __str;
				_M_goff[2] = __from.egptr() - __str;
			}
			if (__from.pbase())
			{
				_M_poff[0] = __from.pbase() - __str;
				_M_poff[1] = __from.pptr() - __from.pbase();
				_M_poff[2] = __from.epptr() - __str;
			}
		}

		~__xfer_bufptrs()
		{
			char_type* __str = const_cast<char_type*>(_M_to->_M_string.data());
			if (_M_goff[0] != -1)
			_M_to->setg(__str+_M_goff[0], __str+_M_goff[1], __str+_M_goff[2]);
			if (_M_poff[0] != -1)
			_M_to->_M_pbump(__str+_M_poff[0], __str+_M_poff[2], _M_poff[1]);
		}

		basic_spanbuf* _M_to;
		off_type _M_goff[3];
		off_type _M_poff[3];
	};

	// The move constructor initializes an __xfer_bufptrs temporary then
	// delegates to this constructor to performs moves during its lifetime.
	basic_spanbuf(basic_spanbuf&& __rhs, __xfer_bufptrs&&)
	: __streambuf_type(static_cast<__streambuf_type const &>(__rhs)),
	_M_mode(__rhs._M_mode), _M_string(std::move(__rhs._M_string))
	{}
};

// [27.7.2] Template class basic_ispanstream
/**
 *  @brief  Controlling input for std::span.
 *  @ingroup io
 *
 *  @tparam _CharT  Type of character stream.
 *  @tparam _Traits  Traits for character type, defaults to
 *                   char_traits<_CharT>.
 *
 *  This class supports reading from objects of type std::basic_span,
 *  using the inherited functions from std::basic_istream.  To control
 *  the associated sequence, an instance of std::basic_spanbuf is used,
 *  which this page refers to as @c sb.
 */
template<typename _CharT, typename _Traits>
class basic_ispanstream: public basic_istream<_CharT, _Traits> {
public:
	// Types:
	typedef _CharT char_type;
	typedef _Traits traits_type;
	typedef typename traits_type::int_type int_type;
	typedef typename traits_type::pos_type pos_type;
	typedef typename traits_type::off_type off_type;

	// Non-standard types:
	typedef basic_spanbuf<_CharT, _Traits> __spanbuf_type;
	typedef basic_istream<char_type, traits_type> __istream_type;
	typedef std::/*experimental::*/span<char_type> __span_type;
	using   __cspan=std::/*experimental::*/span<_CharT const>;
	typedef typename __span_type::size_type __size_type;

private:
	__spanbuf_type _M_stringbuf;

public:
	// Constructors:
	/**
	 *  @brief  Starts with an existing span buffer.
	 *  @param  __str  A span to copy as a starting buffer.
	 *  @param  __mode  Whether the buffer can read, or write, or both.
	 *
	 *  @c ios_base::in is automatically included in @a mode.
	 *
	 *  Initializes @c sb using @a str and @c mode|in, and passes @c &sb
	 *  to the base class initializer.
	 *
	 *  That's a lie.  We initialize the base class with NULL, because the
	 *  span class does its own memory management.
	 */
	explicit basic_ispanstream(__span_type  __str, ios_base::openmode __mode = ios_base::in) :
			__istream_type { }, _M_stringbuf { __str, __mode | ios_base::in } {
		this->init(&_M_stringbuf);
	}
			template<std::ranges::borrowed_range R>
			requires (not std::convertible_to<R, std::span<_CharT>>) &&
			std::convertible_to<R, __cspan>
			explicit basic_ispanstream(R&& __rr):
			basic_ispanstream(__span_type{const_cast<char_type *>(std::data(__rr)),std::size(__rr)}){}
//			basic_ispanstream(__span_type{const_cast<char_type *>(__rr.data()),__rr.size()}){}

//    explicit basic_ispanstream(__cspan  __str) :
//    		basic_ispanstream(__span_type{const_cast<char_type *>(__str.data()),__str.size()}){}

	/**
	 *  @brief  The destructor does nothing.
	 *
	 *  The buffer is deallocated by the stringbuf object, not the
	 *  formatting stream.
	 */
	~basic_ispanstream() {
	}

	basic_ispanstream(basic_ispanstream const &) = delete;

	basic_ispanstream(basic_ispanstream&& __rhs) = delete;
//      : __istream_type(std::move(__rhs)),
//      _M_stringbuf(std::move(__rhs._M_stringbuf))
//      { __istream_type::set_rdbuf(&_M_stringbuf); }

	// 27.8.3.2 Assign and swap:

	basic_ispanstream&
	operator=(basic_ispanstream const &) = delete;

	basic_ispanstream&
	operator=(basic_ispanstream&& __rhs) = delete;
//      {
//	__istream_type::operator=(std::move(__rhs));
//	_M_stringbuf = std::move(__rhs._M_stringbuf);
//	return *this;
//      }

	void swap(basic_ispanstream& __rhs) {
		__istream_type::swap(__rhs);
		_M_stringbuf.swap(__rhs._M_stringbuf);
	}

	// Members:
	/**
	 *  @brief  Accessing the underlying buffer.
	 *  @return  The current basic_spanbuf buffer.
	 *
	 *  This hides both signatures of std::basic_ios::rdbuf().
	 */
	__spanbuf_type* rdbuf() const
	{	return const_cast<__spanbuf_type*>(&_M_stringbuf);}

	/**
	 *  @brief  Access the underlying buffer.
	 *  @return  @c rdbuf()->span()
	 */
	__cspan
	span() const
	{	return  _M_stringbuf.span(); }
	/**
	 *  @brief  Setting a new buffer.
	 *  @param  __s  The span to use as a new sequence.
	 *
	 *  Calls @c rdbuf()->span(s).
	 */
	void
	span(__span_type __s)
	{
		_M_stringbuf.span(__s);
	}
	template<std::ranges::borrowed_range ROS>
	requires (not std::convertible_to<ROS, std::span<_CharT>>) &&	std::convertible_to<ROS, __cspan>
	void
	span(ROS&& __s)
	{
		_M_stringbuf.span(__span_type{const_cast<char_type *>(ranges::data(__s)),ranges::size(__s)});
	}
};

// [27.7.3] Template class basic_ospanstream
/**
 *  @brief  Controlling output for std::span.
 *  @ingroup io
 *
 *  @tparam _CharT  Type of character stream.
 *  @tparam _Traits  Traits for character type, defaults to
 *                   char_traits<_CharT>.
 *
 *  This class supports writing to objects of type std::basic_span,
 *  using the inherited functions from std::basic_ostream.  To control
 *  the associated sequence, an instance of std::basic_spanbuf is used,
 *  which this page refers to as @c sb.
 */
template<typename _CharT, typename _Traits>
class basic_ospanstream: public basic_ostream<_CharT, _Traits> {
public:
	// Types:
	typedef _CharT char_type;
	typedef _Traits traits_type;
	typedef typename traits_type::int_type int_type;
	typedef typename traits_type::pos_type pos_type;
	typedef typename traits_type::off_type off_type;

	// Non-standard types:
	typedef basic_spanbuf<_CharT, _Traits> __spanbuf_type;
	typedef basic_ostream<char_type, traits_type> __ostream_type;
	typedef std::/*experimental::*/span<char_type> __span_type;
	typedef typename __span_type::size_type __size_type;

private:
	__spanbuf_type _M_stringbuf;

public:
	// Constructors/destructor:
	/**
	 *  @brief  Starts with an existing span buffer.
	 *  @param  __str  A span to copy as a starting buffer.
	 *  @param  __mode  Whether the buffer can read, or write, or both.
	 *
	 *  @c ios_base::out is automatically included in @a mode.
	 *
	 *  Initializes @c sb using @a str and @c mode|out, and passes @c &sb
	 *  to the base class initializer.
	 *
	 *  That's a lie.  We initialize the base class with NULL, because the
	 *  span class does its own memory management.
	 */
	explicit basic_ospanstream(__span_type __str, ios_base::openmode __mode = ios_base::out) :
			__ostream_type { }, _M_stringbuf { __str, __mode | ios_base::out } {
		this->init(&_M_stringbuf);
	}

	/**
	 *  @brief  The destructor does nothing.
	 *
	 */
	~basic_ospanstream() {
	}

	basic_ospanstream(basic_ospanstream const &) = delete;

	basic_ospanstream(basic_ospanstream&& __rhs) = delete;
//      : __ostream_type(std::move(__rhs)),
//      _M_stringbuf(std::move(__rhs._M_stringbuf))
//      { __ostream_type::set_rdbuf(&_M_stringbuf); }

	// 27.8.3.2 Assign and swap:

	basic_ospanstream&
	operator=(basic_ospanstream const &) = delete;

	basic_ospanstream&
	operator=(basic_ospanstream&& __rhs) = delete;
//      {
//	__ostream_type::operator=(std::move(__rhs));
//	_M_stringbuf = std::move(__rhs._M_stringbuf);
//	return *this;
//      }

	void swap(basic_ospanstream& __rhs) {
		__ostream_type::swap(__rhs);
		_M_stringbuf.swap(__rhs._M_stringbuf);
	}

	// Members:
	/**
	 *  @brief  Accessing the underlying buffer.
	 *  @return  The current basic_spanbuf buffer.
	 *
	 *  This hides both signatures of std::basic_ios::rdbuf().
	 */
	__spanbuf_type* rdbuf() const
	{	return const_cast<__spanbuf_type*>(&_M_stringbuf);}

	/**
	 *  @brief  Access the underlying buffer.
	 *  @return  @c rdbuf()->span()
	 */
	__span_type
	span() const
	{	return _M_stringbuf.span();}
	/**
	 *  @brief  Setting a new buffer.
	 *  @param  __s  The span to use as a new sequence.
	 *
	 *  Calls @c rdbuf()->span(s).
	 */
	void
	span(__span_type __s)
	{
		_M_stringbuf.span(__s);
	}

};

// [27.7.4] Template class basic_spanstream
/**
 *  @brief  Controlling input and output for std::span.
 *  @ingroup io
 *
 *  @tparam _CharT  Type of character stream.
 *  @tparam _Traits  Traits for character type, defaults to
 *                   char_traits<_CharT>.
 *
 *  This class supports reading from and writing to buffers represented by
 *  std::span<_CharT>, using the inherited functions from
 *  std::basic_iostream.  To control the associated sequence, an instance
 *  of std::basic_spanbuf is used, which this page refers to as @c sb.
 */
template<typename _CharT, typename _Traits>
class basic_spanstream: public basic_iostream<_CharT, _Traits> {
public:
	// Types:
	typedef _CharT char_type;
	typedef _Traits traits_type;
	// _GLIBCXX_RESOLVE_LIB_DEFECTS
	// 251. basic_spanbuf missing allocator_type
	typedef typename traits_type::int_type int_type;
	typedef typename traits_type::pos_type pos_type;
	typedef typename traits_type::off_type off_type;

	// Non-standard Types:
	typedef basic_spanbuf<_CharT, _Traits> __stringbuf_type;
	typedef basic_iostream<char_type, traits_type> __iostream_type;
	typedef std::/*experimental::*/span<char_type> __span_type;
	typedef typename __span_type::size_type __size_type;

private:
	__stringbuf_type _M_stringbuf;

public:
	// Constructors/destructors

	/**
	 *  @brief  Starts with an existing span buffer.
	 *  @param  __str  A span used as a starting buffer.
	 *  @param  __m  Whether the buffer can read, or write, or both.
	 *
	 *  Initializes @c sb using @a __str and @c __m, and passes @c &sb
	 *  to the base class initializer.
	 *
	 */
	explicit basic_spanstream(__span_type __str, ios_base::openmode __m = ios_base::out | ios_base::in) :
			__iostream_type { }, _M_stringbuf { __str, __m } {
		this->init(&_M_stringbuf);
	}

	/**
	 *  @brief  The destructor does nothing.
	 *
	 *  The buffer is deallocated by the stringbuf object, not the
	 *  formatting stream.
	 */
	~basic_spanstream() =default;

	basic_spanstream(basic_spanstream const &) = delete;

	basic_spanstream(basic_spanstream&& __rhs) = delete;
//      : __iostream_type(std::move(__rhs)),
//      _M_stringbuf(std::move(__rhs._M_stringbuf))
//      { __iostream_type::set_rdbuf(&_M_stringbuf); }

	// 27.8.3.2 Assign and swap:

	basic_spanstream&
	operator=(basic_spanstream const &) = delete;

	basic_spanstream&
	operator=(basic_spanstream&& __rhs) = delete;
//      {
//	__iostream_type::operator=(std::move(__rhs));
//	_M_stringbuf = std::move(__rhs._M_stringbuf);
//	return *this;
//      }

	void swap(basic_spanstream& __rhs) {
		__iostream_type::swap(__rhs);
		_M_stringbuf.swap(__rhs._M_stringbuf);
	}

	// Members:
	/**
	 *  @brief  Accessing the underlying buffer.
	 *  @return  The current basic_spanbuf buffer.
	 *
	 *  This hides both signatures of std::basic_ios::rdbuf().
	 */
	__stringbuf_type* rdbuf() const
	{	return const_cast<__stringbuf_type*>(&_M_stringbuf);}

	/**
	 *  @brief  Direct access to the buffer.
	 *  @return  @c rdbuf()->span()
	 */
	__span_type
	span() const
	{	return _M_stringbuf.span();}
	/**
	 *  @brief  Setting a new buffer.
	 *  @param  __s  The span to use as a new sequence.
	 *
	 *  Calls @c rdbuf()->span(s).
	 */
	void
	span(__span_type __s)
	{
		_M_stringbuf.span(__s);
	}
};

/// Swap specialization for spanbufs.
template<class _CharT, class _Traits>
inline void swap(basic_spanbuf<_CharT, _Traits>& __x, basic_spanbuf<_CharT, _Traits>& __y) noexcept {
	__x.swap(__y);
}

/// Swap specialization for ispanstreams.
template<class _CharT, class _Traits>
inline void swap(basic_ispanstream<_CharT, _Traits>& __x, basic_ispanstream<_CharT, _Traits>& __y) noexcept {
	__x.swap(__y);
}

/// Swap specialization for ospanstreams.
template<class _CharT, class _Traits>
inline void swap(basic_ospanstream<_CharT, _Traits>& __x, basic_ospanstream<_CharT, _Traits>& __y) noexcept {
	__x.swap(__y);
}

/// Swap specialization for spanstreams.
template<class _CharT, class _Traits>
inline void swap(basic_spanstream<_CharT, _Traits>& __x, basic_spanstream<_CharT, _Traits>& __y) noexcept {
	__x.swap(__y);
}

// Inhibit implicit instantiations for required instantiations,
// which are defined via explicit instantiations elsewhere (spanstream.cpp)

extern template class basic_spanbuf<char> ;
extern template class basic_ispanstream<char> ;
extern template class basic_ospanstream<char> ;
extern template class basic_spanstream<char> ;

extern template class basic_spanbuf<wchar_t> ;
extern template class basic_ispanstream<wchar_t> ;
extern template class basic_ospanstream<wchar_t> ;
extern template class basic_spanstream<wchar_t> ;

}
} // namespace std

#endif /* SRC_SPANSTREAM_H_ */
