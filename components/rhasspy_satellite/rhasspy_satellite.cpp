#include "esphome/core/log.h"
#include "rhasspy_satellite.h"

namespace esphome {
namespace rhasspy_satellite {

static const char *TAG = "rhasspy_satellite.component";

void RhasspySatelliteComponent::setup() {

}

void RhasspySatelliteComponent::loop() {

}

void RhasspySatelliteComponent::dump_config(){
    ESP_LOGCONFIG(TAG, "RhasspySatellite Component");
}


}  // namespace rhasspy_satellite
}  // namespace esphome