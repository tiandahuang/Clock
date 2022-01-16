#line 1 "\\\\tienda\\network_files\\Projects\\Clock\\src\\serial-input.cpp"
/**
 * @file serial-input.cpp
 */

#include "../inc/serial-input.h"
#include <Arduino.h>
#include <string.h>

#define SERIAL_TIMEOUT 2000
#define SERIAL_MAX_LENGTH 30
char input_buffer[SERIAL_MAX_LENGTH + 1] = {'\0'};   // last char reserved for null terminator

/** Serial inialization
 */
void serial_init() {
    Serial.begin(115200);
}

/** Read Byte Safe
 * @brief Read a byte from Serial with timeout
 * 
 * @return >=0 if valid read, -1 if timeout
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
 * @return Length of the message read, -1 if Serial times out
 */
int serial_read_safe(const char *term, char *buf, uint8_t length) {
    // find number of terminators to speed up byte reading
    uint8_t num_terminators = strlen(term);
    uint8_t i;

    for (i = 0; i < length; i++) {
        char byte_in = read_byte_safe();
        if (byte_in == -1) {    // timeout
            i = -1;
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