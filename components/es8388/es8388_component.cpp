#include "es8388_component.h"

#include <soc/io_mux_reg.h>

namespace esphome {
namespace es8388 {

/* ES8388 register */
const uint8_t ES8388_CONTROL1 = 0x00;
const uint8_t ES8388_CONTROL2 = 0x01;
const uint8_t ES8388_CHIPPOWER = 0x02;
const uint8_t ES8388_ADCPOWER = 0x03;
const uint8_t ES8388_DACPOWER = 0x04;
const uint8_t ES8388_CHIPLOPOW1 = 0x05;
const uint8_t ES8388_CHIPLOPOW2 = 0x06;
const uint8_t ES8388_ANAVOLMANAG = 0x0;
const uint8_t ES8388_MASTERMODE = 0x08;

/* ADC */
const uint8_t ES8388_ADCCONTROL1 = 0x09;
const uint8_t ES8388_ADCCONTROL2 = 0x0a;
const uint8_t ES8388_ADCCONTROL3 = 0x0b;
const uint8_t ES8388_ADCCONTROL4 = 0x0c;
const uint8_t ES8388_ADCCONTROL5 = 0x0d;
const uint8_t ES8388_ADCCONTROL6 = 0x0e;
const uint8_t ES8388_ADCCONTROL7 = 0x0f;
const uint8_t ES8388_ADCCONTROL8 = 0x10;
const uint8_t ES8388_ADCCONTROL9 = 0x11;
const uint8_t ES8388_ADCCONTROL10 = 0x12;
const uint8_t ES8388_ADCCONTROL11 = 0x13;
const uint8_t ES8388_ADCCONTROL12 = 0x14;
const uint8_t ES8388_ADCCONTROL13 = 0x15;
const uint8_t ES8388_ADCCONTROL14 = 0x16;

/* DAC */
const uint8_t ES8388_DACCONTROL1 = 0x17;
const uint8_t ES8388_DACCONTROL2 = 0x18;
const uint8_t ES8388_DACCONTROL3 = 0x19;
const uint8_t ES8388_DACCONTROL4 = 0x1a;
const uint8_t ES8388_DACCONTROL5 = 0x1b;
const uint8_t ES8388_DACCONTROL6 = 0x1c;
const uint8_t ES8388_DACCONTROL7 = 0x1d;
const uint8_t ES8388_DACCONTROL8 = 0x1e;
const uint8_t ES8388_DACCONTROL9 = 0x1f;
const uint8_t ES8388_DACCONTROL10 = 0x20;
const uint8_t ES8388_DACCONTROL11 = 0x21;
const uint8_t ES8388_DACCONTROL12 = 0x22;
const uint8_t ES8388_DACCONTROL13 = 0x23;
const uint8_t ES8388_DACCONTROL14 = 0x24;
const uint8_t ES8388_DACCONTROL15 = 0x25;
const uint8_t ES8388_DACCONTROL16 = 0x26;
const uint8_t ES8388_DACCONTROL17 = 0x27;
const uint8_t ES8388_DACCONTROL18 = 0x28;
const uint8_t ES8388_DACCONTROL19 = 0x29;
const uint8_t ES8388_DACCONTROL20 = 0x2a;
const uint8_t ES8388_DACCONTROL21 = 0x2b;
const uint8_t ES8388_DACCONTROL22 = 0x2c;
const uint8_t ES8388_DACCONTROL23 = 0x2d;
const uint8_t ES8388_DACCONTROL24 = 0x2e;
const uint8_t ES8388_DACCONTROL25 = 0x2f;
const uint8_t ES8388_DACCONTROL26 = 0x30;
const uint8_t ES8388_DACCONTROL27 = 0x31;
const uint8_t ES8388_DACCONTROL28 = 0x32;
const uint8_t ES8388_DACCONTROL29 = 0x33;
const uint8_t ES8388_DACCONTROL30 = 0x34;

/**
 * @brief (un)mute one of the two outputs or main dac output of the ES8388 by switching of the output register bits.
 * Does not really mute the selected output, causes an attenuation. hence should be used in conjunction with
 * appropriate volume setting. Main dac output mute does mute both outputs
 *
 * @param out
 * @param muted
 */
void ES8388Component::mute(const OutputType out, const bool muted) {
  uint8_t reg_addr;
  uint8_t mask_mute;
  uint8_t mask_val;

  switch (out) {
    case OUTPUT_TYPE_OUT1:
      reg_addr = ES8388_DACPOWER;
      mask_mute = (3 << 4);
      mask_val = muted ? 0 : mask_mute;
      break;
    case OUTPUT_TYPE_OUT2:
      reg_addr = ES8388_DACPOWER;
      mask_mute = (3 << 2);
      mask_val = muted ? 0 : mask_mute;
      break;
    case OUTPUT_TYPE_MAIN:
    default:
      reg_addr = ES8388_DACCONTROL3;
      mask_mute = 1 << 2;
      mask_val = muted ? mask_mute : 0;
      break;
  }

  uint8_t reg;
  if (this->read_byte(reg_addr, &reg)) {
    reg = (reg & ~mask_mute) | (mask_val & mask_mute);
    this->write_bytes(reg_addr, {reg});
  }
}

/**
 * @brief Set volume gain for the main dac, or for one of the two output channels. Final gain = main gain + out channel
 * gain
 *
 * @param out which gain setting to control
 * @param vol 0-100 (100 is max)
 */
void ES8388Component::volume(const OutputType out, const uint8_t vol) {
  const uint32_t max_vol = 100;  // max input volume value

  const int32_t max_vol_val =
      out == OutputType::OUTPUT_TYPE_MAIN ? 96 : 0x21;  // max register value for ES8388 out volume

  uint8_t lreg = 0, rreg = 0;

  switch (out) {
    case OUTPUT_TYPE_MAIN:
      lreg = ES8388_DACCONTROL4;
      rreg = ES8388_DACCONTROL5;
      break;
    case OUTPUT_TYPE_OUT1:
      lreg = ES8388_DACCONTROL24;
      rreg = ES8388_DACCONTROL25;
      break;
    case OUTPUT_TYPE_OUT2:
      lreg = ES8388_DACCONTROL26;
      rreg = ES8388_DACCONTROL27;
      break;
  }

  uint8_t vol_val = vol > max_vol ? max_vol_val : (max_vol_val * vol) / max_vol;

  // main dac volume control is reverse scale (lowest value is loudest)
  // hence we reverse the calculated value
  if (out == OUTPUT_TYPE_MAIN) {
    vol_val = max_vol_val - vol_val;
  }

  this->write_bytes(lreg, {vol_val});
  this->write_bytes(rreg, {vol_val});
}

void ES8388Component::setup() {
  PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0_CLK_OUT1);
  WRITE_PERI_REG(PIN_CTRL, READ_PERI_REG(PIN_CTRL) & 0xFFFFFFF0);

  /* mute DAC during setup, power up all systems, slave mode */
  this->write_bytes(ES8388_DACCONTROL3, {0x04});
  this->write_bytes(ES8388_CONTROL2, {0x50});
  this->write_bytes(ES8388_CHIPPOWER, {0x00});
  this->write_bytes(ES8388_MASTERMODE, {0x00});

  /* power up DAC and enable LOUT1+2 / ROUT1+2, ADC sample rate = DAC sample rate */
  this->write_bytes(ES8388_DACPOWER, {0x3e});
  this->write_bytes(ES8388_CONTROL1, {0x12});

  /* DAC I2S setup: 16 bit word length, I2S format; MCLK / Fs = 256*/
  this->write_bytes(ES8388_DACCONTROL1, {0x18});
  this->write_bytes(ES8388_DACCONTROL2, {0x02});

  /* DAC to output route mixer configuration: ADC MIX TO OUTPUT */
  this->write_bytes(ES8388_DACCONTROL16, {0x1B});
  this->write_bytes(ES8388_DACCONTROL17, {0x90});
  this->write_bytes(ES8388_DACCONTROL20, {0x90});

  /* DAC and ADC use same LRCK, enable MCLK input; output resistance setup */
  this->write_bytes(ES8388_DACCONTROL21, {0x80});
  this->write_bytes(ES8388_DACCONTROL23, {0x00});

  /* DAC volume control: 0dB (maximum, unattenuated)  */
  this->write_bytes(ES8388_DACCONTROL5, {0x00});
  this->write_bytes(ES8388_DACCONTROL4, {0x00});

  /* power down ADC while configuring; volume: +9dB for both channels */
  this->write_bytes(ES8388_ADCPOWER, {0xff});
  this->write_bytes(ES8388_ADCCONTROL1, {0x88});  // +24db

  /* select LINPUT2 / RINPUT2 as ADC input; stereo; 16 bit word length, format right-justified, MCLK / Fs = 256 */
  this->write_bytes(ES8388_ADCCONTROL2, {0xf0});  // 50
  this->write_bytes(ES8388_ADCCONTROL3, {0x80});  // 00
  this->write_bytes(ES8388_ADCCONTROL4, {0x0e});
  this->write_bytes(ES8388_ADCCONTROL5, {0x02});

  /* set ADC volume */
  this->write_bytes(ES8388_ADCCONTROL8, {0x20});
  this->write_bytes(ES8388_ADCCONTROL9, {0x20});

  /* set LOUT1 / ROUT1 volume: 0dB (unattenuated) */
  this->write_bytes(ES8388_DACCONTROL24, {0x1e});  // 0x1e
  this->write_bytes(ES8388_DACCONTROL25, {0x1e});

  /* set LOUT2 / ROUT2 volume: 0dB (unattenuated) */
  this->write_bytes(ES8388_DACCONTROL26, {0x1e});
  this->write_bytes(ES8388_DACCONTROL27, {0x1e});

  /* power up and enable DAC; power up ADC (no MIC bias) */
  this->write_bytes(ES8388_DACPOWER, {0x3c});
  this->write_bytes(ES8388_DACCONTROL3, {0x00});
  this->write_bytes(ES8388_ADCPOWER, {0x00});
}
}  // namespace es8388
}  // namespace esphome
