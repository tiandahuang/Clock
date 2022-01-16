/**
 * @file Clock.ino
 */
#include "debug.h"
#include "inc/serial-input.h"
#include "inc/motor-drivers.h"

SplitFlapUnit SFU_hour_tens(SplitFlapUnit::HOUR_TENS);
SplitFlapUnit SFU_hour_ones(SplitFlapUnit::HOUR_ONES);
SplitFlapUnit SFU_min_tens(SplitFlapUnit::MIN_TENS);
SplitFlapUnit SFU_min_ones(SplitFlapUnit::MIN_ONES);

void setup() {
    serial_init();
    stepper_init();
    dbprint("Starting up ...\n");
}

void loop() {
    
}
