#include "common_for_tests.h"
#include "UnitConversion.h"
#include "cute.h"
namespace{
void UnitConversion_length()
{
	double test = convert<meters, nanometers>(0.000000001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-20);
	test = convert<meters, micrometers>(0.000001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-20);
	test = convert<meters, millimeters>(0.001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-20);
	test = convert<meters, centimeters>(0.01);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-20);
	test = convert<meters, kilometers>(1000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-20);
	test = convert<meters, meters>(1.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-20);
	test = convert<meters, feet>(0.3048);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-20);
	test = convert<meters, miles>(1609.344);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-20);
	test = convert<meters, inches>(0.0254);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-20);
	test = convert<meters, nauticalMiles>(1852.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-20);
	test = convert<meters, astronicalUnits>(149597870700.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-20);
	test = convert<meters, lightyears>(9460730472580800.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-20);
	test = convert<meters, parsec>(3.08567758e16);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e7);

	test = convert<feet, feet>(6.3);
	ASSERT_EQUAL_DELTA(6.3, test, 5.0e-5);
	test = convert<feet, inches>(6.0);
	ASSERT_EQUAL_DELTA(72.0, test, 5.0e-5);
	test = convert<inches, feet>(6.0);
	ASSERT_EQUAL_DELTA(0.5, test, 5.0e-5);
	test = convert<meter, feet>(1.0);
	ASSERT_EQUAL_DELTA(3.28084, test, 5.0e-5);
	test = convert<miles, nauticalMiles>(6.3);
	ASSERT_EQUAL_DELTA(5.47455, test, 5.0e-6);
	test = convert<miles, meters>(11.0);
	ASSERT_EQUAL_DELTA(17702.8, test, 5.0e-2);
	test = convert<meters, chains>(1.0);
	ASSERT_EQUAL_DELTA(0.0497097, test, 5.0e-7);

}

void UnitConversion_mass()
{
	double test = convert<kilograms, grams>(1.0e-3);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-6);
	test = convert<kilograms, micrograms>(1.0e-9);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-6);
	test = convert<kilograms, milligrams>(1.0e-6);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-6);
	test = convert<kilograms, kilograms>(1.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-6);
	test = convert<kilograms, metric_tons>(1000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-6);
	test = convert<kilograms, pounds>(0.453592);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-6);
	test = convert<kilograms, long_tons>(1016.05);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-6);
	test = convert<kilograms, short_tons>(907.185);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-6);
	test = convert<kilograms, mass::ounces>(0.0283495);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-6);
	test = convert<kilograms, carats>(0.0002);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-6);
	test = convert<slugs, kilograms>(1.0);
	ASSERT_EQUAL_DELTA(14.593903, test, 5.0e-7);

	test = convert<pounds, carats>(6.3);
	ASSERT_EQUAL_DELTA(14288.2, test, 5.0e-2);
}

void UnitConversion_time()
{
	double result = 0;
	double daysPerYear = 365;
	double hoursPerDay = 24;
	double minsPerHour = 60;
	double secsPerMin = 60;
	double daysPerWeek = 7;

	result = 2 * daysPerYear * hoursPerDay * minsPerHour * secsPerMin *
		(1 / minsPerHour) * (1 / secsPerMin) * (1 / hoursPerDay) * (1 / daysPerWeek);
	ASSERT_EQUAL_DELTA(104.286, result, 5.0e-4);

	year_t twoYears(2.0);
	week_t twoYearsInWeeks = twoYears;
	ASSERT_EQUAL_DELTA(week_t(104.286).to<double>(), twoYearsInWeeks.to<double>(), 5.0e-4);

	double test = convert<seconds, seconds>(1.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-20);
	test = convert<seconds, nanoseconds>(1.0e-9);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-20);
	test = convert<seconds, microseconds>(1.0e-6);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-20);
	test = convert<seconds, milliseconds>(1.0e-3);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-20);
	test = convert<seconds, minutes>(60.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-20);
	test = convert<seconds, hours>(3600.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-20);
	test = convert<seconds, days>(86400.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-20);
	test = convert<seconds, weeks>(604800.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-20);
	test = convert<seconds, years>(3.154e7);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e3);

	test = convert<years, weeks>(2.0);
	ASSERT_EQUAL_DELTA(104.2857142857143, test, 5.0e-14);
	test = convert<hours, minutes>(4.0);
	ASSERT_EQUAL_DELTA(240.0, test, 5.0e-14);
	test = convert<julian_years, days>(1.0);
	ASSERT_EQUAL_DELTA(365.25, test, 5.0e-14);
	test = convert<gregorian_years, days>(1.0);
	ASSERT_EQUAL_DELTA(365.2425, test, 5.0e-14);
}

void UnitConversion_angle()
{
	angle::degree_t quarterCircleDeg(90.0);
	angle::radian_t quarterCircleRad = quarterCircleDeg;
	ASSERT_EQUAL_DELTA(angle::radian_t(constants::detail::PI_VAL<> / 2.0).to<double>(), quarterCircleRad.to<double>(), 5.0e-12);

	double test = convert<angle::radians, angle::radians>(1.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-20);
	test = convert<angle::radians, angle::milliradians>(0.001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-4);
	test = convert<angle::radians, angle::degrees>(0.0174533);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-7);
	test = convert<angle::radians, angle::arcminutes>(0.000290888);
	ASSERT_EQUAL_DELTA(0.99999928265913, test, 5.0e-8);
	test = convert<angle::radians, angle::arcseconds>(4.8481e-6);
	ASSERT_EQUAL_DELTA(0.999992407, test, 5.0e-10);
	test = convert<angle::radians, angle::turns>(6.28319);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-6);
	test = convert<angle::radians, angle::gradians>(0.015708);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-6);

	test = convert<angle::radians, angle::radians>(2.1);
	ASSERT_EQUAL_DELTA(2.1, test, 5.0e-6);
	test = convert<angle::arcseconds, angle::gradians>(2.1);
	ASSERT_EQUAL_DELTA(0.000648148, test, 5.0e-6);
	test = convert<angle::radians, angle::degrees>(constants::detail::PI_VAL<>);
	ASSERT_EQUAL_DELTA(180.0, test, 5.0e-6);
	test = convert<angle::degrees, angle::radians>(90.0);
	ASSERT_EQUAL_DELTA(constants::detail::PI_VAL<> / 2, test, 5.0e-6);
}

void UnitConversion_current()
{
	double test = convert<current::A, current::mA>(2.1);
	ASSERT_EQUAL_DELTA(2100.0, test, 5.0e-6);
}

void UnitConversion_temperature()
{
	// temp conversion are weird/hard since they involve translations AND scaling.
	double test = convert<kelvin, kelvin>(72.0);
	ASSERT_EQUAL_DELTA(72.0, test, 5.0e-5);
	test = convert<fahrenheit, fahrenheit>(72.0);
	ASSERT_EQUAL_DELTA(72.0, test, 5.0e-5);
	test = convert<kelvin, fahrenheit>(300.0);
	ASSERT_EQUAL_DELTA(80.33, test, 5.0e-5);
	test = convert<fahrenheit, kelvin>(451.0);
	ASSERT_EQUAL_DELTA(505.928, test, 5.0e-4);
	test = convert<kelvin, celsius>(300.0);
	ASSERT_EQUAL_DELTA(26.85, test, 5.0e-3);
	test = convert<celsius, kelvin>(451.0);
	ASSERT_EQUAL_DELTA(724.15, test, 5.0e-3);
	test = convert<fahrenheit, celsius>(72.0);
	ASSERT_EQUAL_DELTA(22.2222, test, 5.0e-5);
	test = convert<celsius, fahrenheit>(100.0);
	ASSERT_EQUAL_DELTA(212.0, test, 5.0e-5);
	test = convert<fahrenheit, celsius>(32.0);
	ASSERT_EQUAL_DELTA(0.0, test, 5.0e-5);
	test = convert<celsius, fahrenheit>(0.0);
	ASSERT_EQUAL_DELTA(32.0, test, 5.0e-5);
	test = convert<rankine, kelvin>(100.0);
	ASSERT_EQUAL_DELTA(55.5556, test, 5.0e-5);
	test = convert<kelvin, rankine>(100.0);
	ASSERT_EQUAL_DELTA(180.0, test, 5.0e-5);
	test = convert<fahrenheit, rankine>(100.0);
	ASSERT_EQUAL_DELTA(559.67, test, 5.0e-5);
	test = convert<rankine, fahrenheit>(72.0);
	ASSERT_EQUAL_DELTA(-387.67, test, 5.0e-5);
	test = convert<reaumur, kelvin>(100.0);
	ASSERT_EQUAL_DELTA(398.0, test, 5.0e-1);
	test = convert<reaumur, celsius>(80.0);
	ASSERT_EQUAL_DELTA(100.0, test, 5.0e-5);
	test = convert<celsius, reaumur>(212.0);
	ASSERT_EQUAL_DELTA(169.6, test, 5.0e-2);
	test = convert<reaumur, fahrenheit>(80.0);
	ASSERT_EQUAL_DELTA(212.0, test, 5.0e-5);
	test = convert<fahrenheit, reaumur>(37.0);
	ASSERT_EQUAL_DELTA(2.222, test, 5.0e-3);
}

void UnitConversion_luminous_intensity()
{
	double test = convert<candela, millicandela>(72.0);
	ASSERT_EQUAL_DELTA(72000.0, test, 5.0e-5);
	test = convert<millicandela, candela>(376.0);
	ASSERT_EQUAL_DELTA(0.376, test, 5.0e-5);
}

void UnitConversion_solid_angle()
{
	bool same = std::is_same<traits::base_unit_of<steradians>, traits::base_unit_of<degrees_squared>>::value;
	ASSERT(same);

	double test = convert<steradians, steradians>(72.0);
	ASSERT_EQUAL_DELTA(72.0, test, 5.0e-5);
	test = convert<steradians, degrees_squared>(1.0);
	ASSERT_EQUAL_DELTA(3282.8, test, 5.0e-2);
	test = convert<steradians, spats>(8.0);
	ASSERT_EQUAL_DELTA(0.636619772367582, test, 5.0e-14);
	test = convert<degrees_squared, steradians>(3282.8);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<degrees_squared, degrees_squared>(72.0);
	ASSERT_EQUAL_DELTA(72.0, test, 5.0e-5);
	test = convert<degrees_squared, spats>(3282.8);
	ASSERT_EQUAL_DELTA(1.0 / (4 * constants::detail::PI_VAL<>), test, 5.0e-5);
	test = convert<spats, steradians>(1.0 / (4 * constants::detail::PI_VAL<>));
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-14);
	test = convert<spats, degrees_squared>(1.0 / (4 * constants::detail::PI_VAL<>));
	ASSERT_EQUAL_DELTA(3282.8, test, 5.0e-2);
	test = convert<spats, spats>(72.0);
	ASSERT_EQUAL_DELTA(72.0, test, 5.0e-5);
}

void UnitConversion_frequency()
{
	double test = convert<hertz, kilohertz>(63000.0);
	ASSERT_EQUAL_DELTA(63.0, test, 5.0e-5);
	test = convert<hertz, hertz>(6.3);
	ASSERT_EQUAL_DELTA(6.3, test, 5.0e-5);
	test = convert<kilohertz, hertz>(5.0);
	ASSERT_EQUAL_DELTA(5000.0, test, 5.0e-5);
	test = convert<megahertz, hertz>(1.0);
	ASSERT_EQUAL_DELTA(1.0e6, test, 5.0e-5);
}

void UnitConversion_velocity()
{
	bool same = std::is_same<meters_per_second, unit<std_peter::ratio<1>, category::velocity_unit>>::value;
	ASSERT(same);
	same = traits::is_convertible_unit<miles_per_hour, meters_per_second>;
	ASSERT(same);

	double test = convert<meters_per_second, miles_per_hour>(1250.0);
	ASSERT_EQUAL_DELTA(2796.17, test, 5.0e-3);
	test = convert<feet_per_second, kilometers_per_hour>(2796.17);
	ASSERT_EQUAL_DELTA(3068.181418, test, 5.0e-7);
	test = convert<knots, miles_per_hour>(600.0);
	ASSERT_EQUAL_DELTA(690.468, test, 5.0e-4);
	test = convert<miles_per_hour, feet_per_second>(120.0);
	ASSERT_EQUAL_DELTA(176.0, test, 5.0e-5);
	test = convert<feet_per_second, meters_per_second>(10.0);
	ASSERT_EQUAL_DELTA(3.048, test, 5.0e-5);
}

void UnitConversion_angular_velocity()
{
	bool same = std::is_same<radians_per_second, unit<std_peter::ratio<1>, category::angular_velocity_unit>>::value;
	ASSERT(same);
	same = traits::is_convertible_unit<rpm, radians_per_second>;
	ASSERT(same);

	double test = convert<radians_per_second, milliarcseconds_per_year>(1.0);
	ASSERT_EQUAL_DELTA(6.504e15, test, 1.0e12);
	test = convert<degrees_per_second, radians_per_second>(1.0);
	ASSERT_EQUAL_DELTA(0.0174533, test, 5.0e-8);
	test = convert<rpm, radians_per_second>(1.0);
	ASSERT_EQUAL_DELTA(0.10471975512, test, 5.0e-13);
	test = convert<milliarcseconds_per_year, radians_per_second>(1.0);
	ASSERT_EQUAL_DELTA(1.537e-16, test, 5.0e-20);
}

void UnitConversion_acceleration()
{
	double test = convert<standard_gravity, meters_per_second_squared>(1.0);
	ASSERT_EQUAL_DELTA(9.80665, test, 5.0e-10);
}
void UnitConversion_force()
{
	double test = convert<units::force::newton, units::force::newton>(1.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<units::force::newton, units::force::pounds>(6.3);
	ASSERT_EQUAL_DELTA(1.4163, test, 5.0e-5);
	test = convert<units::force::newton, units::force::dynes>(5.0);
	ASSERT_EQUAL_DELTA(500000.0, test, 5.0e-5);
	test = convert<units::force::newtons, units::force::poundals>(2.1);
	ASSERT_EQUAL_DELTA(15.1893, test, 5.0e-5);
	test = convert<units::force::newtons, units::force::kiloponds>(173.0);
	ASSERT_EQUAL_DELTA(17.6411, test, 5.0e-5);
	test = convert<units::force::poundals, units::force::kiloponds>(21.879);
	ASSERT_EQUAL_DELTA(0.308451933, test, 5.0e-10);
}

void UnitConversion_area()
{
	double test = convert<hectares, acres>(6.3);
	ASSERT_EQUAL_DELTA(15.5676, test, 5.0e-5);
	test = convert<square_miles, square_kilometers>(10.0);
	ASSERT_EQUAL_DELTA(25.8999, test, 5.0e-5);
	test = convert<square_inch, square_meter>(4.0);
	ASSERT_EQUAL_DELTA(0.00258064, test, 5.0e-9);
	test = convert<acre, square_foot>(5.0);
	ASSERT_EQUAL_DELTA(217800.0, test, 5.0e-5);
	test = convert<square_meter, square_foot>(1.0);
	ASSERT_EQUAL_DELTA(10.7639, test, 5.0e-5);
}

void UnitConversion_pressure()
{
	double test = convert<pascals, torr>(1.0);
	ASSERT_EQUAL_DELTA(0.00750062, test, 5.0e-5);
	test = convert<bar, psi>(2.2);
	ASSERT_EQUAL_DELTA(31.9083, test, 5.0e-5);
	test = convert<atmospheres, bar>(4.0);
	ASSERT_EQUAL_DELTA(4.053, test, 5.0e-5);
	test = convert<torr, pascals>(800.0);
	ASSERT_EQUAL_DELTA(106657.89474, test, 5.0e-5);
	test = convert<psi, atmospheres>(38.0);
	ASSERT_EQUAL_DELTA(2.58575, test, 5.0e-5);
	test = convert<psi, pascals>(1.0);
	ASSERT_EQUAL_DELTA(6894.76, test, 5.0e-3);
	test = convert<pascals, bar>(0.25);
	ASSERT_EQUAL_DELTA(2.5e-6, test, 5.0e-5);
	test = convert<torr, atmospheres>(9.0);
	ASSERT_EQUAL_DELTA(0.0118421, test, 5.0e-8);
	test = convert<bar, torr>(12.0);
	ASSERT_EQUAL_DELTA(9000.74, test, 5.0e-3);
	test = convert<atmospheres, psi>(1.0);
	ASSERT_EQUAL_DELTA(14.6959, test, 5.0e-5);
}

void UnitConversion_charge()
{
	double test = convert<coulombs, ampere_hours>(4.0);
	ASSERT_EQUAL_DELTA(0.00111111, test, 5.0e-9);
	test = convert<ampere_hours, coulombs>(1.0);
	ASSERT_EQUAL_DELTA(3600.0, test, 5.0e-6);
}

void UnitConversion_energy()
{
	double test = convert<joules, calories>(8000.000464);
	ASSERT_EQUAL_DELTA(1912.046, test, 5.0e-4);
	test = convert<therms, joules>(12.0);
	ASSERT_EQUAL_DELTA(1.266e+9, test, 5.0e5);
	test = convert<megajoules, watt_hours>(100.0);
	ASSERT_EQUAL_DELTA(27777.778, test, 5.0e-4);
	test = convert<kilocalories, megajoules>(56.0);
	ASSERT_EQUAL_DELTA(0.234304, test, 5.0e-7);
	test = convert<kilojoules, therms>(56.0);
	ASSERT_EQUAL_DELTA(0.000530904, test, 5.0e-5);
	test = convert<british_thermal_units, kilojoules>(18.56399995447);
	ASSERT_EQUAL_DELTA(19.5860568, test, 5.0e-5);
	test = convert<calories, energy::foot_pounds>(18.56399995447);
	ASSERT_EQUAL_DELTA(57.28776190423856, test, 5.0e-5);
	test = convert<megajoules, calories>(1.0);
	ASSERT_EQUAL_DELTA(239006.0, test, 5.0e-1);
	test = convert<kilocalories, kilowatt_hours>(2.0);
	ASSERT_EQUAL_DELTA(0.00232444, test, 5.0e-9);
	test = convert<therms, kilocalories>(0.1);
	ASSERT_EQUAL_DELTA(2521.04, test, 5.0e-3);
	test = convert<watt_hours, megajoules>(67.0);
	ASSERT_EQUAL_DELTA(0.2412, test, 5.0e-5);
	test = convert<british_thermal_units, watt_hours>(100.0);
	ASSERT_EQUAL_DELTA(29.3071, test, 5.0e-5);
	test = convert<calories, BTU>(100.0);
	ASSERT_EQUAL_DELTA(0.396567, test, 5.0e-5);
}

void UnitConversion_power()
{
	double test = convert<compound_unit<energy::foot_pounds, inverse<seconds>>, watts>(550.0);
	ASSERT_EQUAL_DELTA(745.7, test, 5.0e-2);
	test = convert<watts, gigawatts>(1000000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-4);
	test = convert<microwatts, watts>(200000.0);
	ASSERT_EQUAL_DELTA(0.2, test, 5.0e-4);
	test = convert<horsepower, watts>(100.0);
	ASSERT_EQUAL_DELTA(74570.0, test, 5.0e-1);
	test = convert<horsepower, megawatts>(5.0);
	ASSERT_EQUAL_DELTA(0.0037284994, test, 5.0e-7);
	test = convert<kilowatts, horsepower>(232.0);
	ASSERT_EQUAL_DELTA(311.117, test, 5.0e-4);
	test = convert<milliwatts, horsepower>(1001.0);
	ASSERT_EQUAL_DELTA(0.001342363, test, 5.0e-9);
}

void UnitConversion_voltage()
{
	double test;

	test = convert<volts, millivolts>(10.0);
	ASSERT_EQUAL_DELTA(10000.0, test, 5.0e-5);
	test = convert<picovolts, volts>(1000000000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<nanovolts, volts>(1000000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<microvolts, volts>(1000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<millivolts, volts>(1000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<kilovolts, volts>(0.001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<megavolts, volts>(0.000001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<gigavolts, volts>(0.000000001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<statvolts, volts>(299.792458);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<millivolts, statvolts>(1000.0);
	ASSERT_EQUAL_DELTA(299.792458, test, 5.0e-5);
	test = convert<abvolts, nanovolts>(0.1);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<microvolts, abvolts>(0.01);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
}

void UnitConversion_capacitance()
{
	double test;

	test = convert<farads, millifarads>(10.0);
	ASSERT_EQUAL_DELTA(10000.0, test, 5.0e-5);
	test = convert<picofarads, farads>(1000000000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<nanofarads, farads>(1000000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<microfarads, farads>(1000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<millifarads, farads>(1000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<kilofarads, farads>(0.001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<megafarads, farads>(0.000001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<gigafarads, farads>(0.000000001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
}

void UnitConversion_impedance()
{
	double test;

	test = convert<ohms, milliohms>(10.0);
	ASSERT_EQUAL_DELTA(10000.0, test, 5.0e-5);
	test = convert<picoohms, ohms>(1000000000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<nanoohms, ohms>(1000000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<microohms, ohms>(1000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<milliohms, ohms>(1000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<kiloohms, ohms>(0.001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<megaohms, ohms>(0.000001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<gigaohms, ohms>(0.000000001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
}

void UnitConversion_conductance()
{
	double test;

	test = convert<siemens, millisiemens>(10.0);
	ASSERT_EQUAL_DELTA(10000.0, test, 5.0e-5);
	test = convert<picosiemens, siemens>(1000000000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<nanosiemens, siemens>(1000000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<microsiemens, siemens>(1000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<millisiemens, siemens>(1000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<kilosiemens, siemens>(0.001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<megasiemens, siemens>(0.000001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<gigasiemens, siemens>(0.000000001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
}

void UnitConversion_magnetic_flux()
{
	double test;

	test = convert<webers, milliwebers>(10.0);
	ASSERT_EQUAL_DELTA(10000.0, test, 5.0e-5);
	test = convert<picowebers, webers>(1000000000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<nanowebers, webers>(1000000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<microwebers, webers>(1000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<milliwebers, webers>(1000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<kilowebers, webers>(0.001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<megawebers, webers>(0.000001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<gigawebers, webers>(0.000000001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<maxwells, webers>(100000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<nanowebers, maxwells>(10.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
}

void UnitConversion_magnetic_field_strength()
{
	double test;

	test = convert<teslas, milliteslas>(10.0);
	ASSERT_EQUAL_DELTA(10000.0, test, 5.0e-5);
	test = convert<picoteslas, teslas>(1000000000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<nanoteslas, teslas>(1000000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<microteslas, teslas>(1000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<milliteslas, teslas>(1000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<kiloteslas, teslas>(0.001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<megateslas, teslas>(0.000001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<gigateslas, teslas>(0.000000001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<gauss, teslas>(10000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<nanoteslas, gauss>(100000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
}

void UnitConversion_inductance()
{
	double test;

	test = convert<henries, millihenries>(10.0);
	ASSERT_EQUAL_DELTA(10000.0, test, 5.0e-5);
	test = convert<picohenries, henries>(1000000000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<nanohenries, henries>(1000000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<microhenries, henries>(1000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<millihenries, henries>(1000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<kilohenries, henries>(0.001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<megahenries, henries>(0.000001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<gigahenries, henries>(0.000000001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
}

void UnitConversion_luminous_flux()
{
	double test;

	test = convert<lumens, millilumens>(10.0);
	ASSERT_EQUAL_DELTA(10000.0, test, 5.0e-5);
	test = convert<picolumens, lumens>(1000000000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<nanolumens, lumens>(1000000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<microlumens, lumens>(1000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<millilumens, lumens>(1000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<kilolumens, lumens>(0.001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<megalumens, lumens>(0.000001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<gigalumens, lumens>(0.000000001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
}

void UnitConversion_illuminance()
{
	double test;

	test = convert<luxes, milliluxes>(10.0);
	ASSERT_EQUAL_DELTA(10000.0, test, 5.0e-5);
	test = convert<picoluxes, luxes>(1000000000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<nanoluxes, luxes>(1000000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<microluxes, luxes>(1000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<milliluxes, luxes>(1000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<kiloluxes, luxes>(0.001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<megaluxes, luxes>(0.000001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<gigaluxes, luxes>(0.000000001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);

	test = convert<footcandles, luxes>(0.092903);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<lux, lumens_per_square_inch>(1550.0031000062);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-13);
	test = convert<phots, luxes>(0.0001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
}

void UnitConversion_radiation()
{
	double test;

	test = convert<becquerels, millibecquerels>(10.0);
	ASSERT_EQUAL_DELTA(10000.0, test, 5.0e-5);
	test = convert<picobecquerels, becquerels>(1000000000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<nanobecquerels, becquerels>(1000000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<microbecquerels, becquerels>(1000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<millibecquerels, becquerels>(1000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<kilobecquerels, becquerels>(0.001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<megabecquerels, becquerels>(0.000001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<gigabecquerels, becquerels>(0.000000001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);

	test = convert<grays, milligrays>(10.0);
	ASSERT_EQUAL_DELTA(10000.0, test, 5.0e-5);
	test = convert<picograys, grays>(1000000000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<nanograys, grays>(1000000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<micrograys, grays>(1000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<milligrays, grays>(1000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<kilograys, grays>(0.001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<megagrays, grays>(0.000001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<gigagrays, grays>(0.000000001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);

	test = convert<sieverts, millisieverts>(10.0);
	ASSERT_EQUAL_DELTA(10000.0, test, 5.0e-5);
	test = convert<picosieverts, sieverts>(1000000000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<nanosieverts, sieverts>(1000000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<microsieverts, sieverts>(1000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<millisieverts, sieverts>(1000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<kilosieverts, sieverts>(0.001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<megasieverts, sieverts>(0.000001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<gigasieverts, sieverts>(0.000000001);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);

	test = convert<becquerels, curies>(37.0e9);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<becquerels, rutherfords>(1000000.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<rads, grays>(100.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
}

void UnitConversion_torque()
{
	double test;

	test = convert<torque::foot_pounds, newton_meter>(1.0);
	ASSERT_EQUAL_DELTA(1.355817948, test, 5.0e-5);
	test = convert<inch_pounds, newton_meter>(1.0);
	ASSERT_EQUAL_DELTA(0.112984829, test, 5.0e-5);
	test = convert<foot_poundals, newton_meter>(1.0);
	ASSERT_EQUAL_DELTA(4.214011009e-2, test, 5.0e-5);
	test = convert<meter_kilograms, newton_meter>(1.0);
	ASSERT_EQUAL_DELTA(9.80665, test, 5.0e-5);
	test = convert<inch_pound, meter_kilogram>(86.79616930855788);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<foot_poundals, inch_pound>(2.681170713);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);

}

void UnitConversion_volume()
{
	double test;

	test = convert<cubic_meters, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<cubic_millimeters, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(1.0e-9, test, 5.0e-5);
	test = convert<cubic_kilometers, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(1.0e9, test, 5.0e-5);
	test = convert<liters, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(0.001, test, 5.0e-5);
	test = convert<milliliters, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(1.0e-6, test, 5.0e-5);
	test = convert<cubic_inches, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(1.6387e-5, test, 5.0e-10);
	test = convert<cubic_feet, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(0.0283168, test, 5.0e-8);
	test = convert<cubic_yards, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(0.764555, test, 5.0e-7);
	test = convert<cubic_miles, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(4.168e+9, test, 5.0e5);
	test = convert<gallons, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(0.00378541, test, 5.0e-8);
	test = convert<quarts, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(0.000946353, test, 5.0e-10);
	test = convert<pints, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(0.000473176, test, 5.0e-10);
	test = convert<cups, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(0.00024, test, 5.0e-6);
	test = convert<volume::fluid_ounces, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(2.9574e-5, test, 5.0e-5);
	test = convert<barrels, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(0.158987294928, test, 5.0e-13);
	test = convert<bushels, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(0.0352391, test, 5.0e-8);
	test = convert<cords, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(3.62456, test, 5.0e-6);
	test = convert<cubic_fathoms, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(6.11644, test, 5.0e-6);
	test = convert<tablespoons, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(1.4787e-5, test, 5.0e-10);
	test = convert<teaspoons, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(4.9289e-6, test, 5.0e-11);
	test = convert<pinches, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(616.11519921875e-9, test, 5.0e-20);
	test = convert<dashes, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(308.057599609375e-9, test, 5.0e-20);
	test = convert<drops, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(82.14869322916e-9, test, 5.0e-9);
	test = convert<fifths, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(0.00075708236, test, 5.0e-12);
	test = convert<drams, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(3.69669e-6, test, 5.0e-12);
	test = convert<gills, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(0.000118294, test, 5.0e-10);
	test = convert<pecks, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(0.00880977, test, 5.0e-9);
	test = convert<sacks, cubic_meter>(9.4591978);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<shots, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(4.43603e-5, test, 5.0e-11);
	test = convert<strikes, cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(0.07047814033376, test, 5.0e-5);
	test = convert<volume::fluid_ounces, milliliters>(1.0);
	ASSERT_EQUAL_DELTA(29.5735, test, 5.0e-5);
}

void UnitConversion_density()
{
	double test;

	test = convert<kilograms_per_cubic_meter, kilograms_per_cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(1.0, test, 5.0e-5);
	test = convert<grams_per_milliliter, kilograms_per_cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(1000.0, test, 5.0e-5);
	test = convert<kilograms_per_liter, kilograms_per_cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(1000.0, test, 5.0e-5);
	test = convert<ounces_per_cubic_foot, kilograms_per_cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(1.001153961, test, 5.0e-10);
	test = convert<ounces_per_cubic_inch, kilograms_per_cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(1.729994044e3, test, 5.0e-7);
	test = convert<ounces_per_gallon, kilograms_per_cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(7.489151707, test, 5.0e-10);
	test = convert<pounds_per_cubic_foot, kilograms_per_cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(16.01846337, test, 5.0e-9);
	test = convert<pounds_per_cubic_inch, kilograms_per_cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(2.767990471e4, test, 5.0e-6);
	test = convert<pounds_per_gallon, kilograms_per_cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(119.8264273, test, 5.0e-8);
	test = convert<slugs_per_cubic_foot, kilograms_per_cubic_meter>(1.0);
	ASSERT_EQUAL_DELTA(515.3788184, test, 5.0e-6);
}

void UnitConversion_concentration()
{
	double test;

	test = ppm_t(1.0);
	ASSERT_EQUAL_DELTA(1.0e-6, test, 5.0e-12);
	test = ppb_t(1.0);
	ASSERT_EQUAL_DELTA(1.0e-9, test, 5.0e-12);
	test = ppt_t(1.0);
	ASSERT_EQUAL_DELTA(1.0e-12, test, 5.0e-12);
	test = percent_t(18.0);
	ASSERT_EQUAL_DELTA(0.18, test, 5.0e-12);
}

void UnitConversion_data()
{
	ASSERT_EQUAL(8, (convert<byte, bit>(1)));

	ASSERT_EQUAL(1000, (convert<kilobytes, bytes>(1)));
	ASSERT_EQUAL(1000, (convert<megabytes, kilobytes>(1)));
	ASSERT_EQUAL(1000, (convert<gigabytes, megabytes>(1)));
	ASSERT_EQUAL(1000, (convert<terabytes, gigabytes>(1)));
	ASSERT_EQUAL(1000, (convert<petabytes, terabytes>(1)));
	ASSERT_EQUAL(1000, (convert<exabytes, petabytes>(1)));

	ASSERT_EQUAL(1024, (convert<kibibytes, bytes>(1)));
	ASSERT_EQUAL(1024, (convert<mebibytes, kibibytes>(1)));
	ASSERT_EQUAL(1024, (convert<gibibytes, mebibytes>(1)));
	ASSERT_EQUAL(1024, (convert<tebibytes, gibibytes>(1)));
	ASSERT_EQUAL(1024, (convert<pebibytes, tebibytes>(1)));
	ASSERT_EQUAL(1024, (convert<exbibytes, pebibytes>(1)));

	ASSERT_EQUAL(93750000, (convert<gigabytes, kibibits>(12)));

	ASSERT_EQUAL(1000, (convert<kilobits, bits>(1)));
	ASSERT_EQUAL(1000, (convert<megabits, kilobits>(1)));
	ASSERT_EQUAL(1000, (convert<gigabits, megabits>(1)));
	ASSERT_EQUAL(1000, (convert<terabits, gigabits>(1)));
	ASSERT_EQUAL(1000, (convert<petabits, terabits>(1)));
	ASSERT_EQUAL(1000, (convert<exabits, petabits>(1)));

	ASSERT_EQUAL(1024, (convert<kibibits, bits>(1)));
	ASSERT_EQUAL(1024, (convert<mebibits, kibibits>(1)));
	ASSERT_EQUAL(1024, (convert<gibibits, mebibits>(1)));
	ASSERT_EQUAL(1024, (convert<tebibits, gibibits>(1)));
	ASSERT_EQUAL(1024, (convert<pebibits, tebibits>(1)));
	ASSERT_EQUAL(1024, (convert<exbibits, pebibits>(1)));

	// Source: https://en.wikipedia.org/wiki/Binary_prefix
	ASSERT_EQUAL_DELTA(percent_t(2.4), kibibyte_t(1) / kilobyte_t(1) - 1, 0.005);
	ASSERT_EQUAL_DELTA(percent_t(4.9), mebibyte_t(1) / megabyte_t(1) - 1, 0.005);
	ASSERT_EQUAL_DELTA(percent_t(7.4), gibibyte_t(1) / gigabyte_t(1) - 1, 0.005);
	ASSERT_EQUAL_DELTA(percent_t(10.0), tebibyte_t(1) / terabyte_t(1) - 1, 0.005);
	ASSERT_EQUAL_DELTA(percent_t(12.6), pebibyte_t(1) / petabyte_t(1) - 1, 0.005);
	ASSERT_EQUAL_DELTA(percent_t(15.3), exbibyte_t(1) / exabyte_t(1) - 1, 0.005);
}

void UnitConversion_data_transfer_rate()
{
	ASSERT_EQUAL(8, (convert<bytes_per_second, bits_per_second>(1)));

	ASSERT_EQUAL(1000, (convert<kilobytes_per_second, bytes_per_second>(1)));
	ASSERT_EQUAL(1000, (convert<megabytes_per_second, kilobytes_per_second>(1)));
	ASSERT_EQUAL(1000, (convert<gigabytes_per_second, megabytes_per_second>(1)));
	ASSERT_EQUAL(1000, (convert<terabytes_per_second, gigabytes_per_second>(1)));
	ASSERT_EQUAL(1000, (convert<petabytes_per_second, terabytes_per_second>(1)));
	ASSERT_EQUAL(1000, (convert<exabytes_per_second, petabytes_per_second>(1)));

	ASSERT_EQUAL(1024, (convert<kibibytes_per_second, bytes_per_second>(1)));
	ASSERT_EQUAL(1024, (convert<mebibytes_per_second, kibibytes_per_second>(1)));
	ASSERT_EQUAL(1024, (convert<gibibytes_per_second, mebibytes_per_second>(1)));
	ASSERT_EQUAL(1024, (convert<tebibytes_per_second, gibibytes_per_second>(1)));
	ASSERT_EQUAL(1024, (convert<pebibytes_per_second, tebibytes_per_second>(1)));
	ASSERT_EQUAL(1024, (convert<exbibytes_per_second, pebibytes_per_second>(1)));

	ASSERT_EQUAL(93750000, (convert<gigabytes_per_second, kibibits_per_second>(12)));

	ASSERT_EQUAL(1000, (convert<kilobits_per_second, bits_per_second>(1)));
	ASSERT_EQUAL(1000, (convert<megabits_per_second, kilobits_per_second>(1)));
	ASSERT_EQUAL(1000, (convert<gigabits_per_second, megabits_per_second>(1)));
	ASSERT_EQUAL(1000, (convert<terabits_per_second, gigabits_per_second>(1)));
	ASSERT_EQUAL(1000, (convert<petabits_per_second, terabits_per_second>(1)));
	ASSERT_EQUAL(1000, (convert<exabits_per_second, petabits_per_second>(1)));

	ASSERT_EQUAL(1024, (convert<kibibits_per_second, bits_per_second>(1)));
	ASSERT_EQUAL(1024, (convert<mebibits_per_second, kibibits_per_second>(1)));
	ASSERT_EQUAL(1024, (convert<gibibits_per_second, mebibits_per_second>(1)));
	ASSERT_EQUAL(1024, (convert<tebibits_per_second, gibibits_per_second>(1)));
	ASSERT_EQUAL(1024, (convert<pebibits_per_second, tebibits_per_second>(1)));
	ASSERT_EQUAL(1024, (convert<exbibits_per_second, pebibits_per_second>(1)));

	// Source: https://en.wikipedia.org/wiki/Binary_prefix
	ASSERT_EQUAL_DELTA(percent_t(2.4), kibibytes_per_second_t(1) / kilobytes_per_second_t(1) - 1, 0.005);
	ASSERT_EQUAL_DELTA(percent_t(4.9), mebibytes_per_second_t(1) / megabytes_per_second_t(1) - 1, 0.005);
	ASSERT_EQUAL_DELTA(percent_t(7.4), gibibytes_per_second_t(1) / gigabytes_per_second_t(1) - 1, 0.005);
	ASSERT_EQUAL_DELTA(percent_t(10.0), tebibytes_per_second_t(1) / terabytes_per_second_t(1) - 1, 0.005);
	ASSERT_EQUAL_DELTA(percent_t(12.6), pebibytes_per_second_t(1) / petabytes_per_second_t(1) - 1, 0.005);
	ASSERT_EQUAL_DELTA(percent_t(15.3), exbibytes_per_second_t(1) / exabytes_per_second_t(1) - 1, 0.005);
}

void UnitConversion_pi()
{
	ASSERT(units::traits::is_dimensionless_unit<decltype(constants::pi)>);
	ASSERT(units::traits::is_dimensionless_unit<constants::PI>);

	// implicit conversion/arithmetic
	ASSERT_EQUAL_DELTA(3.14159, constants::pi, 5.0e-6);
	ASSERT_EQUAL_DELTA(6.28318531, (2 * constants::pi), 5.0e-9);
	ASSERT_EQUAL_DELTA(6.28318531, (constants::pi + constants::pi), 5.0e-9);
	ASSERT_EQUAL_DELTA(0.0, (constants::pi - constants::pi), 5.0e-9);
	ASSERT_EQUAL_DELTA(31.00627668, units::math::cpow<3>(constants::pi), 5.0e-10);
	ASSERT_EQUAL_DELTA(0.0322515344, (1.0 / units::math::cpow<3>(constants::pi)), 5.0e-11);
	ASSERT(constants::detail::PI_VAL<> == constants::pi);
	ASSERT(1.0 != constants::pi);
	ASSERT(4.0 > constants::pi);
	ASSERT(3.0 < constants::pi);
	ASSERT(constants::pi > 3.0);
	ASSERT(constants::pi < 4.0);

	// explicit conversion
	ASSERT_EQUAL_DELTA(3.14159, constants::pi.to<double>(), 5.0e-6);

	// auto multiplication
	ASSERT((std::is_same<meter_t, decltype(constants::pi * meter_t(1))>::value));
	ASSERT((std::is_same<meter_t, decltype(meter_t(1) * constants::pi)>::value));

	ASSERT_EQUAL_DELTA(constants::detail::PI_VAL<>, (constants::pi * meter_t(1)).to<double>(), 5.0e-10);
	ASSERT_EQUAL_DELTA(constants::detail::PI_VAL<>, (meter_t(1) * constants::pi).to<double>(), 5.0e-10);

	// explicit multiplication
	meter_t a = constants::pi * meter_t(1);
	meter_t b = meter_t(1) * constants::pi;

	ASSERT_EQUAL_DELTA(constants::detail::PI_VAL<>, a.to<double>(), 5.0e-10);
	ASSERT_EQUAL_DELTA(constants::detail::PI_VAL<>, b.to<double>(), 5.0e-10);

	// auto division
	ASSERT((std::is_same<hertz_t, decltype(constants::pi / second_t(1))>::value));
	ASSERT((std::is_same<second_t, decltype(second_t(1) / constants::pi)>::value));

	ASSERT_EQUAL_DELTA(constants::detail::PI_VAL<>, (constants::pi / second_t(1)).to<double>(), 5.0e-10);
	ASSERT_EQUAL_DELTA(1.0 / constants::detail::PI_VAL<>, (second_t(1) / constants::pi).to<double>(), 5.0e-10);

	// explicit
	hertz_t c = constants::pi / second_t(1);
	second_t d = second_t(1) / constants::pi;

	ASSERT_EQUAL_DELTA(constants::detail::PI_VAL<>, c.to<double>(), 5.0e-10);
	ASSERT_EQUAL_DELTA(1.0 / constants::detail::PI_VAL<>, d.to<double>(), 5.0e-10);
}

void UnitConversion_constants()
{
	// Source: NIST "2014 CODATA recommended values"
	ASSERT_EQUAL_DELTA(299792458, constants::c(), 5.0e-9);
	ASSERT_EQUAL_DELTA(6.67408e-11, constants::G(), 5.0e-17);
	ASSERT_EQUAL_DELTA(6.626070040e-34, constants::h(), 5.0e-44);
	ASSERT_EQUAL_DELTA(1.2566370614e-6, constants::mu0(), 5.0e-17);
	ASSERT_EQUAL_DELTA(8.854187817e-12, constants::epsilon0(), 5.0e-21);
	ASSERT_EQUAL_DELTA(376.73031346177, constants::Z0(), 5.0e-12);
	ASSERT_EQUAL_DELTA(8987551787.3681764, constants::k_e(), 5.0e-6);
	ASSERT_EQUAL_DELTA(1.6021766208e-19, constants::e(), 5.0e-29);
	ASSERT_EQUAL_DELTA(9.10938356e-31, constants::m_e(), 5.0e-40);
	ASSERT_EQUAL_DELTA(1.672621898e-27, constants::m_p(), 5.0e-37);
	ASSERT_EQUAL_DELTA(9.274009994e-24, constants::mu_B(), 5.0e-32);
	ASSERT_EQUAL_DELTA(6.022140857e23, constants::N_A(), 5.0e14);
	ASSERT_EQUAL_DELTA(8.3144598, constants::R(), 5.0e-8);
	ASSERT_EQUAL_DELTA(1.38064852e-23, constants::k_B(), 5.0e-31);
	ASSERT_EQUAL_DELTA(96485.33289, constants::F(), 5.0e-5);
	ASSERT_EQUAL_DELTA(5.670367e-8, constants::sigma(), 5.0e-14);
}

void UnitConversion_std_chrono()
{
	nanosecond_t a = std::chrono::nanoseconds(10);
	ASSERT_EQUAL(nanosecond_t(10), a);
	microsecond_t b = std::chrono::microseconds(10);
	ASSERT_EQUAL(microsecond_t(10), b);
	millisecond_t c = std::chrono::milliseconds(10);
	ASSERT_EQUAL(millisecond_t(10), c);
	second_t d = std::chrono::seconds(1);
	ASSERT_EQUAL(second_t(1), d);
	minute_t e = std::chrono::minutes(120);
	ASSERT_EQUAL(minute_t(120), e);
	hour_t f = std::chrono::hours(2);
	ASSERT_EQUAL(hour_t(2), f);

	std::chrono::nanoseconds g = nanosecond_t(100);
	ASSERT_EQUAL(std::chrono::duration_cast<std::chrono::nanoseconds>(g).count(), 100);
	std::chrono::nanoseconds h = microsecond_t(2);
	ASSERT_EQUAL(std::chrono::duration_cast<std::chrono::nanoseconds>(h).count(), 2000);
	std::chrono::nanoseconds i = millisecond_t(1);
	ASSERT_EQUAL(std::chrono::duration_cast<std::chrono::nanoseconds>(i).count(), 1000000);
	std::chrono::nanoseconds j = second_t(1);
	ASSERT_EQUAL(std::chrono::duration_cast<std::chrono::nanoseconds>(j).count(), 1000000000);
	std::chrono::nanoseconds k = minute_t(1);
	ASSERT_EQUAL(std::chrono::duration_cast<std::chrono::nanoseconds>(k).count(), 60000000000);
	std::chrono::nanoseconds l = hour_t(1);
	ASSERT_EQUAL(std::chrono::duration_cast<std::chrono::nanoseconds>(l).count(), 3600000000000);
}
}
cute::suite make_suite_UnitConversion() {
	cute::suite s { };
	s.push_back(CUTE(UnitConversion_length));
	s.push_back(CUTE(UnitConversion_mass));
	s.push_back(CUTE(UnitConversion_time));
	s.push_back(CUTE(UnitConversion_angle));
	s.push_back(CUTE(UnitConversion_current));
	s.push_back(CUTE(UnitConversion_temperature));
	s.push_back(CUTE(UnitConversion_luminous_intensity));
	s.push_back(CUTE(UnitConversion_solid_angle));
	s.push_back(CUTE(UnitConversion_frequency));
	s.push_back(CUTE(UnitConversion_velocity));
	s.push_back(CUTE(UnitConversion_angular_velocity));
	s.push_back(CUTE(UnitConversion_acceleration));
	s.push_back(CUTE(UnitConversion_force));
	s.push_back(CUTE(UnitConversion_area));
	s.push_back(CUTE(UnitConversion_pressure));
	s.push_back(CUTE(UnitConversion_charge));
	s.push_back(CUTE(UnitConversion_energy));
	s.push_back(CUTE(UnitConversion_power));
	s.push_back(CUTE(UnitConversion_voltage));
	s.push_back(CUTE(UnitConversion_capacitance));
	s.push_back(CUTE(UnitConversion_impedance));
	s.push_back(CUTE(UnitConversion_conductance));
	s.push_back(CUTE(UnitConversion_magnetic_flux));
	s.push_back(CUTE(UnitConversion_magnetic_field_strength));
	s.push_back(CUTE(UnitConversion_inductance));
	s.push_back(CUTE(UnitConversion_luminous_flux));
	s.push_back(CUTE(UnitConversion_illuminance));
	s.push_back(CUTE(UnitConversion_radiation));
	s.push_back(CUTE(UnitConversion_torque));
	s.push_back(CUTE(UnitConversion_volume));
	s.push_back(CUTE(UnitConversion_density));
	s.push_back(CUTE(UnitConversion_concentration));
	s.push_back(CUTE(UnitConversion_data));
	s.push_back(CUTE(UnitConversion_data_transfer_rate));
	s.push_back(CUTE(UnitConversion_pi));
	s.push_back(CUTE(UnitConversion_constants));
	s.push_back(CUTE(UnitConversion_std_chrono));
	return s;
}
