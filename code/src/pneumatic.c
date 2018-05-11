#include "pneumatic.h"
#include "hal.h"

extern void pneumatic_init(void) {
    stop_pump();
    close_valve(VALVE_2);
    close_valve(VALVE_1);
    close_valve(VALVE_3);
    close_valve(VALVE_4);
}

extern void start_pump(void) {
    palSetPad(GPIOB, GPIOB_PUMP);
}

extern void stop_pump(void) {
    palClearPad(GPIOB, GPIOB_PUMP);
}

void open_valve(valve_t id) {
    switch (id) {
        case VALVE_1:
            palSetPad(GPIOB, GPIOB_VALVE_1);
            break;
        case VALVE_2:
            palSetPad(GPIOB, GPIOB_VALVE_2);
            break;
        case VALVE_3:
            palSetPad(GPIOB, GPIOB_VALVE_3);
            break;
        case VALVE_4:
            palSetPad(GPIOA, GPIOA_VALVE_4);
            break;
        default:
            break;
    }
}

void close_valve(valve_t id) {
    switch (id) {
        case VALVE_1:
            palClearPad(GPIOB, GPIOB_VALVE_1);
            break;
        case VALVE_2:
            palClearPad(GPIOB, GPIOB_VALVE_2);
            break;
        case VALVE_3:
            palClearPad(GPIOB, GPIOB_VALVE_3);
            break;
        case VALVE_4:
            palClearPad(GPIOA, GPIOA_VALVE_4);
            break;
        default:
            break;
    }
}

extern void extend_arms(void) {
    stop_pump();

    open_valve(VALVE_2);
    close_valve(VALVE_1);

    open_valve(VALVE_4);
    close_valve(VALVE_3);

    start_pump();
    chThdSleepMilliseconds(1500U);
    stop_pump();
}

extern void retract_arms(void) {
    stop_pump();

    open_valve(VALVE_1);
    close_valve(VALVE_2);

    open_valve(VALVE_3);
    close_valve(VALVE_4);

    start_pump();
    chThdSleepMilliseconds(1500U);
    stop_pump();
}
