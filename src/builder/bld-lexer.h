// SPDX-FileCopyrightText: 2022 Gavin Distaso
// SPDX-License-Identifier: MIT License

#ifndef BUILD_BLD_LEXER_H
#define BUILD_BLD_LEXER_H

typedef struct LEXER_CTX{
    char* filedata;
} LEXER_CTX;

void BLD_freeLexer(LEXER_CTX* ctx);

_Bool BLD_readSource(LEXER_CTX* ctx, const char* filepath);

#endif // BUILD_BLD_LEXER_H