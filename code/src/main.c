#include "ch.h"
#include "hal.h"

static int main(int argc, char* argv[])
{
    halInit();
    chSysInit();

    while(1) {
        chThreadSleepMilliseconds(500);
        palTogglePad(GPIOC, GPIOC_DEBUG_LED);
    }
}
