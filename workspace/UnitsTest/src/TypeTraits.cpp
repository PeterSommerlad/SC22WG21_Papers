#include "TypeTraits.h"
#include "cute.h"
#include "common_for_tests.h"

void TypeTraits_isRatio()
{
	ASSERT(traits::is_ratio<std::ratio<1>>);
	ASSERT(!traits::is_ratio<double>);
}

void TypeTraits_ratio_sqrt()
{
	using rt2 = ratio_sqrt<std::ratio<2>>;
	ASSERT_LESS(std::abs(std::sqrt(2 / (double)1) - rt2::num / (double)rt2::den), 5e-9);

	using rt4 = ratio_sqrt<std::ratio<4>>;
	ASSERT_LESS(std::abs(std::sqrt(4 / (double)1) - rt4::num / (double)rt4::den), 5e-9);

	using rt10 = ratio_sqrt<std::ratio<10>>;
	ASSERT_LESS(std::abs(std::sqrt(10 / (double)1) - rt10::num / (double)rt10::den), 5e-9);

	using rt30 = ratio_sqrt<std::ratio<30>>;
	ASSERT_LESS(std::abs(std::sqrt(30 / (double)1) - rt30::num / (double)rt30::den), 5e-9);

	using rt61 = ratio_sqrt<std::ratio<61>>;
	ASSERT_LESS(std::abs(std::sqrt(61 / (double)1) - rt61::num / (double)rt61::den), 5e-9);

	using rt100 = ratio_sqrt<std::ratio<100>>;
	ASSERT_LESS(std::abs(std::sqrt(100 / (double)1) - rt100::num / (double)rt100::den), 5e-9);

	using rt1000 = ratio_sqrt<std::ratio<1000>>;
	ASSERT_LESS(std::abs(std::sqrt(1000 / (double)1) - rt1000::num / (double)rt1000::den), 5e-9);

	using rt10000 = ratio_sqrt<std::ratio<10000>>;
	ASSERT_LESS(std::abs(std::sqrt(10000 / (double)1) - rt10000::num / (double)rt10000::den), 5e-9);
}

void TypeTraits_is_unit()
{
	ASSERT(!traits::is_unit<std::ratio<1>>);
	ASSERT(!traits::is_unit<double>);
	ASSERT(traits::is_unit<meters>);
	ASSERT(traits::is_unit<feet>);
	ASSERT(traits::is_unit<degrees_squared>);
	ASSERT(!traits::is_unit<meter_t>);
}

void TypeTraits_is_unit_t()
{
	ASSERT(!traits::is_quantity<std::ratio<1>>);
	ASSERT(!traits::is_quantity<double>);
	ASSERT(!traits::is_quantity<meters>);
	ASSERT(!traits::is_quantity<feet>);
	ASSERT(!traits::is_quantity<degrees_squared>);
	ASSERT(traits::is_quantity<meter_t>);
}

void TypeTraits_unit_traits()
{
	ASSERT((std::is_same<void, traits::unit_traits<double>::conversion_ratio>::value));
	ASSERT(!(std::is_same<void, traits::unit_traits<meters>::conversion_ratio>::value));
}

void TypeTraits_unit_t_traits()
{
	ASSERT((std::is_same<void, traits::quantity_traits<double>::underlying_type>::value));
	ASSERT((std::is_same<UNIT_LIB_DEFAULT_TYPE, traits::quantity_traits<meter_t>::underlying_type>::value));
	ASSERT((std::is_same<void, traits::quantity_traits<double>::value_type>::value));
	ASSERT((std::is_same<UNIT_LIB_DEFAULT_TYPE, traits::quantity_traits<meter_t>::value_type>::value));
}

void TypeTraits_all_true()
{
	ASSERT(all_true<true>);
	ASSERT((all_true<true, true>));
	ASSERT((all_true<true, true, true>));
	ASSERT(!all_true<false>);
	ASSERT(!(all_true<true, false>));
	ASSERT(!(all_true<true, true, false>));
	ASSERT(!(all_true<false, false, false>));
}

void TypeTraits_is_convertible_unit()
{
	ASSERT((traits::is_convertible_unit<meters, meters>));
	ASSERT((traits::is_convertible_unit<meters, astronicalUnits>));
	ASSERT((traits::is_convertible_unit<meters, parsecs>));

	ASSERT((traits::is_convertible_unit<meters, meters>));
	ASSERT((traits::is_convertible_unit<astronicalUnits, meters>));
	ASSERT((traits::is_convertible_unit<parsecs, meters>));
	ASSERT((traits::is_convertible_unit<years, weeks>));

	ASSERT(!(traits::is_convertible_unit<meters, seconds>));
	ASSERT(!(traits::is_convertible_unit<seconds, meters>));
	ASSERT(!(traits::is_convertible_unit<years, meters>));
}

void TypeTraits_inverse()
{
	double test;

	using htz = inverse<seconds>;
	bool shouldBeTrue = std::is_same<htz, hertz>::value;
	ASSERT(shouldBeTrue);

	test = convert<inverse<celsius>, inverse<fahrenheit>>(1.0);
	ASSERT_EQUAL_DELTA(5.0 / 9.0, test, 5.0e-5);

	test = convert<inverse<kelvin>, inverse<fahrenheit>>(6.0);
	ASSERT_EQUAL_DELTA(10.0 / 3.0, test, 5.0e-5);
}

void TypeTraits_base_unit_of()
{
	using base = traits::base_unit_of<years>;
	bool shouldBeTrue = std::is_same<base, category::time_unit>::value;

	ASSERT(shouldBeTrue);
}

void TypeTraits_has_linear_scale()
{
	ASSERT((traits::has_linear_scale<scalar_t>));
	ASSERT((traits::has_linear_scale<meter_t>));
	ASSERT((traits::has_linear_scale<foot_t>));
	ASSERT((traits::has_linear_scale<watt_t, scalar_t>));
	ASSERT((traits::has_linear_scale<scalar_t, meter_t>));
	ASSERT((traits::has_linear_scale<meters_per_second_t>));
	ASSERT(!(traits::has_linear_scale<dB_t>));
	ASSERT(!(traits::has_linear_scale<dB_t, meters_per_second_t>));
}

void TypeTraits_has_decibel_scale()
{
	ASSERT(!(traits::has_decibel_scale<scalar_t>::value));
	ASSERT(!(traits::has_decibel_scale<meter_t>::value));
	ASSERT(!(traits::has_decibel_scale<foot_t>::value));
	ASSERT((traits::has_decibel_scale<dB_t>::value));
	ASSERT((traits::has_decibel_scale<dBW_t>::value));

	ASSERT((traits::has_decibel_scale<dBW_t, dB_t>::value));
	ASSERT((traits::has_decibel_scale<dBW_t, dBm_t>::value));
	ASSERT((traits::has_decibel_scale<dB_t, dB_t>::value));
	ASSERT((traits::has_decibel_scale<dB_t, dB_t, dB_t>::value));
	ASSERT(!(traits::has_decibel_scale<dB_t, dB_t, meter_t>::value));
	ASSERT(!(traits::has_decibel_scale<meter_t, dB_t>::value));
}

void TypeTraits_is_same_scale()
{
	ASSERT((traits::is_same_scale<scalar_t, dimensionless_t>));
	ASSERT((traits::is_same_scale<dB_t, dBW_t>));
	ASSERT(!(traits::is_same_scale<dB_t, scalar_t>));
}

void TypeTraits_is_dimensionless_unit()
{
	ASSERT((traits::is_dimensionless_unit<scalar_t>));
	ASSERT((traits::is_dimensionless_unit<const scalar_t>));
	ASSERT((traits::is_dimensionless_unit<const scalar_t&>));
	ASSERT((traits::is_dimensionless_unit<dimensionless_t>));
	ASSERT((traits::is_dimensionless_unit<dB_t>));
	ASSERT((traits::is_dimensionless_unit<dB_t, scalar_t>));
	ASSERT((traits::is_dimensionless_unit<ppm_t>));
	ASSERT(!(traits::is_dimensionless_unit<meter_t>));
	ASSERT(!(traits::is_dimensionless_unit<dBW_t>));
	ASSERT(!(traits::is_dimensionless_unit<dBW_t, scalar_t>));
}

void TypeTraits_is_length_unit()
{
	ASSERT((traits::is_length_unit<meter>));
	ASSERT((traits::is_length_unit<cubit>));
	ASSERT(!(traits::is_length_unit<year>));
	ASSERT(!(traits::is_length_unit<double>));

	ASSERT((traits::is_length_unit<meter_t>));
	ASSERT((traits::is_length_unit<const meter_t>));
	ASSERT((traits::is_length_unit<const meter_t&>));
	ASSERT((traits::is_length_unit<cubit_t>));
	ASSERT(!(traits::is_length_unit<year_t>));
	ASSERT((traits::is_length_unit<meter_t, cubit_t>));
	ASSERT(!(traits::is_length_unit<meter_t, year_t>));
}

void TypeTraits_is_mass_unit()
{

	ASSERT((traits::is_mass_unit<kilogram>));
	ASSERT((traits::is_mass_unit<stone>));
	ASSERT(!(traits::is_mass_unit<meter>));
	ASSERT(!(traits::is_mass_unit<double>));

	ASSERT((traits::is_mass_unit<kilogram_t>));
	ASSERT((traits::is_mass_unit<const kilogram_t>));
	ASSERT((traits::is_mass_unit<const kilogram_t&>));
	ASSERT((traits::is_mass_unit<stone_t>));
	ASSERT(!(traits::is_mass_unit<meter_t>));
	ASSERT((traits::is_mass_unit<kilogram_t, stone_t>));
	ASSERT(!(traits::is_mass_unit<kilogram_t, meter_t>));
}


void TypeTraits_is_time_unit()
{

	ASSERT((traits::is_time_unit<second>));
	ASSERT((traits::is_time_unit<year>));
	ASSERT(!(traits::is_time_unit<meter>));
	ASSERT(!(traits::is_time_unit<double>));

	ASSERT((traits::is_time_unit<second_t>));
	ASSERT((traits::is_time_unit<const second_t>));
	ASSERT((traits::is_time_unit<const second_t&>));
	ASSERT((traits::is_time_unit<year_t>));
	ASSERT(!(traits::is_time_unit<meter_t>));
	ASSERT((traits::is_time_unit<second_t, year_t>));
	ASSERT(!(traits::is_time_unit<second_t, meter_t>));
}

void TypeTraits_is_angle_unit()
{

	ASSERT((traits::is_angle_unit<angle::radian>));
	ASSERT((traits::is_angle_unit<angle::degree>));
	ASSERT(!(traits::is_angle_unit<watt>));
	ASSERT(!(traits::is_angle_unit<double>));

	ASSERT((traits::is_angle_unit<angle::radian_t>));
	ASSERT((traits::is_angle_unit<const angle::radian_t>));
	ASSERT((traits::is_angle_unit<const angle::radian_t&>));
	ASSERT((traits::is_angle_unit<angle::degree_t>));
	ASSERT(!(traits::is_angle_unit<watt_t>));
	ASSERT((traits::is_angle_unit<angle::radian_t, angle::degree_t>));
	ASSERT(!(traits::is_angle_unit<angle::radian_t, watt_t>));
}

void TypeTraits_is_current_unit()
{

	ASSERT((traits::is_current_unit<current::ampere>));
	ASSERT(!(traits::is_current_unit<volt>));
	ASSERT(!(traits::is_current_unit<double>));

	ASSERT((traits::is_current_unit<current::ampere_t>));
	ASSERT((traits::is_current_unit<const current::ampere_t>));
	ASSERT((traits::is_current_unit<const current::ampere_t&>));
	ASSERT(!(traits::is_current_unit<volt_t>));
	ASSERT((traits::is_current_unit<current::ampere_t, current::milliampere_t>));
	ASSERT(!(traits::is_current_unit<current::ampere_t, volt_t>));
}

void TypeTraits_is_temperature_unit()
{

	ASSERT((traits::is_temperature_unit<fahrenheit>));
	ASSERT((traits::is_temperature_unit<kelvin>));
	ASSERT(!(traits::is_temperature_unit<cubit>));
	ASSERT(!(traits::is_temperature_unit<double>));

	ASSERT((traits::is_temperature_unit<fahrenheit_t>));
	ASSERT((traits::is_temperature_unit<const fahrenheit_t>));
	ASSERT((traits::is_temperature_unit<const fahrenheit_t&>));
	ASSERT((traits::is_temperature_unit<kelvin_t>));
	ASSERT(!(traits::is_temperature_unit<cubit_t>));
	ASSERT((traits::is_temperature_unit<fahrenheit_t, kelvin_t>));
	ASSERT(!(traits::is_temperature_unit<cubit_t, fahrenheit_t>));
}

void TypeTraits_is_substance_unit()
{

	ASSERT((traits::is_substance_unit<substance::mol>));
	ASSERT(!(traits::is_substance_unit<year>));
	ASSERT(!(traits::is_substance_unit<double>));

	ASSERT((traits::is_substance_unit<substance::mole_t>));
	ASSERT((traits::is_substance_unit<const substance::mole_t>));
	ASSERT((traits::is_substance_unit<const substance::mole_t&>));
	ASSERT(!(traits::is_substance_unit<year_t>));
	ASSERT((traits::is_substance_unit<substance::mole_t, substance::mole_t>));
	ASSERT(!(traits::is_substance_unit<year_t, substance::mole_t>));
}

void TypeTraits_is_luminous_intensity_unit()
{

	ASSERT((traits::is_luminous_intensity_unit<candela>));
	ASSERT(!(traits::is_luminous_intensity_unit<units::radiation::rad>));
	ASSERT(!(traits::is_luminous_intensity_unit<double>));

	ASSERT((traits::is_luminous_intensity_unit<candela_t>));
	ASSERT((traits::is_luminous_intensity_unit<const candela_t>));
	ASSERT((traits::is_luminous_intensity_unit<const candela_t&>));
	ASSERT(!(traits::is_luminous_intensity_unit<rad_t>));
	ASSERT((traits::is_luminous_intensity_unit<candela_t, candela_t>));
	ASSERT(!(traits::is_luminous_intensity_unit<rad_t, candela_t>));
}

void TypeTraits_is_solid_angle_unit()
{

	ASSERT((traits::is_solid_angle_unit<steradian>));
	ASSERT((traits::is_solid_angle_unit<degree_squared>));
	ASSERT(!(traits::is_solid_angle_unit<angle::degree>));
	ASSERT(!(traits::is_solid_angle_unit<double>));

	ASSERT((traits::is_solid_angle_unit<steradian_t>));
	ASSERT((traits::is_solid_angle_unit<const steradian_t>));
	ASSERT((traits::is_solid_angle_unit<const degree_squared_t&>));
	ASSERT(!(traits::is_solid_angle_unit<angle::degree_t>));
	ASSERT((traits::is_solid_angle_unit<degree_squared_t, steradian_t>));
	ASSERT(!(traits::is_solid_angle_unit<angle::degree_t, steradian_t>));
}

void TypeTraits_is_frequency_unit()
{

	ASSERT((traits::is_frequency_unit<hertz>));
	ASSERT(!(traits::is_frequency_unit<second>));
	ASSERT(!(traits::is_frequency_unit<double>));

	ASSERT((traits::is_frequency_unit<hertz_t>));
	ASSERT((traits::is_frequency_unit<const hertz_t>));
	ASSERT((traits::is_frequency_unit<const hertz_t&>));
	ASSERT(!(traits::is_frequency_unit<second_t>));
	ASSERT((traits::is_frequency_unit<const hertz_t&, gigahertz_t>));
	ASSERT(!(traits::is_frequency_unit<second_t, hertz_t>));
}

void TypeTraits_is_velocity_unit()
{

	ASSERT((traits::is_velocity_unit<meters_per_second>));
	ASSERT((traits::is_velocity_unit<miles_per_hour>));
	ASSERT(!(traits::is_velocity_unit<meters_per_second_squared>));
	ASSERT(!(traits::is_velocity_unit<double>));

	ASSERT((traits::is_velocity_unit<meters_per_second_t>));
	ASSERT((traits::is_velocity_unit<const meters_per_second_t>));
	ASSERT((traits::is_velocity_unit<const meters_per_second_t&>));
	ASSERT((traits::is_velocity_unit<miles_per_hour_t>));
	ASSERT(!(traits::is_velocity_unit<meters_per_second_squared_t>));
	ASSERT((traits::is_velocity_unit<miles_per_hour_t, meters_per_second_t>));
	ASSERT(!(traits::is_velocity_unit<meters_per_second_squared_t, meters_per_second_t>));
}

void TypeTraits_is_acceleration_unit()
{
	ASSERT((traits::is_acceleration_unit<meters_per_second_squared>));
	ASSERT((traits::is_acceleration_unit<acceleration::standard_gravity>));
	ASSERT(!(traits::is_acceleration_unit<inch>));
	ASSERT(!(traits::is_acceleration_unit<double>));

	ASSERT((traits::is_acceleration_unit<meters_per_second_squared_t>));
	ASSERT((traits::is_acceleration_unit<const meters_per_second_squared_t>));
	ASSERT((traits::is_acceleration_unit<const meters_per_second_squared_t&>));
	ASSERT((traits::is_acceleration_unit<standard_gravity_t>));
	ASSERT(!(traits::is_acceleration_unit<inch_t>));
	ASSERT((traits::is_acceleration_unit<standard_gravity_t, meters_per_second_squared_t>));
	ASSERT(!(traits::is_acceleration_unit<inch_t, meters_per_second_squared_t>));
}

void TypeTraits_is_force_unit()
{

	ASSERT((traits::is_force_unit<units::force::newton>));
	ASSERT((traits::is_force_unit<units::force::dynes>));
	ASSERT(!(traits::is_force_unit<meter>));
	ASSERT(!(traits::is_force_unit<double>));

	ASSERT((traits::is_force_unit<units::force::newton_t>));
	ASSERT((traits::is_force_unit<const units::force::newton_t>));
	ASSERT((traits::is_force_unit<const units::force::newton_t&>));
	ASSERT((traits::is_force_unit<units::force::dyne_t>));
	ASSERT(!(traits::is_force_unit<watt_t>));
	ASSERT((traits::is_force_unit<units::force::dyne_t, units::force::newton_t>));
	ASSERT(!(traits::is_force_unit<watt_t, units::force::newton_t>));
}

void TypeTraits_is_pressure_unit()
{
	ASSERT((traits::is_pressure_unit<pressure::pascals>));
	ASSERT((traits::is_pressure_unit<atmosphere>));
	ASSERT(!(traits::is_pressure_unit<year>));
	ASSERT(!(traits::is_pressure_unit<double>));

	ASSERT((traits::is_pressure_unit<pascal_t>));
	ASSERT((traits::is_pressure_unit<const pascal_t>));
	ASSERT((traits::is_pressure_unit<const pascal_t&>));
	ASSERT((traits::is_pressure_unit<atmosphere_t>));
	ASSERT(!(traits::is_pressure_unit<year_t>));
	ASSERT((traits::is_pressure_unit<atmosphere_t, pressure::pascal_t>));
	ASSERT(!(traits::is_pressure_unit<year_t, pressure::pascal_t>));
}

void TypeTraits_is_charge_unit()
{
	ASSERT((traits::is_charge_unit<coulomb>));
	ASSERT(!(traits::is_charge_unit<watt>));
	ASSERT(!(traits::is_charge_unit<double>));

	ASSERT((traits::is_charge_unit<coulomb_t>));
	ASSERT((traits::is_charge_unit<const coulomb_t>));
	ASSERT((traits::is_charge_unit<const coulomb_t&>));
	ASSERT(!(traits::is_charge_unit<watt_t>));
	ASSERT((traits::is_charge_unit<const coulomb_t&, coulomb_t>));
	ASSERT(!(traits::is_charge_unit<watt_t, coulomb_t>));
}

void TypeTraits_is_energy_unit()
{
	ASSERT((traits::is_energy_unit<joule>));
	ASSERT((traits::is_energy_unit<calorie>));
	ASSERT(!(traits::is_energy_unit<watt>));
	ASSERT(!(traits::is_energy_unit<double>));

	ASSERT((traits::is_energy_unit<joule_t>));
	ASSERT((traits::is_energy_unit<const joule_t>));
	ASSERT((traits::is_energy_unit<const joule_t&>));
	ASSERT((traits::is_energy_unit<calorie_t>));
	ASSERT(!(traits::is_energy_unit<watt_t>));
	ASSERT((traits::is_energy_unit<calorie_t, joule_t>));
	ASSERT(!(traits::is_energy_unit<watt_t, joule_t>));
}

void TypeTraits_is_power_unit()
{
	ASSERT((traits::is_power_unit<watt>));
	ASSERT(!(traits::is_power_unit<henry>));
	ASSERT(!(traits::is_power_unit<double>));

	ASSERT((traits::is_power_unit<watt_t>));
	ASSERT((traits::is_power_unit<const watt_t>));
	ASSERT((traits::is_power_unit<const watt_t&>));
	ASSERT(!(traits::is_power_unit<henry_t>));
	ASSERT((traits::is_power_unit<const watt_t&, watt_t>));
	ASSERT(!(traits::is_power_unit<henry_t, watt_t>));
}

void TypeTraits_is_voltage_unit()
{
	ASSERT((traits::is_voltage_unit<volt>));
	ASSERT(!(traits::is_voltage_unit<henry>));
	ASSERT(!(traits::is_voltage_unit<double>));

	ASSERT((traits::is_voltage_unit<volt_t>));
	ASSERT((traits::is_voltage_unit<const volt_t>));
	ASSERT((traits::is_voltage_unit<const volt_t&>));
	ASSERT(!(traits::is_voltage_unit<henry_t>));
	ASSERT((traits::is_voltage_unit<const volt_t&, volt_t>));
	ASSERT(!(traits::is_voltage_unit<henry_t, volt_t>));
}

void TypeTraits_is_capacitance_unit()
{
	ASSERT((traits::is_capacitance_unit<farad>));
	ASSERT(!(traits::is_capacitance_unit<ohm>));
	ASSERT(!(traits::is_capacitance_unit<double>));

	ASSERT((traits::is_capacitance_unit<farad_t>));
	ASSERT((traits::is_capacitance_unit<const farad_t>));
	ASSERT((traits::is_capacitance_unit<const farad_t&>));
	ASSERT(!(traits::is_capacitance_unit<ohm_t>));
	ASSERT((traits::is_capacitance_unit<const farad_t&, millifarad_t>));
	ASSERT(!(traits::is_capacitance_unit<ohm_t, farad_t>));
}

void TypeTraits_is_impedance_unit()
{

	ASSERT((traits::is_impedance_unit<ohm>));
	ASSERT(!(traits::is_impedance_unit<farad>));
	ASSERT(!(traits::is_impedance_unit<double>));

	ASSERT((traits::is_impedance_unit<ohm_t>));
	ASSERT((traits::is_impedance_unit<const ohm_t>));
	ASSERT((traits::is_impedance_unit<const ohm_t&>));
	ASSERT(!(traits::is_impedance_unit<farad_t>));
	ASSERT((traits::is_impedance_unit<const ohm_t&, milliohm_t>));
	ASSERT(!(traits::is_impedance_unit<farad_t, ohm_t>));
}

void TypeTraits_is_conductance_unit()
{
	ASSERT((traits::is_conductance_unit<siemen>));
	ASSERT(!(traits::is_conductance_unit<volt>));
	ASSERT(!(traits::is_conductance_unit<double>));

	ASSERT((traits::is_conductance_unit<siemen_t>));
	ASSERT((traits::is_conductance_unit<const siemen_t>));
	ASSERT((traits::is_conductance_unit<const siemen_t&>));
	ASSERT(!(traits::is_conductance_unit<volt_t>));
	ASSERT((traits::is_conductance_unit<const siemen_t&, millisiemen_t>));
	ASSERT(!(traits::is_conductance_unit<volt_t, siemen_t>));
}

void TypeTraits_is_magnetic_flux_unit()
{
	ASSERT((traits::is_magnetic_flux_unit<weber>));
	ASSERT((traits::is_magnetic_flux_unit<maxwell>));
	ASSERT(!(traits::is_magnetic_flux_unit<inch>));
	ASSERT(!(traits::is_magnetic_flux_unit<double>));

	ASSERT((traits::is_magnetic_flux_unit<weber_t>));
	ASSERT((traits::is_magnetic_flux_unit<const weber_t>));
	ASSERT((traits::is_magnetic_flux_unit<const weber_t&>));
	ASSERT((traits::is_magnetic_flux_unit<maxwell_t>));
	ASSERT(!(traits::is_magnetic_flux_unit<inch_t>));
	ASSERT((traits::is_magnetic_flux_unit<maxwell_t, weber_t>));
	ASSERT(!(traits::is_magnetic_flux_unit<inch_t, weber_t>));
}

void TypeTraits_is_magnetic_field_strength_unit()
{

	ASSERT((traits::is_magnetic_field_strength_unit<units::magnetic_field_strength::tesla>));
	ASSERT((traits::is_magnetic_field_strength_unit<gauss>));
	ASSERT(!(traits::is_magnetic_field_strength_unit<volt>));
	ASSERT(!(traits::is_magnetic_field_strength_unit<double>));

	ASSERT((traits::is_magnetic_field_strength_unit<tesla_t>));
	ASSERT((traits::is_magnetic_field_strength_unit<const tesla_t>));
	ASSERT((traits::is_magnetic_field_strength_unit<const tesla_t&>));
	ASSERT((traits::is_magnetic_field_strength_unit<gauss_t>));
	ASSERT(!(traits::is_magnetic_field_strength_unit<volt_t>));
	ASSERT((traits::is_magnetic_field_strength_unit<gauss_t, tesla_t>));
	ASSERT(!(traits::is_magnetic_field_strength_unit<volt_t, tesla_t>));
}

void TypeTraits_is_inductance_unit()
{

	ASSERT((traits::is_inductance_unit<henry>));
	ASSERT(!(traits::is_inductance_unit<farad>));
	ASSERT(!(traits::is_inductance_unit<double>));

	ASSERT((traits::is_inductance_unit<henry_t>));
	ASSERT((traits::is_inductance_unit<const henry_t>));
	ASSERT((traits::is_inductance_unit<const henry_t&>));
	ASSERT(!(traits::is_inductance_unit<farad_t>));
	ASSERT((traits::is_inductance_unit<const henry_t&, millihenry_t>));
	ASSERT(!(traits::is_inductance_unit<farad_t, henry_t>));
}

void TypeTraits_is_luminous_flux_unit()
{

	ASSERT((traits::is_luminous_flux_unit<lumen>));
	ASSERT(!(traits::is_luminous_flux_unit<pound>));
	ASSERT(!(traits::is_luminous_flux_unit<double>));

	ASSERT((traits::is_luminous_flux_unit<lumen_t>));
	ASSERT((traits::is_luminous_flux_unit<const lumen_t>));
	ASSERT((traits::is_luminous_flux_unit<const lumen_t&>));
	ASSERT(!(traits::is_luminous_flux_unit<pound_t>));
	ASSERT((traits::is_luminous_flux_unit<const lumen_t&, millilumen_t>));
	ASSERT(!(traits::is_luminous_flux_unit<pound_t, lumen_t>));
}

void TypeTraits_is_illuminance_unit()
{

	ASSERT((traits::is_illuminance_unit<illuminance::footcandle>));
	ASSERT((traits::is_illuminance_unit<illuminance::lux>));
	ASSERT(!(traits::is_illuminance_unit<meter>));
	ASSERT(!(traits::is_illuminance_unit<double>));

	ASSERT((traits::is_illuminance_unit<footcandle_t>));
	ASSERT((traits::is_illuminance_unit<const footcandle_t>));
	ASSERT((traits::is_illuminance_unit<const footcandle_t&>));
	ASSERT((traits::is_illuminance_unit<lux_t>));
	ASSERT(!(traits::is_illuminance_unit<meter_t>));
	ASSERT((traits::is_illuminance_unit<lux_t, footcandle_t>));
	ASSERT(!(traits::is_illuminance_unit<meter_t, footcandle_t>));
}

void TypeTraits_is_radioactivity_unit()
{

	ASSERT((traits::is_radioactivity_unit<becquerel>));
	ASSERT(!(traits::is_radioactivity_unit<year>));
	ASSERT(!(traits::is_radioactivity_unit<double>));

	ASSERT((traits::is_radioactivity_unit<becquerel_t>));
	ASSERT((traits::is_radioactivity_unit<const becquerel_t>));
	ASSERT((traits::is_radioactivity_unit<const becquerel_t&>));
	ASSERT(!(traits::is_radioactivity_unit<year_t>));
	ASSERT((traits::is_radioactivity_unit<const becquerel_t&, millibecquerel_t>));
	ASSERT(!(traits::is_radioactivity_unit<year_t, becquerel_t>));
}

void TypeTraits_is_torque_unit()
{
	ASSERT((traits::is_torque_unit<torque::newton_meter>));
	ASSERT((traits::is_torque_unit<torque::foot_pound>));
	ASSERT(!(traits::is_torque_unit<volume::cubic_meter>));
	ASSERT(!(traits::is_torque_unit<double>));

	ASSERT((traits::is_torque_unit<torque::newton_meter_t>));
	ASSERT((traits::is_torque_unit<const torque::newton_meter_t>));
	ASSERT((traits::is_torque_unit<const torque::newton_meter_t&>));
	ASSERT((traits::is_torque_unit<torque::foot_pound_t>));
	ASSERT(!(traits::is_torque_unit<volume::cubic_meter_t>));
	ASSERT((traits::is_torque_unit<torque::foot_pound_t, torque::newton_meter_t>));
	ASSERT(!(traits::is_torque_unit<volume::cubic_meter_t, torque::newton_meter_t>));
}

void TypeTraits_is_area_unit()
{

	ASSERT((traits::is_area_unit<square_meter>));
	ASSERT((traits::is_area_unit<hectare>));
	ASSERT(!(traits::is_area_unit<astronicalUnit>));
	ASSERT(!(traits::is_area_unit<double>));

	ASSERT((traits::is_area_unit<square_meter_t>));
	ASSERT((traits::is_area_unit<const square_meter_t>));
	ASSERT((traits::is_area_unit<const square_meter_t&>));
	ASSERT((traits::is_area_unit<hectare_t>));
	ASSERT(!(traits::is_area_unit<astronicalUnit_t>));
	ASSERT((traits::is_area_unit<hectare_t, square_meter_t>));
	ASSERT(!(traits::is_area_unit<astronicalUnit_t, square_meter_t>));
}

void TypeTraits_is_volume_unit()
{
	ASSERT((traits::is_volume_unit<cubic_meter>));
	ASSERT((traits::is_volume_unit<cubic_foot>));
	ASSERT(!(traits::is_volume_unit<square_feet>));
	ASSERT(!(traits::is_volume_unit<double>));

	ASSERT((traits::is_volume_unit<cubic_meter_t>));
	ASSERT((traits::is_volume_unit<const cubic_meter_t>));
	ASSERT((traits::is_volume_unit<const cubic_meter_t&>));
	ASSERT((traits::is_volume_unit<cubic_inch_t>));
	ASSERT(!(traits::is_volume_unit<foot_t>));
	ASSERT((traits::is_volume_unit<cubic_inch_t, cubic_meter_t>));
	ASSERT(!(traits::is_volume_unit<foot_t, cubic_meter_t>));
}

void TypeTraits_is_density_unit()
{
	ASSERT((traits::is_density_unit<kilograms_per_cubic_meter>));
	ASSERT((traits::is_density_unit<ounces_per_cubic_foot>));
	ASSERT(!(traits::is_density_unit<year>));
	ASSERT(!(traits::is_density_unit<double>));

	ASSERT((traits::is_density_unit<kilograms_per_cubic_meter_t>));
	ASSERT((traits::is_density_unit<const kilograms_per_cubic_meter_t>));
	ASSERT((traits::is_density_unit<const kilograms_per_cubic_meter_t&>));
	ASSERT((traits::is_density_unit<ounces_per_cubic_foot_t>));
	ASSERT(!(traits::is_density_unit<year_t>));
	ASSERT((traits::is_density_unit<ounces_per_cubic_foot_t, kilograms_per_cubic_meter_t>));
	ASSERT(!(traits::is_density_unit<year_t, kilograms_per_cubic_meter_t>));
}

void TypeTraits_is_data_unit()
{
	ASSERT((traits::is_data_unit<bit>));
	ASSERT((traits::is_data_unit<byte>));
	ASSERT((traits::is_data_unit<exabit>));
	ASSERT((traits::is_data_unit<exabyte>));
	ASSERT(!(traits::is_data_unit<year>));
	ASSERT(!(traits::is_data_unit<double>));

	ASSERT((traits::is_data_unit<bit_t>));
	ASSERT((traits::is_data_unit<const bit_t>));
	ASSERT((traits::is_data_unit<const bit_t&>));
	ASSERT((traits::is_data_unit<byte_t>));
	ASSERT(!(traits::is_data_unit<year_t>));
	ASSERT((traits::is_data_unit<bit_t, byte_t>));
	ASSERT(!(traits::is_data_unit<year_t, byte_t>));
}

void TypeTraits_is_data_transfer_rate_unit()
{
	ASSERT((traits::is_data_transfer_rate_unit<Gbps>));
	ASSERT((traits::is_data_transfer_rate_unit<GBps>));
	ASSERT(!(traits::is_data_transfer_rate_unit<year>));
	ASSERT(!(traits::is_data_transfer_rate_unit<double>));

	ASSERT((traits::is_data_transfer_rate_unit<gigabits_per_second_t>));
	ASSERT((traits::is_data_transfer_rate_unit<const gigabytes_per_second_t>));
	ASSERT((traits::is_data_transfer_rate_unit<const gigabytes_per_second_t&>));
	ASSERT((traits::is_data_transfer_rate_unit<gigabytes_per_second_t>));
	ASSERT(!(traits::is_data_transfer_rate_unit<year_t>));
	ASSERT((traits::is_data_transfer_rate_unit<gigabits_per_second_t, gigabytes_per_second_t>));
	ASSERT(!(traits::is_data_transfer_rate_unit<year_t, gigabytes_per_second_t>));
}

cute::suite make_suite_TypeTraits() {
	cute::suite s { };

	s.push_back(CUTE(TypeTraits_isRatio));
	s.push_back(CUTE(TypeTraits_ratio_sqrt));
	s.push_back(CUTE(TypeTraits_is_unit));
	s.push_back(CUTE(TypeTraits_is_unit_t));
	s.push_back(CUTE(TypeTraits_unit_traits));
	s.push_back(CUTE(TypeTraits_unit_t_traits));
	s.push_back(CUTE(TypeTraits_all_true));
	s.push_back(CUTE(TypeTraits_is_convertible_unit));
	s.push_back(CUTE(TypeTraits_inverse));
	s.push_back(CUTE(TypeTraits_base_unit_of));
	s.push_back(CUTE(TypeTraits_has_linear_scale));
	s.push_back(CUTE(TypeTraits_has_decibel_scale));
	s.push_back(CUTE(TypeTraits_is_same_scale));
	s.push_back(CUTE(TypeTraits_is_dimensionless_unit));
	s.push_back(CUTE(TypeTraits_is_length_unit));
	s.push_back(CUTE(TypeTraits_is_mass_unit));
	s.push_back(CUTE(TypeTraits_is_time_unit));
	s.push_back(CUTE(TypeTraits_is_angle_unit));
	s.push_back(CUTE(TypeTraits_is_current_unit));
	s.push_back(CUTE(TypeTraits_is_temperature_unit));
	s.push_back(CUTE(TypeTraits_is_substance_unit));
	s.push_back(CUTE(TypeTraits_is_luminous_intensity_unit));
	s.push_back(CUTE(TypeTraits_is_solid_angle_unit));
	s.push_back(CUTE(TypeTraits_is_frequency_unit));
	s.push_back(CUTE(TypeTraits_is_velocity_unit));
	s.push_back(CUTE(TypeTraits_is_acceleration_unit));
	s.push_back(CUTE(TypeTraits_is_force_unit));
	s.push_back(CUTE(TypeTraits_is_pressure_unit));
	s.push_back(CUTE(TypeTraits_is_charge_unit));
	s.push_back(CUTE(TypeTraits_is_energy_unit));
	s.push_back(CUTE(TypeTraits_is_power_unit));
	s.push_back(CUTE(TypeTraits_is_voltage_unit));
	s.push_back(CUTE(TypeTraits_is_capacitance_unit));
	s.push_back(CUTE(TypeTraits_is_impedance_unit));
	s.push_back(CUTE(TypeTraits_is_conductance_unit));
	s.push_back(CUTE(TypeTraits_is_magnetic_flux_unit));
	s.push_back(CUTE(TypeTraits_is_magnetic_field_strength_unit));
	s.push_back(CUTE(TypeTraits_is_inductance_unit));
	s.push_back(CUTE(TypeTraits_is_luminous_flux_unit));
	s.push_back(CUTE(TypeTraits_is_illuminance_unit));
	s.push_back(CUTE(TypeTraits_is_radioactivity_unit));
	s.push_back(CUTE(TypeTraits_is_torque_unit));
	s.push_back(CUTE(TypeTraits_is_area_unit));
	s.push_back(CUTE(TypeTraits_is_volume_unit));
	s.push_back(CUTE(TypeTraits_is_density_unit));
	s.push_back(CUTE(TypeTraits_is_data_unit));
	s.push_back(CUTE(TypeTraits_is_data_transfer_rate_unit));
	return s;
}
