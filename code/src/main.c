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
static volatile arms_position_t arms_position = ARMS_NONE;

static void cmd_cb10(void *arg) {
    (void)arg;
    emergency_stop_enable = false;
    target_distance = 300;
}

static void cmd_cb9(void *arg) {
    (void)arg;
    if (color == GREEN) {
        target_orientation = DEGREE_TO_IMU_UNIT(270);
    } else {
        target_orientation = DEGREE_TO_IMU_UNIT(90);
    }
    NEXT_COMMAND(cmd_cb10, 3);
}

static void cmd_cb8(void *arg) {
    (void)arg;
    target_distance = 150;
    arms_position = ARMS_MIDDLE;
    NEXT_COMMAND(cmd_cb9, 3);
}

static void cmd_cb7(void *arg) {
    (void)arg;
    target_orientation = DEGREE_TO_IMU_UNIT(0);
    eyes_up();
    NEXT_COMMAND(cmd_cb8, 3);
}

static void cmd_cb6(void *arg) {
    (void)arg;
    emergency_stop_enable = true;
    target_distance = -300;
    eyes_down();
    NEXT_COMMAND(cmd_cb7, 6);
}

static void cmd_cb5(void *arg) {
    (void)arg;
    emergency_stop_enable = false;
    target_distance = 400;
    NEXT_COMMAND(cmd_cb6, 6);
}

static void cmd_cb4(void *arg) {
    (void)arg;
    target_distance = 300;
    NEXT_COMMAND(cmd_cb5, 6);
}

static void cmd_cb3(void *arg) {
    (void)arg;
    if (color == GREEN) {
        target_orientation = DEGREE_TO_IMU_UNIT(270);
    } else {
        target_orientation = DEGREE_TO_IMU_UNIT(90);
    }
    arms_position = ARMS_DOWN;
    NEXT_COMMAND(cmd_cb4, 3);
}

static void cmd_cb2(void *arg) {
    (void)arg;
    target_distance = 300;
    NEXT_COMMAND(cmd_cb3, 7);
}

static void cmd_cb1(void *arg) {
    (void)arg;
    if (color == GREEN) {
        target_orientation = DEGREE_TO_IMU_UNIT(45);
    } else {
        target_orientation = DEGREE_TO_IMU_UNIT(315);
    }
    arms_position = ARMS_UP;
    NEXT_COMMAND(cmd_cb2, 2);
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
        if (arms_position != ARMS_NONE) {
            set_arms(arms_position);
            arms_position = ARMS_NONE;
        }
    }

    return 1;
}
