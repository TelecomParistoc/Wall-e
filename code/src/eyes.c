#include "eyes.h"
#include "hal.h"
#include "RTT/SEGGER_RTT.h"

#define PWM_MAX 1000
#define CLK_KHZ 72000
#define PWM_FREQUENCY_KHZ 0.05

extern void init_eyes(void) {

    RCC->APB2ENR |= 0x00010000; // enable clocks for TIM15

	TIM15->CCER  = 0x00;      // disable capture/compare during setup
	TIM15->CR2   = 0x00;
	TIM15->BDTR  = 0x8C00;    // enable outputs
	TIM15->DIER  = 0x00;      // disable DMA and interrupts
	TIM15->SMCR  = 0x00;      // master mode
	TIM15->CCMR1 = 0x6868;    // PWM mode 1 on channels 1 and 2
	TIM15->ARR   = PWM_MAX - 1;
	TIM15->PSC   = CLK_KHZ/(PWM_FREQUENCY_KHZ * PWM_MAX) - 1; // setup prescaler
	TIM15->CCR1  = 0; // turn off motors at startup
	TIM15->CCR2  = 0; // turn off motors at startup
	TIM15->CCER  = 0x31;      // enable CC2 and CC1
	TIM15->EGR   = 0x01;      // generate an update event to load the setup values
TIM15->CR1 = 0x81; // enable counter
}

static void ext_cb(EXTDriver* driver, expchannel_t channel);

const EXTConfig ext_config = {
    {
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOB, ext_cb},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOA, ext_cb},
        {EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOA, ext_cb},
        {EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOA, ext_cb},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL}
    }
};

extern void set_pos(eye_t eye, int8_t pos) {
    switch (eye) {
        case EYE_LEFT:
            TIM15->CCR1 = pos;
            break;
        case EYE_RIGHT:
            TIM15->CCR2 = PWM_MAX - pos;
            break;
        default:
            break;
    }
}

static void ext_cb(EXTDriver* driver, expchannel_t channel) {
    (void)driver;

    printf("interrupt on channel %d\n", channel);
}

extern void eyes_up(void) {
    set_pos(EYE_LEFT, 50);
    set_pos(EYE_RIGHT, 73);
}

extern void eyes_down(void) {
    set_pos(EYE_LEFT, 73);
    set_pos(EYE_RIGHT, 50);

}
