#ifndef MOTOR_H
#define MOTOR_H

#include "hal.h"

typedef enum motor_t {
    MOTOR_LEFT,
    MOTOR_RIGHT
} motor_t;

extern void init_motors(void);

extern void set_speed(motor_t motor, int8_t speed);

#endif
