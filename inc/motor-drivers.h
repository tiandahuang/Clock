/**
 * @file motor-drivers.h
 * 
 * 28BYJ-48 Stepper Motor with ULN2003 Driver
 * Pin order: IN1, IN3, IN2, IN4
 * 
 * With 4 pins per motor and 4 motors, pins D2-D13, A0-A3 will be used
 * 
 * Per motor power: ~300mA @ 5V
 * Effective max steps/second: 1000
 * 4096 steps/revolution (half-steps)
 */

#pragma once



void stepper_init();

/** Update All Positions
 * @brief Step once on all motors according to the speed set. 
 * @note Call really fast repeatedly until 'false' is returned to run all motors to their positions.
 * 
 * @return true if any motor is still running to its position, false if all motors are done moving
 */
bool update_all_positions();

/** Update All Positions (Blocking)
 * @brief Run all motors until their positions are reached
 */
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

    /** Get Position
     * @return uint8_t current position (0 - 10)
     */
    uint8_t getPos();

    /** Set Position
     * @brief Set next position and the index to lookup steps with
     * 
     * @param pos Next position (0 - 10)
     */
    void setPos(uint8_t pos);

    /** Set Steps
     * @brief Evaluate and set the steps to reach the target position
     * 
     * @return true if steps correctly set, false if called when motor is in motion
     */
    bool setSteps();

    /** Trim Steps
     * @brief Set steps to move by the amount specified
     * @note Does not actually move motor
     * 
     * @param trim Steps to move
     */
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