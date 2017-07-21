#include "UnitManipulators.h"
#include "cute.h"

#include "common_for_tests.h"

void UnitManipulators_squared()
{
	double test = convert<squared<meters>, square_feet>(0.092903);
	ASSERT_EQUAL_DELTA(0.99999956944, test, 5.0e-12);

	using scalar_2 = squared<scalar>;	// this is actually nonsensical, and should also result in a scalar.
	bool isSame = std::is_same<typename std::decay<scalar_t>::type, typename std::decay<quantity<scalar_2>>::type>::value;
	ASSERT(isSame);
}

void UnitManipulators_cubed()
{
	double test = convert<cubed<meters>, cubic_feet>(0.0283168);
	ASSERT_EQUAL_DELTA(0.999998354619, test, 5.0e-13);
}

void UnitManipulators_square_root()
{
	double test = convert<square_root<square_kilometer>, meter>(1.0);
	ASSERT((traits::is_convertible_unit<typename std::decay<square_root<square_kilometer>>::type, kilometer>));
	ASSERT_EQUAL_DELTA(1000.0, test, 5.0e-13);
}

void UnitManipulators_compound_unit()
{
	using acceleration1 = unit<std::ratio<1>, category::acceleration_unit>;
	using acceleration2 = compound_unit<meters, inverse<seconds>, inverse<seconds>>;
	using acceleration3 = unit<std::ratio<1>, base_unit<std::ratio<1>, std::ratio<0>, std::ratio<-2>>>;
	using acceleration4 = compound_unit<meters, inverse<squared<seconds>>>;
	using acceleration5 = compound_unit<meters, squared<inverse<seconds>>>;

	bool areSame12 = std::is_same<acceleration1, acceleration2>::value;
	bool areSame23 = std::is_same<acceleration2, acceleration3>::value;
	bool areSame34 = std::is_same<acceleration3, acceleration4>::value;
	bool areSame45 = std::is_same<acceleration4, acceleration5>::value;

	ASSERT(areSame12);
	ASSERT(areSame23);
	ASSERT(areSame34);
	ASSERT(areSame45);

	// test that thing with translations still compile
	using arbitrary1 = compound_unit<meters, inverse<celsius>>;
	using arbitrary2 = compound_unit<meters, celsius>;
	using arbitrary3 = compound_unit<arbitrary1, arbitrary2>;
	ASSERT((std::is_same<square_meters, arbitrary3>::value));
}

void UnitManipulators_dimensionalAnalysis()
{
	// these look like 'compound units', but the dimensional analysis can be REALLY handy if the
	// unit types aren't know (i.e. they themselves are template parameters), as you can get the resulting unit of the
	// operation.

	using velocity = units::detail::unit_divide<meters, second>;
	bool shouldBeTrue = std::is_same<meters_per_second, velocity>::value;
	ASSERT(shouldBeTrue);

	using acceleration1 = unit<std::ratio<1>, category::acceleration_unit>;
	using acceleration2 = units::detail::unit_divide<meters, units::detail::unit_multiply<seconds, seconds>>;
	shouldBeTrue = std::is_same<acceleration1, acceleration2>::value;
	ASSERT(shouldBeTrue);
}


cute::suite make_suite_UnitManipulators() {
	cute::suite s { };
	s.push_back(CUTE(UnitManipulators_squared));
	s.push_back(CUTE(UnitManipulators_cubed));
	s.push_back(CUTE(UnitManipulators_square_root));
	s.push_back(CUTE(UnitManipulators_compound_unit));
	s.push_back(CUTE(UnitManipulators_dimensionalAnalysis));
	return s;
}
