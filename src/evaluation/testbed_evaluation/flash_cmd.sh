#!/bin/bash

IMAGE_FILE=$1
IMAGE_OFFSET=$2 # 0x08000000

sh -c "openocd \
      -c 'set stlink_version 2-1' \
      -c 'source [find interface/stlink-v2-1.cfg]' \
      -c 'transport select hla_swd' \
      -c 'source [find target/stm32f7x.cfg]' \
      -c 'reset_config connect_assert_srst' \
      -c 'init' \
      -c 'targets' \
      -c 'reset halt' \
      -c 'flash write_image erase \"${IMAGE_FILE}\" ${IMAGE_OFFSET}' \
      -c 'verify_image \"${IMAGE_FILE}\" ${IMAGE_OFFSET}' \
      -c 'reset run' \
      -c 'shutdown'"