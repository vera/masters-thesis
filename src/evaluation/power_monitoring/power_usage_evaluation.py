#!/usr/bin/python3

import argparse
import csv
import os.path
import sys

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--file', help="path to log file" , type=str, required=True)
    parser.add_argument('--time-path', help="path to timing logs (folder)" , type=str, required=True)
    args = parser.parse_args()
    
    time_file_name = os.path.basename(args.file).replace("_read", "_time")
    time_file = os.path.join(args.time_path, time_file_name)

    with open(time_file) as f:
        reader = csv.reader(f, delimiter="\t")
        rows = list(reader)
        try:
            start_time = float(rows[0][0])
            end_time = float(rows[0][1])
        except:
            return

    with open(args.file) as f:
        reader = csv.reader(f, delimiter="\t")
        rows = list(reader)

        rows = list(filter(lambda x: float(x[0]) >= start_time, rows))

        rows = list(filter(lambda x: float(x[0]) <= end_time, rows))

        total_power_usage = 0
        for i in range(1, len(rows)):
            if (len(rows[i - 1]) == 1):
                continue
            interval = float(rows[i][0]) - float(rows[i - 1][0])
            total_power_usage += (float(rows[i][1]) + float(rows[i - 1][1])) / 2 * interval
            i += 1
        print("{}".format(total_power_usage / 3600 * 1000)) # mWh

if __name__ == "__main__":
    main()
