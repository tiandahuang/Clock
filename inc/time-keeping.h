/**
 * @file time-keeping.h
 * 
 * 24-Hour time updated with Second-granularity
 */
#pragma once

class Time
{
public:
    const uint8_t& hour = hours_;
    const uint8_t& minute = minutes_;
    const uint8_t& second = seconds_;
    
    /**
     * Semaphore set when the time is changed during an update
     * Clear this to acknowledge the time update
     */
    bool sem_time_changed;

    Time();
    /**
     * Does not check if the parameters are valid
     */
    Time(uint8_t hours_, uint8_t minutes_, uint8_t seconds_);
    
    /**
     * @brief Change time to new desired time
     * @note Will not mess up the time if big numbers are inputted
     */
    bool updateTime(uint8_t hours_, uint8_t minutes_, uint8_t seconds_);

    /**
     * @brief Update stored time
     * @note Call as frequently as possible, at least once per second
     */
    void updateTime();

private:
    /**
     * Hour from 0 - 23
     * Minute, Second from 0 - 59
     */
    uint8_t hours_, minutes_, seconds_;

    /**
     * Milliseconds from 0 - 999
     * This may not be that accurate depending on how often updateTime() is called
     * In the worst case, milliseconds can be updated after 1 minute and still
     * update the time correctly
     */
    uint16_t milliseconds_;

    /**
     * System time at the previous updateTime() call
     */
    unsigned long previous_millis_;
};