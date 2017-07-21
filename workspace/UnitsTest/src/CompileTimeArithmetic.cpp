#include "common_for_tests.h"
#include "CompileTimeArithmetic.h"
#include "cute.h"

#ifndef DO_NOT_USE_COMPILE_TIME_ARITHMETIC
void CompileTimeArithmetic_unit_value_t()
{
	typedef unit_value_t<meters, 3, 2> mRatio;
	ASSERT_EQUAL(meter_t(1.5), mRatio::value());
}

void CompileTimeArithmetic_is_unit_value_t()
{
	typedef unit_value_t<meters, 3, 2> mRatio;

	ASSERT((traits::is_unit_value_t<mRatio>));
	ASSERT(!(traits::is_unit_value_t<meter_t>));
	ASSERT(!(traits::is_unit_value_t<double>));

	ASSERT((traits::is_unit_value_t<mRatio, meters>));
	ASSERT(!(traits::is_unit_value_t<meter_t, meters>));
	ASSERT(!(traits::is_unit_value_t<double, meters>));
}

void CompileTimeArithmetic_is_unit_value_t_category()
{
	typedef unit_value_t<feet, 3, 2> mRatio;
	ASSERT((traits::is_unit_value_t_category<category::length_unit, mRatio>));
	ASSERT(!(traits::is_unit_value_t_category<category::angle_unit, mRatio>));
	ASSERT(!(traits::is_unit_value_t_category<category::length_unit, meter_t>));
	ASSERT(!(traits::is_unit_value_t_category<category::length_unit, double>));
}

void CompileTimeArithmetic_unit_value_add()
{
	typedef unit_value_t<meters, 3, 2> mRatio;

	using sum = unit_value_add<mRatio, mRatio>;
	ASSERT_EQUAL(meter_t(3.0), sum::value());
	ASSERT((traits::is_unit_value_t_category<category::length_unit, sum>));

	typedef unit_value_t<feet, 1> ftRatio;

	using sumf = unit_value_add<ftRatio, mRatio>;
	ASSERT((std::is_same_v<typename std::decay<foot_t>::type, typename std::decay<decltype(sumf::value())>::type>));
	ASSERT_EQUAL_DELTA(5.92125984, sumf::value().to<double>(), 5.0e-8);
	ASSERT((traits::is_unit_value_t_category<category::length_unit, sumf>));

	typedef unit_value_t<celsius, 1> cRatio;
	typedef unit_value_t<fahrenheit, 2> fRatio;

	using sumc = unit_value_add<cRatio, fRatio>;
	ASSERT((std::is_same<typename std::decay<celsius_t>::type, typename std::decay<decltype(sumc::value())>::type>::value));
	ASSERT_EQUAL_DELTA(2.11111111111, sumc::value().to<double>(), 5.0e-8);
	ASSERT((traits::is_unit_value_t_category<category::temperature_unit, sumc>));

	typedef unit_value_t<angle::radian, 1> rRatio;
	typedef unit_value_t<angle::degree, 3> dRatio;

	using sumr = unit_value_add<rRatio, dRatio>;
	ASSERT((std::is_same<typename std::decay<angle::radian_t>::type, typename std::decay<decltype(sumr::value())>::type>::value));
	ASSERT_EQUAL_DELTA(1.05235988, sumr::value().to<double>(), 5.0e-8);
	ASSERT((traits::is_unit_value_t_category<category::angle_unit, sumr>));
}

void CompileTimeArithmetic_unit_value_subtract()
{
	typedef unit_value_t<meters, 3, 2> mRatio;

	using diff = unit_value_subtract<mRatio, mRatio>;
	ASSERT_EQUAL(meter_t(0), diff::value());
	ASSERT((traits::is_unit_value_t_category<category::length_unit, diff>));

	typedef unit_value_t<feet, 1> ftRatio;

	using difff = unit_value_subtract<ftRatio, mRatio>;
	ASSERT((std::is_same_v< std::decay_t<foot_t>,  std::decay_t<decltype(difff::value())>>));
	ASSERT_EQUAL_DELTA(-3.92125984, difff::value().to<double>(), 5.0e-8);
	ASSERT((traits::is_unit_value_t_category<category::length_unit, difff>));

	typedef unit_value_t<celsius, 1> cRatio;
	typedef unit_value_t<fahrenheit, 2> fRatio;

	using diffc = unit_value_subtract<cRatio, fRatio>;
	ASSERT((std::is_same<typename std::decay<celsius_t>::type, typename std::decay<decltype(diffc::value())>::type>::value));
	ASSERT_EQUAL_DELTA(-0.11111111111, diffc::value().to<double>(), 5.0e-8);
	ASSERT((traits::is_unit_value_t_category<category::temperature_unit, diffc>));

	typedef unit_value_t<angle::radian, 1> rRatio;
	typedef unit_value_t<angle::degree, 3> dRatio;

	using diffr = unit_value_subtract<rRatio, dRatio>;
	ASSERT((std::is_same<typename std::decay<angle::radian_t>::type, typename std::decay<decltype(diffr::value())>::type>::value));
	ASSERT_EQUAL_DELTA(0.947640122, diffr::value().to<double>(), 5.0e-8);
	ASSERT((traits::is_unit_value_t_category<category::angle_unit, diffr>));
}

void CompileTimeArithmetic_unit_value_multiply()
{
	typedef unit_value_t<meters, 2> mRatio;
	typedef unit_value_t<feet, 656168, 100000> ftRatio;	// 2 meter

	using product = unit_value_multiply<mRatio, mRatio>;
	ASSERT_EQUAL(square_meter_t(4), product::value());
	ASSERT((traits::is_unit_value_t_category<category::area_unit, product>));

	using productM = unit_value_multiply<mRatio, ftRatio>;

	ASSERT( (std::is_same_v< std::decay_t<square_meter_t>, std::decay_t<decltype(productM::value())>>));
	ASSERT_EQUAL_DELTA(4.0, productM::value().to<double>(), 5.0e-7);
	ASSERT((traits::is_unit_value_t_category<category::area_unit, productM>));

	using productF = unit_value_multiply<ftRatio, mRatio>;
	ASSERT((std::is_same<typename std::decay<square_foot_t>::type, typename std::decay<decltype(productF::value())>::type>::value));
	ASSERT_EQUAL_DELTA(43.0556444224, productF::value().to<double>(), 5.0e-6);
	ASSERT((traits::is_unit_value_t_category<category::area_unit, productF>));

	using productF2 = unit_value_multiply<ftRatio, ftRatio>;
	ASSERT((std::is_same<typename std::decay<square_foot_t>::type, typename std::decay<decltype(productF2::value())>::type>::value));
	ASSERT_EQUAL_DELTA(43.0556444224, productF2::value().to<double>(), 5.0e-8);
	ASSERT((traits::is_unit_value_t_category<category::area_unit, productF2>));

	typedef unit_value_t<units::force::newton, 5> nRatio;

	using productN = unit_value_multiply<nRatio, ftRatio>;
	ASSERT(!(std::is_same<typename std::decay<torque::newton_meter_t>::type, typename std::decay<decltype(productN::value())>::type>::value));
	ASSERT((std::is_convertible<typename std::decay<torque::newton_meter_t>::type, typename std::decay<decltype(productN::value())>::type>::value));
	ASSERT_EQUAL_DELTA(32.8084, productN::value().to<double>(), 5.0e-8);
	ASSERT_EQUAL_DELTA(10.0, (productN::value().convert<newton_meter>().to<double>()), 5.0e-7);
	ASSERT((traits::is_unit_value_t_category<category::torque_unit, productN>));

	typedef unit_value_t<angle::radian, 11, 10> r1Ratio;
	typedef unit_value_t<angle::radian, 22, 10> r2Ratio;

	using productR = unit_value_multiply<r1Ratio, r2Ratio>;
	ASSERT((std::is_same<typename std::decay<steradian_t>::type, typename std::decay<decltype(productR::value())>::type>::value));
	ASSERT_EQUAL_DELTA(2.42, productR::value().to<double>(), 5.0e-8);
	ASSERT_EQUAL_DELTA(7944.39137, (productR::value().convert<degrees_squared>().to<double>()), 5.0e-6);
	ASSERT((traits::is_unit_value_t_category<category::solid_angle_unit, productR>));
}

void CompileTimeArithmetic_unit_value_divide()
{
	typedef unit_value_t<meters, 2> mRatio;
	typedef unit_value_t<feet, 656168, 100000> ftRatio;	// 2 meter

	using product = unit_value_divide<mRatio, mRatio>;
	ASSERT_EQUAL(scalar_t(1), product::value());
	ASSERT((traits::is_unit_value_t_category<category::scalar_unit, product>));

	using productM = unit_value_divide<mRatio, ftRatio>;

	ASSERT((std::is_same<typename std::decay<scalar_t>::type, typename std::decay<decltype(productM::value())>::type>::value));
	ASSERT_EQUAL_DELTA(1., productM::value().to<double>(), 5.0e-7);
	ASSERT((traits::is_unit_value_t_category<category::scalar_unit, productM>));

	using productF = unit_value_divide<ftRatio, mRatio>;
	ASSERT((std::is_same<typename std::decay<scalar_t>::type, typename std::decay<decltype(productF::value())>::type>::value));
	ASSERT_EQUAL_DELTA(1.0, productF::value().to<double>(), 5.0e-6);
	ASSERT((traits::is_unit_value_t_category<category::scalar_unit, productF>));

	using productF2 = unit_value_divide<ftRatio, ftRatio>;
	ASSERT((std::is_same<typename std::decay<scalar_t>::type, typename std::decay<decltype(productF2::value())>::type>::value));
	ASSERT_EQUAL_DELTA(1.0, productF2::value().to<double>(), 5.0e-8);
	ASSERT((traits::is_unit_value_t_category<category::scalar_unit, productF2>));

	typedef unit_value_t<seconds, 10> sRatio;

	using productMS = unit_value_divide<mRatio, sRatio>;
	ASSERT((std::is_same<typename std::decay<meters_per_second_t>::type, typename std::decay<decltype(productMS::value())>::type>::value));
	ASSERT_EQUAL_DELTA(0.2, productMS::value().to<double>(), 5.0e-8);
	ASSERT((traits::is_unit_value_t_category<category::velocity_unit, productMS>));

	typedef unit_value_t<angle::radian, 20> rRatio;

	using productRS = unit_value_divide<rRatio, sRatio>;
	ASSERT((std::is_same<typename std::decay<radians_per_second_t>::type, typename std::decay<decltype(productRS::value())>::type>::value));
	ASSERT_EQUAL_DELTA(2, productRS::value().to<double>(), 5.0e-8);
	ASSERT_EQUAL_DELTA(114.592, (productRS::value().convert<degrees_per_second>().to<double>()), 5.0e-4);
	ASSERT((traits::is_unit_value_t_category<category::angular_velocity_unit, productRS>));
}

void CompileTimeArithmetic_unit_value_power()
{
	typedef unit_value_t<meters, 2> mRatio;

	using sq = unit_value_power<mRatio, 2>;
	ASSERT((std::is_convertible<typename std::decay<square_meter_t>::type, typename std::decay<decltype(sq::value())>::type>::value));
	ASSERT_EQUAL_DELTA(4, sq::value().to<double>(), 5.0e-8);
	ASSERT((traits::is_unit_value_t_category<category::area_unit, sq>));

	typedef unit_value_t<angle::radian, 18, 10> rRatio;

	using sqr = unit_value_power<rRatio, 2>;
	ASSERT((std::is_convertible<typename std::decay<steradian_t>::type, typename std::decay<decltype(sqr::value())>::type>::value));
	ASSERT_EQUAL_DELTA(3.24, sqr::value().to<double>(), 5.0e-8);
	ASSERT_EQUAL_DELTA(10636.292574038049895092690529904, (sqr::value().convert<degrees_squared>().to<double>()), 5.0e-10);
	ASSERT((traits::is_unit_value_t_category<category::solid_angle_unit, sqr>));
}

void CompileTimeArithmetic_unit_value_sqrt()
{
	typedef unit_value_t<square_meters, 10> mRatio;

	using root = unit_value_sqrt<mRatio>;
	ASSERT((std::is_convertible<typename std::decay<meter_t>::type, typename std::decay<decltype(root::value())>::type>::value));
	ASSERT_EQUAL_DELTA(3.16227766017, root::value().to<double>(), 5.0e-9);
	ASSERT((traits::is_unit_value_t_category<category::length_unit, root>));

	typedef unit_value_t<hectare, 51, 7> hRatio;

	using rooth = unit_value_sqrt<hRatio, 100000000>;
	ASSERT((std::is_convertible<typename std::decay<mile_t>::type, typename std::decay<decltype(rooth::value())>::type>::value));
	ASSERT_EQUAL_DELTA(2.69920623253, rooth::value().to<double>(), 5.0e-8);
	ASSERT_EQUAL_DELTA(269.920623, rooth::value().convert<meters>().to<double>(), 5.0e-6);
	ASSERT((traits::is_unit_value_t_category<category::length_unit, rooth>));

	typedef unit_value_t<steradian, 18, 10> rRatio;

	using rootr = unit_value_sqrt<rRatio>;
	ASSERT((traits::is_angle_unit<decltype(rootr::value())>));
	ASSERT_EQUAL_DELTA(1.3416407865, rootr::value().to<double>(), 5.0e-8);
	ASSERT_EQUAL_DELTA(76.870352574, rootr::value().convert<angle::degrees>().to<double>(), 5.0e-6);
	ASSERT((traits::is_unit_value_t_category<category::angle_unit, rootr>));
}
#endif
cute::suite make_suite_CompileTimeArithmetic() {
	cute::suite s { };
#ifndef DO_NOT_USE_COMPILE_TIME_ARITHMETIC
	s.push_back(CUTE(CompileTimeArithmetic_unit_value_t));
	s.push_back(CUTE(CompileTimeArithmetic_is_unit_value_t));
	s.push_back(CUTE(CompileTimeArithmetic_is_unit_value_t_category));
	s.push_back(CUTE(CompileTimeArithmetic_unit_value_add));
	s.push_back(CUTE(CompileTimeArithmetic_unit_value_subtract));
	s.push_back(CUTE(CompileTimeArithmetic_unit_value_multiply));
	s.push_back(CUTE(CompileTimeArithmetic_unit_value_divide));
	s.push_back(CUTE(CompileTimeArithmetic_unit_value_power));
	s.push_back(CUTE(CompileTimeArithmetic_unit_value_sqrt));
#endif
	return s;
}
