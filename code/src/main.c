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
#include "ax12driver.h"
#include "move.h"
//#define TEST

void cb(void) {
    printf("cb\n");
}

typedef enum {
    ORANGE,
    GREEN
} color_t;
color_t color;

static virtual_timer_t end_match_timer;

// static void cmd_cb12(void *arg) {
//     (void)arg;
//     emergency_stop_enable = true;
// }
//
// static void cmd_cb11(void *arg) {
//     (void)arg;
//     arms_position = ARMS_UP;
//     target_distance = 100;
//     NEXT_COMMAND(cmd_cb12, 2);
// }
//
// static void cmd_cb10(void *arg) {
//     (void)arg;
//     emergency_stop_enable = false;
//     target_distance = -800;
//     NEXT_COMMAND(cmd_cb11, 16);
// }
//
// static void cmd_cb9(void *arg) {
//     (void)arg;
//     target_orientation = DEGREE_TO_IMU_UNIT(0);
//     NEXT_COMMAND(cmd_cb10, 4);
// }
//
// static void cmd_cb8(void *arg) {
//     (void)arg;
//     target_distance = 150;
//     arms_position = ARMS_MIDDLE;
//     NEXT_COMMAND(cmd_cb9, 3);
// }
//
// static void cmd_cb7(void *arg) {
//     (void)arg;
//     if (color == GREEN) {
//         target_orientation = DEGREE_TO_IMU_UNIT(270);
//     } else {
//         target_orientation = DEGREE_TO_IMU_UNIT(90);
//     }
//     eyes_up();
//     NEXT_COMMAND(cmd_cb8, 5);
// }
//
// static void cmd_cb6(void *arg) {
//     (void)arg;
//     target_distance = -300;
//     eyes_down();
//     NEXT_COMMAND(cmd_cb7, 7);
// }
//
// static void cmd_cb5(void *arg) {
//     (void)arg;
//     target_distance = 400;
//     NEXT_COMMAND(cmd_cb6, 8);
// }
//
// static void cmd_cb4(void *arg) {
//     (void)arg;
//     target_distance = 300;
//     NEXT_COMMAND(cmd_cb5, 6);
// }
//
// static void cmd_cb3(void *arg) {
//     (void)arg;
//     target_orientation = DEGREE_TO_IMU_UNIT(180);
//     arms_position = ARMS_DOWN;
//     NEXT_COMMAND(cmd_cb4, 3);
// }
//
// static void cmd_cb2(void *arg) {
//     (void)arg;
//     target_distance = 270;
//     arms_position = ARMS_UP;
//     NEXT_COMMAND(cmd_cb3, 6);
// }

// static void cmd_cb1(void *arg) {
//     (void)arg;
//     next_target_distance = -180;
//     emergency_stop_enable = false;
//     arms_position = ARMS_BALL;
// }

static void end_match_cb(void *arg) {
    (void)arg;
    end_match = true;
    printf("end match\n");
}

int main(void)
{
    arms_position_t current_arm_pos = ARMS_NONE;
    int sign = 1;
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

    // Starting "jack"
    set_arms(ARMS_UP);
#ifndef TEST
    while (check_obstacle()) {
        chThdSleepMilliseconds(100);
        palTogglePad(GPIOC, GPIOC_DEBUG_LED);
    }
#endif
    chVTObjectInit(&end_match_timer);
    chVTSet(&end_match_timer, S2ST(100), end_match_cb, NULL);

    while(1) {
        chThdSleepMilliseconds(100);
        palTogglePad(GPIOC, GPIOC_DEBUG_LED);
        if (translation_ended() && rotation_ended()) {
            if (movements[movementIndex].valid) {
                movementIndex++;
            }
            if (movements[movementIndex].valid) {
                target_distance = movements[movementIndex].target_distance;
                target_orientation = sign * movements[movementIndex].target_orientation;
                emergency_stop_enable = movements[movementIndex].emergency_stop_enable;
            }
        }
        if (movements[movementIndex].arms_pos != current_arm_pos) {
            current_arm_pos = movements[movementIndex].arms_pos;
            set_arms(current_arm_pos);
        }
    }

    return 1;
}
