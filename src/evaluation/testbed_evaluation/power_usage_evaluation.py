#!/usr/bin/python3

import argparse
import csv

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--file', help="path to log file" , type=str, required=True)
    parser.add_argument('--start-time', help="start of evaluation" , type=float, required=False)
    parser.add_argument('--end-time', help="end of evaluation" , type=float, required=False)
    args = parser.parse_args()

    with open(args.file) as f:
        reader = csv.reader(f, delimiter="\t")
        rows = list(reader)

        if args.start_time:
            rows = list(filter(lambda x: float(x[0]) >= args.start_time, rows))

        if args.end_time:
            rows = list(filter(lambda x: float(x[0]) <= args.end_time, rows))

        total_power_usage = 0
        for i in range(1, len(rows)):
            interval = float(rows[i][0]) - float(rows[i - 1][0])
            total_power_usage += int(rows[i][1]) * 10 ** int(rows[i][5]) * interval
            i += 1
        print("{} mWh".format(total_power_usage / 3600 * 1000))

if __name__ == "__main__":
    main()