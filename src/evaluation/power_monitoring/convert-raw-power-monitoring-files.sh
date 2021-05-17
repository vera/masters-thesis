#!/bin/bash

# Command-line arguments:
# 1. Path to folder where power monitoring logs are.
# 2. Path to folder where time logs are.
# 3. Path to file where results should be stored.

touch $3

for f in "$1"/*; do
	`dirname $0`/power_usage_evaluation.py --file "$f" --time-path "$2" >> $3
done
