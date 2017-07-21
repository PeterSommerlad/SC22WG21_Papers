/*
 * common_for_tests.h
 *
 *  Created on: 14 Jul 2017
 *      Author: sop
 */

#ifndef COMMON_FOR_TESTS_H_
#define COMMON_FOR_TESTS_H_
#include <units.h>
using namespace units;
using namespace units::dimensionless;
using namespace units::length;
using namespace units::mass;
using namespace units::angle;
using namespace units::time;
using namespace units::frequency;
using namespace units::area;
using namespace units::velocity;
using namespace units::angular_velocity;
using namespace units::temperature;
using namespace units::luminous_intensity;
using namespace units::solid_angle;
using namespace units::frequency;
using namespace units::acceleration;
using namespace units::pressure;
using namespace units::charge;
using namespace units::energy;
using namespace units::power;
using namespace units::voltage;
using namespace units::capacitance;
using namespace units::impedance;
using namespace units::conductance;
using namespace units::magnetic_flux;
using namespace units::magnetic_field_strength;
using namespace units::inductance;
using namespace units::luminous_flux;
using namespace units::illuminance;
using namespace units::radiation;
using namespace units::torque;
using namespace units::volume;
using namespace units::density;
using namespace units::concentration;
using namespace units::data;
using namespace units::data_transfer_rate;
using namespace units::math;

#if !defined(_MSC_VER) || _MSC_VER > 1800
using namespace units::literals;
#endif


//#define TEST_F(a,b) void a##_##b()
//#define EXPECT_TRUE(a) ASSERT(a)
//#define EXPECT_FALSE(a) ASSERT(not(a))
//#define EXPECT_EQ(e,a) ASSERT_EQUAL((e),(a))
//#define EXPECT_STREQ(e,a) ASSERT_EQUAL((e),(a))
//#define EXPECT_LT(e,a) ASSERT_LESS((e),(a))
//#define EXPECT_NEAR(e,a,d) ASSERT_EQUAL_DELTA((e),(a),(d))
//#define EXPECT_DOUBLE_EQ(e,a) ASSERT_EQUAL((e),(a))
//#define UNIT_LIB_DISABLE_IOSTREAM 1



#endif /* COMMON_FOR_TESTS_H_ */
