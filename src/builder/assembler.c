// SPDX-FileCopyrightText: 2022 Gavin Distaso
// SPDX-License-Identifier: MIT License

#include "assembler.h"
#include <parser/parser.h>
#include <cli/logger.h>
#include <biteVM/instructions.h>

#include <stdlib.h>
#include <string.h>

#include <stdint.h>

void BLD_appendOpcodes(BAB_CTX* biteASM, const char* opcodes, int len){
    biteASM->opcodes = realloc(biteASM->opcodes, biteASM->opcodeLen + len);
    memcpy(biteASM->opcodes + biteASM->opcodeLen, opcodes, len);
    biteASM->opcodeLen += len;
}

void BLD_appendInt(BAB_CTX* biteASM, void* x, int size){
    biteASM->opcodes = realloc(biteASM->opcodes, biteASM->opcodeLen + size);
    for(int i = 0; i < size; i++){
        biteASM->opcodes[biteASM->opcodeLen + i] = 
            ((*(int*)x)>>((size-i-1) * 8)) & 0xff;
    }
    biteASM->opcodeLen += size;
}

static inline char assembleToken(PARSER_CTX* parser, int i);
int BLD_getRegisterIndex(const char* reg);

#define tknStrCmp(str) !strncmp(parser->filedata + start, str, len)

void BLD_assemble(BAB_CTX* biteASM, const char* asmCode, int len){
    PARSER_CTX parser;
    parser.filedata = calloc(len+1, 1);
    memcpy(parser.filedata, asmCode, len);
    parser.fileLength = len;

    PRS_tokenize(&parser);

    PRS_prune(&parser);

    for(int i = 0; i < parser.tokenCount * 2; i+=2){
        int start   = parser.tokens[i],
            len     = parser.tokens[i+1];

        char opcode = assembleToken(&parser, i);
        BLD_appendOpcodes(biteASM, &opcode, 1);

        // special cases that handle the next value

        if      (opcode == NUM || opcode == MEM || opcode == REG){
            i+=2;
            start   = parser.tokens[i];
            len     = parser.tokens[i+1];

            if(opcode == NUM || opcode == MEM){
                char s[len+1]; memset(s, 0, len+1);
                memcpy(s, parser.filedata + start, len);
                int v = atoi(s);

                BLD_appendInt(biteASM, &v, sizeof(int));
            }else{
                char* tokStr = parser.filedata + start;
                
                int regIndex = BLD_getRegisterIndex(tokStr);

                CLI_logStatus(STATUS_LOG, "%i", regIndex);

                BLD_appendInt(biteASM, &regIndex, sizeof(char));
            }
        }
    }

    PRS_freeParser(&parser);
}

static inline char assembleToken(PARSER_CTX* parser, int i){
    int start   = parser->tokens[i],
        len     = parser->tokens[i+1];

    if      (tknStrCmp("ADC"))      return ADC;
    else if (tknStrCmp("ADD"))      return ADD;
    else if (tknStrCmp("SUB"))      return SUB;
    else if (tknStrCmp("MUL"))      return MUL;
    else if (tknStrCmp("DIV"))      return DIV;
    else if (tknStrCmp("POW"))      return POW;
    else if (tknStrCmp("MOD"))      return MOD;

    else if (tknStrCmp("AND"))      return AND;
    else if (tknStrCmp("OR"))       return OR;
    else if (tknStrCmp("NOT"))      return NOT;
    else if (tknStrCmp("XOR"))      return XOR;
    else if (tknStrCmp("SHL"))      return SHL;
    else if (tknStrCmp("SHR"))      return SHR;
    else if (tknStrCmp("ROL"))      return ROL;
    else if (tknStrCmp("ROR"))      return ROR;

    else if (tknStrCmp("MOV"))      return MOV;
    else if (tknStrCmp("JMP"))      return JMP;
    else if (tknStrCmp("CALL"))     return CALL;
    else if (tknStrCmp("RET"))      return RET;

    else if (tknStrCmp("CMP"))      return CMP;
    else if (tknStrCmp("FUNC"))     return FUNC;
    else if (tknStrCmp("END"))      return END;

    else if (tknStrCmp("LABEL"))    return LABEL;
    else if (tknStrCmp("GOTO"))     return GOTO;

    else if (tknStrCmp("JME"))      return JME;
    else if (tknStrCmp("JML"))      return JML;
    else if (tknStrCmp("JMG"))      return JMG;
    else if (tknStrCmp("CAE"))      return CAE;
    else if (tknStrCmp("CAL"))      return CAL;
    else if (tknStrCmp("CAG"))      return CAG;

    else if (tknStrCmp("SYS"))      return SYS;
    else if (tknStrCmp("INT"))      return INT;
    else if (tknStrCmp("PUSH"))     return PUSH;
    else if (tknStrCmp("POP"))      return POP;
    else if (tknStrCmp("VAR2REG"))  return VAR2REG;
    
    else if (tknStrCmp("REG"))      return REG;
    else if (tknStrCmp("NUM"))      return NUM;
    else if (tknStrCmp("MEM"))      return MEM;

    else if(len == 1) return parser->filedata[start];

    else CLI_logStatus(
            STATUS_FATAL, "Unknown instruction '%.*s'", 
            len, parser->filedata + start);
}

int BLD_getRegisterIndex(const char* reg){
    char type = reg[1];
    char s[3]; memset(s, 0, 3);
    memcpy(s, reg+2, 2);

    int num     = atoi(s),
        offset  = (num == 64 ? 0 : (num == 32 ? 1 : (num == 16 ? 2 : 3)));

    char base = offset;
    if      (type == 'A') base += 10;
    else if (type == 'S') base += 20;
    else if (type == 'B') base += 30;
    else if (type == 'D') base += 40;
    else if (type == 'X') base += 50;
    else return -1;

    return base;
}
