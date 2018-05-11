#ifndef PNEUMATIC_H
#define PNEUMATIC_H

#define VALVE_NB 4

typedef enum valve_t {
    VALVE_1, // intérieur rouge
    VALVE_2, //interieur (pompe coude)
    VALVE_3, // interieur pompe droit
    VALVE_4 // exterieur
} valve_t;

extern void pneumatic_init(void);

extern void start_pump(void);
extern void stop_pump(void);

extern void extend_arms(void);
extern void retract_arms(void);

#endif
