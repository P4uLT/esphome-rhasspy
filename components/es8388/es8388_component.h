#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"

namespace esphome {
namespace es8388 {

enum OutputType {
  OUTPUT_TYPE_MAIN,  // this is the DAC output volume (both outputs)
  OUTPUT_TYPE_OUT1,  // this is the additional gain for OUT1
  OUTPUT_TYPE_OUT2   // this is the additional gain for OUT2
};

class ES8388Component : public Component, public i2c::I2CDevice {
 public:
  void setup() override;
  void mute(const OutputType out, const bool muted);
  void volume(const OutputType out, const uint8_t vol);
  float get_setup_priority() const override { return setup_priority::LATE - 1; }
};

}  // namespace es8388
}  // namespace esphome
