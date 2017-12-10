#include "move.h"
#include "roof.h"
#include "motor.h"
#if USE_IMU
#include "imu_driver.h"
#endif /* USE_IMU */
#include "RTT/SEGGER_RTT.h"

static const int16_t angle_time_factor_right = 25;
static const int16_t angle_time_factor_left = 30;

#define SIGN(x) ((x > 0) ? 1 : (-1))
#define ABS(x) ((x > 0) ? x : (-x))

#define DEFAULT_SPEED 30

void move_forward(uint32_t time) {
    uint32_t clock = 0;
    bool stopped = TRUE;
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
            angle_error = (getHeading() - initial_angle) % MAX_RANGE;
            if (angle_error > last_angle_error) {
                motor_right_angle_correction++;
                motor_left_angle_correction--;
            } else if (angle_error < last_angle_error) {
                motor_right_angle_correction--;
                motor_left_angle_correction++;
            }
#endif /* USE_IMU */
            set_speed(MOTOR_LEFT, DEFAULT_SPEED + motor_left_angle_correction);
            set_speed(MOTOR_RIGHT, DEFAULT_SPEED + motor_right_angle_correction);
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
