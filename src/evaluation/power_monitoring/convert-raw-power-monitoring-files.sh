#!/bin/bash

# Command-line arguments:
# 1. Path to folder where power monitoring logs are.
# 2. Path to folder where time logs are.
# 3. Path to file where results should be stored.
# 4. Optional. Set this to enable wireless evaluation mode.

if [ -f $3 ]; then
	rm $3
fi

for f in "$1"/*; do
	if [ -z "$4" ]; then
		`dirname $0`/power_usage_evaluation.py --file "$f" --time-path "$2" >> $3
	else
		`dirname $0`/power_usage_evaluation.py --file "$f" --time-path "$2" --wireless >> $3
	fi
done
