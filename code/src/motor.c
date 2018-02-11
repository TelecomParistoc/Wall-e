#include "motor.h"
#include "hal.h"

#define PWM_MAX 100
#define PWM_FREQUENCY_KHZ 20

static PWMConfig pwm_config_tim2 = {
    PWM_FREQUENCY_KHZ * PWM_MAX * 1000U,
    PWM_MAX,
    NULL,
    {
        {PWM_OUTPUT_DISABLED, NULL},
        {PWM_OUTPUT_DISABLED, NULL},
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_ACTIVE_HIGH, NULL}
    },
    0,
    0
};

extern void init_motors(void) {
    pwmStart(&PWMD2, &pwm_config_tim2);
    set_speed(MOTOR_LEFT, 0);
    set_speed(MOTOR_RIGHT, 0);
}

extern void set_speed(motor_t motor, int8_t speed) {
    if (speed <= PWM_MAX / 2) {
        switch (motor) {
            case MOTOR_LEFT:
                pwmEnableChannel(&PWMD2, 2, -speed + PWM_MAX / 2);
                break;
            case MOTOR_RIGHT:
                pwmEnableChannel(&PWMD2, 3, -speed + PWM_MAX / 2);
                break;
            default:
                break;
        }
    }
}
