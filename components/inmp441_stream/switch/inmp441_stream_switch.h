#pragma once

#ifdef USE_ESP32_FRAMEWORK_ARDUINO

#include "../inmp441_stream.h"
#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"

namespace esphome {
namespace inmp441_stream {

class Inmp441StreamComponent;

class Inmp441StreamComponentSwitch : public switch_::Switch, public Component {
 public:
  void setup() override;
  void set_parent(Inmp441StreamComponent *parent) { this->parent_ = parent; };
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::HARDWARE; }

 protected:
  void write_state(bool state) override;
  class Inmp441StreamComponent *parent_;
};

}  // namespace inmp441_stream
}  // namespace esphome

#endif  // USE_ESP32_FRAMEWORK_ARDUINO
