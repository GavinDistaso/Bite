// SPDX-FileCopyrightText: 2022 Gavin Distaso
// SPDX-License-Identifier: MIT License

#include "cli-logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define NP(str) fputs(str, stdout); break;

static void changeOutputColor(int colorCode, _Bool light){
    #ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= 0x0004;
        SetConsoleMode(hOut, dwMode);
    #endif

    printf("\x1b[%i%s", colorCode, light ? ";1m" : "m");
    fflush(stdout);
}


/* Internal Functions Within The Header */
void I_CLI_printHeader(STATUS_TYPE s){
    changeOutputColor(s - (0xff * (s > 0xff)), s > 0xff);
    
    switch (s)
    {
        case STATUS_LOG: NP("[LOG]")
        case STATUS_WARNING: NP("[WARNING]")
        case STATUS_ERROR: NP("[ERROR]")
        case STATUS_FATAL: NP("[FATAL]")
        case STATUS_CMD: NP("[CMD]")
    }
    fputs(" ", stdout);
    changeOutputColor(0, 0);
}

void I_CLI_vprintf(const char* format, ...){
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    puts("");
}

void I_CLI_executeSpecial(STATUS_TYPE s){
    if(s == STATUS_FATAL) exit(-1);
}