# name of your application
APPLICATION = empty

DISABLE_MODULE = auto_init

# If no BOARD is found in the environment, use this default:
BOARD ?= iotlab-m3

QUIET ?= 1

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../src/RIOT/

IDLE ?= 0

ifneq (0,$(IDLE))
  CFLAGS += -DIDLE_TEST
endif

include $(RIOTBASE)/Makefile.include
