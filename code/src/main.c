#include "ch.h"
#include "hal.h"
#include "motor.h"
#include "eyes.h"
#include "pneumatic.h"
#include "roof.h"
#include "move.h"
#if USE_IMU
#include "imudriver.h"
#endif /* USE_IMU */
#include "ax12driver.h"
#include "RTT/SEGGER_RTT.h"

void cb(void) {
    printf("cb\n");
}

int main(void)
{
    int sign;
    int status;
    halInit();
    chSysInit();

    SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_TRIM);

    init_motors();
    init_eyes();
    pneumatic_init();

    set_speed(MOTOR_RIGHT, 0);
    set_speed(MOTOR_LEFT, 0);
    initAX12(115200);

#if USE_IMU
    chThdSleepMilliseconds(2000);
    sdStart(&SD1, &imu_serial_conf);
    status = initIMU(&SD1);
    printf("initIMU %d\n", status);
#endif /* USE_IMU */


    if (read_button(BUTTON_1) == 1) { // left button
        /* Yellow team */
        set_led_on(LED_2);
        sign = 1;
    } else {
        /* Blue team */
        set_led_on(LED_1);
        sign = -1;
    }
    int id = 130;
    printf("res %d\n", AX12setMode(id, DEFAULT_MODE));

    AX12Config_t ax12Config = {0};
    ax12Config.id = id;
/*    if (AX12dump(&ax12Config) == 0) {
        printf("CW %d, CCW %d\n", ax12Config.CWLimit, ax12Config.CCWLimit);
    }

    double i;
    for (i = 0; i < 100; i++) {
        printf("pos %d (%d)\n", (int)AX12getPosition(id), (int)i);
        printf("%d\n", AX12move(id, i, cb));
    }*/

    while(1) {
        chThdSleepMilliseconds(100);
        palTogglePad(GPIOC, GPIOC_DEBUG_LED);
//        printf("heading %d\n", getHeading());
        sdPut(&SD1, 0x55);
    }

    return 1;
}
