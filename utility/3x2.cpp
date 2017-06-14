// ----------------------------------------------------------------------------
// 3x2.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "3x2.h"


#if defined(__MK20DX256__)

namespace led_controller
{
namespace constants
{
const modular_server::HardwareInfo hardware_info =
  {
    .name_ptr=&hardware_name,
    .part_number=1161,
    .version_major=1,
    .version_minor=0,
  };

const size_t enable_pins[CHANNEL_COUNT] = {22,23};
const size_t dir_a_pins[CHANNEL_COUNT] = {18,20};
const size_t dir_b_pins[CHANNEL_COUNT] = {19,21};
const size_t sense_pins[CHANNEL_COUNT] = {0,1};
const size_t user_enable_pins[CHANNEL_COUNT] = {5,6};
const uint8_t user_enabled_polarity[CHANNEL_COUNT] = {LOW,LOW};

// Interrupts

// Units

// Properties
const bool polarity_reversed_default[CHANNEL_COUNT] = {false,false};

const bool channels_enabled_default[CHANNEL_COUNT] = {true,true};

// Parameters

// Functions

// Callbacks

// Errors
}
}
#endif
