#include "move.h"
#include "roof.h"
#include "motor.h"
#if USE_IMU
#include "imudriver.h"
#endif /* USE_IMU */
#include "RTT/SEGGER_RTT.h"
#include "control.h"

static const int16_t angle_time_factor_right = 25;
static const int16_t angle_time_factor_left = 30;

#define SIGN(x) ((x > 0) ? 1 : (-1))
#define ABS(x) ((x > 0) ? x : (-x))
#define MIN(x, y) ((x) > (y) ? (y) : (x))

static int8_t limit_interval(int8_t arg, int8_t min, int8_t max) {
    if (min > max) {
        return arg;
    }

    if (arg < min) {
        return min;
    } else if (arg > max) {
        return max;
    } else {
        return arg;
    }
}

void move_forward(uint32_t time) {
    uint32_t clock = 0;
    bool stopped = TRUE;
    int8_t linear_speed = DEFAULT_SPEED;
    int8_t motor_right_angle_correction = 0;
    int8_t motor_left_angle_correction = 0;
#if USE_IMU
    uint16_t initial_angle;
    int16_t angle_error = 0;
    int16_t last_angle_error = 0;

    initial_angle = getHeading();
#endif /* USE_IMU */

    while (clock < time) {
        if (is_something_detected()) {
            set_speed(MOTOR_LEFT, 0);
            set_speed(MOTOR_RIGHT, 0);
            stopped = TRUE;
        } else if (stopped == TRUE){
#if USE_IMU
            motor_right_angle_correction = command_angular_correction;
            motor_left_angle_correction = -command_angular_correction;
            linear_speed = MIN(DEFAULT_SPEED, MAX_SPEED - ABS(command_angular_correction));
#endif /* USE_IMU */
            set_speed(MOTOR_LEFT, limit_interval(linear_speed + motor_left_angle_correction, -MAX_SPEED, MAX_SPEED));
            set_speed(MOTOR_RIGHT, limit_interval(linear_speed + motor_right_angle_correction, -MAX_SPEED, MAX_SPEED));
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
