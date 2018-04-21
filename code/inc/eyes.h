#ifndef EYES_H
#define EYES_H

#include "hal.h"

typedef enum eye_t {
    EYE_LEFT,
    EYE_RIGHT
} eye_t;

extern const EXTConfig ext_config;

extern void init_eyes(void);

extern void set_pos(eye_t eye, int8_t pos);

extern void eyes_up(void);

extern void eyes_down(void);

#endif /* EYES_H */
