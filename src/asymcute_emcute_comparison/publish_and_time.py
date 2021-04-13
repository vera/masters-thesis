#!/usr/bin/env python3

# Copyright (C) 2021 Vera Clemens <mail@veraclemens.org>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

import sys
import time

import paho.mqtt.client as mqtt

TOPIC_MESSAGES = "test"
TOPIC_STATUS = "status"
STATUS_DONE = "done"

NUM_MESSAGES = 1500

def on_connect(client, userdata, flags, rc):
    client.subscribe(TOPIC_STATUS, 1)
        
def on_subscribe(client, userdata, mid, granted_qos):
    global start

    start = time.time()
    for x in range(NUM_MESSAGES):
        client.publish(TOPIC_MESSAGES, payload=b'1' * 64, qos=1)

def on_message(client, userdata, msg):
    global end
    
    end = time.time()
    print(end - start)
    sys.exit(0)

client = mqtt.Client(client_id="publish_and_time")
client.on_connect = on_connect
client.on_message = on_message
client.on_subscribe = on_subscribe

client.connect("localhost", 1883, 60)

client.loop_forever()
