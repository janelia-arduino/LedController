// ----------------------------------------------------------------------------
// 5x3.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "5x3.h"


#if defined(__MK64FX512__)

namespace led_controller
{
namespace constants
{
const modular_server::HardwareInfo hardware_info =
{
  .name_ptr=&hardware_name,
  .part_number=1160,
  .version_major=1,
  .version_minor=0,
};

const size_t enable_pins[CHANNEL_COUNT] = {8,9,10,11};
const size_t dir_a_pins[CHANNEL_COUNT] = {30,32,34,36};
const size_t dir_b_pins[CHANNEL_COUNT] = {31,33,35,37};
const size_t sense_pins[CHANNEL_COUNT] = {0,1,2,3};

// Pins

// Units

// Properties
const bool polarity_reversed_default[CHANNEL_COUNT] = {false,false,false,false};

const bool channels_enabled_default[CHANNEL_COUNT] = {true,true,true,true};

// Parameters

// Functions

// Callbacks

// Errors
}
}

#elif defined(__AVR_ATmega2560__)

namespace led_controller
{
namespace constants
{
const modular_server::HardwareInfo hardware_info =
{
  .name_ptr=&hardware_name,
  .part_number=1160,
  .version_major=1,
  .version_minor=0,
};

const size_t enable_pins[CHANNEL_COUNT] = {8,9,10,11};
const size_t dir_a_pins[CHANNEL_COUNT] = {30,32,34,36};
const size_t dir_b_pins[CHANNEL_COUNT] = {31,33,35,37};
const size_t sense_pins[CHANNEL_COUNT] = {0,1,2,3};

// Pins
CONSTANT_STRING(bnc_b_pin_name,"bnc_b");
const size_t bnc_b_pin = 3;

CONSTANT_STRING(switch_0_pin_name,"switch_0");
const size_t switch_0_pin = 21;

CONSTANT_STRING(switch_1_pin_name,"switch_1");
const size_t switch_1_pin = 20;

CONSTANT_STRING(switch_2_pin_name,"switch_2");
const size_t switch_2_pin = 2;

// Units

// Properties
const bool polarity_reversed_default[CHANNEL_COUNT] = {false,false,false,false};

const bool channels_enabled_default[CHANNEL_COUNT] = {true,true,true,true};

// Parameters

// Functions

// Callbacks

// Errors
}
}
#endif
