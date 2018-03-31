#include "orientation.h"
#include "imudriver.h"

uint16_t get_orientation(void) {
    return getHeading();
}
