#ifndef CONTROL_H
#define CONTROL_H

#include <stdint.h>
#include "ch.h"

extern uint16_t target_orientation;
extern int32_t target_distance;
extern uint8_t command_angular_correction;
extern volatile bool emergency_stop;
extern volatile bool emergency_stop_enable;

extern void init_control(void);

extern THD_WORKING_AREA(wa_control, 2048);
extern THD_FUNCTION(control_thread, p);

#endif // CONTROL_H
