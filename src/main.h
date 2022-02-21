// SPDX-FileCopyrightText: 2022 Gavin Distaso
// SPDX-License-Identifier: MIT License

#ifndef MAIN_H
#define MAIN_H

#define SETTINGS_CTX() {0, (char*)0, (char*)0}

typedef struct SETTINGS_CTX{
    int     buildMode; // 0 -> compile, 1 -> interpret, 2 -> convert
    char    *convertLanguage,
            *outputFileloc;
} SETTINGS_CTX;

#endif // MAIN_H