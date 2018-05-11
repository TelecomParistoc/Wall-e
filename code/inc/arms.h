#ifndef ARMS_H
#define ARMS_H

#include "hal.h"

typedef enum {
    ARMS_UP,
    ARMS_DOWN,
    ARMS_MIDDLE,
    ARMS_BALL,
    ARMS_NONE
} arms_position_t;

void init_arms(void);

void set_arms(arms_position_t position);

#endif /* ARMS_H */
