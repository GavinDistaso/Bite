// SPDX-FileCopyrightText: 2022 Gavin Distaso
// SPDX-License-Identifier: MIT License

#ifndef BLD_BUILDER_H
#define BLD_BUILDER_H

#include <parser/parser.h>

typedef struct BITE_ASM_BUILDER_CTX{
    unsigned char*  opcodes;
    unsigned int    opcodeLen, 
                    tokenIndex;
    int openBracketCount; // counts how many closing brackets are needed
} BAB_CTX;

/* function defines */

void BLD_buildTokens(BAB_CTX* biteASM, PARSER_CTX* parser);

#endif // BLD_BUILDER_H