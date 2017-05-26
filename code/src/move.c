#include "move.h"
#include "roof.h"
#include "motor.h"
#include "RTT/SEGGER_RTT.h"

static const int16_t angle_time_factor_right = 25;
static const int16_t angle_time_factor_left = 30;

#define SIGN(x) ((x > 0) ? 1 : (-1))
#define ABS(x) ((x > 0) ? x : (-x))

void move_forward(uint32_t time) {
    uint32_t clock = 0;
    bool stopped = TRUE;
    while (clock < time) {
        if (is_something_detected()) {
            set_speed(MOTOR_LEFT, 0);
            set_speed(MOTOR_RIGHT, 0);
            stopped = TRUE;
        } else if (stopped == TRUE){
            set_speed(MOTOR_LEFT, 30);
            set_speed(MOTOR_RIGHT, 34);
            stopped = FALSE;
            clock++;
        } else if (stopped == FALSE) {
            clock++;
        }
        chThdSleepMilliseconds(1);
    }

    set_speed(MOTOR_LEFT, 0);
    set_speed(MOTOR_RIGHT, 0);
}

void turn(int16_t angle) {
    uint32_t clock = 0;
    bool stopped = TRUE;
    uint32_t limit;
    if (angle > 0) {
        limit = angle * angle_time_factor_right;
    } else {
        limit = ABS(angle) * angle_time_factor_left;
    }

    while (clock < (uint32_t)limit) {
        if (is_something_detected()) {
            printf("stop\r\n");
            set_speed(MOTOR_LEFT, 0);
            set_speed(MOTOR_RIGHT, 0);
            stopped = TRUE;
        } else if (stopped == TRUE) {
            printf("go\r\n");
            set_speed(MOTOR_LEFT, SIGN(angle) * 20);
            set_speed(MOTOR_RIGHT, -SIGN(angle) * 20);
            stopped = FALSE;
            clock++;
        } else if (stopped == FALSE) {
            clock++;
        }
        chThdSleepMilliseconds(1);
    //    printf("c %d / %d \r\n", clock, ABS(angle) * angle_time_factor);
    }

    set_speed(MOTOR_LEFT, 0);
    set_speed(MOTOR_RIGHT, 0);
}
