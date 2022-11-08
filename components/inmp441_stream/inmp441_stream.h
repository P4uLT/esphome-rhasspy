#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/switch/switch.h"

// #include "StateMachine.hpp"

namespace esphome {
namespace inmp441_stream {

class Inmp441StreamComponent : public Component {
 public:
  void setup() override;
  float get_setup_priority() const override { return esphome::setup_priority::LATE; }

  void loop() override;

  void dump_config() override;

  void set_sck_pin(uint8_t pin) { this->sck_pin_ = pin; }
  void set_ws_pin(uint8_t pin) { this->ws_pin_ = pin; }
  void set_sd_pin(uint8_t pin) { this->sd_pin_ = pin; }

  void set_mute_binary_sensor(binary_sensor::BinarySensor *mute_binary_sensor) {
    mute_binary_sensor_ = mute_binary_sensor;
  }

  void set_output_switch(switch_::Switch *output_switch) { output_switch_ = output_switch; }

 protected:
  binary_sensor::BinarySensor *mute_binary_sensor_;
  switch_::Switch *output_switch_;

  uint8_t sck_pin_;
  uint8_t ws_pin_;
  uint8_t sd_pin_;
};

}  // namespace inmp441_stream
}  // namespace esphome
