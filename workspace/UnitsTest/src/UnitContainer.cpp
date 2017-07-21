#include "UnitContainer.h"
#include "cute.h"
#include "common_for_tests.h"
#include <iomanip>


//#ifdef _MSC_VER
//#	if (_MSC_VER >= 1900)
void UnitContainer_trivial()
{
	ASSERT((std::is_trivial<meter_t>::value));
	ASSERT((std::is_trivially_assignable<meter_t, meter_t>::value));
	ASSERT((std::is_trivially_constructible<meter_t>::value));
	ASSERT((std::is_trivially_copy_assignable<meter_t>::value));
	ASSERT((std::is_trivially_copy_constructible<meter_t>::value));
	ASSERT((std::is_trivially_copyable<meter_t>::value));
	ASSERT((std::is_trivially_default_constructible<meter_t>::value));
	ASSERT((std::is_trivially_destructible<meter_t>::value));
	ASSERT((std::is_trivially_move_assignable<meter_t>::value));
	ASSERT((std::is_trivially_move_constructible<meter_t>::value));

	ASSERT((std::is_trivial<dB_t>::value));
	ASSERT((std::is_trivially_assignable<dB_t, dB_t>::value));
	ASSERT((std::is_trivially_constructible<dB_t>::value));
	ASSERT((std::is_trivially_copy_assignable<dB_t>::value));
	ASSERT((std::is_trivially_copy_constructible<dB_t>::value));
	ASSERT((std::is_trivially_copyable<dB_t>::value));
	ASSERT((std::is_trivially_default_constructible<dB_t>::value));
	ASSERT((std::is_trivially_destructible<dB_t>::value));
	ASSERT((std::is_trivially_move_assignable<dB_t>::value));
	ASSERT((std::is_trivially_move_constructible<dB_t>::value));
}
//#	endif
//#endif

void UnitContainer_has_value_member()
{
	ASSERT((traits::has_value_member<linear_scale<double>, double>));
	ASSERT(!(traits::has_value_member<meter, double>));
}

void UnitContainer_make_unit()
{
	auto dist = units::make_quantity<meter_t>(5);
	ASSERT_EQUAL(meter_t(5), dist);
}

void UnitContainer_unitTypeAddition()
{
	// units
	meter_t a_m(1.0), c_m;
	foot_t b_ft(3.28084);

	double d = convert<feet, meters>(b_ft());
	ASSERT_EQUAL_DELTA(1.0, d, 5.0e-5);

	c_m = a_m + b_ft;
	ASSERT_EQUAL_DELTA(2.0, c_m(), 5.0e-5);

	c_m = b_ft + meter_t(3);
	ASSERT_EQUAL_DELTA(4.0, c_m(), 5.0e-5);

	auto e_ft = b_ft + meter_t(3);
	ASSERT_EQUAL_DELTA(13.12336, e_ft(), 5.0e-6);

	// scalar
	scalar_t sresult = scalar_t(1.0) + scalar_t(1.0);
	ASSERT_EQUAL_DELTA(2.0, sresult, 5.0e-6);

	sresult = scalar_t(1.0) + 1.0;
	ASSERT_EQUAL_DELTA(2.0, sresult, 5.0e-6);

	sresult = 1.0 + scalar_t(1.0);
	ASSERT_EQUAL_DELTA(2.0, sresult, 5.0e-6);

	d = scalar_t(1.0) + scalar_t(1.0);
	ASSERT_EQUAL_DELTA(2.0, d, 5.0e-6);

	d = scalar_t(1.0) + 1.0;
	ASSERT_EQUAL_DELTA(2.0, d, 5.0e-6);

	d = 1.0 + scalar_t(1.0);
	ASSERT_EQUAL_DELTA(2.0, d, 5.0e-6);
}

void UnitContainer_unitTypeSubtraction()
{
	meter_t a_m(1.0), c_m;
	foot_t b_ft(3.28084);

	c_m = a_m - b_ft;
	ASSERT_EQUAL_DELTA(0.0, c_m(), 5.0e-5);

	c_m = b_ft - meter_t(1);
	ASSERT_EQUAL_DELTA(0.0, c_m(), 5.0e-5);

	auto e_ft = b_ft - meter_t(1);
	ASSERT_EQUAL_DELTA(0.0, e_ft(), 5.0e-6);

	scalar_t sresult = scalar_t(1.0) - scalar_t(1.0);
	ASSERT_EQUAL_DELTA(0.0, sresult, 5.0e-6);

	sresult = scalar_t(1.0) - 1.0;
	ASSERT_EQUAL_DELTA(0.0, sresult, 5.0e-6);

	sresult = 1.0 - scalar_t(1.0);
	ASSERT_EQUAL_DELTA(0.0, sresult, 5.0e-6);

	double d = scalar_t(1.0) - scalar_t(1.0);
	ASSERT_EQUAL_DELTA(0.0, d, 5.0e-6);

	d = scalar_t(1.0) - 1.0;
	ASSERT_EQUAL_DELTA(0.0, d, 5.0e-6);

	d = 1.0 - scalar_t(1.0);
	ASSERT_EQUAL_DELTA(0.0, d, 5.0e-6);
}

void UnitContainer_unitTypeMultiplication()
{
	meter_t a_m(1.0), b_m(2.0);
	foot_t a_ft(3.28084);

	auto c_m2 = a_m * b_m;
	ASSERT_EQUAL_DELTA(2.0, c_m2(), 5.0e-5);

	c_m2 = b_m * meter_t(2);
	ASSERT_EQUAL_DELTA(4.0, c_m2(), 5.0e-5);

	c_m2 = b_m *a_ft;
	ASSERT_EQUAL_DELTA(2.0, c_m2(), 5.0e-5);

	auto c_m = b_m * 2.0;
	ASSERT_EQUAL_DELTA(4.0, c_m(), 5.0e-5);

	c_m = 2.0 * b_m;
	ASSERT_EQUAL_DELTA(4.0, c_m(), 5.0e-5);

	double convert = scalar_t(3.14);
	ASSERT_EQUAL_DELTA(3.14, convert, 5.0e-5);

	scalar_t sresult = scalar_t(5.0) * scalar_t(4.0);
	ASSERT_EQUAL_DELTA(20.0, sresult(), 5.0e-5);

	sresult = scalar_t(5.0) * 4.0;
	ASSERT_EQUAL_DELTA(20.0, sresult(), 5.0e-5);

	sresult = 4.0 * scalar_t(5.0);
	ASSERT_EQUAL_DELTA(20.0, sresult(), 5.0e-5);

	double result = scalar_t(5.0) * scalar_t(4.0);
	ASSERT_EQUAL_DELTA(20.0, result, 5.0e-5);

	result = scalar_t(5.0) * 4.0;
	ASSERT_EQUAL_DELTA(20.0, result, 5.0e-5);

	result = 4.0 * scalar_t(5.0);
	ASSERT_EQUAL_DELTA(20.0, result, 5.0e-5);
}

void UnitContainer_unitTypeMixedUnitMultiplication()
{
	meter_t a_m(1.0);
	foot_t b_ft(3.28084);
	quantity<inverse<meter>> i_m(2.0);

	// resultant unit is square of leftmost unit
	auto c_m2 = a_m * b_ft;
	ASSERT_EQUAL_DELTA(1.0, c_m2(), 5.0e-5);

	auto c_ft2 = b_ft * a_m;
	ASSERT_EQUAL_DELTA(10.7639111056, c_ft2(), 5.0e-7);

	// you can get whatever (compatible) type you want if you ask explicitly
	square_meter_t d_m2 = b_ft * a_m;
	ASSERT_EQUAL_DELTA(1.0, d_m2(), 5.0e-5);

	// a unit times a sclar ends up with the same units.
	meter_t e_m = a_m * scalar_t(3.0);
	ASSERT_EQUAL_DELTA(3.0, e_m(), 5.0e-5);

	e_m = scalar_t(4.0) * a_m;
	ASSERT_EQUAL_DELTA(4.0, e_m(), 5.0e-5);

	// unit times its inverse results in a scalar
	scalar_t s = a_m * i_m;
	ASSERT_EQUAL_DELTA(2.0, s, 5.0e-5);

	c_m2 = b_ft * meter_t(2);
	ASSERT_EQUAL_DELTA(2.0, c_m2(), 5.0e-5);

	auto e_ft2 = b_ft * meter_t(3);
	ASSERT_EQUAL_DELTA(32.2917333168, e_ft2(), 5.0e-6);

	auto mps = meter_t(10.0) * quantity<inverse<seconds>>(1.0);
	ASSERT_EQUAL(mps, meters_per_second_t(10));
}

void UnitContainer_unitTypeScalarMultiplication()
{
	meter_t a_m(1.0);
	foot_t b_ft(3.28084);

	auto result_m = scalar_t(3.0) * a_m;
	ASSERT_EQUAL_DELTA(3.0, result_m(), 5.0e-5);

	result_m = a_m * scalar_t(4.0);
	ASSERT_EQUAL_DELTA(4.0, result_m(), 5.0e-5);

	result_m = 3.0 * a_m;
	ASSERT_EQUAL_DELTA(3.0, result_m(), 5.0e-5);

	result_m = a_m * 4.0;
	ASSERT_EQUAL_DELTA(4.0, result_m(), 5.0e-5);

	bool isSame = std::is_same<decltype(result_m), meter_t>::value;
	ASSERT(isSame);
}

void UnitContainer_unitTypeDivision()
{
	meter_t a_m(1.0), b_m(2.0);
	foot_t a_ft(3.28084);
	second_t a_sec(10.0);
	bool isSame;

	auto c = a_m / a_ft;
	ASSERT_EQUAL_DELTA(1.0, c, 5.0e-5);
	isSame = std::is_same<decltype(c), scalar_t>::value;
	ASSERT(isSame);

	c = a_m / b_m;
	ASSERT_EQUAL_DELTA(0.5, c, 5.0e-5);
	isSame = std::is_same<decltype(c), scalar_t>::value;
	ASSERT(isSame);

	c = a_ft / a_m;
	ASSERT_EQUAL_DELTA(1.0, c, 5.0e-5);
	isSame = std::is_same<decltype(c), scalar_t>::value;
	ASSERT(isSame);

	c = scalar_t(1.0) / 2.0;
	ASSERT_EQUAL_DELTA(0.5, c, 5.0e-5);
	isSame = std::is_same<decltype(c), scalar_t>::value;
	ASSERT(isSame);

	c = 1.0 / scalar_t(2.0);
	ASSERT_EQUAL_DELTA(0.5, c, 5.0e-5);
	isSame = std::is_same<decltype(c), scalar_t>::value;
	ASSERT(isSame);

	double d = scalar_t(1.0) / 2.0;
	ASSERT_EQUAL_DELTA(0.5, d, 5.0e-5);

	auto e = a_m / a_sec;
	ASSERT_EQUAL_DELTA(0.1, e(), 5.0e-5);
	isSame = std::is_same<decltype(e), meters_per_second_t>::value;
	ASSERT(isSame);

	auto f = a_m / 8.0;
	ASSERT_EQUAL_DELTA(0.125, f(), 5.0e-5);
	isSame = std::is_same<decltype(f), meter_t>::value;
	ASSERT(isSame);

	auto g = 4.0 / b_m;
	ASSERT_EQUAL_DELTA(2.0, g(), 5.0e-5);
	isSame = std::is_same<decltype(g), quantity<inverse<meters>>>::value;
	ASSERT(isSame);

	auto mph = mile_t(60.0) / hour_t(1.0);
	meters_per_second_t mps = mph;
	ASSERT_EQUAL_DELTA(26.8224, mps(), 5.0e-5);
}

void UnitContainer_compoundAssignmentAddition()
{
	// units
	meter_t a(0.0);
	a += meter_t(1.0);

	ASSERT_EQUAL(meter_t(1.0), a);

	a += foot_t(meter_t(1));

	ASSERT_EQUAL(meter_t(2.0), a);

	// scalars
	scalar_t b(0);
	b += scalar_t(1.0);

	ASSERT_EQUAL(scalar_t(1.0), b);

	b += 1;

	ASSERT_EQUAL(scalar_t(2.0), b);
}

void UnitContainer_compoundAssignmentSubtraction()
{
	// units
	meter_t a(2.0);
	a -= meter_t(1.0);

	ASSERT_EQUAL(meter_t(1.0), a);

	a -= foot_t(meter_t(1));

	ASSERT_EQUAL(meter_t(0.0), a);

	// scalars
	scalar_t b(2);
	b -= scalar_t(1.0);

	ASSERT_EQUAL(scalar_t(1.0), b);

	b -= 1;

	ASSERT_EQUAL(scalar_t(0), b);
}

void UnitContainer_compoundAssignmentMultiplication()
{
	// units
	meter_t a(2.0);
	a *= scalar_t(2.0);

	ASSERT_EQUAL(meter_t(4.0), a);

	a *= 2.0;

	ASSERT_EQUAL(meter_t(8.0), a);

	// scalars
	scalar_t b(2);
	b *= scalar_t(2.0);

	ASSERT_EQUAL(scalar_t(4.0), b);

	b *= 2;

	ASSERT_EQUAL(scalar_t(8.0), b);
}

void UnitContainer_compoundAssignmentDivision()
{
	// units
	meter_t a(8.0);
	a /= scalar_t(2.0);

	ASSERT_EQUAL(meter_t(4.0), a);

	a /= 2.0;

	ASSERT_EQUAL(meter_t(2.0), a);

	// scalars
	scalar_t b(8);
	b /= scalar_t(2.0);

	ASSERT_EQUAL(scalar_t(4.0), b);

	b /= 2;

	ASSERT_EQUAL(scalar_t(2.0), b);
}

void UnitContainer_scalarTypeImplicitConversion()
{
	double test = scalar_t(3.0);
	ASSERT_EQUAL(3.0, test);

	scalar_t testS = 3.0;
	ASSERT_EQUAL(3.0, testS);


	scalar_t test3(ppm_t(10));
	ASSERT_EQUAL(0.00001, test3);

	scalar_t test4;
	test4 = ppm_t(1);
	ASSERT_EQUAL(0.000001, test4);
}

void UnitContainer_valueMethod()
{
	double test = meter_t(3.0).to<double>();
	ASSERT_EQUAL(3.0, test);

	auto test2 = meter_t(4.0).value();
	ASSERT_EQUAL(4.0, test2);
	ASSERT((std::is_same<decltype(test2), double>::value));
}

void UnitContainer_convertMethod()
{
	double test = meter_t(3.0).convert<feet>().to<double>();
	ASSERT_EQUAL_DELTA(9.84252, test, 5.0e-6);
}



void UnitContainer_cout() {
	std::ostringstream out{};
	out << degree_t(349.87);
	ASSERT_EQUAL("349.87 deg", out.str());

	out.str("");
	out << meter_t(1.0);
	ASSERT_EQUAL("1 m", out.str());

	out.str("");
	out << dB_t(31.0);
	ASSERT_EQUAL("31 dB", out.str());

	out.str("");
	out << volt_t(21.79);

	ASSERT_EQUAL("21.79 V", out.str());

	out.str("");
	out << dBW_t(12.0);

	ASSERT_EQUAL("12 dBW", out.str());

	out.str("");
	out << dBm_t(120.0);

	ASSERT_EQUAL("120 dBm", out.str());

	out.str("");
	out << miles_per_hour_t(72.1);

	ASSERT_EQUAL("72.1 mph", out.str());

	// undefined unit
	out.str("");
	out << units::math::cpow<4>(meter_t(2));

	ASSERT_EQUAL("16 m^4", out.str());

	out.str("");
	out << units::math::cpow<3>(foot_t(2));

	ASSERT_EQUAL("8 cu_ft", out.str());

	out.str("");
	out << std::setprecision(9) << units::math::cpow<4>(foot_t(2));

	ASSERT_EQUAL("0.138095597 m^4", out.str());

	// constants
	out.str("");
	out << std::setprecision(8) << constants::k_B;

#if defined(_MSC_VER) && (_MSC_VER <= 1800)
	ASSERT_EQUAL("1.3806485e-023 m^2 kg s^-2 K^-1", out.str());
#else
	ASSERT_EQUAL("1.3806485e-23 m^2 kg s^-2 K^-1", out.str());
#endif

	out.str("");
	out << std::setprecision(9) << constants::mu_B;

#if defined(_MSC_VER) && (_MSC_VER <= 1800)
	ASSERT_EQUAL("9.27400999e-024 m^2 A", out.str());
#else
	ASSERT_EQUAL("9.27400999e-24 m^2 A", out.str());
#endif

	out.str("");
	out << std::setprecision(7) << constants::sigma;

#if defined(_MSC_VER) && (_MSC_VER <= 1800)
	ASSERT_EQUAL("5.670367e-008 kg s^-3 K^-4", out.str());
#else
	ASSERT_EQUAL("5.670367e-08 kg s^-3 K^-4", out.str());
#endif
}


void UnitContainer_negative()
{
	meter_t a(5.3);
	meter_t b(-5.3);
	ASSERT_EQUAL_DELTA(a.to<double>(), -b.to<double>(), 5.0e-320);
	ASSERT_EQUAL_DELTA(b.to<double>(), -a.to<double>(), 5.0e-320);

	dB_t c(2.87);
	dB_t d(-2.87);
	ASSERT_EQUAL_DELTA(c.to<double>(), -d.to<double>(), 5.0e-320);
	ASSERT_EQUAL_DELTA(d.to<double>(), -c.to<double>(), 5.0e-320);

	ppm_t e = -1 * ppm_t(10);
	ASSERT_EQUAL(e, -ppm_t(10));
	ASSERT_EQUAL_DELTA(-0.00001, e, 5.0e-10);
}

void UnitContainer_concentration()
{
	ppb_t a(ppm_t(1));
	ASSERT_EQUAL(ppb_t(1000), a);
	ASSERT_EQUAL(0.000001, a);
	ASSERT_EQUAL(0.000001, a.to<double>());

	scalar_t b(ppm_t(1));
	ASSERT_EQUAL(0.000001, b);

	scalar_t c = ppb_t(1);
	ASSERT_EQUAL(0.000000001, c);
}

void UnitContainer_dBConversion()
{
	dBW_t a_dbw(23.1);
	watt_t a_w = a_dbw;
	dBm_t a_dbm = a_dbw;

	ASSERT_EQUAL_DELTA(204.173794, a_w(), 5.0e-7);
	ASSERT_EQUAL_DELTA(53.1, a_dbm(), 5.0e-7);

	milliwatt_t b_mw(100000.0);
	watt_t b_w = b_mw;
	dBm_t b_dbm = b_mw;
	dBW_t b_dbw = b_mw;

	ASSERT_EQUAL_DELTA(100.0, b_w(), 5.0e-7);
	ASSERT_EQUAL_DELTA(50.0, b_dbm(), 5.0e-7);
	ASSERT_EQUAL_DELTA(20.0, b_dbw(), 5.0e-7);
}

void UnitContainer_dBAddition()
{
	bool isSame;

	auto result_dbw = dBW_t(10.0) + dB_t(30.0);
	ASSERT_EQUAL_DELTA(40.0, result_dbw(), 5.0e-5);
	result_dbw = dB_t(12.0) + dBW_t(30.0);
	ASSERT_EQUAL_DELTA(42.0, result_dbw(), 5.0e-5);
	isSame = std::is_same<decltype(result_dbw), dBW_t>::value;
	ASSERT(isSame);

	auto result_dbm = dB_t(30.0) + dBm_t(20.0);
	ASSERT_EQUAL_DELTA(50.0, result_dbm(), 5.0e-5);

	// adding dBW to dBW is something you probably shouldn't do, but let's see if it works...
	auto result_dBW2 = dBW_t(10.0) + dBm_t(40.0);
	ASSERT_EQUAL_DELTA(20.0, result_dBW2(), 5.0e-5);
	isSame = std::is_same<decltype(result_dBW2), quantity<squared<watts>, double, decibel_scale>>::value;
	ASSERT(isSame);
}

void UnitContainer_dBSubtraction()
{
	bool isSame;

	auto result_dbw = dBW_t(10.0) - dB_t(30.0);
	ASSERT_EQUAL_DELTA(-20.0, result_dbw(), 5.0e-5);
	isSame = std::is_same<decltype(result_dbw), dBW_t>::value;
	ASSERT(isSame);

	auto result_dbm = dBm_t(100.0) - dB_t(30.0);
	ASSERT_EQUAL_DELTA(70.0, result_dbm(), 5.0e-5);
	isSame = std::is_same<decltype(result_dbm), dBm_t>::value;
	ASSERT(isSame);

	auto result_db = dBW_t(100.0) - dBW_t(80.0);
	ASSERT_EQUAL_DELTA(20.0, result_db(), 5.0e-5);
	isSame = std::is_same<decltype(result_db), dB_t>::value;
	ASSERT(isSame);

	result_db = dB_t(100.0) - dB_t(80.0);
	ASSERT_EQUAL_DELTA(20.0, result_db(), 5.0e-5);
	isSame = std::is_same<decltype(result_db), dB_t>::value;
	ASSERT(isSame);
}

void UnitContainer_unit_cast()
{
	meter_t		test1(5.7);
	hectare_t	test2(16);

	double		dResult1 = 5.7;

	double		dResult2 = 16;
	int			iResult2 = 16;

	ASSERT_EQUAL(dResult1, unit_cast<double>(test1));
	ASSERT_EQUAL(dResult2, unit_cast<double>(test2));
	ASSERT_EQUAL(iResult2, unit_cast<int>(test2));

	ASSERT((std::is_same<double, decltype(unit_cast<double>(test1))>::value));
	ASSERT((std::is_same<int, decltype(unit_cast<int>(test2))>::value));
}

// literal syntax is only supported in GCC 4.7+ and MSVC2015+
#if !defined(_MSC_VER) || _MSC_VER > 1800
void UnitContainer_literals()
{
	// basic functionality testing
	ASSERT((std::is_same<decltype(16.2_m), meter_t>::value));
	ASSERT(meter_t(16.2) == 16.2_m);
	ASSERT(meter_t(16) == 16_m);

	ASSERT((std::is_same<decltype(11.2_ft), foot_t>::value));
	ASSERT(foot_t(11.2) == 11.2_ft);
	ASSERT(foot_t(11) == 11_ft);

	// auto using literal syntax
	auto x = 10.0_m;
	ASSERT((std::is_same<decltype(x), meter_t>::value));
	ASSERT(meter_t(10) == x);

	// conversion using literal syntax
	foot_t y = 0.3048_m;
	ASSERT(1_ft == y);

	// Pythagorean theorem
	meter_t a = 3_m;
	meter_t b = 4_m;
	meter_t c = sqrt(pow<2>(a) + pow<2>(b));
	ASSERT(c == 5_m);
}
#endif

cute::suite make_suite_UnitContainer() {
	cute::suite s { };
	s.push_back(CUTE(UnitContainer_trivial)); //?
	s.push_back(CUTE(UnitContainer_has_value_member));
	s.push_back(CUTE(UnitContainer_make_unit));
	s.push_back(CUTE(UnitContainer_unitTypeAddition));
	s.push_back(CUTE(UnitContainer_unitTypeSubtraction));
	s.push_back(CUTE(UnitContainer_unitTypeMultiplication));
	s.push_back(CUTE(UnitContainer_unitTypeMixedUnitMultiplication));
	s.push_back(CUTE(UnitContainer_unitTypeScalarMultiplication));
	s.push_back(CUTE(UnitContainer_unitTypeDivision));
	s.push_back(CUTE(UnitContainer_compoundAssignmentAddition));
	s.push_back(CUTE(UnitContainer_compoundAssignmentSubtraction));
	s.push_back(CUTE(UnitContainer_compoundAssignmentMultiplication));
	s.push_back(CUTE(UnitContainer_compoundAssignmentDivision));
	s.push_back(CUTE(UnitContainer_scalarTypeImplicitConversion));
	s.push_back(CUTE(UnitContainer_valueMethod));
	s.push_back(CUTE(UnitContainer_convertMethod));
	s.push_back(CUTE(UnitContainer_cout));
	s.push_back(CUTE(UnitContainer_negative));
	s.push_back(CUTE(UnitContainer_concentration));
	s.push_back(CUTE(UnitContainer_dBConversion));
	s.push_back(CUTE(UnitContainer_dBAddition));
	s.push_back(CUTE(UnitContainer_dBSubtraction));
	s.push_back(CUTE(UnitContainer_unit_cast));
	s.push_back(CUTE(UnitContainer_literals));
	return s;
}
