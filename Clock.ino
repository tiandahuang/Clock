/**
 * @file Clock.ino
 */
#include "debug.h"
#include "inc/serial-input.h"
#include "inc/motor-drivers.h"
#include "inc/time-keeping.h"

SplitFlapUnit SFU_hour_tens(SplitFlapUnit::HOUR_TENS);
SplitFlapUnit SFU_hour_ones(SplitFlapUnit::HOUR_ONES);
SplitFlapUnit SFU_min_tens(SplitFlapUnit::MIN_TENS);
SplitFlapUnit SFU_min_ones(SplitFlapUnit::MIN_ONES);

SplitFlapUnit* SFU_list[] = {&SFU_hour_tens, &SFU_hour_ones, &SFU_min_tens, &SFU_min_ones};

Time The_time;

void setup() {
    serial_init();
    stepper_init();
    dbprint("Starting up ...\n");
}

void loop() {
    // Update time()
    //The_time.updateTime();

    // Parse command (if there is one)
    if (Serial.available() > 0) {
        int32_t *input = serial_parse_read();
        if (input != nullptr) {
            switch (input[0]) {
                case MOVE :
                    if ((input[1] < 0) || (input[1] > 3)) {
                        Serial.println("ERROR: Invalid motor number");
                    }
                    SFU_list[input[1]]->trimSteps(input[2]);
                    break;
                case STOP : // TODO: implement this to break move commands
                    break;
                case TIME :
                    The_time.updateTime(input[1], input[2], input[3], true);
                    break;
                case SETF :
                    The_time.updateTime(input[1], input[2], 0, false);
                    break;
            }
        }
    }

    // Set positions for new times
    if (The_time.sem_minute_changed) {
        SFU_hour_tens.setPos(The_time.hour / 10);
        SFU_hour_ones.setPos(The_time.hour % 10);
        SFU_min_tens.setPos(The_time.minute / 10);
        SFU_min_ones.setPos(The_time.minute % 10);
        SFU_hour_tens.setSteps();
        SFU_hour_ones.setSteps();
        SFU_min_tens.setSteps();
        SFU_min_ones.setSteps();
        The_time.sem_minute_changed = false;
    }
    
    // Move motors (blocks until done)
    if (check_pending_move()) {
        update_all_positions_blocking();
    }
}

// TODO: tune motor speed
// TODO: physical stuff misbehaving
