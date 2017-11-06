#include "bitset.h"
#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"
namespace std {
namespace __detail {
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   * @brief Struct for delimited strings.
   */
  template<typename _String, typename _CharT>
    struct _MyQuoted_string
    {
//	static_assert(is_reference<_String>::value
//		   || is_pointer<_String>::value,
//		      "String type must be pointer or reference");

	_MyQuoted_string(_String __str, _CharT __del, _CharT __esc)
	: _M_string(__str), _M_delim{__del}, _M_escape{__esc}
	{ }

	_MyQuoted_string&
	operator=(_MyQuoted_string&) = delete;

	_String _M_string;
	_CharT _M_delim;
	_CharT _M_escape;
    };

template<typename _CharT, typename _Traits, typename _String>
  std::basic_ostream<_CharT, _Traits>&
  operator<<(std::basic_ostream<_CharT, _Traits>& __os,
	 const _MyQuoted_string<_String, _CharT>& __str)
  {
std::basic_ostringstream<_CharT, _Traits> __ostr;
__ostr << __str._M_delim;
for (auto& __c : __str._M_string)
  {
    if (__c == __str._M_delim || __c == __str._M_escape)
      __ostr << __str._M_escape;
    __ostr << __c;
  }
__ostr << __str._M_delim;

return __os << __ostr.str();
  }

}

//template<typename _CharT, typename _Traits=char_traits<_CharT>>
//  inline auto
//  quotedsv( basic_string_view<_CharT, _Traits> __string,
//	   _CharT __delim = _CharT('"'), _CharT __escape = _CharT('\\'))
//  {
//    return __detail::_MyQuoted_string<
//			 basic_string_view<_CharT, _Traits>, _CharT>(
//				__string, __delim, __escape);
//  }
//template auto quotedsv<char,char_traits<char>>(string_view,char,char);
//template auto quotedsv<wchar_t,char_traits<wchar_t>>(wstring_view,wchar_t,wchar_t);
inline auto
quotedsv( string_view __string,
	   char __delim = char('"'), char __escape = char('\\'))
{
  return __detail::_MyQuoted_string<
			 string_view, char>(
				__string, __delim, __escape);
}
inline auto
quotedsv( wstring_view __string,
	   wchar_t __delim = wchar_t('"'), wchar_t __escape = wchar_t('\\'))
{
  return __detail::_MyQuoted_string<
			 wstring_view, wchar_t>(
				__string, __delim, __escape);
}
//  template<typename _CharT>
//    inline auto
//    quotedsv(const _CharT* __string,
//	   _CharT __delim = _CharT('"'), _CharT __escape = _CharT('\\'))
//    {
//      return __detail::_MyQuoted_string<basic_string_view<_CharT>
//      	  	  	  ,_CharT>(__string, __delim,
//							     __escape);
//    }
//  template<typename _CharT, typename _Traits, typename _Alloc>
//    inline auto
//    quotedsv(const basic_string<_CharT, _Traits, _Alloc>& __string,
//	   _CharT __delim = _CharT('"'), _CharT __escape = _CharT('\\'))
//    {
//      return __detail::_MyQuoted_string<
//			const basic_string<_CharT, _Traits, _Alloc>&, _CharT>(
//				__string, __delim, __escape);
//    }

}

	using namespace std::literals;
	void testQuotedSV(){
		std::ostringstream os{};
		os << std::quotedsv("hello world"sv,' ',':');
		ASSERT_EQUAL(" hello: world ",os.str());
	}
	void testQuotedSVcharPtr(){
		std::ostringstream os{};
		os << std::quotedsv("hello world",' ','.');
		ASSERT_EQUAL(" hello. world ",os.str());
	}
	void testQuotedSVstring(){
		std::ostringstream os{};
		os << std::quotedsv("hello world"s,' ','.');
		ASSERT_EQUAL(" hello. world ",os.str());
	}

	void testWQuotedSV(){
		std::wostringstream os{};
		os << std::quotedsv(L"hello world"sv,L' ',L':');
		ASSERT_EQUAL(L" hello: world ",os.str());
	}
	void testWQuotedSVcharPtr(){
		std::wostringstream os{};
		os << std::quotedsv(L"hello world",L' ',L'.');
		ASSERT_EQUAL(L" hello. world ",os.str());
	}
	void testWQuotedSVstring(){
		std::wostringstream os{};
		os << std::quotedsv(L"hello world"s,L' ',L'.');
		ASSERT_EQUAL(L" hello. world ",os.str());
	}


void thisIsATest() {
	std::ext::bitset<32> b{"01010101"sv};
	std::ext::bitset<32> c("01010101"sv);
	ASSERT_EQUAL(std::ext::bitset<32>{},(b^c));
}

bool runAllTests(int argc, char const *argv[]) {
	cute::suite s { };
	//TODO add your test here
	s.push_back(CUTE(thisIsATest));
	s.push_back(CUTE(testQuotedSV));
	s.push_back(CUTE(testQuotedSVcharPtr));
	s.push_back(CUTE(testQuotedSVstring));
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
	auto runner = cute::makeRunner(lis, argc, argv);
	bool success = runner(s, "AllTests");
	return success;
}

int main(int argc, char const *argv[]) {
    return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
