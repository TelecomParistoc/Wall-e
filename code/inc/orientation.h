#ifndef ORIENTATION_H
#define ORIENTATION_H

#include <stdint.h>

#define USE_IMU 1

#define DEGREE_TO_IMU_UNIT(angle) (16 * (angle))

extern uint16_t get_orientation(void);

#endif // ORIENTATION_H
