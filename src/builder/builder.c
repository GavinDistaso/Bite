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
    "resource", "func", "return", "asm", "}",

    "int8",     "int16",    "int32",    "int64",
    "uint8",    "uint16",   "uint32",   "uint64", "ptr",

    "if", "else", "for", "while", "break", "continue"
};

const unsigned int KEYWORDS_LEN = sizeof(KEYWORDS) / sizeof(char*);

/* function defines */

static _Bool executeKeyword(BAB_CTX* biteASM, PARSER_CTX* parser, int* i);
static void convKwrdIndex2Op(BAB_CTX* biteASM, PARSER_CTX* parser, int* i, int keyword);

void BLD_buildTokens(BAB_CTX* biteASM, PARSER_CTX* parser){
    biteASM->tokenIndex = biteASM->opcodeLen = 0;
    biteASM->opcodes = NULL;
    biteASM->openBracketCount = 0;

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

    // look through all possible keywords
    for(; j < KEYWORDS_LEN; j++){
        if(!strncmp(parser->filedata + start, KEYWORDS[j], len)){
            flag = 1;
            break;
        }
    }

    if(!flag) return 0;

    convKwrdIndex2Op(biteASM, parser, i, j);

    return 1;
}

static const char* getNextToken(PARSER_CTX* parser, int* i, int* len);
static _Bool memStrCmp(const void* mem, int memLen, const char* s);

// convert keyword index to opcode
static void convKwrdIndex2Op(BAB_CTX* biteASM, PARSER_CTX* parser, int* i, int keyword){
    
    //CLI_logStatus(STATUS_LOG, "%i", keyword);

    // index is based on `KEYWORD` array, which is defined above
    switch(keyword){
        case 0: {BLD_assemble(biteASM, "SOME ASM CODE");} break;   // resource
        case 1: {   //* ===== func =====
            // funcs are stored in opcodes as so:
            // [\x5][func name][\x0][arg1 type][arg1 name][\x0][arg2 type]...[\x1]
            // add func name to opcodes (padded with \0)
            int funcNameLen;
            const char* funcName = getNextToken(parser, i, &funcNameLen);

            char* opcodes = calloc(funcNameLen + 2, 1);
            opcodes[0] = FUNC;
            memcpy(opcodes + 1, funcName, funcNameLen);

            BLD_appendOpcodes(biteASM, opcodes, funcNameLen + 2);
            free(opcodes);

            // open perentheses check
            int perenLen;
            const char* peren = getNextToken(parser, i, &perenLen);
            if(!memStrCmp(peren, perenLen, "("))
                CLI_logStatus(STATUS_FATAL, "( expected");

            // add arguments to opcodes
            int argLen, argTypeLen;
            char    *argType = (char*)getNextToken(parser, i, &argTypeLen),
                    *arg     = (char*)getNextToken(parser, i, &argLen);

            while(!memStrCmp(argType, argTypeLen, ")")){
                char dataType = 0;
                if(argType[0] == 'u'){
                    dataType += 4; argType += 1; argTypeLen--;
                }
                if(memStrCmp(argType, argTypeLen, "int8"))       dataType += 0;
                else if(memStrCmp(argType, argTypeLen, "int16")) dataType += 1;
                else if(memStrCmp(argType, argTypeLen, "int32")) dataType += 2;
                else if(memStrCmp(argType, argTypeLen, "int64")) dataType += 3;
                else if(memStrCmp(argType, argTypeLen, "ptr")) dataType += 5;
                else CLI_logStatus(STATUS_FATAL, "unknown type '%.*s'", argTypeLen, argType);
                
                BLD_appendOpcodes(biteASM, &dataType, 1);
                BLD_appendOpcodes(biteASM, arg, argLen);
                BLD_appendOpcodes(biteASM, "\0", 1);

                restart:
                argType = (char*)getNextToken(parser, i, &argTypeLen);
                if(memStrCmp(argType, argTypeLen, ",")) goto restart;
                arg = (char*)getNextToken(parser, i, &argLen);
            }
            BLD_appendOpcodes(biteASM, "\1", 1);

            biteASM->openBracketCount++;

            // open bracket check
            //int bracketLen;
            //const char* bracket = getNextToken(parser, i, &bracketLen);
            //CLI_logStatus(STATUS_CMD, "%.*s", bracketLen, bracket);
            if(!memStrCmp(parser->filedata + parser->tokens[*i], parser->tokens[(*i)+1], "{"))
                CLI_logStatus(STATUS_FATAL, "{ expected");
        } break;   
        case 2: {   //* ===== return =====
            // [\x33][var name][\x0]
            int varNameLen;
            char* variableName = (char*)getNextToken(parser, i, &varNameLen);

            char* opcodes = calloc(varNameLen + 2, 1);
            opcodes[0] = RET;
            memcpy(opcodes+1, variableName, varNameLen);

            BLD_appendOpcodes(biteASM, opcodes, varNameLen + 2);
            free(opcodes);
        } break;
        case 3: {} break;   // asm
        case 4: {   //* ===== } =====
            if(biteASM->openBracketCount <= 0)
                CLI_logStatus(STATUS_FATAL, "} unexpected");
            
            char opcode[1] = {END};
            BLD_appendOpcodes(biteASM, opcode, 1);
            biteASM->openBracketCount--;
        } break;

        case 5: {} break;   // int8
        case 6: {} break;   // int16
        case 7: {} break;   // int32
        case 8: {} break;   // int64
        case 9: {} break;   // uint8
        case 10: {} break;  // uint16
        case 11: {} break;  // uint32
        case 12: {} break;  // uint64
        case 13: {} break;  // ptr

        case 14: {} break;  // if
        case 15: {} break;  // else
        case 16: {} break;  // for
        case 17: {} break;  // while
        case 18: {} break;  // break
        case 19: {} break;  // continue
    }
}

static const char* getNextToken(PARSER_CTX* parser, int* i, int* len){
    *len = parser->tokens[((*i) += 2) + 1];
    return parser->filedata + parser->tokens[*i];
}

// checks if non-terminating string is equal to a normal string, w/ extra checks
static _Bool memStrCmp(const void* mem, int memLen, const char* s){
    int sLen = strlen(s);
    if(sLen != memLen) return 0;
    return !memcmp(mem, s, sLen);
}