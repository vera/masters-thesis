/*
 * Copyright (C) 2015 Freie Universität Berlin
 *               2021 Vera Clemens <mail@veraclemens.org>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file COPYING.LESSER in the
 * top level directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Sample application using MQTT-SN library emCute,
 *              based on RIOT's examples/emcute_mqttsn
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 * @author      Vera Clemens <mail@veraclemens.org>
 *
 * @}
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define EMCUTE_ID           ("emcute_client")
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)

#define TOPIC_MESSAGES      "test"
#define TOPIC_STATUS        "status"
#define STATUS_DONE         "done"

#define NUM_MESSAGES        1500

#define GATEWAY_PORT        10000
#define GATEWAY_ADDRESS     "2001:db8::1"

#define FLAG_DONE           0x1234


static char stack[THREAD_STACKSIZE_DEFAULT];

static emcute_sub_t subscription;
static char *topic = TOPIC_MESSAGES;

uint16_t msg_counter = 0;

static void *emcute_thread(void *arg)
{
    (void)arg;
    emcute_run(CONFIG_EMCUTE_DEFAULT_PORT, EMCUTE_ID);
    return NULL;    /* should never be reached */
}

static void on_pub(const emcute_topic_t *topic __attribute__((unused)),
                   void *data __attribute__((unused)),
                   size_t len __attribute__((unused)))
{
    msg_counter++;

    if (msg_counter == NUM_MESSAGES) {
        msg_counter = 0;
        /* set flag for main thread */
        thread_flags_set(thread_get(1), FLAG_DONE);
    }
}

int main(void)
{
    /* start the emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* connect to gateway */
    sock_udp_ep_t gw = { .family = AF_INET6, .port = GATEWAY_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, GATEWAY_ADDRESS);

    int res;
con:
    if ((res = emcute_con(&gw, true, NULL, NULL, 0, 0)) != EMCUTE_OK) {
        puts("con failed\n");
        goto con;
    }

    /* subscribe to topic */
    subscription.cb = on_pub;
    subscription.topic.name = topic;
sub:
    if (emcute_sub(&subscription, EMCUTE_QOS_1) != EMCUTE_OK) {
        puts("sub failed\n");
        goto sub;
    }
    puts("ready\n");

    while (true) {
        /* wait for done signal */
        thread_flags_wait_any(FLAG_DONE);

        /* publish done message */
        emcute_topic_t t;
        t.name = TOPIC_STATUS;
    reg:
        if (emcute_reg(&t) != EMCUTE_OK) {
            printf("reg failed\n");
            goto reg;
        }

    pub:
        if (emcute_pub(&t, STATUS_DONE, strlen(STATUS_DONE),
                        EMCUTE_QOS_1) != EMCUTE_OK) {
            puts("pub failed\n");
            goto pub;
        }

        thread_flags_clear(FLAG_DONE);
    }

    /* should be never reached */
    return 0;
}
