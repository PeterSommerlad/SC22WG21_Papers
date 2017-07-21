#include "common_for_tests.h"
#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"
#include "TypeTraits.h"
#include "UnitConversion.h"
#include "CompileTimeArithmetic.h"
#include "UnitMath.h"
#include "UnitContainer.h"
#include "UnitManipulators.h"
#include "Constexpr.h"
//#include <gtest/gtest.h>
#include <chrono>
#include <string>
#include <type_traits>
#include <array>









void CaseStudies_radarRangeEquation()
{
	watt_t			P_t;				// transmit power
	scalar_t		G;					// gain
	meter_t			lambda;				// wavelength
	square_meter_t	sigma;				// radar cross section
	meter_t			R;					// range
	kelvin_t		T_s;				// system noise temp
	hertz_t			B_n;				// bandwidth
	scalar_t		L;					// loss

	P_t = megawatt_t(1.4);
	G = dB_t(33.0);
	lambda = constants::c / megahertz_t(2800);
	sigma = square_meter_t(1.0);
	R = meter_t(111000.0);
	T_s = kelvin_t(950.0);
	B_n = megahertz_t(1.67);
	L = dB_t(8.0);

	scalar_t SNR = (P_t * math::pow<2>(G) * math::pow<2>(lambda) * sigma) /
		(math::pow<3>(4 * constants::pi) * math::pow<4>(R) * constants::k_B * T_s * B_n * L);

	ASSERT_EQUAL_DELTA(1.535, SNR(), 5.0e-4);
}

void CaseStudies_pythagoreanTheorum()
{
	struct RightTriangle
	{
		using a = unit_value_t<meters, 3>;
		using b = unit_value_t<meters, 4>;
		using c = unit_value_sqrt<unit_value_add<unit_value_power<a, 2>, unit_value_power<b, 2>>>;
	};

	ASSERT_EQUAL(meter_t(3), RightTriangle::a::value());
	ASSERT_EQUAL(meter_t(4), RightTriangle::b::value());
	ASSERT_EQUAL(meter_t(5), RightTriangle::c::value());
	ASSERT(pow<2>(RightTriangle::a::value()) + pow<2>(RightTriangle::b::value())
		== pow<2>(RightTriangle::c::value()));
}

int main1(int argc, char* argv[])
{
	//::testing::InitGoogleTest(&argc, argv);
	return 0;//RUN_ALL_TESTS();
}

void newTestFunction(){
	ASSERTM("start writing tests", false);
}


void runAllTests(int argc, const char *argv[]) {
	cute::suite s;
	//TODO add your test here

	s+=make_suite_TypeTraits();
	s+=make_suite_UnitContainer();
	s+=make_suite_UnitMath();
	s+=make_suite_UnitManipulators();
	s+=make_suite_Constexpr();
	s+= make_suite_CompileTimeArithmetic();
	s.push_back(CUTE(CaseStudies_pythagoreanTheorum));
	s.push_back(CUTE(CaseStudies_radarRangeEquation));

	s += make_suite_UnitConversion();

	cute::xml_file_opener xmlfile(argc,argv);
	cute::xml_listener<cute::ide_listener<> >  lis(xmlfile.out);
	cute::makeRunner(lis,argc,argv)(s, "AllTests");
}

int main(int argc, const char *argv[]) {
	runAllTests(argc,argv);
	return 0;
}

