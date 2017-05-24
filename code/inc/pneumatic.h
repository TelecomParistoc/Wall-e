#ifndef PNEUMATIC_H
#define PNEUMATIC_H

#define VALVE_NB 4

typedef enum valve_t {
    VALVE_1,
    VALVE_2,
    VALVE_3,
    VALVE_4
} valve_t;

extern void pneumatic_init(void);

extern void start_pump(void);
extern void stop_pump(void);

extern void extend_arms(void);
extern void retract_arms(void);

#endif
