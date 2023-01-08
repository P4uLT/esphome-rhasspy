import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.const import CONF_ID, CONF_CHANNELS
from esphome.core import CORE

AUTO_LOAD = ["switch"]
DEPENDENCIES = ["esp32"]


inmp441_stream_ns = cg.esphome_ns.namespace("inmp441_stream")
Inmp441StreamComponent = inmp441_stream_ns.class_(
    "Inmp441StreamComponent", cg.Component
)

MULTI_CONF = True

CONF_INMP441_STREAM_ID = "inmp441_stream_id"

CONF_SAMPLE_RATE = "sample_rate"
CONF_BITS_PER_SAMPLE = "bits_per_sample"

CONF_I2S_SCK_PIN = "i2s_sck_pin"
CONF_I2S_WS_PIN = "i2s_ws_pin"
CONF_I2S_SD_PIN = "i2s_sd_pin"


INMP441_STREAM_CONFIG_SCHEMA = cv.Schema(
    {cv.GenerateID(CONF_INMP441_STREAM_ID): cv.use_id(Inmp441StreamComponent)}
)


CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(Inmp441StreamComponent),
            cv.Required(CONF_I2S_SCK_PIN): pins.internal_gpio_output_pin_number,
            cv.Required(CONF_I2S_WS_PIN): pins.internal_gpio_output_pin_number,
            cv.Required(CONF_I2S_SD_PIN): pins.internal_gpio_output_pin_number,
            cv.Optional(CONF_SAMPLE_RATE, default=22500): cv.int_,
            cv.Optional(CONF_CHANNELS, default=2): cv.int_range(min=1, max=2),
            cv.Optional(CONF_BITS_PER_SAMPLE, default=16): cv.int_,
        }
    ).extend(cv.COMPONENT_SCHEMA),
    cv.only_with_arduino,
)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    cg.add(var.set_sck_pin(config[CONF_I2S_SCK_PIN]))
    cg.add(var.set_ws_pin(config[CONF_I2S_WS_PIN]))
    cg.add(var.set_sd_pin(config[CONF_I2S_SD_PIN]))

    cg.add(var.set_sample_rate(config[CONF_SAMPLE_RATE]))
    cg.add(var.set_channels(config[CONF_CHANNELS]))
    cg.add(var.set_bits_per_sample(config[CONF_BITS_PER_SAMPLE]))

    if CORE.is_esp32:
        cg.add_library("WiFiClientSecure", None)
        cg.add_library("HTTPClient", None)
        cg.add_library("https://github.com/pschatzmann/arduino-audio-tools", None)
        cg.add_library("https://github.com/digint/tinyfsm", None)
