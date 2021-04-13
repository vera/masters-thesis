/*
 * Copyright (C) 2015 Freie Universität Berlin
 *               2021 Vera Clemens <mail@veraclemens.org>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file COPYING.LESSER in the
 * top level directory for more details.
 */

/**
 * @{
 *
 * @file
 * @brief       Sample application using MQTT-SN library Asymcute,
 *              based on RIOT's examples/asymcute_mqttsn
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 * @author      Vera Clemens <mail@veraclemens.org>
 *
 * @}
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "shell.h"
#include "mutex.h"
#include "thread.h"
#include "net/asymcute.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"

#ifndef REQ_CTX_NUMOF
#define REQ_CTX_NUMOF       (1U)
#endif

#define ASYMCUTE_ID         ("asymcute_client")

#define TOPIC_MESSAGES      "test"
#define TOPIC_STATUS        "status"
#define STATUS_DONE         "done"

#define NUM_MESSAGES        1500

#define GATEWAY_PORT        "10000"
#define GATEWAY_ADDRESS     "fe80::1"

static asymcute_con_t _connection;
static asymcute_req_t _reqs[REQ_CTX_NUMOF];
static asymcute_sub_t _subscription;
static asymcute_topic_t _topic;
static asymcute_topic_t _topic_status;

uint16_t msg_counter = 0;

static asymcute_req_t *_get_req_ctx(void)
{
    for (unsigned i = 0; i < REQ_CTX_NUMOF; i++) {
        if (!asymcute_req_in_use(&_reqs[i])) {
            return &_reqs[i];
        }
    }
    puts("error: no request context available\n");
    return NULL;
}

static void pub_done(void) {
    asymcute_req_t *req = _get_req_ctx();
    if (req == NULL) {
        puts("error: unable to obtain request context");
        return;
    }

    int res = asymcute_publish(&_connection, req, &_topic_status,
                                STATUS_DONE, strlen(STATUS_DONE),
                                MQTTSN_QOS_1);
    if (res != ASYMCUTE_OK) {
        printf("error: unable to send PUBLISH message (%i)\n", res);
        pub_done();
    }
    msg_counter = 0;
}

static void reg_done(void) {
    asymcute_req_t *req = _get_req_ctx();
    if (req == NULL) {
        puts("error: unable to allocate request context");
        return;
    }

    asymcute_topic_init(&_topic_status, TOPIC_STATUS, 0);

    int res = asymcute_register(&_connection, req, &_topic_status);
    if (res == ASYMCUTE_REGERR) {
        pub_done();
    }
    else if (res != ASYMCUTE_OK) {
        printf("error: unable to send REGISTER request (%i)\n", res);
        reg_done();
    }
}

static void _on_pub_evt(const asymcute_sub_t *sub, unsigned evt_type,
                        const void *data __attribute__((unused)),
                        size_t len __attribute__((unused)), void *arg)
{
    (void)arg;

    if (evt_type == ASYMCUTE_PUBLISHED) {
        msg_counter++;

        if (msg_counter == NUM_MESSAGES) {
            /* register topic for done message */
            reg_done();
        }
    }
    else if (evt_type == ASYMCUTE_CANCELED) {
        printf("subscription -> topic #%i [%s]: CANCELED\n",
               (int)sub->topic->id, sub->topic->name);
    }
}

static void _on_con_evt(asymcute_req_t *req, unsigned evt_type)
{
    printf("Request %p: ", (void *)req);
    switch (evt_type) {
        case ASYMCUTE_TIMEOUT:
            puts("Timeout");
            break;
        case ASYMCUTE_REJECTED:
            puts("Rejected by gateway");
            break;
        case ASYMCUTE_CONNECTED:
            puts("Connection to gateway established");

            /* subscribe to topic */
            asymcute_req_t *req = _get_req_ctx();
            if (req == NULL) {
                puts("error: unable to obtain request context");
                return;
            }

sub:
            asymcute_topic_init(&_topic, TOPIC_MESSAGES, 0);
            int res = asymcute_subscribe(&_connection, req, &_subscription,
                                         &_topic, _on_pub_evt, NULL,
                                         MQTTSN_QOS_1);
            if (res != ASYMCUTE_OK) {
                goto sub;
            }
            break;
        case ASYMCUTE_DISCONNECTED:
            puts("Connection to gateway closed");
            break;
        case ASYMCUTE_REGISTERED:
            puts("Topic registered");

            /* publish done message */
            pub_done();
            break;
        case ASYMCUTE_PUBLISHED:
            puts("Data was published");
            break;
        case ASYMCUTE_SUBSCRIBED:
            puts("ready");
            break;
        case ASYMCUTE_UNSUBSCRIBED:
            puts("Unsubscribed topic");
            break;
        case ASYMCUTE_CANCELED:
            puts("Canceled");
            break;
        default:
            puts("unknown event");
            break;
    }
}

int main(void)
{
    /* connect to gateway */
    sock_udp_ep_t ep;
    sock_udp_str2ep(&ep, "[" GATEWAY_ADDRESS "]:" GATEWAY_PORT);

    /* get request context */
    asymcute_req_t *req = _get_req_ctx();
    if (req == NULL) {
        return 1;
    }

    int res;
con:
    res = asymcute_connect(&_connection, req, &ep, ASYMCUTE_ID,
                               true, NULL, _on_con_evt);
    if (res != ASYMCUTE_OK) {
        printf("error: failed to issue CONNECT request (%i)\n", res);
        goto con;
    }

    thread_yield();

    /* should be never reached */
    return 0;
}
