/**
 * @file serial-input.h
 */

#pragma once

// Input commands
enum InputCommand : uint8_t {
    MOVE = 0,
    STOP,
    TIME,
    SETF,
    NUM_COMMANDS
};

void serial_init();

/** Serial Parse and Read
 * @brief Read serial message and parse the command and parameters
 * @note Ideally call only when serial is available, nullptr will be returned if unavailable
 * 
 * @return int32_t* pointer to array of parsed parameters, nullptr if invalid
 */
int32_t* serial_parse_read();
