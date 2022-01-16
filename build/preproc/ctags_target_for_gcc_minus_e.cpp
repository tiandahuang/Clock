# 1 "\\\\tienda\\network_files\\Projects\\Clock\\Clock.ino"
/**

 * @file Clock.ino

 */
# 4 "\\\\tienda\\network_files\\Projects\\Clock\\Clock.ino"
# 5 "\\\\tienda\\network_files\\Projects\\Clock\\Clock.ino" 2
# 6 "\\\\tienda\\network_files\\Projects\\Clock\\Clock.ino" 2
# 7 "\\\\tienda\\network_files\\Projects\\Clock\\Clock.ino" 2

SplitFlapUnit SFU_hour_tens(SplitFlapUnit::HOUR_TENS);
SplitFlapUnit SFU_hour_ones(SplitFlapUnit::HOUR_ONES);
SplitFlapUnit SFU_min_tens(SplitFlapUnit::MIN_TENS);
SplitFlapUnit SFU_min_ones(SplitFlapUnit::MIN_ONES);

void setup() {
    serial_init();
    stepper_init();
    print_debug("Starting up ...\n");
}

void loop() {
    delay(3000);
    SFU_hour_tens.trimSteps(4096);
    SFU_hour_ones.trimSteps(4096);
    SFU_min_tens.trimSteps(4096);
    SFU_min_ones.trimSteps(4096);
    update_all_positions_blocking();

    delay(3000);
    SFU_hour_tens.trimSteps(-4096);
    SFU_hour_ones.trimSteps(-4096);
    SFU_min_tens.trimSteps(-4096);
    SFU_min_ones.trimSteps(-4096);
    update_all_positions_blocking();
}
