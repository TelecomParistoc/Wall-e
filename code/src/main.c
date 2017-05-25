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

    chThdSleepMilliseconds(6000);

    set_speed(MOTOR_LEFT, 30);
    set_speed(MOTOR_RIGHT, 30);

    chThdSleepMilliseconds(5000);

    set_speed(MOTOR_RIGHT, 0);
    set_speed(MOTOR_LEFT, 0);

    while(1) {
        chThdSleepMilliseconds(500);
        palTogglePad(GPIOC, GPIOC_DEBUG_LED);
    }

    return 1;
}
