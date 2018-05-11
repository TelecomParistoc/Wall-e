#include "control.h"
#include "orientation.h"
#include "motor.h"
#include "eyes.h"
#include "RTT/SEGGER_RTT.h"
#include "imudriver.h"

uint16_t target_orientation;
int32_t target_distance;
int8_t linear_command;
static uint16_t cur_orientation = 0;
volatile bool emergency_stop;
volatile bool emergency_stop_enable;
volatile bool end_match = false;

typedef struct pid_s {
    double p;
    double i;
    double d;
} pid_t;

#define SIGN(x) ((x > 0) ? 1 : (-1))
#define ABS(x) ((x > 0) ? (x) : (-(x)))
#define MIN(x, y) ((x) > (y) ? (y) : (x))

pid_t angular_coeff = {0.2, 0.0, 0.0};

void init_control(void) {
    emergency_stop = true;
    emergency_stop_enable = true;
    end_match = false;
    target_distance = 0;
    target_orientation = 0;
}

bool translation_ended(void) {
    return (target_distance == 0);
}

bool rotation_ended(void) {
    return (ABS(target_orientation - cur_orientation) < ANGULAR_ALLOWANCE);
}

THD_WORKING_AREA(wa_control, 2048);

THD_FUNCTION(control_thread, p) {
    (void)p;

    int8_t command_angular_correction = 0;
#if USE_IMU
    int16_t delta_orientation = 0;
    int16_t last_delta_orientation = 0;
    int16_t delta_orientation_sum = 0;
    pid_t angular_pid;
#endif // USE_IMU
    int32_t current_distance = 0;
    int tmp;

    while (TRUE) {
        chThdSleepMilliseconds(10);

        //if obstacle, set speed to 0
        if (emergency_stop && emergency_stop_enable) {
            printf("Emergency stop\n");
            set_speed(MOTOR_RIGHT, 0);
            set_speed(MOTOR_LEFT, 0);

            emergency_stop = check_obstacle();
            continue;
        }

        if (end_match) {
            set_speed(MOTOR_RIGHT, 0);
            set_speed(MOTOR_LEFT, 0);
            return;
        }

        // Angular correction
        command_angular_correction = 0;
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

            tmp = (int)(angular_pid.p + angular_pid.i + angular_pid.d);
            if (ABS(tmp) > MAX_SPEED_ANGULAR) {
                command_angular_correction = SIGN(tmp) * MAX_SPEED_ANGULAR;
            } else {
                command_angular_correction = (int8_t)tmp;
            }

            last_delta_orientation = delta_orientation;
        }
#else
        if (target_orientation != 0) {
            cur_orientation++;
            command_angular_correction = SIGN(target_orientation) * DEFAULT_SPEED;

            if (cur_orientation == ABS(target_orientation)) {
                target_orientation = 0;
            }
        }
#endif

        // Linear command
        linear_command = 0;
        if (target_distance != 0) {
            current_distance++;

            linear_command = SIGN(target_distance) * MIN(DEFAULT_SPEED, MAX_SPEED - ABS(command_angular_correction));

            printf("current_distance %d target_distance %d\n", current_distance, target_distance);
            if (current_distance == ABS(target_distance)) {
                target_distance = 0;
                current_distance = 0;
            }
        }
        printf("linear %d\n", linear_command);

        // Set speed
        set_speed(MOTOR_RIGHT, linear_command - command_angular_correction);
        set_speed(MOTOR_LEFT, linear_command + command_angular_correction);
    }

}
