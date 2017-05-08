#include "ch.h"
#include "hal.h"
#include "motor.h"

int main(void)
{
    halInit();
    chSysInit();

    init_motors();

    while(1) {
        chThdSleepMilliseconds(500);
        palTogglePad(GPIOC, GPIOC_DEBUG_LED);
    }

    return 1;
}
