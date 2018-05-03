#ifndef BOARD_H
#define BOARD_H

/*
 * Setup for Motorboardv2
 */

/*
 * Board identifier.
 */
#define BOARD_ST_STM32F3_DISCOVERY
#define BOARD_NAME                  "Wall-e_OBC"

/*
 * Board oscillators-related settings.
 * NOTE: LSE not fitted.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                0U
#endif

#define STM32_LSEDRV                (3U << 3U)

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                8000000U
#endif

#define STM32_LSE_BYPASS
#define STM32_HSE_BYPASS

/*
 * MCU type as defined in the ST header.
 */
#define STM32F302x8

/*
 * IO pins assignments.
 */
#define GPIOA_BUTTON_2               0U
#define GPIOA_BUTTON_1               1U
#define GPIOA_MOTOR1_PWM             2U
#define GPIOA_MOTOR2_PWM             3U
#define GPIOA_VALVE_4                4U
#define GPIOA_JACK                   5U
#define GPIOA_LED_3                  6U
#define GPIOA_LED_2                  7U
#define GPIOA_CODEUSE_1_TIM          8U
#define GPIOA_CODEUSE_1              9U
#define GPIOA_SENSOR_2               10U
#define GPIOA_SENSOR_3               11U
#define GPIOA_SENSOR_4               12U
#define GPIOA_SWDIO                  13U
#define GPIOA_SWCLK                  14U
#define GPIOA_CODEUSE_2_TIM          15U

#define GPIOB_VALVE_3                0U
#define GPIOB_VALVE_2                1U
#define GPIOB_VALVE_1                2U
#define GPIOB_CODEUSE_2              3U
#define GPIOB_BUTTON_3               4U
#define GPIOB_SENSOR_1               5U
#define GPIOB_IMU_PROC_TX            6U
#define GPIOB_IMU_PROC_RX            7U
#define GPIOB_AX12_TX                8U
#define GPIOB_AX12_RX                9U
#define GPIOB_LED_4                  10U
#define GPIOB_LED_1                  11U
#define GPIOB_PUMP                   12U
#define GPIOB_PIN_13                 13U
#define GPIOB_EYE_1                  14U
#define GPIOB_EYE_2                  15U

#define GPIOC_DEBUG_LED              13U
#define GPIOC_PIN_14                 14U
#define GPIOC_PIN_15                 15U

#define GPIOF_PIN_0                  0U
#define GPIOF_PIN_1                  1U

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2U))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2U))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2U))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2U))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_VERYLOW(n)       (0U << ((n) * 2U))
#define PIN_OSPEED_LOW(n)           (1U << ((n) * 2U))
#define PIN_OSPEED_MEDIUM(n)        (2U << ((n) * 2U))
#define PIN_OSPEED_HIGH(n)          (3U << ((n) * 2U))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2U))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2U))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2U))
#define PIN_AFIO_AF(n, v)           ((v) << (((n) % 8U) * 4U))

// [MODE OTYPE OSPEED PUPDR]

/*
 * GPIOA setup:
 *
 * PA0  - BUTTON_2                  (input floating).
 * PA1  - BUTTON_1                  (input floating).
 * PA2  - MOTOR_1_PWM               (alternate 9).
 * PA3  - MOTOR_2_PWM               (alternate 9).
 * PA4  - VALVE_4                   (output push-pull).
 * PA5  - JACK                      (input floating).
 * PA6  - LED_3                     (output push-pull).
 * PA7  - LED_2                     (output push-pull).
 * PA8  - CODEUSE_1_TIM             (input floating).
 * PA9  - CODEUSE_1                 (input floating).
 * PA10 - SENSOR_2                  (input floating).
 * PA11 - SENSOR_3                  (input floating).
 * PA12 - SENSOR_4                  (input floating).
 * PA13 - SWDIO                     (alternate 0 pull-up).
 * PA14 - SWCLK                     (alternate 0 pull-down).
 * PA15 - CODEUSE_2_TIM             (input floating).
 */
#define VAL_GPIOA_MODER             (PIN_MODE_INPUT(GPIOA_BUTTON_2) |       \
                                     PIN_MODE_INPUT(GPIOA_BUTTON_1) |       \
                                     PIN_MODE_ALTERNATE(GPIOA_MOTOR1_PWM) | \
                                     PIN_MODE_ALTERNATE(GPIOA_MOTOR2_PWM)|  \
                                     PIN_MODE_OUTPUT(GPIOA_VALVE_4) |       \
                                     PIN_MODE_INPUT(GPIOA_JACK) |           \
                                     PIN_MODE_OUTPUT(GPIOA_LED_3) |         \
                                     PIN_MODE_OUTPUT(GPIOA_LED_2) |         \
                                     PIN_MODE_INPUT(GPIOA_CODEUSE_1_TIM) |  \
                                     PIN_MODE_INPUT(GPIOA_CODEUSE_1) |      \
                                     PIN_MODE_INPUT(GPIOA_SENSOR_2) |       \
                                     PIN_MODE_INPUT(GPIOA_SENSOR_3) |       \
                                     PIN_MODE_INPUT(GPIOA_SENSOR_4) |       \
                                     PIN_MODE_ALTERNATE(GPIOA_SWDIO) |      \
                                     PIN_MODE_ALTERNATE(GPIOA_SWCLK) |      \
                                     PIN_MODE_INPUT(GPIOA_CODEUSE_2_TIM))
#define VAL_GPIOA_OTYPER    0x00000000
#define VAL_GPIOA_OSPEEDR   0xFFFFFFFF
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_PULLUP(GPIOA_BUTTON_2) |     \
                                     PIN_PUPDR_PULLUP(GPIOA_BUTTON_1) |     \
                                     PIN_PUPDR_PULLUP(GPIOA_SWDIO)    |     \
                                     PIN_PUPDR_PULLDOWN(GPIOA_SWCLK)  |     \
                                     PIN_PUPDR_PULLUP(GPIOA_SENSOR_2) |     \
                                     PIN_PUPDR_PULLUP(GPIOA_SENSOR_3) |     \
                                     PIN_PUPDR_PULLUP(GPIOA_SENSOR_4))
#define VAL_GPIOA_ODR       0x00000000
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIOA_MOTOR1_PWM, 1U) |    \
                                     PIN_AFIO_AF(GPIOA_MOTOR2_PWM, 1U))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_SWDIO, 0U)   |       \
                                     PIN_AFIO_AF(GPIOA_SWCLK, 0U))

/*
 * GPIOB setup:
 *
 * PB0  - VALVE_3                   (output push-pull).
 * PB1  - VALVE_2                   (output push-pull).
 * PB2  - VALVE_1                   (output push-pull).
 * PB3  - CODEUSE_2                 (input floating).
 * PB4  - BUTTON_3                  (input floating).
 * PB5  - SENSOR_1                  (input floating).
 * PB6  - IMU_PROC_TX               (alternate 4).
 * PB7  - IMU_PROC_RX               (alternate 4).
 * PB8  - AX12_TX                   (alternate 7).
 * PB9  - AX12_RX                   (alternate 7).
 * PB10 - LED_4                     (output push-pull).
 * PB11 - LED_1                     (output push-pull).
 * PB12 - PUMP                      (output push-pull).
 * PB13 - PIN13                     (input floating).
 * PB14 - EYE_START                 (input floating).
 * PB15 - EYE2                      (alternate 1).
 */
#define VAL_GPIOB_MODER             (PIN_MODE_OUTPUT(GPIOB_VALVE_3) |       \
                                     PIN_MODE_OUTPUT(GPIOB_VALVE_2) |       \
                                     PIN_MODE_OUTPUT(GPIOB_VALVE_1) |       \
                                     PIN_MODE_INPUT(GPIOB_CODEUSE_2) |      \
                                     PIN_MODE_INPUT(GPIOB_BUTTON_3) |       \
                                     PIN_MODE_INPUT(GPIOB_SENSOR_1) |       \
                                     PIN_MODE_ALTERNATE(GPIOB_IMU_PROC_TX) |\
                                     PIN_MODE_ALTERNATE(GPIOB_IMU_PROC_RX) |\
                                     PIN_MODE_ALTERNATE(GPIOB_AX12_TX) |    \
                                     PIN_MODE_ALTERNATE(GPIOB_AX12_RX) |    \
                                     PIN_MODE_OUTPUT(GPIOB_LED_4) |         \
                                     PIN_MODE_OUTPUT(GPIOB_LED_1) |         \
                                     PIN_MODE_OUTPUT(GPIOB_PUMP) |          \
                                     PIN_MODE_INPUT(GPIOB_PIN_13) |         \
                                     PIN_MODE_ALTERNATE(GPIOB_EYE_1) |  \
                                     PIN_MODE_ALTERNATE(GPIOB_EYE_2))
#define VAL_GPIOB_OTYPER            0x00000000
#define VAL_GPIOB_OSPEEDR   0xFFFFFFFF
#define VAL_GPIOB_PUPDR             (PIN_PUPDR_PULLUP(GPIOB_VALVE_3) |      \
                                     PIN_PUPDR_PULLUP(GPIOB_VALVE_2) |      \
                                     PIN_PUPDR_PULLUP(GPIOB_VALVE_1) |      \
                                     PIN_PUPDR_PULLUP(GPIOB_BUTTON_3)|      \
                                     PIN_PUPDR_PULLUP(GPIOB_SENSOR_1)|      \
                                     PIN_PUPDR_PULLUP(GPIOB_LED_4) |        \
                                     PIN_PUPDR_PULLUP(GPIOB_LED_1) |        \
                                     PIN_PUPDR_PULLUP(GPIOB_PUMP))
#define VAL_GPIOB_ODR       0x00000000
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOB_IMU_PROC_TX, 7U) | \
                                     PIN_AFIO_AF(GPIOB_IMU_PROC_RX, 7U))
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(GPIOB_AX12_TX, 7U) |       \
                                     PIN_AFIO_AF(GPIOB_AX12_RX, 7U) |       \
                                     PIN_AFIO_AF(GPIOB_EYE_1, 1U)   |       \
                                     PIN_AFIO_AF(GPIOB_EYE_2, 1U))

/*
 * GPIOC setup:
 *
 * PC13 - DEBUG_LED                 (output push-pull).
 * PC14 - PIN14                     (input floating).
 * PC15 - PIN15                     (input floating).
 */
#define VAL_GPIOC_MODER     (PIN_MODE_OUTPUT(GPIOC_DEBUG_LED))
#define VAL_GPIOC_OTYPER    0x00000000
#define VAL_GPIOC_OSPEEDR   0xFFFFFFFF
#define VAL_GPIOC_PUPDR     (PIN_PUPDR_PULLUP(GPIOC_DEBUG_LED))
#define VAL_GPIOC_ODR       0x00000000
#define VAL_GPIOC_AFRL      0x00000000
#define VAL_GPIOC_AFRH      0x00000000

/*
 * GPIOD and E doesn't exist but seems to be required by ChibiOS
 */
#define VAL_GPIOD_MODER     0x00000000
#define VAL_GPIOD_OTYPER    0x00000000
#define VAL_GPIOD_OSPEEDR   0xFFFFFFFF
#define VAL_GPIOD_PUPDR     0x00000000
#define VAL_GPIOD_ODR       0x00000000
#define VAL_GPIOD_AFRL      0x00000000
#define VAL_GPIOD_AFRH      0x00000000

#define VAL_GPIOE_MODER     0x00000000
#define VAL_GPIOE_OTYPER    0x00000000
#define VAL_GPIOE_OSPEEDR   0xFFFFFFFF
#define VAL_GPIOE_PUPDR     0x00000000
#define VAL_GPIOE_ODR       0x00000000
#define VAL_GPIOE_AFRL      0x00000000
#define VAL_GPIOE_AFRH      0x00000000

/*
 * GPIOF setup:
 *
 * PF0  - PIN_0                    (input floating).
 * PF1  - PIN_1                    (input floating).
 */
#define VAL_GPIOF_MODER     0x00000000
#define VAL_GPIOF_OTYPER    0x00000000
#define VAL_GPIOF_OSPEEDR   0xFFFFFFFF
#define VAL_GPIOF_PUPDR     0x00000000
#define VAL_GPIOF_ODR       0x00000000
#define VAL_GPIOF_AFRL      0x00000000
#define VAL_GPIOF_AFRH      0x00000000

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* BOARD_H */
