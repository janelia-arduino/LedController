// ----------------------------------------------------------------------------
// 5x3.h
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef LED_CONTROLLER_5X3_CONSTANTS_H
#define LED_CONTROLLER_5X3_CONSTANTS_H
#include "Constants.h"


#if defined(__MK64FX512__)

namespace led_controller
{
namespace constants
{
//MAX values must be >= 1, >= created/copied count, < RAM limit
enum{PIN_COUNT_MAX=4};

enum {CHANNEL_COUNT=4};

extern const size_t enable_pins[CHANNEL_COUNT];
extern const size_t dir_a_pins[CHANNEL_COUNT];
extern const size_t dir_b_pins[CHANNEL_COUNT];
extern const size_t sense_pins[CHANNEL_COUNT];
extern const size_t user_enable_pins[CHANNEL_COUNT];
extern const uint8_t user_enabled_polarity[CHANNEL_COUNT];

// Pins

// Units

// Properties
// Property values must be long, double, bool, long[], double[], bool[], char[], ConstantString *, (ConstantString *)[]
extern const bool polarity_reversed_default[CHANNEL_COUNT];

extern const bool channels_enabled_default[CHANNEL_COUNT];

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
//MAX values must be >= 1, >= created/copied count, < RAM limit
enum{PIN_COUNT_MAX=1};

enum {CHANNEL_COUNT=4};

extern const size_t enable_pins[CHANNEL_COUNT];
extern const size_t dir_a_pins[CHANNEL_COUNT];
extern const size_t dir_b_pins[CHANNEL_COUNT];
extern const size_t sense_pins[CHANNEL_COUNT];

// Pins

// Units

// Properties
// Property values must be long, double, bool, long[], double[], bool[], char[], ConstantString *, (ConstantString *)[]
extern const bool polarity_reversed_default[CHANNEL_COUNT];

extern const bool channels_enabled_default[CHANNEL_COUNT];

// Parameters

// Functions

// Callbacks

// Errors
}
}
#endif
#endif
