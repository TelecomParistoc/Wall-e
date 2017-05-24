#ifndef ROOF_H
#define ROOF_H

#include "hal.h"

typedef enum button_t {
    BUTTON_1,
    BUTTON_2,
    BUTTON_3
} button_t;

typedef enum led_t {
    LED_1,
    LED_2,
    LED_3,
    LED_4
} led_t;

/**
 * @brief Read the status of the JACK.
 *
 * @return A boolean indicating whether the jack is present or not.
 * @retval TRUE Jack is present.
 * @retval FALSE Jack has been removed.
 */
bool get_jack_status(void);

/**
 * @brief Read the status of the button.
 *
 * @param[in] id The id of the button to read the status of.
 *
 * @return An int indicating the status of the button.
 * @retval 0 Wrong parameter.
 * @retval 1 Button in position 1.
 * @retval 2 Button in position 2.
 */
int read_button(button_t id);

/**
 * @brief Turn on the led.
 *
 * @param[in] id The id of the led to turn on.
 */
void set_led_on(led_t id);

/**
 * @brief Turn off the led.
 *
 * @param[in] id The id of the led to turn off.
 */
void set_led_off(led_t id);

#endif /* ROOF_H */
