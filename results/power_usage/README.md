The power usages of the Nucleo board and the transceiver are measured using an ESP32 connected to two INA 3221 power monitors.
The Python program `/src/evaluation/testbed_evaluation.py` is used to read the log output of the ESP32 and running the `/src/evaluation/power_monitoring/main.c` program using the following command:

```
./testbed_evaluation.py --port /dev/ttyUSB0 --cmd read > results_read.txt
```

# Power usage during firmware updates

Instructions for running a full firmware update using a Nucleo F767-ZI board can be found here: [https://gist.github.com/vera/1b385b0fcd5632bfc73e624cd2f26a4e](https://gist.github.com/vera/1b385b0fcd5632bfc73e624cd2f26a4e)

The DES-Cript file describing the experiment setup in the MIoT Lab testbed can be found in the folder `/src/descript`.

The power monitoring logs of multiple repetitions can then be evaluated using the following command:

```
/src/evaluation/power_monitoring/convert-raw-power-monitoring-files.sh <PATH_TO_THESIS_REPO>/<PATH_TO_FOLDER_CONTAINING_POWER_MONITORING_LOGS> <PATH_TO_THESIS_REPO>/<PATH_TO_FOLDER_CONTAINING_UPDATE_DURATION_LOGS> results.txt
```

The calculated total power usages in Wh are written to `results.txt`.

# Power usage in low-power mode

To measure the energy consumption in the lowest power modes, the microcontroller's power mode was set to 0 and the `NETOPT_STATE` of the AT86 transceiver was set to `NETOPT_STATE_SLEEP` (see `src/empty`).
