
#ifndef _PETER_RATIO
#define _PETER_RATIO 1
// prevent other libs including <ratio>
#define _LIBCPP_RATIO
#define _GLIBCXX_RATIO 1


#include <type_traits>
#include <cstdint>

// a naive C++17 ratio implentation using variable templates whereever possible

namespace std//_peter
{
//using namespace std;
  /**
   * @defgroup ratio Rational Arithmetic
   * @ingroup utilities
   *
   * Compile time representation of finite rational numbers.
   * @{
   */

  template<intmax_t _Pn>
    constexpr intmax_t __static_sign{
	  	  (_Pn < 0) ? -1 : 1
    };

  template<intmax_t _Pn>
    constexpr intmax_t  __static_abs{
     _Pn * __static_sign<_Pn>
     };

  template<intmax_t _Pn, intmax_t _Qn>
    constexpr intmax_t __static_gcd{
     __static_gcd<_Qn, (_Pn % _Qn)>
     };

  template<intmax_t _Pn>
    constexpr intmax_t __static_gcd<_Pn, 0>{
           __static_abs<_Pn>
    };

  template<intmax_t _Qn>
    constexpr intmax_t __static_gcd<0, _Qn>{
       __static_abs<_Qn>
    };

  // Let c = 2^(half # of bits in an intmax_t)
  // then we find a1, a0, b1, b0 s.t. N = a1*c + a0, M = b1*c + b0
  // The multiplication of N and M becomes,
  // N * M = (a1 * b1)c^2 + (a0 * b1 + b0 * a1)c + a0 * b0
  // Multiplication is safe if each term and the sum of the terms
  // is representable by intmax_t.
//  template<intmax_t _Pn, intmax_t _Qn>
//    struct __safe_multiply
//    {
//    private:
//      static const uintmax_t __c = uintmax_t(1) << (sizeof(intmax_t) * 4);
//
//      static const uintmax_t __a0 = __static_abs<_Pn> % __c;
//      static const uintmax_t __a1 = __static_abs<_Pn> / __c;
//      static const uintmax_t __b0 = __static_abs<_Qn> % __c;
//      static const uintmax_t __b1 = __static_abs<_Qn> / __c;
//
//      static_assert(__a1 == 0 || __b1 == 0,
//		    "overflow in multiplication");
//      static_assert(__a0 * __b1 + __b0 * __a1 < (__c >> 1),
//		    "overflow in multiplication");
//      static_assert(__b0 * __a0 <= __INTMAX_MAX__,
//		    "overflow in multiplication");
//      static_assert((__a0 * __b1 + __b0 * __a1) * __c
//		    <= __INTMAX_MAX__ -  __b0 * __a0,
//		    "overflow in multiplication");
//
//    public:
//      static const intmax_t value = _Pn * _Qn;
//    };
//

    template<intmax_t _Pn, intmax_t _Qn>
      constexpr intmax_t __multiply{ _Pn*_Qn};

  // Some double-precision utilities, where numbers are represented as
  // __hi*2^(8*sizeof(uintmax_t)) + __lo.
  template<uintmax_t __hi1, uintmax_t __lo1, uintmax_t __hi2, uintmax_t __lo2>
    struct __big_less
    : integral_constant<bool, (__hi1 < __hi2
			       || (__hi1 == __hi2 && __lo1 < __lo2))>
    { };

  template<uintmax_t __hi1, uintmax_t __lo1, uintmax_t __hi2, uintmax_t __lo2>
    struct __big_add
    {
      static constexpr uintmax_t __lo = __lo1 + __lo2;
      static constexpr uintmax_t __hi = (__hi1 + __hi2 +
					 (__lo1 + __lo2 < __lo1)); // carry
    };

  // Subtract a number from a bigger one.
  template<uintmax_t __hi1, uintmax_t __lo1, uintmax_t __hi2, uintmax_t __lo2>
    struct __big_sub
    {
      static_assert(!__big_less<__hi1, __lo1, __hi2, __lo2>::value,
		    "Internal library error");
      static constexpr uintmax_t __lo = __lo1 - __lo2;
      static constexpr uintmax_t __hi = (__hi1 - __hi2 -
					 (__lo1 < __lo2)); // carry
    };

  // Same principle as __safe_multiply.
  template<uintmax_t __x, uintmax_t __y>
    struct __big_mul
    {
    private:
      static constexpr uintmax_t __c = uintmax_t(1) << (sizeof(intmax_t) * 4);
      static constexpr uintmax_t __x0 = __x % __c;
      static constexpr uintmax_t __x1 = __x / __c;
      static constexpr uintmax_t __y0 = __y % __c;
      static constexpr uintmax_t __y1 = __y / __c;
      static constexpr uintmax_t __x0y0 = __x0 * __y0;
      static constexpr uintmax_t __x0y1 = __x0 * __y1;
      static constexpr uintmax_t __x1y0 = __x1 * __y0;
      static constexpr uintmax_t __x1y1 = __x1 * __y1;
      static constexpr uintmax_t __mix = __x0y1 + __x1y0; // possible carry...
      static constexpr uintmax_t __mix_lo = __mix * __c;
      static constexpr uintmax_t __mix_hi
      = __mix / __c + ((__mix < __x0y1) ? __c : 0); // ... added here
      typedef __big_add<__mix_hi, __mix_lo, __x1y1, __x0y0> _Res;
    public:
      static constexpr uintmax_t __hi = _Res::__hi;
      static constexpr uintmax_t __lo = _Res::__lo;
    };

  // Adapted from __udiv_qrnnd_c in longlong.h
  // This version assumes that the high bit of __d is 1.
  template<uintmax_t __n1, uintmax_t __n0, uintmax_t __d>
    struct __big_div_impl
    {
    private:
      static_assert(__d >= (uintmax_t(1) << (sizeof(intmax_t) * 8 - 1)),
		    "Internal library error");
      static_assert(__n1 < __d, "Internal library error");
      static constexpr uintmax_t __c = uintmax_t(1) << (sizeof(intmax_t) * 4);
      static constexpr uintmax_t __d1 = __d / __c;
      static constexpr uintmax_t __d0 = __d % __c;

      static constexpr uintmax_t __q1x = __n1 / __d1;
      static constexpr uintmax_t __r1x = __n1 % __d1;
      static constexpr uintmax_t __m = __q1x * __d0;
      static constexpr uintmax_t __r1y = __r1x * __c + __n0 / __c;
      static constexpr uintmax_t __r1z = __r1y + __d;
      static constexpr uintmax_t __r1
      = ((__r1y < __m) ? ((__r1z >= __d) && (__r1z < __m))
	 ? (__r1z + __d) : __r1z : __r1y) - __m;
      static constexpr uintmax_t __q1
      = __q1x - ((__r1y < __m)
		 ? ((__r1z >= __d) && (__r1z < __m)) ? 2 : 1 : 0);
      static constexpr uintmax_t __q0x = __r1 / __d1;
      static constexpr uintmax_t __r0x = __r1 % __d1;
      static constexpr uintmax_t __n = __q0x * __d0;
      static constexpr uintmax_t __r0y = __r0x * __c + __n0 % __c;
      static constexpr uintmax_t __r0z = __r0y + __d;
      static constexpr uintmax_t __r0
      = ((__r0y < __n) ? ((__r0z >= __d) && (__r0z < __n))
	 ? (__r0z + __d) : __r0z : __r0y) - __n;
      static constexpr uintmax_t __q0
      = __q0x - ((__r0y < __n) ? ((__r0z >= __d)
				  && (__r0z < __n)) ? 2 : 1 : 0);

    public:
      static constexpr uintmax_t __quot = __q1 * __c + __q0;
      static constexpr uintmax_t __rem = __r0;

    private:
      typedef __big_mul<__quot, __d> _Prod;
      typedef __big_add<_Prod::__hi, _Prod::__lo, 0, __rem> _Sum;
      static_assert(_Sum::__hi == __n1 && _Sum::__lo == __n0,
		    "Internal library error");
  };

  template<uintmax_t __n1, uintmax_t __n0, uintmax_t __d>
    struct __big_div
    {
    private:
      static_assert(__d != 0, "Internal library error");
      static_assert(sizeof (uintmax_t) == sizeof (unsigned long long),
		    "This library calls __builtin_clzll on uintmax_t, which "
		    "is unsafe on your platform. Please complain to "
		    "http://gcc.gnu.org/bugzilla/");
      static constexpr int __shift = __builtin_clzll(__d);
      static constexpr int __coshift_ = sizeof(uintmax_t) * 8 - __shift;
      static constexpr int __coshift = (__shift != 0) ? __coshift_ : 0;
      static constexpr uintmax_t __c1 = uintmax_t(1) << __shift;
      static constexpr uintmax_t __c2 = uintmax_t(1) << __coshift;
      static constexpr uintmax_t __new_d = __d * __c1;
      static constexpr uintmax_t __new_n0 = __n0 * __c1;
      static constexpr uintmax_t __n1_shifted = (__n1 % __d) * __c1;
      static constexpr uintmax_t __n0_top = (__shift != 0) ? (__n0 / __c2) : 0;
      static constexpr uintmax_t __new_n1 = __n1_shifted + __n0_top;
      typedef __big_div_impl<__new_n1, __new_n0, __new_d> _Res;

    public:
      static constexpr uintmax_t __quot_hi = __n1 / __d;
      static constexpr uintmax_t __quot_lo = _Res::__quot;
      static constexpr uintmax_t __rem = _Res::__rem / __c1;

    private:
      typedef __big_mul<__quot_lo, __d> _P0;
      typedef __big_mul<__quot_hi, __d> _P1;
      typedef __big_add<_P0::__hi, _P0::__lo, _P1::__lo, __rem> _Sum;
      // No overflow.
      static_assert(_P1::__hi == 0, "Internal library error");
      static_assert(_Sum::__hi >= _P0::__hi, "Internal library error");
      // Matches the input data.
      static_assert(_Sum::__hi == __n1 && _Sum::__lo == __n0,
		    "Internal library error");
      static_assert(__rem < __d, "Internal library error");
    };

  /**
   *  @brief Provides compile-time rational arithmetic.
   *
   *  This class template represents any finite rational number with a
   *  numerator and denominator representable by compile-time constants of
   *  type intmax_t. The ratio is simplified when instantiated.
   *
   *  For example:
   *  @code
   *    ratio<7,-21>::num == -1;
   *    ratio<7,-21>::den == 3;
   *  @endcode
   *
  */
  template<intmax_t _Num, intmax_t _Den=1, typename =void>
    struct ratio;
  template<intmax_t _Num, intmax_t _Den>
    struct ratio<_Num,_Den,  std::enable_if_t<1==__static_sign<_Den> && 1==__static_gcd<_Num,_Den>>>
    {
      static_assert(_Den != 0, "denominator cannot be zero");
      static_assert(_Num >= -__INTMAX_MAX__ && _Den >= -__INTMAX_MAX__,
		    "out of range");
      // Walter's wish:
      static_assert(__static_gcd<_Num,_Den> == 1, "specialization for simplified ratios");

      // Note: sign(N) * abs(N) == N
      static inline constexpr intmax_t num = _Num;
//        _Num * __static_sign<_Den> / __static_gcd<_Num, _Den>;

      static inline constexpr intmax_t den = _Den;
//        __static_abs<_Den> / __static_gcd<_Num, _Den>;

      typedef ratio<num, den> type;
      //PS additons
      constexpr ratio()noexcept=default;
      template<intmax_t _Num_,intmax_t _Den_>
      constexpr ratio(std::integral_constant<intmax_t,_Num_>,std::integral_constant<intmax_t,_Den_>)noexcept{
	  	  static_assert(_Num==this->num,"should always be simplified");
	  	  static_assert(_Den==this->den,"should always be simplified");
      }
      template<typename NUMERIC>
      explicit constexpr operator NUMERIC() const{
    	  	  return static_cast<NUMERIC>(num)/static_cast<NUMERIC>(den);
      }
      template<typename NUMERIC>
      static inline constexpr NUMERIC quotient{
    	  	   static_cast<NUMERIC>(num)/static_cast<NUMERIC>(den)
      };
      template<typename NUMERIC>
      constexpr static
	  NUMERIC scale(NUMERIC factor) {
    	  	  return (factor*static_cast<NUMERIC>(num))/static_cast<NUMERIC>(den);
      }
    };
  template<intmax_t _Num, intmax_t _Den, typename >
    struct ratio
	:ratio<_Num * __static_sign<_Den> / __static_gcd<_Num, _Den>,
	       __static_abs<_Den> / __static_gcd<_Num, _Den>>{
      static_assert(_Den != 0, "denominator cannot be zero");
      static_assert(_Num >= -__INTMAX_MAX__ && _Den >= -__INTMAX_MAX__,		    "out of range");
  };

//  template<intmax_t N1, intmax_t D1, intmax_t N2, intmax_t D2>
//  struct is_same<ratio<N1,D1>,ratio<N2,D2>>
//  :bool_constant<ratio<N1,D1>::num==ratio<N2,D2>::num&&ratio<N1,D1>::den==ratio<N2,D2>::den>{};

//  static_assert(is_same_v<ratio<3,6>,ratio<2,4>>,"ratio is same works");

  //PS: simplification with deduction guide from integral_constants:
  template <intmax_t _Num_, intmax_t _Den_>
  ratio(std::integral_constant<intmax_t,_Num_>,std::integral_constant<intmax_t,_Den_>)
  ->ratio<_Num_ * __static_sign<_Den_> / __static_gcd<_Num_, _Den_>,
  __static_abs<_Den_> / __static_gcd<_Num_, _Den_>>;
// this should be the real ratio type.
  template <intmax_t _Num, intmax_t _Den = 1>
  using ratio_t = ratio< _Num * __static_sign<_Den> / __static_gcd<_Num, _Den>
		  , __static_abs<_Den> / __static_gcd<_Num, _Den>>;

  template<typename _R1, typename _R2>
    struct __ratio_multiply
    {
    private:
      static const intmax_t __gcd1 =
        __static_gcd<_R1::num, _R2::den>;
      static const intmax_t __gcd2 =
        __static_gcd<_R2::num, _R1::den>;

    public:
      typedef ratio<
        __multiply<(_R1::num / __gcd1),
                        (_R2::num / __gcd2)>,
        __multiply<(_R1::den / __gcd2),
                        (_R2::den / __gcd1)>> type;

      static inline constexpr intmax_t num = type::num;
      static inline constexpr intmax_t den = type::den;
    };


  /// ratio_multiply
  template<typename _R1, typename _R2>
    using ratio_multiply = typename __ratio_multiply<_R1, _R2>::type;

  template<typename _R1, typename _R2>
    struct __ratio_divide
    {
      static_assert(_R2::num != 0, "division by 0");

      typedef typename __ratio_multiply<
        _R1,
        ratio<_R2::den, _R2::num>>::type type;

      static inline constexpr intmax_t num = type::num;
      static inline constexpr intmax_t den = type::den;
    };

  /// ratio_divide
  template<typename _R1, typename _R2>
    using ratio_divide = typename __ratio_divide<_R1, _R2>::type;

  /// ratio_equal
  template<typename _R1, typename _R2>
    struct ratio_equal
    : integral_constant<bool, _R1::num == _R2::num && _R1::den == _R2::den>
    { };

  /// ratio_not_equal
  template<typename _R1, typename _R2>
    struct ratio_not_equal
    : integral_constant<bool, !ratio_equal<_R1, _R2>::value>
    { };

  // Both numbers are positive.
  template<typename _R1, typename _R2,
           typename _Left = __big_mul<_R1::num,_R2::den>,
           typename _Right = __big_mul<_R2::num,_R1::den> >
    struct __ratio_less_impl_1
    : integral_constant<bool, __big_less<_Left::__hi, _Left::__lo,
           _Right::__hi, _Right::__lo>::value>
    { };

  template<typename _R1, typename _R2,
	   bool = (_R1::num == 0 || _R2::num == 0
		   || (__static_sign<_R1::num>
		       != __static_sign<_R2::num>)),
	   bool = (__static_sign<_R1::num> == -1
		   && __static_sign<_R2::num> == -1)>
    struct __ratio_less_impl
    : __ratio_less_impl_1<_R1, _R2>::type
    { };

  template<typename _R1, typename _R2>
    struct __ratio_less_impl<_R1, _R2, true, false>
    : integral_constant<bool, _R1::num < _R2::num>
    { };

  template<typename _R1, typename _R2>
    struct __ratio_less_impl<_R1, _R2, false, true>
    : __ratio_less_impl_1<ratio<-_R2::num, _R2::den>,
           ratio<-_R1::num, _R1::den> >::type
    { };

  /// ratio_less
  template<typename _R1, typename _R2>
    struct ratio_less
    : __ratio_less_impl<_R1, _R2>::type
    { };

  /// ratio_less_equal
  template<typename _R1, typename _R2>
    struct ratio_less_equal
    : integral_constant<bool, !ratio_less<_R2, _R1>::value>
    { };

  /// ratio_greater
  template<typename _R1, typename _R2>
    struct ratio_greater
    : integral_constant<bool, ratio_less<_R2, _R1>::value>
    { };

  /// ratio_greater_equal
  template<typename _R1, typename _R2>
    struct ratio_greater_equal
    : integral_constant<bool, !ratio_less<_R1, _R2>::value>
    { };

#if __cplusplus > 201402L
  template <typename _R1, typename _R2>
    inline constexpr bool ratio_equal_v = ratio_equal<_R1, _R2>::value;
  template <typename _R1, typename _R2>
    inline constexpr bool ratio_not_equal_v = ratio_not_equal<_R1, _R2>::value;
  template <typename _R1, typename _R2>
    inline constexpr bool ratio_less_v = ratio_less<_R1, _R2>::value;
  template <typename _R1, typename _R2>
    inline constexpr bool ratio_less_equal_v =
      ratio_less_equal<_R1, _R2>::value;
  template <typename _R1, typename _R2>
    inline constexpr bool ratio_greater_v = ratio_greater<_R1, _R2>::value;
  template <typename _R1, typename _R2>
    inline constexpr bool ratio_greater_equal_v
    = ratio_greater_equal<_R1, _R2>::value;
#endif // C++17

  template<typename _R1, typename _R2,
      bool = (_R1::num >= 0),
      bool = (_R2::num >= 0),
      bool = ratio_less<ratio<__static_abs<_R1::num>, _R1::den>,
        ratio<__static_abs<_R2::num>, _R2::den> >::value>
    struct __ratio_add_impl
    {
    private:
      typedef typename __ratio_add_impl<
        ratio<-_R1::num, _R1::den>,
        ratio<-_R2::num, _R2::den> >::type __t;
    public:
      typedef ratio<-__t::num, __t::den> type;
    };

  // True addition of nonnegative numbers.
  template<typename _R1, typename _R2, bool __b>
    struct __ratio_add_impl<_R1, _R2, true, true, __b>
    {
    private:
      static constexpr uintmax_t __g = __static_gcd<_R1::den, _R2::den>;
      static constexpr uintmax_t __d2 = _R2::den / __g;
      typedef __big_mul<_R1::den, __d2> __d;
      typedef __big_mul<_R1::num, _R2::den / __g> __x;
      typedef __big_mul<_R2::num, _R1::den / __g> __y;
      typedef __big_add<__x::__hi, __x::__lo, __y::__hi, __y::__lo> __n;
      static_assert(__n::__hi >= __x::__hi, "Internal library error");
      typedef __big_div<__n::__hi, __n::__lo, __g> __ng;
      static constexpr uintmax_t __g2 = __static_gcd<__ng::__rem, __g>;
      typedef __big_div<__n::__hi, __n::__lo, __g2> __n_final;
      static_assert(__n_final::__rem == 0, "Internal library error");
      static_assert(__n_final::__quot_hi == 0 &&
        __n_final::__quot_lo <= __INTMAX_MAX__, "overflow in addition");
      typedef __big_mul<_R1::den / __g2, __d2> __d_final;
      static_assert(__d_final::__hi == 0 &&
        __d_final::__lo <= __INTMAX_MAX__, "overflow in addition");
    public:
      typedef ratio<__n_final::__quot_lo, __d_final::__lo> type;
    };

  template<typename _R1, typename _R2>
    struct __ratio_add_impl<_R1, _R2, false, true, true>
    : __ratio_add_impl<_R2, _R1>
    { };

  // True subtraction of nonnegative numbers yielding a nonnegative result.
  template<typename _R1, typename _R2>
    struct __ratio_add_impl<_R1, _R2, true, false, false>
    {
    private:
      static constexpr uintmax_t __g = __static_gcd<_R1::den, _R2::den>;
      static constexpr uintmax_t __d2 = _R2::den / __g;
      typedef __big_mul<_R1::den, __d2> __d;
      typedef __big_mul<_R1::num, _R2::den / __g> __x;
      typedef __big_mul<-_R2::num, _R1::den / __g> __y;
      typedef __big_sub<__x::__hi, __x::__lo, __y::__hi, __y::__lo> __n;
      typedef __big_div<__n::__hi, __n::__lo, __g> __ng;
      static constexpr uintmax_t __g2 = __static_gcd<__ng::__rem, __g>;
      typedef __big_div<__n::__hi, __n::__lo, __g2> __n_final;
      static_assert(__n_final::__rem == 0, "Internal library error");
      static_assert(__n_final::__quot_hi == 0 &&
        __n_final::__quot_lo <= __INTMAX_MAX__, "overflow in addition");
      typedef __big_mul<_R1::den / __g2, __d2> __d_final;
      static_assert(__d_final::__hi == 0 &&
        __d_final::__lo <= __INTMAX_MAX__, "overflow in addition");
    public:
      typedef ratio<__n_final::__quot_lo, __d_final::__lo> type;
    };

  template<typename _R1, typename _R2>
    struct __ratio_add
    {
      typedef typename __ratio_add_impl<_R1, _R2>::type type;
      static constexpr intmax_t num = type::num;
      static constexpr intmax_t den = type::den;
    };

  template<typename _R1, typename _R2>
    constexpr intmax_t __ratio_add<_R1, _R2>::num;

  template<typename _R1, typename _R2>
    constexpr intmax_t __ratio_add<_R1, _R2>::den;

  /// ratio_add
  template<typename _R1, typename _R2>
    using ratio_add = typename __ratio_add<_R1, _R2>::type;

  template<typename _R1, typename _R2>
    struct __ratio_subtract
    {
      typedef typename __ratio_add<
        _R1,
        ratio<-_R2::num, _R2::den>>::type type;

      static inline constexpr intmax_t num = type::num;
      static inline constexpr intmax_t den = type::den;
    };


  /// ratio_subtract
  template<typename _R1, typename _R2>
    using ratio_subtract = typename __ratio_subtract<_R1, _R2>::type;


  typedef ratio<1,       1000000000000000000> atto;
  typedef ratio<1,          1000000000000000> femto;
  typedef ratio<1,             1000000000000> pico;
  typedef ratio<1,                1000000000> nano;
  typedef ratio<1,                   1000000> micro;
  typedef ratio<1,                      1000> milli;
  typedef ratio<1,                       100> centi;
  typedef ratio<1,                        10> deci;
  typedef ratio<                       10, 1> deca;
  typedef ratio<                      100, 1> hecto;
  typedef ratio<                     1000, 1> kilo;
  typedef ratio<                  1000000, 1> mega;
  typedef ratio<               1000000000, 1> giga;
  typedef ratio<            1000000000000, 1> tera;
  typedef ratio<         1000000000000000, 1> peta;
  typedef ratio<      1000000000000000000, 1> exa;


//PS_additions for value operators:
template<intmax_t N1,intmax_t D1,intmax_t N2,intmax_t D2>
constexpr auto operator+(ratio<N1,D1>,ratio<N2,D2>){
	return ratio_add<ratio<N1,D1>,ratio<N2,D2>>{};
}
template<intmax_t N1,intmax_t D1,intmax_t N2,intmax_t D2>
constexpr auto operator-(ratio<N1,D1>,ratio<N2,D2>){
	return ratio_subtract<ratio<N1,D1>,ratio<N2,D2>>{};
}
template<intmax_t N1,intmax_t D1,intmax_t N2,intmax_t D2>
constexpr auto operator*(ratio<N1,D1>,ratio<N2,D2>){
	return ratio_multiply<ratio<N1,D1>,ratio<N2,D2>>{};
}
template<intmax_t N1,intmax_t D1,intmax_t N2,intmax_t D2>
constexpr auto operator/(ratio<N1,D1>,ratio<N2,D2>){
	return ratio_divide<ratio<N1,D1>,ratio<N2,D2>>{};
}
template<intmax_t N1,intmax_t D1>
constexpr auto operator-(ratio<N1,D1>){
	return ratio<-N1,D1>{};
}
template<intmax_t N1,intmax_t D1,intmax_t N2,intmax_t D2>
constexpr auto operator==(ratio<N1,D1>,ratio<N2,D2>){
	return ratio_equal<ratio<N1,D1>,ratio<N2,D2>>{};
}
template<intmax_t N1,intmax_t D1,intmax_t N2,intmax_t D2>
constexpr auto operator!=(ratio<N1,D1>,ratio<N2,D2>){
	return ratio_not_equal<ratio<N1,D1>,ratio<N2,D2>>{};
}
template<intmax_t N1,intmax_t D1,intmax_t N2,intmax_t D2>
constexpr auto operator<(ratio<N1,D1>,ratio<N2,D2>){
	return ratio_less<ratio<N1,D1>,ratio<N2,D2>>{};
}
template<intmax_t N1,intmax_t D1,intmax_t N2,intmax_t D2>
constexpr auto operator<=(ratio<N1,D1>,ratio<N2,D2>){
	return ratio_less_equal<ratio<N1,D1>,ratio<N2,D2>>{};
}
template<intmax_t N1,intmax_t D1,intmax_t N2,intmax_t D2>
constexpr auto operator>(ratio<N1,D1>,ratio<N2,D2>){
	return ratio_greater<ratio<N1,D1>,ratio<N2,D2>>{};
}
template<intmax_t N1,intmax_t D1,intmax_t N2,intmax_t D2>
constexpr auto operator>=(ratio<N1,D1>,ratio<N2,D2>){
	return ratio_greater_equal<ratio<N1,D1>,ratio<N2,D2>>{};
}

} // namespace
namespace std_peter=std;
#endif //_PETER_RATIO
