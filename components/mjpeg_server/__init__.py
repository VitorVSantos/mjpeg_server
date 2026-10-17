import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_PORT, CONF_FPS

mjpeg_ns = cg.esphome_ns.namespace("mjpeg_server")
MJPEGServer = mjpeg_ns.class_("MJPEGServer", cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(MJPEGServer),
    cv.Optional(CONF_PORT, default=3333): cv.port,
    cv.Optional(CONF_FPS, default=10): cv.int_range(min=1, max=30),
})

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    cg.add(var.set_port(config[CONF_PORT]))
    cg.add(var.set_fps(config[CONF_FPS]))
