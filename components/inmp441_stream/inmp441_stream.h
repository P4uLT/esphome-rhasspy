#pragma once

#ifdef USE_ESP32_FRAMEWORK_ARDUINO

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/switch/switch.h"

#include <AudioTools.h>

namespace esphome {
namespace inmp441_stream {

class Inmp441StreamComponent : public Component {
 public:
  Inmp441StreamComponent();
  void setup() override;
  float get_setup_priority() const override { return esphome::setup_priority::LATE; }

  void loop() override;

  void dump_config() override;

  void set_sck_pin(uint8_t pin) { this->sck_pin_ = pin; }
  void set_ws_pin(uint8_t pin) { this->ws_pin_ = pin; }
  void set_sd_pin(uint8_t pin) { this->sd_pin_ = pin; }

  void set_sample_rate(uint16_t sample_rate) { this->sample_rate_ = sample_rate; }
  void set_channels(uint8_t channels) { this->channels_ = channels; }
  void set_bits_per_sample(uint8_t bits_per_sample) { this->bits_per_sample_ = bits_per_sample; }

  void set_mute_binary_sensor(binary_sensor::BinarySensor *mute_binary_sensor) {
    mute_binary_sensor_ = mute_binary_sensor;
  }

  void set_output_switch(switch_::Switch *output_switch) { output_switch_ = output_switch; }

 protected:
  std::unique_ptr<audio_tools::I2SStream> in_;
  std::unique_ptr<audio_tools::I2SStream> out_;
  std::unique_ptr<audio_tools::StreamCopy> copier_;

  binary_sensor::BinarySensor *mute_binary_sensor_{nullptr};
  switch_::Switch *output_switch_{nullptr};

  uint16_t sample_rate_{};
  uint8_t channels_{};
  uint8_t bits_per_sample_{};

  uint8_t sck_pin_{};
  uint8_t ws_pin_{};
  uint8_t sd_pin_{};
};

}  // namespace inmp441_stream
}  // namespace esphome

#endif  // USE_ESP32_FRAMEWORK_ARDUINO
