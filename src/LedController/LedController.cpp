// ----------------------------------------------------------------------------
// LedController.cpp
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#include "../LedController.h"


using namespace led_controller;

LedController::LedController()
{
}

void LedController::setup()
{
  // Parent Setup
  ModularDeviceBase::setup();

  // Event Controller Setup
  event_controller_.setup();

  // Pin Setup
  for (int channel=0; channel<constants::CHANNEL_COUNT; ++channel)
  {
    pinMode(constants::enable_pins[channel],OUTPUT);
    digitalWrite(constants::enable_pins[channel],LOW);
    pinMode(constants::dir_a_pins[channel],OUTPUT);
    pinMode(constants::dir_b_pins[channel],OUTPUT);
    pinMode(constants::user_enable_pins[channel],INPUT_PULLUP);
  }

  // Set Device ID
  modular_server_.setDeviceName(constants::device_name);

  // Add Hardware
  modular_server_.addHardware(constants::hardware_info,
    pins_);

  // Pins

  // Add Firmware
  modular_server_.addFirmware(constants::firmware_info,
    properties_,
    parameters_,
    functions_,
    callbacks_);
  // Properties
  modular_server::Property & polarity_reversed_property = modular_server_.createProperty(constants::polarity_reversed_property_name,constants::polarity_reversed_default);

  modular_server::Property & channels_enabled_property = modular_server_.createProperty(constants::channels_enabled_property_name,constants::channels_enabled_default);
  channels_enabled_property.attachPostSetElementValueFunctor(makeFunctor((Functor1<size_t> *)0,*this,&LedController::setChannelOff));

  // Parameters
  modular_server::Parameter & channel_parameter = modular_server_.createParameter(constants::channel_parameter_name);
  channel_parameter.setRange(0,constants::CHANNEL_COUNT-1);

  modular_server::Parameter & channels_parameter = modular_server_.createParameter(constants::channels_parameter_name);
  channels_parameter.setRange(0,constants::CHANNEL_COUNT-1);
  channels_parameter.setArrayLengthRange(1,constants::CHANNEL_COUNT);

  modular_server::Parameter & polarity_parameter = modular_server_.createParameter(constants::polarity_parameter_name);
  polarity_parameter.setTypeString();
  polarity_parameter.setSubset(constants::polarity_ptr_subset);

  modular_server::Parameter & delay_parameter = modular_server_.createParameter(constants::delay_parameter_name);
  delay_parameter.setRange(constants::delay_min,constants::delay_max);
  delay_parameter.setUnits(constants::ms_units);

  modular_server::Parameter & period_parameter = modular_server_.createParameter(constants::period_parameter_name);
  period_parameter.setRange(constants::period_min,constants::period_max);
  period_parameter.setUnits(constants::ms_units);

  modular_server::Parameter & on_duration_parameter = modular_server_.createParameter(constants::on_duration_parameter_name);
  on_duration_parameter.setRange(constants::on_duration_min,constants::on_duration_max);
  on_duration_parameter.setUnits(constants::ms_units);

  modular_server::Parameter & count_parameter = modular_server_.createParameter(constants::count_parameter_name);
  count_parameter.setRange(constants::count_min,constants::count_max);
  count_parameter.setUnits(constants::ms_units);

  modular_server::Parameter & pwm_index_parameter = modular_server_.createParameter(constants::pwm_index_parameter_name);
  pwm_index_parameter.setRange(0,constants::INDEXED_PULSES_COUNT_MAX-1);

  // Functions
  modular_server::Function & set_channel_on_function = modular_server_.createFunction(constants::set_channel_on_function_name);
  set_channel_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LedController::setChannelOnHandler));
  set_channel_on_function.addParameter(channel_parameter);
  set_channel_on_function.addParameter(polarity_parameter);

  modular_server::Function & set_channel_off_function = modular_server_.createFunction(constants::set_channel_off_function_name);
  set_channel_off_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LedController::setChannelOffHandler));
  set_channel_off_function.addParameter(channel_parameter);

  modular_server::Function & set_channels_on_function = modular_server_.createFunction(constants::set_channels_on_function_name);
  set_channels_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LedController::setChannelsOnHandler));
  set_channels_on_function.addParameter(channels_parameter);
  set_channels_on_function.addParameter(polarity_parameter);

  modular_server::Function & set_channels_off_function = modular_server_.createFunction(constants::set_channels_off_function_name);
  set_channels_off_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LedController::setChannelsOffHandler));
  set_channels_off_function.addParameter(channels_parameter);

  modular_server::Function & set_all_channels_on_function = modular_server_.createFunction(constants::set_all_channels_on_function_name);
  set_all_channels_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LedController::setAllChannelsOnHandler));
  set_all_channels_on_function.addParameter(polarity_parameter);

  modular_server::Function & set_all_channels_off_function = modular_server_.createFunction(constants::set_all_channels_off_function_name);
  set_all_channels_off_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LedController::setAllChannelsOffHandler));

  modular_server::Function & channel_on_function = modular_server_.createFunction(constants::channel_on_function_name);
  channel_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LedController::channelOnHandler));
  channel_on_function.addParameter(channel_parameter);
  channel_on_function.setResultTypeBool();

  modular_server::Function & channels_on_function = modular_server_.createFunction(constants::channels_on_function_name);
  channels_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LedController::channelsOnHandler));
  channels_on_function.setResultTypeArray();
  channels_on_function.setResultTypeLong();

  modular_server::Function & add_pwm_function = modular_server_.createFunction(constants::add_pwm_function_name);
  add_pwm_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LedController::addPwmHandler));
  add_pwm_function.addParameter(channels_parameter);
  add_pwm_function.addParameter(polarity_parameter);
  add_pwm_function.addParameter(delay_parameter);
  add_pwm_function.addParameter(period_parameter);
  add_pwm_function.addParameter(on_duration_parameter);
  add_pwm_function.addParameter(count_parameter);
  add_pwm_function.setResultTypeLong();

  modular_server::Function & start_pwm_function = modular_server_.createFunction(constants::start_pwm_function_name);
  start_pwm_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LedController::startPwmHandler));
  start_pwm_function.addParameter(channels_parameter);
  start_pwm_function.addParameter(polarity_parameter);
  start_pwm_function.addParameter(delay_parameter);
  start_pwm_function.addParameter(period_parameter);
  start_pwm_function.addParameter(on_duration_parameter);
  start_pwm_function.setResultTypeLong();

  modular_server::Function & add_toggle_pwm_function = modular_server_.createFunction(constants::add_toggle_pwm_function_name);
  add_toggle_pwm_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LedController::addTogglePwmHandler));
  add_toggle_pwm_function.addParameter(channels_parameter);
  add_toggle_pwm_function.addParameter(polarity_parameter);
  add_toggle_pwm_function.addParameter(delay_parameter);
  add_toggle_pwm_function.addParameter(period_parameter);
  add_toggle_pwm_function.addParameter(on_duration_parameter);
  add_toggle_pwm_function.addParameter(count_parameter);
  add_toggle_pwm_function.setResultTypeLong();

  modular_server::Function & start_toggle_pwm_function = modular_server_.createFunction(constants::start_toggle_pwm_function_name);
  start_toggle_pwm_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LedController::startTogglePwmHandler));
  start_toggle_pwm_function.addParameter(channels_parameter);
  start_toggle_pwm_function.addParameter(polarity_parameter);
  start_toggle_pwm_function.addParameter(delay_parameter);
  start_toggle_pwm_function.addParameter(period_parameter);
  start_toggle_pwm_function.addParameter(on_duration_parameter);
  start_toggle_pwm_function.setResultTypeLong();

  modular_server::Function & stop_pwm_function = modular_server_.createFunction(constants::stop_pwm_function_name);
  stop_pwm_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LedController::stopPwmHandler));
  stop_pwm_function.addParameter(pwm_index_parameter);

  modular_server::Function & stop_all_pwm_function = modular_server_.createFunction(constants::stop_all_pwm_function_name);
  stop_all_pwm_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LedController::stopAllPwmHandler));

  modular_server::Function & board_switch_enabled_function = modular_server_.createFunction(constants::board_switch_enabled_function_name);
  board_switch_enabled_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LedController::boardSwitchEnabledHandler));
  board_switch_enabled_function.setResultTypeArray();
  board_switch_enabled_function.setResultTypeBool();

  modular_server::Function & board_switch_and_property_enabled_function = modular_server_.createFunction(constants::board_switch_and_property_enabled_function_name);
  board_switch_and_property_enabled_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LedController::boardSwitchAndPropertyEnabledHandler));
  board_switch_and_property_enabled_function.setResultTypeArray();
  board_switch_and_property_enabled_function.setResultTypeBool();

  // Callbacks

}

void LedController::update()
{
  // Parent Update
  ModularDeviceBase::update();

  bool enabled;
  for (int channel=0; channel<constants::CHANNEL_COUNT; ++channel)
  {
    enabled = (digitalRead(constants::user_enable_pins[channel]) ==
      constants::user_enabled_polarity[channel]);
    board_switch_enabled_[channel] = enabled;
    if (!enabled && channelOn(channel))
    {
      setChannelOff(channel);
    }
  }
}

void LedController::setChannelOn(size_t channel,
  const ConstantString & polarity)
{
  if (!boardSwitchAndPropertyEnabled(channel))
  {
    return;
  }
  bool channel_polarity_reversed;
  modular_server_.property(constants::polarity_reversed_property_name).getElementValue(channel,
    channel_polarity_reversed);
  const ConstantString * polarity_corrected_ptr = &polarity;
  if (channel_polarity_reversed)
  {
    polarity_corrected_ptr = ((&polarity == &constants::polarity_positive) ? &constants::polarity_negative : &constants::polarity_positive);
  }
  if (polarity_corrected_ptr == &constants::polarity_positive)
  {
    digitalWrite(constants::dir_a_pins[channel],HIGH);
    digitalWrite(constants::dir_b_pins[channel],LOW);
  }
  else
  {
    digitalWrite(constants::dir_a_pins[channel],LOW);
    digitalWrite(constants::dir_b_pins[channel],HIGH);
  }
  digitalWrite(constants::enable_pins[channel],HIGH);
  channels_on_[channel] = true;
}

void LedController::setChannelOff(size_t channel)
{
  digitalWrite(constants::enable_pins[channel],LOW);
  channels_on_[channel] = false;
}

void LedController::setChannelsOn(uint32_t channels,
  const ConstantString & polarity)
{
  uint32_t bit = 1;
  for (int channel=0; channel<constants::CHANNEL_COUNT; ++channel)
  {
    if (channels & (bit << channel))
    {
      setChannelOn(channel,polarity);
    }
  }
}

void LedController::setChannelsOff(uint32_t channels)
{
  uint32_t bit = 1;
  for (int channel=0; channel<constants::CHANNEL_COUNT; ++channel)
  {
    if (channels & (bit << channel))
    {
      setChannelOff(channel);
    }
  }
}

void LedController::setAllChannelsOn(const ConstantString & polarity)
{
  for (int channel=0; channel<constants::CHANNEL_COUNT; ++channel)
  {
    setChannelOn(channel,polarity);
  }
}

void LedController::setAllChannelsOff()
{
  for (int channel=0; channel<constants::CHANNEL_COUNT; ++channel)
  {
    setChannelOff(channel);
  }
}

bool LedController::channelOn(size_t channel)
{
  return channels_on_[channel];
}

uint32_t LedController::channelsOn()
{
  uint32_t channels_on = 0;
  uint32_t bit = 1;
  for (int channel=0; channel<constants::CHANNEL_COUNT; ++channel)
  {
    if (channelOn(channel))
    {
      channels_on |= bit << channel;
    }
  }
  return channels_on;
}

int LedController::addPwm(uint32_t channels,
  const ConstantString & polarity,
  long delay,
  long period,
  long on_duration,
  long count)
{
  if (indexed_pulses_.full())
  {
    return constants::bad_index;
  }
  led_controller::constants::PulseInfo pulse_info;
  pulse_info.channels = channels;
  pulse_info.polarity_ptr = &polarity;
  int index = indexed_pulses_.add(pulse_info);
  EventIdPair event_id_pair = event_controller_.addPwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&LedController::setChannelsOnHandler),
    makeFunctor((Functor1<int> *)0,*this,&LedController::setChannelsOffHandler),
    delay,
    period,
    on_duration,
    count,
    index);
  event_controller_.addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&LedController::startPwmHandler));
  event_controller_.addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&LedController::stopPwmHandler));
  indexed_pulses_[index].event_id_pair = event_id_pair;
  event_controller_.enable(event_id_pair);
  return index;
}

int LedController::startPwm(uint32_t channels,
  const ConstantString & polarity,
  long delay,
  long period,
  long on_duration)
{
  if (indexed_pulses_.full())
  {
    return -1;
  }
  led_controller::constants::PulseInfo pulse_info;
  pulse_info.channels = channels;
  pulse_info.polarity_ptr = &polarity;
  int index = indexed_pulses_.add(pulse_info);
  EventIdPair event_id_pair = event_controller_.addInfinitePwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&LedController::setChannelsOnHandler),
    makeFunctor((Functor1<int> *)0,*this,&LedController::setChannelsOffHandler),
    delay,
    period,
    on_duration,
    index);
  event_controller_.addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&LedController::startPwmHandler));
  event_controller_.addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&LedController::stopPwmHandler));
  indexed_pulses_[index].event_id_pair = event_id_pair;
  event_controller_.enable(event_id_pair);
  return index;
}

int LedController::addTogglePwm(uint32_t channels,
  const ConstantString & polarity,
  long delay,
  long period,
  long on_duration,
  long count)
{
  if (indexed_pulses_.full())
  {
    return constants::bad_index;
  }
  led_controller::constants::PulseInfo pulse_info;
  pulse_info.channels = channels;
  pulse_info.polarity_ptr = &polarity;
  int index = indexed_pulses_.add(pulse_info);
  EventIdPair event_id_pair = event_controller_.addPwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&LedController::setChannelsOnHandler),
    makeFunctor((Functor1<int> *)0,*this,&LedController::setChannelsOnReversedHandler),
    delay,
    period,
    on_duration,
    count,
    index);
  event_controller_.addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&LedController::startPwmHandler));
  event_controller_.addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&LedController::stopPwmHandler));
  indexed_pulses_[index].event_id_pair = event_id_pair;
  event_controller_.enable(event_id_pair);
  return index;
}

int LedController::startTogglePwm(uint32_t channels,
  const ConstantString & polarity,
  long delay,
  long period,
  long on_duration)
{
  if (indexed_pulses_.full())
  {
    return -1;
  }
  led_controller::constants::PulseInfo pulse_info;
  pulse_info.channels = channels;
  pulse_info.polarity_ptr = &polarity;
  int index = indexed_pulses_.add(pulse_info);
  EventIdPair event_id_pair = event_controller_.addInfinitePwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&LedController::setChannelsOnHandler),
    makeFunctor((Functor1<int> *)0,*this,&LedController::setChannelsOnReversedHandler),
    delay,
    period,
    on_duration,
    index);
  event_controller_.addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&LedController::startPwmHandler));
  event_controller_.addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&LedController::stopPwmHandler));
  indexed_pulses_[index].event_id_pair = event_id_pair;
  event_controller_.enable(event_id_pair);
  return index;
}

void LedController::stopPwm(int pwm_index)
{
  if (pwm_index < 0)
  {
    return;
  }
  if (indexed_pulses_.indexHasValue(pwm_index))
  {
    constants::PulseInfo pulse_info = indexed_pulses_[pwm_index];
    event_controller_.remove(pulse_info.event_id_pair);
  }
}

void LedController::stopAllPwm()
{
  for (size_t i=0; i<constants::INDEXED_PULSES_COUNT_MAX; ++i)
  {
    stopPwm(i);
  }
}

uint32_t LedController::arrayToChannels(ArduinoJson::JsonArray & channels_array)
{
  uint32_t channels = 0;
  uint32_t bit = 1;
  for (ArduinoJson::JsonArray::iterator channels_it=channels_array.begin();
       channels_it != channels_array.end();
       ++channels_it)
  {
    long channel = *channels_it;
    channels |= bit << channel;
  }
  return channels;
}

const ConstantString & LedController::stringToPolarity(const char * string)
{
  if (string == constants::polarity_positive)
  {
    return constants::polarity_positive;
  }
  else
  {
    return constants::polarity_negative;
  }
}

bool LedController::boardSwitchEnabled(size_t channel)
{
  return board_switch_enabled_[channel];
}

bool LedController::boardSwitchAndPropertyEnabled(size_t channel)
{
  bool channel_enabled;
  modular_server_.property(constants::channels_enabled_property_name).getElementValue(channel,
    channel_enabled);
  return (channel_enabled && boardSwitchEnabled(channel));
}

// Handlers must be non-blocking (avoid 'delay')
//
// modular_server_.parameter(parameter_name).getValue(value) value type must be either:
// fixed-point number (int, long, etc.)
// floating-point number (float, double)
// bool
// const char *
// ArduinoJson::JsonArray *
// ArduinoJson::JsonObject *
// const ConstantString *
//
// For more info read about ArduinoJson parsing https://github.com/janelia-arduino/ArduinoJson
//
// modular_server_.property(property_name).getValue(value) value type must match the property default type
// modular_server_.property(property_name).setValue(value) value type must match the property default type
// modular_server_.property(property_name).getElementValue(element_index,value) value type must match the property array element default type
// modular_server_.property(property_name).setElementValue(element_index,value) value type must match the property array element default type

void LedController::startPwmHandler(int index)
{
}

void LedController::stopPwmHandler(int index)
{
  uint32_t & channels = indexed_pulses_[index].channels;
  setChannelsOff(channels);
  indexed_pulses_.remove(index);
}

void LedController::setChannelOnHandler()
{
  int channel;
  modular_server_.parameter(constants::channel_parameter_name).getValue(channel);
  const char * polarity_string;
  modular_server_.parameter(constants::polarity_parameter_name).getValue(polarity_string);
  const ConstantString & polarity = stringToPolarity(polarity_string);
  setChannelOn(channel,polarity);
}

void LedController::setChannelOffHandler()
{
  int channel;
  modular_server_.parameter(constants::channel_parameter_name).getValue(channel);
  setChannelOff(channel);
}

void LedController::setChannelsOnHandler()
{
  ArduinoJson::JsonArray * channels_array_ptr;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array_ptr);
  const char * polarity_string;
  modular_server_.parameter(constants::polarity_parameter_name).getValue(polarity_string);
  const uint32_t channels = arrayToChannels(*channels_array_ptr);
  const ConstantString & polarity = stringToPolarity(polarity_string);
  setChannelsOn(channels,polarity);
}

void LedController::setChannelsOffHandler()
{
  ArduinoJson::JsonArray * channels_array_ptr;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array_ptr);
  const uint32_t channels = arrayToChannels(*channels_array_ptr);
  setChannelsOff(channels);
}

void LedController::setAllChannelsOnHandler()
{
  const char * polarity_string;
  modular_server_.parameter(constants::polarity_parameter_name).getValue(polarity_string);
  const ConstantString & polarity = stringToPolarity(polarity_string);
  setAllChannelsOn(polarity);
}

void LedController::setAllChannelsOffHandler()
{
  setAllChannelsOff();
}

void LedController::channelOnHandler()
{
  int channel;
  modular_server_.parameter(constants::channel_parameter_name).getValue(channel);
  modular_server_.response().returnResult(channelOn(channel));
}

void LedController::channelsOnHandler()
{
  modular_server_.response().writeResultKey();

  modular_server_.response().beginArray();

  for (int channel=0; channel<constants::CHANNEL_COUNT; ++channel)
  {
    modular_server_.response().write(channelOn(channel));
  }

  modular_server_.response().endArray();

}

void LedController::addPwmHandler()
{
  ArduinoJson::JsonArray * channels_array_ptr;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array_ptr);
  const char * polarity_string;
  modular_server_.parameter(constants::polarity_parameter_name).getValue(polarity_string);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  long period;
  modular_server_.parameter(constants::period_parameter_name).getValue(period);
  long on_duration;
  modular_server_.parameter(constants::on_duration_parameter_name).getValue(on_duration);
  long count;
  modular_server_.parameter(constants::count_parameter_name).getValue(count);
  const uint32_t channels = arrayToChannels(*channels_array_ptr);
  const ConstantString & polarity = stringToPolarity(polarity_string);
  int index = addPwm(channels,polarity,delay,period,on_duration,count);
  if (index >= 0)
  {
    modular_server_.response().returnResult(index);
  }
  else
  {
    modular_server_.response().returnError(constants::pwm_error);
  }
}

void LedController::startPwmHandler()
{
  ArduinoJson::JsonArray * channels_array_ptr;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array_ptr);
  const char * polarity_string;
  modular_server_.parameter(constants::polarity_parameter_name).getValue(polarity_string);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  long period;
  modular_server_.parameter(constants::period_parameter_name).getValue(period);
  long on_duration;
  modular_server_.parameter(constants::on_duration_parameter_name).getValue(on_duration);
  const uint32_t channels = arrayToChannels(*channels_array_ptr);
  const ConstantString & polarity = stringToPolarity(polarity_string);
  int index = startPwm(channels,polarity,delay,period,on_duration);
  if (index >= 0)
  {
    modular_server_.response().returnResult(index);
  }
  else
  {
    modular_server_.response().returnError(constants::pwm_error);
  }
}

void LedController::addTogglePwmHandler()
{
  ArduinoJson::JsonArray * channels_array_ptr;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array_ptr);
  const char * polarity_string;
  modular_server_.parameter(constants::polarity_parameter_name).getValue(polarity_string);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  long period;
  modular_server_.parameter(constants::period_parameter_name).getValue(period);
  long on_duration;
  modular_server_.parameter(constants::on_duration_parameter_name).getValue(on_duration);
  long count;
  modular_server_.parameter(constants::count_parameter_name).getValue(count);
  const uint32_t channels = arrayToChannels(*channels_array_ptr);
  const ConstantString & polarity = stringToPolarity(polarity_string);
  int index = addTogglePwm(channels,polarity,delay,period,on_duration,count);
  if (index >= 0)
  {
    modular_server_.response().returnResult(index);
  }
  else
  {
    modular_server_.response().returnError(constants::pwm_error);
  }
}

void LedController::startTogglePwmHandler()
{
  ArduinoJson::JsonArray * channels_array_ptr;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array_ptr);
  const char * polarity_string;
  modular_server_.parameter(constants::polarity_parameter_name).getValue(polarity_string);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  long period;
  modular_server_.parameter(constants::period_parameter_name).getValue(period);
  long on_duration;
  modular_server_.parameter(constants::on_duration_parameter_name).getValue(on_duration);
  const uint32_t channels = arrayToChannels(*channels_array_ptr);
  const ConstantString & polarity = stringToPolarity(polarity_string);
  int index = startTogglePwm(channels,polarity,delay,period,on_duration);
  if (index >= 0)
  {
    modular_server_.response().returnResult(index);
  }
  else
  {
    modular_server_.response().returnError(constants::pwm_error);
  }
}

void LedController::stopPwmHandler()
{
  int pwm_index;
  modular_server_.parameter(constants::pwm_index_parameter_name).getValue(pwm_index);
  stopPwm(pwm_index);
}

void LedController::stopAllPwmHandler()
{
  stopAllPwm();
}

void LedController::boardSwitchEnabledHandler()
{
  modular_server_.response().writeResultKey();

  modular_server_.response().beginArray();

  for (int channel=0; channel<constants::CHANNEL_COUNT; ++channel)
  {
    modular_server_.response().write(boardSwitchEnabled(channel));
  }

  modular_server_.response().endArray();

}

void LedController::boardSwitchAndPropertyEnabledHandler()
{
  modular_server_.response().writeResultKey();

  modular_server_.response().beginArray();

  for (int channel=0; channel<constants::CHANNEL_COUNT; ++channel)
  {
    modular_server_.response().write(boardSwitchAndPropertyEnabled(channel));
  }

  modular_server_.response().endArray();

}

void LedController::setChannelsOnHandler(int index)
{
  uint32_t & channels = indexed_pulses_[index].channels;
  const ConstantString & polarity = *indexed_pulses_[index].polarity_ptr;
  setChannelsOn(channels,polarity);
}

void LedController::setChannelsOffHandler(int index)
{
  uint32_t & channels = indexed_pulses_[index].channels;
  setChannelsOff(channels);
}

void LedController::setChannelsOnReversedHandler(int index)
{
  uint32_t & channels = indexed_pulses_[index].channels;
  const ConstantString & polarity = *indexed_pulses_[index].polarity_ptr;
  const ConstantString * polarity_reversed_ptr;
  polarity_reversed_ptr = ((&polarity == &constants::polarity_positive) ? &constants::polarity_negative : &constants::polarity_positive);
  setChannelsOn(channels,*polarity_reversed_ptr);
}
