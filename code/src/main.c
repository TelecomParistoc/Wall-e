#include "ch.h"
#include "hal.h"
#include "motor.h"
#include "pneumatic.h"
#include "roof.h"
#include "move.h"
#include "RTT/SEGGER_RTT.h"

uint32_t time;

int main(void)
{
    int sign;
    halInit();
    chSysInit();

    SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_TRIM);

    init_motors();
    pneumatic_init();

    set_led_on(LED_1);

    set_speed(MOTOR_RIGHT, 0);
    set_speed(MOTOR_LEFT, 0);

    time = 0;
    if (read_button(BUTTON_1) == 1) {
        /* Yellow team */
        set_led_on(LED_2);
        sign = 1;
    } else {
        /* Blue team */
        set_led_on(LED_3);
        sign = -1;
    }

    move_forward(6000);
    turn(sign * (-90));
    move_forward(3000);
    turn(sign * (-90));
    move_forward(5000);
    turn(sign * 90);
    move_forward(2000);
    turn(sign * 90);
    move_forward(5000);
    turn(sign * 90);
    move_forward(5000);
    turn (sign * 90);
    move_forward(4000);

    while(1) {
        chThdSleepMilliseconds(1);
        palTogglePad(GPIOC, GPIOC_DEBUG_LED);
    }

    return 1;
}
