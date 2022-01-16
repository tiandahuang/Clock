/**
 * @file motor-drivers.h
 * 
 * 28BYJ-48 Stepper Motor with ULN2003 Driver
 * Pin order: IN1, IN3, IN2, IN4
 * 
 * With 4 pins per motor and 4 motors, pins D2-D13, A0-A3 will be used
 * 
 * Power: ~300mA @ 5V
 * Effective max steps/second: 1000
 * 4096 steps/revolution (half-steps)
 */

#pragma once



void stepper_init();
bool update_all_positions();
void update_all_positions_blocking();

class SplitFlapUnit
{
public:
    enum UnitLabel : uint8_t {
        HOUR_TENS = 0,
        HOUR_ONES,
        MIN_TENS,
        MIN_ONES
    };

    /**
     * Each unit is initialized to the 'blank' position.
     * Run calibration to move each unit to the exact position.
     */
    SplitFlapUnit(uint8_t num);

    uint8_t getPos();
    void setPos(uint8_t pos);
    bool setSteps();
    void trimSteps(int32_t trim);

private:
    /**
     * unit 0 - 3 (HOUR_TENS, HOUR_ONES, MIN_TENS, MIN_ONES)
     */
    uint8_t num;

    /**
     * flap positions 0 - 10
     * 0 - 9 correspond to digits 0 - 9
     * flap position 10 is blank
     */
    uint8_t current_pos, next_pos;

    /**
     * index to find steps in the lookup table
     */
    uint8_t current_lookup_idx, next_lookup_idx;
};