#!/bin/bash

# Unpack
tar -xf ../esp32_power_monitoring.tar
# Flash
python /root/esptool-2.8/esptool.py                                     \
--chip esp32 --port /dev/ttyUSB0 --baud 460800 --before default_reset   \
write_flash                                                             \
-z --flash_mode dout --flash_freq 40m                                   \
0x1000 bootloader.bin                                                   \
0x8000 partitions.bin                                                   \
0x10000 power_monitoring.elf.bin
