#include "arms.h"
#include "ax12driver.h"
#include "pneumatic.h"
#include "RTT/SEGGER_RTT.h"

#define ID_LEFT 164
#define ID_RIGHT 130

void init_arms(void) {
    AX12Config_t ax12Config = {0};
    initAX12(115200);
    AX12setMode(ID_LEFT, DEFAULT_MODE);
    AX12setMode(ID_RIGHT, DEFAULT_MODE);
    AX12setSpeed(ID_LEFT, 10);
    AX12setSpeed(ID_RIGHT, 10);

    ax12Config.id = ID_LEFT;
    if (AX12dump(&ax12Config) == 0) {
        printf("CW %d, CCW %d\n", ax12Config.CWLimit, ax12Config.CCWLimit);
    }

    set_arms_down();
}

void set_arms_down(void) {
    AX12move(ID_LEFT, -70, NULL);
    AX12move(ID_RIGHT, -25, NULL);
}

void set_arms_up(void) {
    AX12move(ID_LEFT, -130, NULL);
    AX12move(ID_RIGHT, 45, NULL);
}

void set_arms(arms_position_t position) {
    switch(position) {
    case ARMS_DOWN:
        set_arms_down();
        retract_arms();
        break;
    case ARMS_UP:
        set_arms_up();
        extend_arms();
        break;
    case ARMS_MIDDLE:
        AX12move(ID_LEFT, -95, NULL);
        AX12move(ID_RIGHT, 5, NULL);
        break;
    default:
        break;
    }
}
