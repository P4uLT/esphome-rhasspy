#include "inmp441_stream_switch.h"

#ifdef USE_ESP32_FRAMEWORK_ARDUINO

#include "esphome/core/log.h"
#include "esphome/core/application.h"

namespace esphome {
namespace inmp441_stream {

static const char *const TAG = "inmp441_stream.switch";

void Inmp441StreamComponentSwitch::setup() {
  bool initial_state = this->get_initial_state().value_or(true);
  this->write_state(initial_state);
}

void Inmp441StreamComponentSwitch::dump_config() {}
void Inmp441StreamComponentSwitch::write_state(bool state) { this->publish_state(state); }

}  // namespace inmp441_stream
}  // namespace esphome

#endif  // USE_ESP32_FRAMEWORK_ARDUINO
