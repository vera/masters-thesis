Instructions for running a full firmware update using a Nucleo F767-ZI board can be found here: https://gist.github.com/vera/1b385b0fcd5632bfc73e624cd2f26a4e

The DES-Cript file describing the experiment setup in the MIoT Lab testbed can be found in the folder `/src/descript/`.

The Python program `testbed_evaluation.py` is used to read the log output of an ESP32 connected to two INA 3221 power monitors and running the `/src/evaluation/power_monitoring/main.c` program using the following command:

```
./testbed_evaluation.py --port /dev/ttyUSB0 --cmd read > results_read.txt
```

Multiple power monitoring logs can then be evaluated using the following command:

```
./convert-raw-power-monitoring-files.sh <PATH_TO_THESIS_REPO>/<PATH_TO_FOLDER_CONTAINING_POWER_MONITORING_LOGS> <PATH_TO_THESIS_REPO>/<PATH_TO_FOLDER_CONTAINING_UPDATE_DURATION_LOGS> results.txt
```

The calculated total power usages in Wh are written to `results.txt`.
