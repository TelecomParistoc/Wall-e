#include "ch.h"
#include "hal.h"
#include "motor.h"
#include "eyes.h"
#include "pneumatic.h"
#include "roof.h"
#include "orientation.h"
#if USE_IMU
#include "imudriver.h"
#endif /* USE_IMU */
#include "arms.h"
#include "control.h"
#include "RTT/SEGGER_RTT.h"

void cb(void) {
    printf("cb\n");
}

int main(void)
{
    int sign;
    halInit();
    chSysInit();

    SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_TRIM);

    init_motors();
    init_eyes();
    eyes_up();
    pneumatic_init();
    init_arms();
    init_control();

#if USE_IMU
    int status;
    chThdSleepMilliseconds(2000);
    sdStart(&SD1, &imu_conf);
    status = initIMU(&SD1);
    printf("initIMU %d\n", status);
#endif /* USE_IMU */

    /* Start the ChibiOS ext driver */
    extStart(&EXTD1, &ext_config);

    if (read_button(BUTTON_1) == 1) { // left button
        /* Orange team */
        set_led_on(LED_2);
        sign = 1;
    } else {
        /* Green team */
        set_led_on(LED_1);
        sign = -1;
    }
    chThdCreateStatic(wa_control, sizeof(wa_control), NORMALPRIO + 1, control_thread, NULL);
    sign = 0;
    target_distance = 400;
    while(1) {
        chThdSleepMilliseconds(100);
        palTogglePad(GPIOC, GPIOC_DEBUG_LED);
        //printf("heading %d pitch %d roll %d\n", get_orientation(), getPitch(), getRoll());
        sign++;
        printf("sign %d\n", sign);
        if (sign == 80) {
            emergency_stop_enable = false;
            target_distance = 200;
        }
    }

    return 1;
}
