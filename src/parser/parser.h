// SPDX-FileCopyrightText: 2022 Gavin Distaso
// SPDX-License-Identifier: MIT License

#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H

typedef struct PARSER_CTX{
    char* filedata;
    unsigned int fileLength, tokenCount;
    unsigned int* tokens; // in pairs: [start][len] 
} PARSER_CTX;

void PRS_freeParser(PARSER_CTX* ctx);

_Bool PRS_readSource(PARSER_CTX* ctx, const char* filepath);

// splits into tokens
void PRS_tokenize(PARSER_CTX* ctx);

// removes comments
void PRS_prune(PARSER_CTX* ctx);

#endif // PARSER_PARSER_H