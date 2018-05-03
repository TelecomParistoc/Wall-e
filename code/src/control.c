#include "control.h"
#include "orientation.h"
#include "motor.h"
#include "move.h"
#include "RTT/SEGGER_RTT.h"
#include "imudriver.h"

uint16_t target_orientation;
int32_t target_distance;
uint8_t command_angular_correction;
uint8_t linear_command;

typedef struct pid_s {
    double p;
    double i;
    double d;
} pid_t;

#define SIGN(x) ((x > 0) ? 1 : (-1))
#define ABS(x) ((x > 0) ? x : (-x))
#define MIN(x, y) ((x) > (y) ? (y) : (x))

pid_t angular_coeff = {1.0, 0.0, 0.0};

THD_WORKING_AREA(wa_control, 2048);

THD_FUNCTION(control_thread, p) {
    (void)p;

    uint16_t cur_orientation;
    int16_t delta_orientation = 0;
    int16_t last_delta_orientation = 0;
    int16_t delta_orientation_sum = 0;
    pid_t angular_pid;
    int32_t current_distance;
    command_angular_correction = 0;

    while (TRUE) {
        chThdSleepMilliseconds(10);

        //if obstacle, set speed to 0

        // Angular correction
#if USE_IMU
        cur_orientation = get_orientation();
        if (target_orientation != cur_orientation) {
            delta_orientation = target_orientation - cur_orientation;
            if (delta_orientation > (MAX_RANGE / 2)) {
                delta_orientation -= MAX_RANGE;
            } else if (delta_orientation < (-MAX_RANGE / 2)) {
                delta_orientation += MAX_RANGE;
            }
            delta_orientation_sum += delta_orientation;

            angular_pid.p = angular_coeff.p * delta_orientation;
            angular_pid.i = angular_coeff.i * delta_orientation_sum;
            angular_pid.d = angular_coeff.d * (delta_orientation - last_delta_orientation);

            command_angular_correction = (int8_t)(angular_pid.p + angular_pid.i + angular_pid.d);
            if (ABS(command_angular_correction) > MAX_SPEED) {
                command_angular_correction = SIGN(command_angular_correction) * MAX_SPEED;
            }

            last_delta_orientation = delta_orientation;
        }
#endif

        // Linear command
        linear_command = 0;
        if (target_distance != 0) {
            current_distance++;

            linear_command = MIN(DEFAULT_SPEED, MAX_SPEED - ABS(command_angular_correction));

            if (current_distance == target_distance) {
                target_distance = 0;
                current_distance = 0;
            }
        }

        // Set speed
        set_speed(MOTOR_RIGHT, linear_command + command_angular_correction);
        set_speed(MOTOR_LEFT, linear_command - command_angular_correction);
    }

}
