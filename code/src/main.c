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

//#define TEST

void cb(void) {
    printf("cb\n");
}

typedef enum {
    ORANGE,
    GREEN
} color_t;
color_t color;

static virtual_timer_t cmd_clock;

static void cmd_cb4(void *arg) {
    target_distance = 700;
}

static void cmd_cb3(void *arg) {
    if (color == GREEN) {
        target_orientation = DEGREE_TO_IMU_UNIT(270);
    } else {
        target_orientation = DEGREE_TO_IMU_UNIT(90);
    }
    chSysLockFromISR();
    chVTResetI(&cmd_clock);
    chVTSetI(&cmd_clock, S2ST(3), cmd_cb4, NULL);
    chSysUnlockFromISR();
}

static void cmd_cb2(void *arg) {
    target_distance = 550;
    chSysLockFromISR();
    chVTResetI(&cmd_clock);
    chVTSetI(&cmd_clock, S2ST(6), cmd_cb3, NULL);
    chSysUnlockFromISR();
}

static void cmd_cb1(void *arg) {
    if (color == GREEN) {
        target_orientation = DEGREE_TO_IMU_UNIT(45);
    } else {
        target_orientation = DEGREE_TO_IMU_UNIT(315);
    }
    chSysLockFromISR();
    chVTResetI(&cmd_clock);
    chVTSetI(&cmd_clock, S2ST(2), cmd_cb2, NULL);
    chSysUnlockFromISR();
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

    set_led_on(LED_3);

    if (read_button(BUTTON_1) == 1) { // left button
        /* Orange team */
        set_led_on(LED_2);
        color = ORANGE;
        sign = 1;
    } else {
        /* Green team */
        set_led_on(LED_1);
        color = GREEN;
        sign = -1;
    }

#if USE_IMU
    int status;
    chThdSleepMilliseconds(2000);
    sdStart(&SD1, &imu_conf);
    status = initIMU(&SD1);
    printf("initIMU %d\n", status);
#endif /* USE_IMU */

    /* Start the ChibiOS ext driver */
    extStart(&EXTD1, &ext_config);

    chThdCreateStatic(wa_control, sizeof(wa_control), NORMALPRIO + 1, control_thread, NULL);
    sign = 0;

    // Starting "jack"
#ifndef TEST
    while (check_obstacle()) {
        chThdSleepMilliseconds(100);
        palTogglePad(GPIOC, GPIOC_DEBUG_LED);
    }
#endif
    chVTObjectInit(&cmd_clock);

    target_distance = 100;
    chVTReset(&cmd_clock);
    chVTSet(&cmd_clock, S2ST(2), cmd_cb1, NULL);
    while(1) {
        chThdSleepMilliseconds(100);
        palTogglePad(GPIOC, GPIOC_DEBUG_LED);
    }

    return 1;
}
