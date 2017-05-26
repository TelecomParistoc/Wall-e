#include "ch.h"
#include "hal.h"
#include "motor.h"
#include "pneumatic.h"
#include "roof.h"
#include "RTT/SEGGER_RTT.h"

uint32_t time;

int main(void)
{
    halInit();
    chSysInit();

    SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_TRIM);

    init_motors();
    pneumatic_init();

    set_led_on(LED_1);

    if (read_button(BUTTON_1) == 1) {
        set_led_on(LED_2);
    } else {
        set_led_off(LED_2);
    }

    set_speed(MOTOR_RIGHT, 0);
    set_speed(MOTOR_LEFT, 0);

    time = 0;

    while (is_something_detected()) {
        __asm__ __volatile__ ("nop");
    }

    while(1) {
        chThdSleepMilliseconds(1);
        palTogglePad(GPIOC, GPIOC_DEBUG_LED);
        if (is_something_detected() || (time > 3000)) {
            set_speed(MOTOR_RIGHT, 0);
            set_speed(MOTOR_LEFT, 0);
        } else {
            set_speed(MOTOR_RIGHT, 30);
            set_speed(MOTOR_LEFT, 30);
            time++;
        }
    }

    return 1;
}
