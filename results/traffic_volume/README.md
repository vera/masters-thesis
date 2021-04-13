Instructions for running a full firmware update locally using a Nucleo F767-ZI board can be found here: https://gist.github.com/vera/1b385b0fcd5632bfc73e624cd2f26a4e

The traffic on the `bridge0` interface is captured using Wireshark.
Then, the capture file is analyzed using the self-written traffic analysis program (https://github.com/vera/traffic-analysis) at commit `47c6a42` with the following options:

```
$ traffic_analyser.py --pcap <PATH TO PCAP FILE> --device-addr a6:6a:98:82:cc:7b --server-addr 28:d2:44:f2:d6:6a --src-addr-field eth.src --dst-addr-field eth.dst --check-total --mqttsn-port 10000
```
