// SPDX-FileCopyrightText: 2022 Gavin Distaso
// SPDX-License-Identifier: MIT License

#include "builder.h"
#include "assembler.h"
#include <biteVM/instructions.h>
#include <cli/logger.h>

#include <stdlib.h>
#include <string.h>

/* const defines */

const char* KEYWORDS[] = {
    "resource", "func", "return", "asm",

    "int8",     "int16",    "int32",    "int64",
    "uint8",    "uint16",   "uint32",   "uint64",

    "if", "else", "for", "while", "break", "continue"
};

const unsigned int KEYWORDS_LEN = sizeof(KEYWORDS) / sizeof(char*);

/* function defines */

static _Bool executeKeyword(BAB_CTX* biteASM, PARSER_CTX* parser, int* i);
static void convKwrdIndex2Op(BAB_CTX* biteASM, PARSER_CTX* parser, int i);

void BLD_buildTokens(BAB_CTX* biteASM, PARSER_CTX* parser){
    biteASM->tokenIndex = biteASM->opcodeLen = 0;
    biteASM->opcodes = NULL;

    for(int i = 0; i < parser->tokenCount*2; i+=2){
        if(!executeKeyword(biteASM, parser, &i)){
            CLI_logStatus(STATUS_FATAL, "'%.*s' keyword not defined", 
                parser->tokens[i+1], parser->filedata + parser->tokens[i]);
        }
    }
}

/* internal functions */

static _Bool executeKeyword(BAB_CTX* biteASM, PARSER_CTX* parser, int* i){
    int start   = parser->tokens[*i],
        len     = parser->tokens[(*i)+1],
        j = 0;
    _Bool flag = 0; // changed if keyword is found

    for(; j < KEYWORDS_LEN; j++){
        if(strncmp(parser->filedata + start, KEYWORDS[j], len)){
            flag = 1;
            break;
        }
    }

    if(!flag) return 0;

    convKwrdIndex2Op(biteASM, parser, j);

    return 1;
}

// convert keyword index to opcode
static void convKwrdIndex2Op(BAB_CTX* biteASM, PARSER_CTX* parser, int i){
    

    // index is based on `KEYWORD` array, which is defined above
    switch(i){
        case 0: {BLD_assemble(biteASM, "SOME ASM CODE");} break;   // resource
        case 1: {} break;   // func
        case 2: {} break;   // return
        case 3: {} break;   // asm

        case 4: {} break;   // int8
        case 5: {} break;   // int16
        case 6: {} break;   // int32
        case 7: {} break;   // int64
        case 8: {} break;   // uint8
        case 9: {} break;   // uint16
        case 10: {} break;  // uint32
        case 11: {} break;  // uint64

        case 12: {} break;  // if
        case 13: {} break;  // else
        case 14: {} break;  // for
        case 15: {} break;  // while
        case 16: {} break;  // break
        case 17: {} break;  // continue
    }
}

