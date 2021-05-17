#!/usr/bin/python3

import argparse
import serial
import sys
import time

def wait_for_boot(s):
    while True:
        line = s.read_until()

        if b'This is RIOT!' in line:
            break

def cmd_write(s, command, wait_after_cmd):
    s.write((command + "\n").encode("utf-8"))
    if not wait_after_cmd:
        return
    else:
        while True:
            line = s.read_until()
            if wait_after_cmd.encode("utf-8") in line:
                return

def cmd_read(s):
    while True:
        line = s.read_until()
        try:
        	print("{}\t{}".format(time.time(), line.decode("utf-8")), end="", flush=True)
        except UnicodeDecodeError:
        	continue

def cmd_time(s):
    time_started = False
    while True:
        line = s.read_until()

        if b'trigger received' in line:
            time_start = time.time()
            time_started = True
            print("{}\t".format(time_start), end="")
        elif time_started and b'This is RIOT!' in line:
            time_end = time.time()
            print(time_end)
            return

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--cmd', help="( write | read | time )" , type=str, required=True)
    parser.add_argument('--port', help="serial port device is connected at" , type=str, required=True)
    parser.add_argument('--baud-rate', help="baud rate" , type=int, default=115200)
    parser.add_argument('--write-cmd', help="command to write (if --cmd write)" , type=str)
    parser.add_argument('--wait-after-cmd', help="log output to wait for after sending command (if --cmd write)" , type=str)
    parser.add_argument('--wait-for-boot', help="wait for device to boot",  dest='wait_for_boot', action='store_true')
    parser.set_defaults(wait_for_boot=False)
    args = parser.parse_args()

    if args.cmd not in ["time", "write", "read"] or args.cmd != "write" and args.write_cmd:
        return

    s = serial.Serial(port=args.port, baudrate=args.baud_rate)
    s.reset_input_buffer()
    s.reset_output_buffer()

    if args.wait_for_boot:
        wait_for_boot(s)

    if args.cmd == "time":
        cmd_time(s)
    elif args.cmd == "read":
        cmd_read(s)
    elif args.cmd == "write":
        cmd_write(s, args.write_cmd, args.wait_after_cmd)

if __name__ == "__main__":
    main()
