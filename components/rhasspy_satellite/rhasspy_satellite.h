#pragma once

#include "esphome/core/component.h"

namespace esphome {
namespace rhasspy_satellite {

class RhasspySatelliteComponent : public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
};


}  // namespace rhasspy_satellite
}  // namespace esphome