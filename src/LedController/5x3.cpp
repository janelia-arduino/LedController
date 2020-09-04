// ----------------------------------------------------------------------------
// 5x3.cpp
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
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
const size_t user_enable_pins[CHANNEL_COUNT] = {4,4,4,4};
const uint8_t user_enabled_polarity[CHANNEL_COUNT] = {LOW,LOW,LOW,LOW};

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
