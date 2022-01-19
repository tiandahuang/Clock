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

SplitFlapUnit* SFU_list[] = {&SFU_hour_tens, &SFU_hour_ones, &SFU_min_tens, &SFU_min_ones};

void setup() {
    serial_init();
    stepper_init();
    dbprint("Starting up ...\n");
}

void loop() {
    if (Serial.available() > 0) {
        int32_t *input = serial_parse_read();
        switch (input[0]) {
            case MOVE :
                if ((input[1] < 0) || (input[1] > 3)) {
                    Serial.println("ERROR: Invalid motor number");
                }
                SFU_list[input[1]]->trimSteps(input[2]);
                break;
            case STOP : // TODO: implement this to break move commands
                break;
            case TIME : // TODO: The time part of the clock
                break;
        }
    }
    if (check_pending_move()) {
        update_all_positions_blocking();
    }
}
