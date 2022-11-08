import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import CONF_ICON, CONF_ID

from .. import CONF_INMP441_STREAM_ID, INMP441_STREAM_CONFIG_SCHEMA, inmp441_stream_ns

DEPENDENCIES = ["inmp441_stream"]

ICON_OUTPUT = "mdi:power"

CONF_OUTPUT = "output"

Inmp441StreamComponentSwitch = inmp441_stream_ns.class_(
    "Inmp441StreamComponentSwitch", switch.Switch, cg.Component
)


CONFIG_SCHEMA = INMP441_STREAM_CONFIG_SCHEMA.extend(
    {
        cv.Required(CONF_OUTPUT): switch.SWITCH_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(Inmp441StreamComponentSwitch),
                cv.Optional(CONF_ICON, default=ICON_OUTPUT): cv.icon,
            }
        ).extend(cv.COMPONENT_SCHEMA),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_INMP441_STREAM_ID])
    conf = config[CONF_OUTPUT]

    sw_en_var = await switch.new_switch(conf)
    await cg.register_component(sw_en_var, conf)
    cg.add(hub.set_output_switch(sw_en_var))
    cg.add(sw_en_var.set_parent(hub))
