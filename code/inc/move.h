#ifndef MOVE_H
#define MOVE_H

#include "arms.h"

typedef struct {
    int target_distance;
    int target_orientation;
    arms_position_t arms_pos;
    bool emergency_stop_enable;
    bool valid;
} Movement_t;

volatile int movementIndex = 0;

Movement_t movements[] = {
    {0, 0, 0, true, true},
    {-125, 0, ARMS_UP, true, true},
    {-300, 0, ARMS_MIDDLE, false, true},
    {0, 0, 0, true, false}
};

#endif /* MOVE_H */
