# name of your application
APPLICATION = asymcute_mqttsn

# If no BOARD is found in the environment, use this default:
BOARD ?= nucleo-f767zi

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../RIOT

# Include packages that pull up and auto-init the link layer.
# NOTE: 6LoWPAN will be included if IEEE802.15.4 devices are present
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
# Specify the mandatory networking modules for IPv6 and UDP
USEMODULE += gnrc_ipv6_default
# Include MQTT-SN
USEMODULE += asymcute
# Optimize network stack to for use with a single network interface
USEMODULE += gnrc_netif_single
# include UHCP client
USEMODULE += gnrc_uhcpc

# Static link lokal IPv6 address
IPV6_STATIC_LLADDR ?= '"fe80::cafe:cafe:cafe:1"'
CFLAGS += -DGNRC_IPV6_STATIC_LLADDR=$(IPV6_STATIC_LLADDR)

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 0

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
