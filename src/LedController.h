// ----------------------------------------------------------------------------
// LedController.h
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H
#include <ArduinoJson.h>
#include <JsonStream.h>
#include <Array.h>
#include <Vector.h>
#include <ConstantVariable.h>
#include <SavedVariable.h>
#include <Functor.h>
#include <IndexedContainer.h>
#include <FunctorCallbacks.h>

#include <EventController.h>

#include <ModularClient.h>

#include <ModularServer.h>
#include <ModularDeviceBase.h>

#include "LedController/Constants.h"


class LedController : public ModularDeviceBase
{
public:
  LedController();
  virtual void setup();
  virtual void update();
  void setChannelOn(const size_t channel, const ConstantString & polarity);
  void setChannelOff(const size_t channel);
  void setChannelsOn(const uint32_t channels, const ConstantString & polarity);
  void setChannelsOff(const uint32_t channels);
  void setAllChannelsOn(const ConstantString & polarity);
  void setAllChannelsOff();
  bool channelOn(const size_t channel);
  uint32_t channelsOn();

  int addPwm(const uint32_t channels,
             const ConstantString & polarity,
             const long delay,
             const long period,
             const long on_duration,
             const long count);
  int startPwm(const uint32_t channels,
               const ConstantString & polarity,
               const long delay,
               const long period,
               const long on_duration);
  int addTogglePwm(const uint32_t channels,
                   const ConstantString & polarity,
                   const long delay,
                   const long period,
                   const long on_duration,
                   const long count);
  int startTogglePwm(const uint32_t channels,
                     const ConstantString & polarity,
                     const long delay,
                     const long period,
                     const long on_duration);
  void stopPwm(const int pwm_index);
  void stopAllPwm();
  uint32_t arrayToChannels(ArduinoJson::JsonArray & channels_array);
  const ConstantString & stringToPolarity(const char * string);

  bool boardSwitchEnabled(const size_t channel);
  bool boardSwitchAndPropertyEnabled(const size_t channel);

  // Handlers
  virtual void startPwmHandler(int index);
  virtual void stopPwmHandler(int index);

private:
  modular_server::Pin pins_[led_controller::constants::PIN_COUNT_MAX];

  modular_server::Property properties_[led_controller::constants::PROPERTY_COUNT_MAX];
  modular_server::Parameter parameters_[led_controller::constants::PARAMETER_COUNT_MAX];
  modular_server::Function functions_[led_controller::constants::FUNCTION_COUNT_MAX];
  modular_server::Callback callbacks_[led_controller::constants::CALLBACK_COUNT_MAX];

  EventController<led_controller::constants::EVENT_COUNT_MAX> event_controller_;

  IndexedContainer<led_controller::constants::PulseInfo,
                   led_controller::constants::INDEXED_PULSES_COUNT_MAX> indexed_pulses_;

  bool board_switch_enabled_[led_controller::constants::CHANNEL_COUNT];
  bool channels_on_[led_controller::constants::CHANNEL_COUNT];

  // Handlers
  void setChannelOnHandler();
  void setChannelOffHandler();
  void setChannelsOnHandler();
  void setChannelsOffHandler();
  void setAllChannelsOnHandler();
  void setAllChannelsOffHandler();
  void channelOnHandler();
  void channelsOnHandler();
  void addPwmHandler();
  void startPwmHandler();
  void addTogglePwmHandler();
  void startTogglePwmHandler();
  void stopPwmHandler();
  void stopAllPwmHandler();
  void boardSwitchEnabledHandler();
  void boardSwitchAndPropertyEnabledHandler();
  void setChannelsOnHandler(int index);
  void setChannelsOffHandler(int index);
  void setChannelsOnReversedHandler(int index);

};

#endif
