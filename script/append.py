#!/usr/bin/env python3
from configparser import ConfigParser

file = "esphome/platformio.ini"
section = "common:esp32-arduino"
key = "lib_deps"
config = ConfigParser()
config.read(file)

esp32_arduino = config.options(section)
lib_deps = config.get(section, key).split("\n")
lib_deps.append("https://github.com/pschatzmann/arduino-audio-tools")
config.set(section, "lib_deps", "\n".join(lib_deps))

with open(file, "w") as configfile:  # save
    config.write(configfile)
