#include "ch.h"
#include "hal.h"

int main(void)
{
    halInit();
    chSysInit();

    while(1) {
        chThdSleepMilliseconds(500);
        palTogglePad(GPIOC, GPIOC_DEBUG_LED);
    }

    return 1;
}
