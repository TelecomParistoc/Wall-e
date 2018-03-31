#ifndef MOTOR_H
#define MOTOR_H

#include "hal.h"

#define MAX_SPEED 50
#define DEFAULT_SPEED 30

typedef enum motor_t {
    MOTOR_LEFT,
    MOTOR_RIGHT
} motor_t;

extern void init_motors(void);

extern void set_speed(motor_t motor, int8_t speed);

#endif
