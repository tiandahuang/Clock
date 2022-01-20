/**
 * @file time-keeping.cpp
 */

#include <Arduino.h>
#include "../inc/time-keeping.h"
#include "../debug.h"

Time::Time() : 
    hours_{0}, minutes_{0}, seconds_{0}, milliseconds_{0}, 
    sem_minute_changed{false} {};

Time::Time(uint8_t hours_, uint8_t minutes_, uint8_t seconds_) :
    hours_{hours_}, minutes_{minutes_}, seconds_{seconds_}, milliseconds_{0}, 
    sem_minute_changed{false} {};

/**
 * @brief Change time to new desired time
 * @note Will not mess up the time if big numbers are inputted
 * 
 * @return true if time successfully updated, false if inputs out of bounds
 */
bool Time::updateTime(uint8_t hours_, uint8_t minutes_, uint8_t seconds_) {
    bool invalid = ((hours_ >= 24) || (minutes_ >= 60) || (seconds_ >= 60));
    if (invalid) {
        dbprint("ERROR: Invalid time for updating: ", 
            hours_, " ", minutes_, " ", seconds_, "\n");
        return false;
    }
    this->hours_ = hours_;
    this->minutes_ = minutes_;
    this->seconds_ = seconds_;
    previous_millis_ = millis();
    milliseconds_ = 0;
    sem_minute_changed = true;

    return true;
}

/**
 * @brief Update stored time
 * @note Call as frequently as possible, at least once per second
 */
void Time::updateTime() {
    milliseconds_ += millis() - previous_millis_;
    if (milliseconds_ < 1000) {
        // Exit quickly if seconds has not ticked over
        return;
    }
    else if ((milliseconds_ >= 1000) && (milliseconds_ < 2000)) {
        // Single-increment if time is at most +1 sec
        milliseconds_ -= 1000;
        seconds_++;
        if (seconds_ == 60) {
            seconds_ = 0;
            minutes_++;
            sem_minute_changed = true;
        }
        if (minutes_ == 60) {
            minutes_ = 0;
            hours_++;
        }
        if (hours_ == 24) {
            hours_ = 0;
        }
    }
    else {
        /**
         * This stuff is necessary in the case that a long blocking
         * motor function is called. I'll try to avoid that, but there
         * may be somewhere that it's unavoidable.
         */

        sem_minute_changed = true;
        // Multi-increment time if a long time has passed
        seconds_ += milliseconds_ / 1000;
        milliseconds_ %= 1000;

        if (seconds_ >= 60) {
            minutes_ += seconds_ / 60;
            seconds_ %= 60;
            sem_minute_changed = true;
        }
        // Minutes ticks over by max 2
        if (minutes_ >= 60) {
            minutes_ -= 60;
            hours_++;
        }
        if (hours_ == 24) {
            hours_ = 0;
        }
    }
}

// TODO: stopwatch and timer support
