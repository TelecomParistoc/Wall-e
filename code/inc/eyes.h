#ifndef EYES_H
#define EYES_H

#include "hal.h"

typedef enum eye_t {
    EYE_LEFT,
    EYE_RIGHT
} eye_t;

extern void init_eyes(void);

extern void set_pos(eye_t eye, int8_t pos);

#endif /* EYES_H */
