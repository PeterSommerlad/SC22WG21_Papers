#include "Constexpr.h"
#include "cute.h"

#include "common_for_tests.h"
// Constexpr
#if !defined(_MSC_VER) || _MSC_VER > 1800
void Constexpr_construction()
{
	constexpr meter_t result0(0);
	constexpr auto result1 = make_quantity<meter_t>(1);
	constexpr auto result2 = meter_t(2);

	ASSERT_EQUAL(meter_t(0), result0);
	ASSERT_EQUAL(meter_t(1), result1);
	ASSERT_EQUAL(meter_t(2), result2);

	ASSERT(noexcept(result0));
	ASSERT(noexcept(result1));
	ASSERT(noexcept(result2));
}

void Constexpr_constants()
{
	ASSERT(noexcept(constants::c()));
	ASSERT(noexcept(constants::G()));
	ASSERT(noexcept(constants::h()));
	ASSERT(noexcept(constants::mu0()));
	ASSERT(noexcept(constants::epsilon0()));
	ASSERT(noexcept(constants::Z0()));
	ASSERT(noexcept(constants::k_e()));
	ASSERT(noexcept(constants::e()));
	ASSERT(noexcept(constants::m_e()));
	ASSERT(noexcept(constants::m_p()));
	ASSERT(noexcept(constants::mu_B()));
	ASSERT(noexcept(constants::N_A()));
	ASSERT(noexcept(constants::R()));
	ASSERT(noexcept(constants::k_B()));
	ASSERT(noexcept(constants::F()));
	ASSERT(noexcept(constants::sigma()));
}

void Constexpr_arithmetic()
{
	constexpr auto result0(1_m + 1_m);
	constexpr auto result1(1_m - 1_m);
	constexpr auto result2(1_m * 1_m);
	constexpr auto result3(1_m / 1_m);
	constexpr auto result4(meter_t(1) + meter_t(1));
	constexpr auto result5(meter_t(1) - meter_t(1));
	constexpr auto result6(meter_t(1) * meter_t(1));
	constexpr auto result7(meter_t(1) / meter_t(1));
	constexpr auto result8(units::math::cpow<2>(meter_t(2)));
	constexpr auto result9 = units::math::cpow<3>(2_m);
	constexpr auto result10 = 2_m * 2_m;

	ASSERT(noexcept(result0));
	ASSERT(noexcept(result1));
	ASSERT(noexcept(result2));
	ASSERT(noexcept(result3));
	ASSERT(noexcept(result4));
	ASSERT(noexcept(result5));
	ASSERT(noexcept(result6));
	ASSERT(noexcept(result7));
	ASSERT(noexcept(result8));
	ASSERT(noexcept(result9));
	ASSERT(noexcept(result10));

	ASSERT_EQUAL(8_cu_m, result9);
	ASSERT_EQUAL(4_sq_m, result10);
}

void Constexpr_realtional()
{
	constexpr bool equalityTrue = (1_m == 1_m);
	constexpr bool equalityFalse = (1_m == 2_m);
	constexpr bool lessThanTrue = (1_m < 2_m);
	constexpr bool lessThanFalse = (1_m < 1_m);
	constexpr bool lessThanEqualTrue1 = (1_m <= 1_m);
	constexpr bool lessThanEqualTrue2 = (1_m <= 2_m);
	constexpr bool lessThanEqualFalse = (1_m < 0_m);
	constexpr bool greaterThanTrue = (2_m > 1_m);
	constexpr bool greaterThanFalse = (2_m > 2_m);
	constexpr bool greaterThanEqualTrue1 = (2_m >= 1_m);
	constexpr bool greaterThanEqualTrue2 = (2_m >= 2_m);
	constexpr bool greaterThanEqualFalse = (2_m > 3_m);

	ASSERT(equalityTrue);
	ASSERT(lessThanTrue);
	ASSERT(lessThanEqualTrue1);
	ASSERT(lessThanEqualTrue2);
	ASSERT(greaterThanTrue);
	ASSERT(greaterThanEqualTrue1);
	ASSERT(greaterThanEqualTrue2);
	ASSERT(!equalityFalse);
	ASSERT(!lessThanFalse);
	ASSERT(!lessThanEqualFalse);
	ASSERT(!greaterThanFalse);
	ASSERT(!greaterThanEqualFalse);
}

void Constexpr_stdArray()
{
	constexpr std::array<meter_t, 5> arr = { 0_m, 1_m, 2_m, 3_m, 4_m };
	constexpr bool equal = (arr[3] == 3_m);
	ASSERT(equal);
}

#endif
cute::suite make_suite_Constexpr() {
	cute::suite s { };
	s.push_back(CUTE(Constexpr_construction));
	s.push_back(CUTE(Constexpr_constants));
	s.push_back(CUTE(Constexpr_arithmetic));
	s.push_back(CUTE(Constexpr_realtional));
	s.push_back(CUTE(Constexpr_stdArray));
	return s;
}
