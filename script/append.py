#!/usr/bin/env python3
from configparser import ConfigParser
import argparse
import os
import sys

root_path = os.path.abspath(os.path.normpath(os.path.join(__file__, "..", "..")))
key = "lib_deps"


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-p",
        "--path",
        help="the PlatformIO environment path",
    )

    args = parser.parse_args()

    additionnal_libs_deps_file = os.path.join(root_path, "additionnal_libs_deps.ini")

    file = f"{root_path}/{args.path}/platformio.ini"

    additionnal_lib_config = ConfigParser()
    additionnal_lib_config.read(additionnal_libs_deps_file)
    for section in additionnal_lib_config.sections():
        additionnal_lib_deps = additionnal_lib_config.get(section, key).split("\n")

        platformio_config = ConfigParser()
        platformio_config.read(file)

        lib_deps = platformio_config.get(section, key).split("\n")
        changed = False

        for lib in additionnal_lib_deps:
            if lib not in lib_deps:
                lib_deps.append(lib)
                print(f"Adding {lib} to section {section}")
                changed = True

        if changed:
            platformio_config.set(section, key, "\n".join(lib_deps))
            with open(file, "w", encoding="utf-8") as configfile:  # save
                platformio_config.write(configfile)


if __name__ == "__main__":
    sys.exit(main())
