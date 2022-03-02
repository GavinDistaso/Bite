// SPDX-FileCopyrightText: 2022 Gavin Distaso
// SPDX-License-Identifier: MIT License

#ifndef BUILD_BLD_LEXER_H
#define BUILD_BLD_LEXER_H

typedef struct PARSER_CTX{
    char* filedata;
    unsigned int fileLength, tokenCount;
    unsigned int* tokens; // in pairs: [start][len] 
} PARSER_CTX;

void BLD_freeParser(PARSER_CTX* ctx);

_Bool BLD_readSource(PARSER_CTX* ctx, const char* filepath);

// splits into tokens
void BLD_tokenize(PARSER_CTX* ctx);

// removes comments
void BLD_prune(PARSER_CTX* ctx);

#endif // BUILD_BLD_LEXER_H