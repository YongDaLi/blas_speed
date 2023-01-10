/**
 * Yong Da Li
 * Saturday, July 16, 2022
 *
 * I need a quick and dirty logging thing
 * able to quickly turn on/off debug messages for the encoder
 * https://stackoverflow.com/questions/6508461/logging-library-for-c
 */
#include <stdint.h>  // uint32_t and other types

#ifndef LOGGER_H
#define LOGGER_H

// create log file wherever the top-level makefile is run
#define LOG_TO_FILE 		1
#define LOG_TO_TERMINAL 	1

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)       \
    (byte & 0x80 ? '1' : '0'),     \
        (byte & 0x40 ? '1' : '0'), \
        (byte & 0x20 ? '1' : '0'), \
        (byte & 0x10 ? '1' : '0'), \
        (byte & 0x08 ? '1' : '0'), \
        (byte & 0x04 ? '1' : '0'), \
        (byte & 0x02 ? '1' : '0'), \
        (byte & 0x01 ? '1' : '0')

void logger_init(const char* filename);
void logger_finalize();
void logger(const char* tag, const char* format, ...);

void print_binary_32(uint32_t word);

// printing to binary
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)       \
    (byte & 0x80 ? '1' : '0'),     \
        (byte & 0x40 ? '1' : '0'), \
        (byte & 0x20 ? '1' : '0'), \
        (byte & 0x10 ? '1' : '0'), \
        (byte & 0x08 ? '1' : '0'), \
        (byte & 0x04 ? '1' : '0'), \
        (byte & 0x02 ? '1' : '0'), \
        (byte & 0x01 ? '1' : '0')



#endif /* LOGGER_H */