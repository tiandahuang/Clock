/**
 * @file serial-input.cpp
 */

#include <Arduino.h>
#include "../inc/serial-input.h"
#include "../debug.h"

#define SERIAL_TIMEOUT 1000
#define SERIAL_MAX_LENGTH 32
static char input_buffer[SERIAL_MAX_LENGTH + 1] = {'\0'};   // last char reserved for null terminator

#define MAX_PARAMS 4
static int32_t params[MAX_PARAMS] = {0};

/** Serial inialization
 */
void serial_init() {
    Serial.begin(115200);
}

/** Read Byte Safe
 * @brief Read a byte from Serial with timeout
 * 
 * @return Value of byte read if valid read, -1 if timeout
 */
char read_byte_safe() {
    char byte_in;
    long start_time_millis = millis();
    do {
        byte_in = Serial.read();
        if (byte_in >= 0) return byte_in;
    } while (millis() - start_time_millis < SERIAL_TIMEOUT);
    return -1;
}

/** Serial Read Safe
 * @brief Read from Serial until any of the terminators are reached or until the input buffer is filled
 * 
 * @param char* term: Null-terminated string containing terminators
 * @param char* buf: Input buffer
 * @param uint8_t length: Size of input buffer
 * @return Length of the message read, -1 if error
 */
int32_t serial_read_safe(const char *term, char *buf, uint8_t length) {
    if (Serial.available() == 0) {
        dbprint("ERROR: Empty serial read\n");
        return -1;
    }

    // find number of terminators to speed up byte reading
    uint8_t num_terminators = strlen(term);
    uint8_t i;

    for (i = 0; i < length; i++) {
        char byte_in = read_byte_safe();
        if (byte_in == -1) {    // timeout
            i = -1;
            dbprint("ERROR: Serial timeout\n");
            break;
        }
        // check terminator
        bool term_reached = false;
        for (uint8_t j = 0; j < num_terminators; j++) {
            if (byte_in == term[j]) {
                term_reached = true;
                break;
            }
        }
        if (term_reached) break;

        input_buffer[i] = byte_in;
    }
    if (i > 0) {
        input_buffer[i] = '\0';
    }

    return i;
}

const char cmd_0[] PROGMEM = "move";
const char cmd_1[] PROGMEM = "stop";
const char cmd_2[] PROGMEM = "time";
const char cmd_3[] PROGMEM = "setf";
const char* const cmd_table[] PROGMEM = {cmd_0, cmd_1, cmd_2, cmd_3};

/** Serial Parse and Read
 * @brief Read serial message and parse the command and parameters
 * @note Ideally call only when serial is available, nullptr will be returned if unavailable
 * 
 * @return int32_t* pointer to array of parsed parameters, nullptr if invalid
 */
int32_t* serial_parse_read() {
    if (Serial.available() == 0) {
        return nullptr;
    }

    int32_t message_len = serial_read_safe("\n\r", input_buffer, SERIAL_MAX_LENGTH);
    for (uint8_t i = 0; (i < SERIAL_MAX_LENGTH) && (input_buffer[i] != '\0'); i++) {
        input_buffer[i] = tolower(input_buffer[i]);
    }
    Serial.println(input_buffer);

    char* split = strtok(input_buffer, " ,");
    if (split == nullptr) {
        return nullptr;
    }
    uint8_t command_num;
    for (command_num = 0; command_num < NUM_COMMANDS; command_num++) {
        #ifndef __INTELLISENSE__
        if (strcmp_P(split, (char*)pgm_read_word(&(cmd_table[command_num]))) == 0) {
            break;
        }
        #endif
    }
    if (command_num == NUM_COMMANDS) {
        Serial.println("Invalid command.");
        return nullptr;
    }
    else {
        params[0] = command_num;
        dbprint("Parsed command: [", params[0], "] ");
    }

    for (uint8_t i = 1; i < MAX_PARAMS; i++) {
        split = strtok(nullptr, " ,");
        if (split == nullptr) {
            break;
        }
        params[i] = atoi(split);
        dbprint("[", params[i], "] ");
    }
    dbprint("\n");

    return params;
}

// TODO: Input parsing - return int array of params?
// TODO: param array returned to main() to do stuff in debug loop