#include <stdio.h>
#include <xtimer.h>
#include <saul_reg.h>
#include <phydat.h>

#define SAMPLE_PERIOD       (140 * 6) /* 840 us */

int main(void)
{
    saul_reg_t* dev_u2;
    saul_reg_t* dev_u3;
    do {
        dev_u2 = saul_reg_find_type_and_name(SAUL_SENSE_POWER, "INA3221 U2 power");
    } while (dev_u2 == NULL);
    do {
        dev_u3 = saul_reg_find_type_and_name(SAUL_SENSE_POWER, "INA3221 U3 power");
    } while (dev_u3 == NULL);

    xtimer_ticks32_t last_wakeup = xtimer_now();
    phydat_t res;
    while (true) {
        saul_reg_read(dev_u2, &res);
        printf("%d\t%d\t%d\t%d\t%d", res.val[0], res.val[1], res.val[2], res.unit, res.scale);
        saul_reg_read(dev_u3, &res);
        printf("\t%d\t%d\t%d\t%d\t%d\n", res.val[0], res.val[1], res.val[2], res.unit, res.scale);
        xtimer_periodic_wakeup(&last_wakeup, SAMPLE_PERIOD);
    }

    return 0;
}