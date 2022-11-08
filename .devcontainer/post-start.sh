#!/bin/bash

set -e

if [ ! -L esphome ]; then
    ln -sv $(python3 -c "import esphome as _; print(_.__path__[0])") esphome
fi

pio lib -e esp32-arduino  install https://github.com/pschatzmann/arduino-audio-tools
pio lib -e esp32-arduino  install https://github.com/digint/tinyfsm
pio init --ide vscode -e esp32-arduino
