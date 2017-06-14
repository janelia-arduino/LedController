// ----------------------------------------------------------------------------
// Constants.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef LED_CONTROLLER_CONSTANTS_H
#define LED_CONTROLLER_CONSTANTS_H
#include "ConstantVariable.h"
#include "ModularServer.h"

#include "EventController.h"


namespace led_controller
{
namespace constants
{
//MAX values must be >= 1, >= created/copied count, < RAM limit
enum{PROPERTY_COUNT_MAX=2};
enum{PARAMETER_COUNT_MAX=8};
enum{FUNCTION_COUNT_MAX=16};
enum{CALLBACK_COUNT_MAX=1};

extern ConstantString device_name;

extern ConstantString firmware_name;
extern const modular_server::FirmwareInfo firmware_info;

extern ConstantString hardware_name;
extern const modular_server::HardwareInfo hardware_info;

enum{EVENT_COUNT_MAX=8};
enum{INDEXED_PULSES_COUNT_MAX=4};
extern const int bad_index;

struct PulseInfo
{
  uint32_t channels;
  const ConstantString * polarity_ptr;
  EventIdPair event_id_pair;
};

// Interrupts

// Units
extern ConstantString ms_units;

// Properties
// Property values must be long, double, bool, long[], double[], bool[], char[], ConstantString *, (ConstantString *)[]
extern ConstantString polarity_reversed_property_name;

extern ConstantString channels_enabled_property_name;

// Parameters
extern ConstantString channel_parameter_name;

extern ConstantString channels_parameter_name;

extern ConstantString polarity_parameter_name;
enum{POLARITY_SUBSET_LENGTH=2};
extern ConstantString polarity_positive;
extern ConstantString polarity_negative;
extern modular_server::SubsetMemberType polarity_ptr_subset[POLARITY_SUBSET_LENGTH];

extern ConstantString delay_parameter_name;
extern const long delay_min;
extern const long delay_max;

extern ConstantString period_parameter_name;
extern const long period_min;
extern const long period_max;

extern ConstantString on_duration_parameter_name;
extern const long on_duration_min;
extern const long on_duration_max;

extern ConstantString count_parameter_name;
extern const long count_min;
extern const long count_max;

extern ConstantString pwm_index_parameter_name;

// Functions
extern ConstantString set_channel_on_function_name;
extern ConstantString set_channel_off_function_name;
extern ConstantString set_channels_on_function_name;
extern ConstantString set_channels_off_function_name;
extern ConstantString set_all_channels_on_function_name;
extern ConstantString set_all_channels_off_function_name;
extern ConstantString channel_on_function_name;
extern ConstantString channels_on_function_name;
extern ConstantString add_pwm_function_name;
extern ConstantString start_pwm_function_name;
extern ConstantString add_toggle_pwm_function_name;
extern ConstantString start_toggle_pwm_function_name;
extern ConstantString stop_pwm_function_name;
extern ConstantString stop_all_pwm_function_name;
extern ConstantString board_switch_enabled_function_name;
extern ConstantString board_switch_and_property_enabled_function_name;

// Callbacks

// Errors
extern ConstantString pwm_error;
}
}
#include "5x3.h"
#include "3x2.h"
#endif
