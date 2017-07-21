#include "UnitMath.h"
#include "cute.h"
#include "common_for_tests.h"



void UnitMath_cos()
{
	ASSERT((std::is_same<typename std::decay<scalar_t>::type, typename std::decay<decltype(cos(angle::radian_t(0)))>::type>::value));
	ASSERT_EQUAL_DELTA(scalar_t(-0.41614683654), cos(angle::radian_t(2)), 5.0e-11);
	ASSERT_EQUAL_DELTA(scalar_t(-0.70710678118), cos(angle::degree_t(135)), 5.0e-11);
}

void UnitMath_sin()
{
	ASSERT((std::is_same<typename std::decay<scalar_t>::type, typename std::decay<decltype(sin(angle::radian_t(0)))>::type>::value));
	ASSERT_EQUAL_DELTA(scalar_t(0.90929742682), sin(angle::radian_t(2)), 5.0e-11);
	ASSERT_EQUAL_DELTA(scalar_t(0.70710678118), sin(angle::degree_t(135)), 5.0e-11);
}

void UnitMath_tan()
{
	ASSERT((std::is_same<typename std::decay<scalar_t>::type, typename std::decay<decltype(tan(angle::radian_t(0)))>::type>::value));
	ASSERT_EQUAL_DELTA(scalar_t(-2.18503986326), tan(angle::radian_t(2)), 5.0e-11);
	ASSERT_EQUAL_DELTA(scalar_t(-1.0), tan(angle::degree_t(135)), 5.0e-11);
}

void UnitMath_acos()
{
	ASSERT((std::is_same<typename std::decay<angle::radian_t>::type, typename std::decay<decltype(acos(scalar_t(0)))>::type>::value));
	ASSERT_EQUAL_DELTA(angle::radian_t(2).to<double>(), acos(scalar_t(-0.41614683654)).to<double>(), 5.0e-11);
	ASSERT_EQUAL_DELTA(angle::degree_t(135).to<double>(), angle::degree_t(acos(scalar_t(-0.70710678118654752440084436210485))).to<double>(), 5.0e-12);
}

void UnitMath_asin()
{
	ASSERT((std::is_same<typename std::decay<angle::radian_t>::type, typename std::decay<decltype(asin(scalar_t(0)))>::type>::value));
	ASSERT_EQUAL_DELTA(angle::radian_t(1.14159265).to<double>(), asin(scalar_t(0.90929742682)).to<double>(), 5.0e-9);
	ASSERT_EQUAL_DELTA(angle::degree_t(45).to<double>(), angle::degree_t(asin(scalar_t(0.70710678118654752440084436210485))).to<double>(), 5.0e-12);
}

void UnitMath_atan()
{
	ASSERT((std::is_same<typename std::decay<angle::radian_t>::type, typename std::decay<decltype(atan(scalar_t(0)))>::type>::value));
	ASSERT_EQUAL_DELTA(angle::radian_t(-1.14159265).to<double>(), atan(scalar_t(-2.18503986326)).to<double>(), 5.0e-9);
	ASSERT_EQUAL_DELTA(angle::degree_t(-45).to<double>(), angle::degree_t(atan(scalar_t(-1.0))).to<double>(), 5.0e-12);
}

void UnitMath_atan2()
{
	ASSERT((std::is_same<typename std::decay<angle::radian_t>::type, typename std::decay<decltype(atan2(scalar_t(1), scalar_t(1)))>::type>::value));
	ASSERT_EQUAL_DELTA(angle::radian_t(constants::detail::PI_VAL<> / 4).to<double>(), atan2(scalar_t(2), scalar_t(2)).to<double>(), 5.0e-12);
	ASSERT_EQUAL_DELTA(angle::degree_t(45).to<double>(), angle::degree_t(atan2(scalar_t(2), scalar_t(2))).to<double>(), 5.0e-12);

	ASSERT((std::is_same<typename std::decay<angle::radian_t>::type, typename std::decay<decltype(atan2(scalar_t(1), scalar_t(1)))>::type>::value));
	ASSERT_EQUAL_DELTA(angle::radian_t(constants::detail::PI_VAL<> / 6).to<double>(), atan2(scalar_t(1), scalar_t(sqrt(3))).to<double>(), 5.0e-12);
	ASSERT_EQUAL_DELTA(angle::degree_t(30).to<double>(), angle::degree_t(atan2(scalar_t(1), scalar_t(sqrt(3)))).to<double>(), 5.0e-12);
}

void UnitMath_cosh()
{
	ASSERT((std::is_same<typename std::decay<scalar_t>::type, typename std::decay<decltype(cosh(angle::radian_t(0)))>::type>::value));
	ASSERT_EQUAL_DELTA(scalar_t(3.76219569108), cosh(angle::radian_t(2)), 5.0e-11);
	ASSERT_EQUAL_DELTA(scalar_t(5.32275215), cosh(angle::degree_t(135)), 5.0e-9);
}

void UnitMath_sinh()
{
	ASSERT((std::is_same<typename std::decay<scalar_t>::type, typename std::decay<decltype(sinh(angle::radian_t(0)))>::type>::value));
	ASSERT_EQUAL_DELTA(scalar_t(3.62686040785), sinh(angle::radian_t(2)), 5.0e-11);
	ASSERT_EQUAL_DELTA(scalar_t(5.22797192), sinh(angle::degree_t(135)), 5.0e-9);
}

void UnitMath_tanh()
{
	ASSERT((std::is_same<typename std::decay<scalar_t>::type, typename std::decay<decltype(tanh(angle::radian_t(0)))>::type>::value));
	ASSERT_EQUAL_DELTA(scalar_t(0.96402758007), tanh(angle::radian_t(2)), 5.0e-11);
	ASSERT_EQUAL_DELTA(scalar_t(0.98219338), tanh(angle::degree_t(135)), 5.0e-11);
}

void UnitMath_acosh()
{
	ASSERT((std::is_same<typename std::decay<angle::radian_t>::type, typename std::decay<decltype(acosh(scalar_t(0)))>::type>::value));
	ASSERT_EQUAL_DELTA(angle::radian_t(1.316957896924817).to<double>(), acosh(scalar_t(2.0)).to<double>(), 5.0e-11);
	ASSERT_EQUAL_DELTA(angle::degree_t(75.456129290216893).to<double>(), angle::degree_t(acosh(scalar_t(2.0))).to<double>(), 5.0e-12);
}

void UnitMath_asinh()
{
	ASSERT((std::is_same<typename std::decay<angle::radian_t>::type, typename std::decay<decltype(asinh(scalar_t(0)))>::type>::value));
	ASSERT_EQUAL_DELTA(angle::radian_t(1.443635475178810).to<double>(), asinh(scalar_t(2)).to<double>(), 5.0e-9);
	ASSERT_EQUAL_DELTA(angle::degree_t(82.714219883108939).to<double>(), angle::degree_t(asinh(scalar_t(2))).to<double>(), 5.0e-12);
}

void UnitMath_atanh()
{
	ASSERT((std::is_same<typename std::decay<angle::radian_t>::type, typename std::decay<decltype(atanh(scalar_t(0)))>::type>::value));
	ASSERT_EQUAL_DELTA(angle::radian_t(0.549306144334055).to<double>(), atanh(scalar_t(0.5)).to<double>(), 5.0e-9);
	ASSERT_EQUAL_DELTA(angle::degree_t(31.472923730945389).to<double>(), angle::degree_t(atanh(scalar_t(0.5))).to<double>(), 5.0e-12);
}

void UnitMath_exp()
{
	double val = 10.0;
	ASSERT_EQUAL(std::exp(val), exp(scalar_t(val)));
}

void UnitMath_log()
{
	double val = 100.0;
	ASSERT_EQUAL(std::log(val), log(scalar_t(val)));
}

void UnitMath_log10()
{
	double val = 100.0;
	ASSERT_EQUAL(std::log10(val), log10(scalar_t(val)));
}

void UnitMath_modf()
{
	double val = 100.0;
	double modfr1;
	scalar_t modfr2;
	ASSERT_EQUAL(std::modf(val, &modfr1), modf(scalar_t(val), &modfr2));
	ASSERT_EQUAL(modfr1, modfr2);
}

void UnitMath_exp2()
{
	double val = 10.0;
	ASSERT_EQUAL(std::exp2(val), exp2(scalar_t(val)));
}

void UnitMath_expm1()
{
	double val = 10.0;
	ASSERT_EQUAL(std::expm1(val), expm1(scalar_t(val)));
}

void UnitMath_log1p()
{
	double val = 10.0;
	ASSERT_EQUAL(std::log1p(val), log1p(scalar_t(val)));
}

void UnitMath_log2()
{
	double val = 10.0;
	ASSERT_EQUAL(std::log2(val), log2(scalar_t(val)));
}

void UnitMath_pow()
{
	bool isSame;
	meter_t value(10.0);

	auto sq = pow<2>(value);
	ASSERT_EQUAL_DELTA(100.0, sq(), 5.0e-2);
	isSame = std::is_same<decltype(sq), square_meter_t>::value;
	ASSERT(isSame);

	auto cube = pow<3>(value);
	ASSERT_EQUAL_DELTA(1000.0, cube(), 5.0e-2);
	isSame = std::is_same<decltype(cube), quantity<cubed<meter>>>::value;
	ASSERT(isSame);

	auto fourth = pow<4>(value);
	ASSERT_EQUAL_DELTA(10000.0, fourth(), 5.0e-2);
	isSame = std::is_same<decltype(fourth), quantity<compound_unit<squared<meter>, squared<meter>>>>::value;
	ASSERT(isSame);
}

void UnitMath_sqrt()
{
	ASSERT((std::is_same<typename std::decay<meter_t>::type, typename std::decay<decltype(sqrt(square_meter_t(4.0)))>::type>::value));
	ASSERT_EQUAL_DELTA(meter_t(2.0).to<double>(), sqrt(square_meter_t(4.0)).to<double>(), 5.0e-9);

	ASSERT((std::is_same<typename std::decay<angle::radian_t>::type, typename std::decay<decltype(sqrt(steradian_t(16.0)))>::type>::value));
	ASSERT_EQUAL_DELTA(angle::radian_t(4.0).to<double>(), sqrt(steradian_t(16.0)).to<double>(), 5.0e-9);

	ASSERT((std::is_convertible<typename std::decay<foot_t>::type, typename std::decay<decltype(sqrt(square_foot_t(10.0)))>::type>::value));

	// for rational conversion (i.e. no integral root) let's check a bunch of different ways this could go wrong
	foot_t resultFt = sqrt(square_foot_t(10.0));
	ASSERT_EQUAL_DELTA(foot_t(3.16227766017).to<double>(), sqrt(square_foot_t(10.0)).to<double>(), 5.0e-9);
	ASSERT_EQUAL_DELTA(foot_t(3.16227766017).to<double>(), resultFt.to<double>(), 5.0e-9);
	ASSERT_EQUAL(resultFt, sqrt(square_foot_t(10.0)));
}

void UnitMath_hypot()
{
	ASSERT((std::is_same<typename std::decay<meter_t>::type, typename std::decay<decltype(hypot(meter_t(3.0), meter_t(4.0)))>::type>::value));
	ASSERT_EQUAL_DELTA(meter_t(5.0).to<double>(), (hypot(meter_t(3.0), meter_t(4.0))).to<double>(), 5.0e-9);

	ASSERT((std::is_same<typename std::decay<foot_t>::type, typename std::decay<decltype(hypot(foot_t(3.0), meter_t(1.2192)))>::type>::value));
	ASSERT_EQUAL_DELTA(foot_t(5.0).to<double>(), (hypot(foot_t(3.0), meter_t(1.2192))).to<double>(), 5.0e-9);
}

void UnitMath_ceil()
{
	double val = 101.1;
	ASSERT_EQUAL(ceil(val), ceil(meter_t(val)).to<double>());
	ASSERT((std::is_same<typename std::decay<meter_t>::type, typename std::decay<decltype(ceil(meter_t(val)))>::type>::value));
}

void UnitMath_floor()
{
	double val = 101.1;
	ASSERT_EQUAL(floor(val), floor(scalar_t(val)));
}

void UnitMath_fmod()
{
	ASSERT_EQUAL(fmod(100.0, 101.2), fmod(meter_t(100.0), meter_t(101.2)).to<double>());
}

void UnitMath_trunc()
{
	double val = 101.1;
	ASSERT_EQUAL(trunc(val), trunc(scalar_t(val)));
}

void UnitMath_round()
{
	double val = 101.1;
	ASSERT_EQUAL(round(val), round(scalar_t(val)));
}

void UnitMath_copysign()
{
	double sign = -1;
	meter_t val(5.0);
	ASSERT_EQUAL(meter_t(-5.0), copysign(val, sign));
	ASSERT_EQUAL(meter_t(-5.0), copysign(val, angle::radian_t(sign)));
}

void UnitMath_fdim()
{
	ASSERT_EQUAL(meter_t(0.0), fdim(meter_t(8.0), meter_t(10.0)));
	ASSERT_EQUAL(meter_t(2.0), fdim(meter_t(10.0), meter_t(8.0)));
	ASSERT_EQUAL_DELTA(meter_t(9.3904).to<double>(), fdim(meter_t(10.0), foot_t(2.0)).to<double>(), 5.0e-320);	// not sure why they aren't comparing exactly equal, but clearly they are.
}

void UnitMath_fmin()
{
	ASSERT_EQUAL(meter_t(8.0), fmin(meter_t(8.0), meter_t(10.0)));
	ASSERT_EQUAL(meter_t(8.0), fmin(meter_t(10.0), meter_t(8.0)));
	ASSERT_EQUAL(foot_t(2.0), fmin(meter_t(10.0), foot_t(2.0)));
}

void UnitMath_fmax()
{
	ASSERT_EQUAL(meter_t(10.0), fmax(meter_t(8.0), meter_t(10.0)));
	ASSERT_EQUAL(meter_t(10.0), fmax(meter_t(10.0), meter_t(8.0)));
	ASSERT_EQUAL(meter_t(10.0), fmax(meter_t(10.0), foot_t(2.0)));
}

void UnitMath_fabs()
{
	ASSERT_EQUAL(meter_t(10.0), fabs(meter_t(-10.0)));
	ASSERT_EQUAL(meter_t(10.0), fabs(meter_t(10.0)));
}

void UnitMath_abs()
{
	ASSERT_EQUAL(meter_t(10.0), abs(meter_t(-10.0)));
	ASSERT_EQUAL(meter_t(10.0), abs(meter_t(10.0)));
}

void UnitMath_fma()
{
	meter_t x(2.0);
	meter_t y(3.0);
	square_meter_t z(1.0);
	ASSERT_EQUAL(square_meter_t(7.0), (math::fma(x, y, z)));
}

cute::suite make_suite_UnitMath() {
	cute::suite s { };
	s.push_back(CUTE(UnitMath_cos));
	s.push_back(CUTE(UnitMath_sin));
	s.push_back(CUTE(UnitMath_tan));
	s.push_back(CUTE(UnitMath_acos));
	s.push_back(CUTE(UnitMath_asin));
	s.push_back(CUTE(UnitMath_atan));
	s.push_back(CUTE(UnitMath_atan2));
	s.push_back(CUTE(UnitMath_sinh));
	s.push_back(CUTE(UnitMath_cosh));
	s.push_back(CUTE(UnitMath_tanh));
	s.push_back(CUTE(UnitMath_acosh));
	s.push_back(CUTE(UnitMath_asinh));
	s.push_back(CUTE(UnitMath_atanh));
	s.push_back(CUTE(UnitMath_exp));
	s.push_back(CUTE(UnitMath_log));
	s.push_back(CUTE(UnitMath_log10));
	s.push_back(CUTE(UnitMath_modf));
	s.push_back(CUTE(UnitMath_exp2));
	s.push_back(CUTE(UnitMath_expm1));
	s.push_back(CUTE(UnitMath_log1p));
	s.push_back(CUTE(UnitMath_log2));
	s.push_back(CUTE(UnitMath_pow));
	s.push_back(CUTE(UnitMath_sqrt));
	s.push_back(CUTE(UnitMath_hypot));
	s.push_back(CUTE(UnitMath_ceil));
	s.push_back(CUTE(UnitMath_floor));
	s.push_back(CUTE(UnitMath_fmod));
	s.push_back(CUTE(UnitMath_trunc));
	s.push_back(CUTE(UnitMath_round));
	s.push_back(CUTE(UnitMath_copysign));
	s.push_back(CUTE(UnitMath_fmin));
	s.push_back(CUTE(UnitMath_fmax));
	s.push_back(CUTE(UnitMath_fabs));
	s.push_back(CUTE(UnitMath_abs));
	return s;
}
