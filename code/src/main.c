#include "ch.h"
#include "hal.h"
#include "motor.h"
#include "pneumatic.h"

int main(void)
{
    halInit();
    chSysInit();

    init_motors();
    pneumatic_init();

    while(1) {
        chThdSleepMilliseconds(500);
        palTogglePad(GPIOC, GPIOC_DEBUG_LED);
    }

    return 1;
}
