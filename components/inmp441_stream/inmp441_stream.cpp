#include "inmp441_stream.h"

#include "esphome/core/log.h"

namespace esphome {
namespace inmp441_stream {

static const char *const TAG = "inmp441_stream.component";

// audio_tools::I2SStream in;
// audio_tools::I2SStream out;

// Effect
// Boost boost(1);
// AudioEffects<GeneratorFromStream<effect_t>> effects(in,channels,1.0);
// GeneratedSoundStream<int16_t> in_boosted(effects);

Inmp441StreamComponent::Inmp441StreamComponent() {
  this->in_ = make_unique<I2SStream>();
  this->out_ = make_unique<I2SStream>();
  this->copier_ = make_unique<StreamCopy>();
}

void Inmp441StreamComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Audio...");

  // start I2S in
  audio_tools::I2SConfig config_in = in_->defaultConfig(RX_MODE);
  config_in.sample_rate = this->sample_rate_;
  config_in.bits_per_sample = this->bits_per_sample_;
  config_in.i2s_format = I2S_STD_FORMAT;
  config_in.is_master = true;
  config_in.use_apll = true,

  config_in.port_no = 0;
  config_in.pin_bck = this->sck_pin_;
  config_in.pin_ws = this->ws_pin_;
  config_in.pin_data = this->sd_pin_;
  // config_in.fixed_mclk = sample_rate * 256
  // config_in.pin_mck = 2
  in_->begin(config_in);

  // effects.addEffect(boost);

  // start I2S out
  audio_tools::I2SConfig config_out = out_->defaultConfig(TX_MODE);
  config_out.sample_rate = this->sample_rate_;
  config_out.bits_per_sample = this->bits_per_sample_;
  ;
  config_out.i2s_format = I2S_STD_FORMAT;
  config_out.is_master = true;
  config_out.port_no = 1;
  config_out.pin_bck = 18;
  config_out.pin_ws = 19;
  config_out.pin_data = 23;
  config_out.use_apll = true,

  out_->begin(config_out);
  this->copier_->begin(*out_, *in_);
}

void Inmp441StreamComponent::loop() {
  if (this->output_switch_ != nullptr && this->output_switch_->state) {
    this->mute_binary_sensor_->publish_state(false);
    this->copier_->copy();
  } else {
    this->mute_binary_sensor_->publish_state(true);
  }
}

void Inmp441StreamComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Inmp441StreamComponent:");
  ESP_LOGCONFIG(TAG, "  Sample Rate: %u", this->sample_rate_);
  ESP_LOGCONFIG(TAG, "  Channels: %u", this->channels_);
  ESP_LOGCONFIG(TAG, "  Bits Per Sample: %u", this->bits_per_sample_);
  ESP_LOGCONFIG(TAG, "  SCK Pin: %u", this->sck_pin_);
  ESP_LOGCONFIG(TAG, "  WS  Pin: %u", this->ws_pin_);
  ESP_LOGCONFIG(TAG, "  SD  Pin: %u", this->sd_pin_);
}

}  // namespace inmp441_stream
}  // namespace esphome
