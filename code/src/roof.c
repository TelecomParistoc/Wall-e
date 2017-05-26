#include "roof.h"

bool get_jack_status(void) {
    if (palReadPad(GPIOA, GPIOA_JACK) == PAL_LOW) {
        return TRUE;
    } else {
        return FALSE;
    }
}

int read_button(button_t id) {
    int ret_value;
    switch (id) {
        case BUTTON_1:
            ret_value = palReadPad(GPIOA, GPIOA_BUTTON_1) + 1;
            break;
        case BUTTON_2:
            ret_value = palReadPad(GPIOA, GPIOA_BUTTON_2) + 1;
            break;
        case BUTTON_3:
            ret_value = palReadPad(GPIOB, GPIOB_BUTTON_3) + 1;
            break;
        default:
            ret_value = 0;
            break;
    }

    return ret_value;
}

void set_led_on(led_t id) {
    switch(id) {
        case LED_1:
            palSetPad(GPIOB, GPIOB_LED_1);
            break;
        case LED_2:
            palSetPad(GPIOA, GPIOA_LED_2);
            break;
        case LED_3:
            palSetPad(GPIOA, GPIOA_LED_3);
            break;
        case LED_4:
            palSetPad(GPIOB, GPIOB_LED_4);
            break;
        default:
            break;
    }
}

void set_led_off(led_t id) {
    switch(id) {
        case LED_1:
            palClearPad(GPIOB, GPIOB_LED_1);
            break;
        case LED_2:
            palClearPad(GPIOA, GPIOA_LED_2);
            break;
        case LED_3:
            palClearPad(GPIOA, GPIOA_LED_3);
            break;
        case LED_4:
            palClearPad(GPIOB, GPIOB_LED_4);
            break;
        default:
            break;
    }
}

bool is_something_detected(void) {
    return !palReadPad(GPIOB, GPIOB_EYE_START);
}
