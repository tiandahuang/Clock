/**
 * @file debug.h
 */

#pragma once

#define DEBUG

#ifdef DEBUG

#define dbprint(...) print_debug(__VA_ARGS__)

template<typename T>
void print_debug(T a) {
    Serial.print(a);
}

template<typename T, typename... Args>
void print_debug(T first, Args... args) {
    Serial.print(first);
    print_debug(args...);
}

#else

#define dbprint(...)

#endif