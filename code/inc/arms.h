#ifndef ARMS_H
#define ARMS_H

#include "hal.h"

typedef enum {
    ARMS_UP,
    ARMS_DOWN,
    ARMS_NONE
} arms_position_t;

void init_arms(void);

void set_arms_down(void);
void set_arms_up(void);

void set_arms(arms_position_t position);

#endif /* ARMS_H */
