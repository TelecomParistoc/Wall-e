#ifndef MOVE_H
#define MOVE_H

#include "hal.h"

#define USE_IMU 1

void move_forward(uint32_t time);

void turn (int16_t angle);

#endif /* MOVE_H */
