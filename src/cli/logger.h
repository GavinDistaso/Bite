// SPDX-FileCopyrightText: 2022 Gavin Distaso
// SPDX-License-Identifier: MIT License

#ifndef CLI_LOGGER_H
#define CLI_LOGGER_H

typedef enum STATUS_TYPE{
    STATUS_LOG = 37, 
    STATUS_WARNING = 33, 
    STATUS_ERROR = 31 + 0xff, // + 0xff makes it lighter color
    STATUS_FATAL = 31, //! will quit program
    STATUS_CMD = 34,
    STATUS_SUCCESS = 32,
} STATUS_TYPE;

// /* Internal Functions */
void I_CLI_printHeader(STATUS_TYPE s);
void I_CLI_vprintf(const char* format, ...);
void I_CLI_executeSpecial(STATUS_TYPE s);

// /* Not Internal Function, Not Even A Function :) */
#define CLI_logStatus(s, format, ...) {             \
    I_CLI_printHeader(s);                           \
    I_CLI_vprintf(format, ##__VA_ARGS__);           \
    I_CLI_executeSpecial(s);                        \
}

void CLI_rawPrint(const char* msg);

#endif // CLI_LOGGER_H