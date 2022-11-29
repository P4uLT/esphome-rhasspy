import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_ICON, CONF_ID

from . import CONF_INMP441_STREAM_ID, INMP441_STREAM_CONFIG_SCHEMA

DEPENDENCIES = ["inmp441_stream"]

ICON_OUTPUT = "mdi:power"

CONF_MUTE = "mute"


BINARY_SENSORS = [CONF_MUTE]

CONFIG_SCHEMA = INMP441_STREAM_CONFIG_SCHEMA.extend(
    {
        cv.Required(CONF_MUTE): binary_sensor.BINARY_SENSOR_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(binary_sensor.BinarySensor),
                cv.Optional(CONF_ICON, default=ICON_OUTPUT): cv.icon,
            }
        )
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_INMP441_STREAM_ID])
    for key in BINARY_SENSORS:
        if key in config:
            conf = config[key]
            sens = cg.new_Pvariable(conf[CONF_ID])
            await binary_sensor.register_binary_sensor(sens, conf)
            cg.add(getattr(hub, f"set_{key}_binary_sensor")(sens))
