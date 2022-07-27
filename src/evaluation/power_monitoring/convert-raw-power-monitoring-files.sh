#!/bin/bash

# Command-line arguments:
# 1. Path to folder where power monitoring logs are.
# 2. Path to folder where time logs are.
# 3. Path to file where results should be stored.
# 4. Optional. Set this to enable wireless evaluation mode.

if [[ $# -ne 3 && $# -ne 4 ]]; then
	echo "Usage: ./convert-raw-power-monitoring-files.sh PATH_TO_POWER_LOGS PATH_TO_TIME_LOGS OUT_FILE [WIRELESS_MODE]"
	exit 1
fi

if [ ! -f `dirname $0`/power_usage_evaluation.py ]; then
	echo "The script 'power_usage_evaluation.py' is required and must be placed in the same folder as this script."
	exit 2
fi

echo "Calculating power usage using power monitoring logs from $1 and time monitoring logs from $2"

if [ -z "$4" ]; then
	echo "Wireless mode enabled"
fi

if [ -f $3 ]; then
	echo "$3 already exists. Deleting"
	rm $3
fi

for f in "$1"/*; do
	if [ -z "$4" ]; then
		`dirname $0`/power_usage_evaluation.py --file "$f" --time-path "$2" >> $3
	else
		`dirname $0`/power_usage_evaluation.py --file "$f" --time-path "$2" --wireless >> $3
	fi
done

echo "Done"
