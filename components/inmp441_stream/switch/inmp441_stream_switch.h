#pragma once

#include "../inmp441_stream.h"
#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"

namespace esphome {
namespace inmp441_stream {

class Inmp441StreamComponent;

class Inmp441StreamComponentSwitch : public switch_::Switch, public Component {
 public:
  void set_parent(Inmp441StreamComponent *parent) { this->parent_ = parent; };
  void dump_config() override;
  void loop() override {}
  float get_setup_priority() const override { return setup_priority::DATA; }

 protected:
  void write_state(bool state) override;
  class Inmp441StreamComponent *parent_;
};

}  // namespace inmp441_stream
}  // namespace esphome
