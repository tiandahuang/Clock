/**
 * @file motor-drivers.cpp
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

#include <Arduino.h>
#include "../inc/motor-drivers.h"
#include "../inc/steps-lookup-table.h"
#include <AccelStepper.h>
#include "../debug.h"

#define NUM_MOTORS 4
#define MAX_SPEED 1000
#define STEPS_PER_REVOLUTION 4096

#define MOVE_SPEED MAX_SPEED

/**
 * The pin mappings are for an Arduino Nano 
 * Given the limited number of pins, these won't change
 * Numbers are from left to right
 */

AccelStepper stepper1(AccelStepper::HALF4WIRE, 2, 4, 3, 5); // todo: fix
AccelStepper stepper2(AccelStepper::HALF4WIRE, 6, 8, 7, 9);
AccelStepper stepper3(AccelStepper::HALF4WIRE, A0, A2, A1, A3);
AccelStepper stepper4(AccelStepper::HALF4WIRE, 10, 12, 11, 13);

AccelStepper* stepper_list[] = {&stepper1, &stepper2, &stepper3, &stepper4};

void stepper_init() {
    for (uint8_t i = 0; i < NUM_MOTORS; i++) {
        stepper_list[i]->setMaxSpeed(MOVE_SPEED);
        stepper_list[i]->setSpeed(0);
        stepper_list[i]->disableOutputs();
    }
}

bool update_all_positions() {
    bool running = false;
    for (uint8_t i = 0; i < NUM_MOTORS; i++) {
        if (stepper_list[i]->distanceToGo() != 0) {
            stepper_list[i]->runSpeed();
            running = true;
        }
        else {
            stepper_list[i]->setSpeed(0);
        }
    }
    return running;
}

void update_all_positions_blocking() {
    for (uint8_t i = 0; i < NUM_MOTORS; i++) {
        stepper_list[i]->enableOutputs();
    }

    while (update_all_positions());

    for (uint8_t i = 0; i < NUM_MOTORS; i++) {
        stepper_list[i]->disableOutputs();
    }
}

/**
 * Each unit is initialized to the 'blank' position.
 * Run calibration to move each unit to the exact position.
 */
SplitFlapUnit::SplitFlapUnit(uint8_t num) : num{num}, current_pos{10}, next_pos{10} {};


uint8_t SplitFlapUnit::getPos() {
    return current_pos;
}


void SplitFlapUnit::setPos(uint8_t pos) {
    next_pos = pos;

    if (pos == 10) {
        next_lookup_idx = 10;
    }
    else {
        // for the hour 'hands,' the flap order is flipped
        next_lookup_idx = (num <= HOUR_ONES) ? (9 - pos) : pos;
    }
}


bool SplitFlapUnit::setSteps() {
    if ((stepper_list[num]->distanceToGo() != 0) || (stepper_list[num]->speed() != 0.0)) {
        // steps for next move cannot be set while motor is running
        return false;
    }

    int32_t steps_to_go = steps_lookup[next_lookup_idx] - steps_lookup[current_lookup_idx];
    if (steps_to_go < 0) steps_to_go += STEPS_PER_REVOLUTION;
    if ((num % 2) == 1) steps_to_go *= -1;
    dbprint("Steps for next move:", steps_to_go, "\n");

    this->trimSteps(steps_to_go);
}


void SplitFlapUnit::trimSteps(int32_t trim) {
    stepper_list[num]->setCurrentPosition(0);
    stepper_list[num]->moveTo(trim);
    stepper_list[num]->setSpeed((trim < 0) ? (-MOVE_SPEED) : MOVE_SPEED);
}
