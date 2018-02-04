#include "ch.h"
#include "hal.h"
#include "motor.h"
#include "pneumatic.h"
#include "roof.h"
#include "move.h"
#include "imudriver.h"
#include "RTT/SEGGER_RTT.h"

int main(void)
{
    int sign;
    halInit();
    chSysInit();

    SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_TRIM);

    init_motors();
    pneumatic_init();

    set_speed(MOTOR_RIGHT, 0);
    set_speed(MOTOR_LEFT, 0);

#if USE_IMU
    chThdSleepMilliseconds(2000);
    i2cStart(&I2CD2, &imu_i2c_conf);
    initIMU(&I2CD2);
#endif /* USE_IMU */

    if (read_button(BUTTON_1) == 1) { // left button
        /* Yellow team */
        set_led_on(LED_2);
        sign = 1;
    } else {
        /* Blue team */
        set_led_on(LED_1);
        sign = -1;
    }

    int count = 0;
    while(1) {
        count++;
        chThdSleepMilliseconds(100);
        if (count == 10) {
            extend_arms();
        } else if (count == 40) {
            retract_arms();
        } else if (count == 100) {
            count = 0;
        }
        palTogglePad(GPIOC, GPIOC_DEBUG_LED);
    }

    return 1;
}
