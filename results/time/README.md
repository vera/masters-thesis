Instructions for running a full firmware update using a Nucleo F767-ZI board can be found here: https://gist.github.com/vera/1b385b0fcd5632bfc73e624cd2f26a4e

The DES-Cript file describing the experiment setup in the MIoT Lab testbed can be found in the folder `/data/descript/`.

The Python program `testbed_evaluation.py` is used to obtain timestamps of the update start (when the trigger is received) and end (when the device has successfully rebooted) using the following command:

```
./testbed_evaluation.py --port /dev/ttyACM0 --cmd time > results_time.txt
```

The start timestamp is then subtracted from the end timestamp to obtain the update duration.
