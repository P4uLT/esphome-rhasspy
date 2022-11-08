#include "esphome/core/log.h"
#include "inmp441_stream.h"

#include "AudioTools.h"

namespace esphome {
namespace inmp441_stream {

static const char *TAG = "inmp441_stream.component";

uint16_t sample_rate = 22500;
uint16_t channels = 2;
uint16_t bits_per_sample = 16;  // or try with 24 or 32

I2SStream in;
I2SStream out;

// Effect
// Boost boost(1);
// AudioEffects<GeneratorFromStream<effect_t>> effects(in,channels,1.0);
// GeneratedSoundStream<int16_t> in_boosted(effects);

StreamCopy copier(out, in);  // copies sound into i2s

void Inmp441StreamComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Audio...");

  // start I2S in
  auto config_in = in.defaultConfig(RX_MODE);
  config_in.sample_rate = sample_rate;
  config_in.bits_per_sample = bits_per_sample;
  config_in.i2s_format = I2S_STD_FORMAT;
  config_in.is_master = true;
  config_in.use_apll = true,

  config_in.port_no = 0;
  config_in.pin_bck = this->sck_pin_;
  config_in.pin_ws = this->ws_pin_;
  config_in.pin_data = this->sd_pin_;
  // config_in.fixed_mclk = sample_rate * 256
  // config_in.pin_mck = 2
  in.begin(config_in);

  // effects.addEffect(boost);

  // start I2S out
  auto config_out = out.defaultConfig(TX_MODE);
  config_out.sample_rate = sample_rate;
  config_out.bits_per_sample = bits_per_sample;
  config_out.i2s_format = I2S_STD_FORMAT;
  config_out.is_master = true;
  config_out.port_no = 1;
  config_out.pin_bck = 18;
  config_out.pin_ws = 19;
  config_out.pin_data = 23;
  config_out.use_apll = true,

  out.begin(config_out);
}

void Inmp441StreamComponent::loop() {
  if (this->output_switch_ != nullptr && this->output_switch_->state == true) {
    this->mute_binary_sensor_->publish_state(false);
    copier.copy();
  } else {
    this->mute_binary_sensor_->publish_state(true);
  }
}

void Inmp441StreamComponent::dump_config() { ESP_LOGCONFIG(TAG, "Inmp441StreamComponent Component"); }

}  // namespace inmp441_stream
}  // namespace esphome
