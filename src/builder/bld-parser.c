// SPDX-FileCopyrightText: 2022 Gavin Distaso
// SPDX-License-Identifier: MIT License

#include "bld-parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ===== Format Defines ===== */

// these are delims for tokens
const char  *TOKEN_BREAK    = " \n\t\v\f;",
            *WHITESPACE     = " \n\t\v\f",
*OPPERATORS[] = {
    "+","-","/","*","%","//","**", "--", "++"       // Arithmetic
    "==", "!=", ">=", "<=", ">", "<",               // Relational
    "&&", "||", "!",                                // Logical 
    "&", "|", "~", "<<", ">>", "^"                  // binary
};
const int OPP_LEN = sizeof(OPPERATORS) / sizeof(char*);

const char  STRING          = '"',
          * LINE_COMMENT    = "!>",
          * BLOCK_START     = "!*",
          * BLOCK_END       = "*!";

// these are delims that are also put into there own token
// e.g. (';' is delim) hi;low -> ["hi", ";", "low"]
const char* SOLITARY        = "\n";
/* ===== Function Definitions ===== */

void BLD_freeParser(PARSER_CTX* ctx){
    if(ctx != NULL){
        free(ctx->filedata);
        free(ctx->tokens);
        ctx = (void*)(ctx->tokens = (void*)(ctx->filedata = NULL));
    }
}

bool BLD_readSource(PARSER_CTX* ctx, const char* filepath){
    FILE* fp = fopen(filepath, "r");
    if(fp == NULL) return false;

    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    ctx->filedata = calloc(len + 1, 1);
    fread(ctx->filedata, len, 1, fp);

    ctx->fileLength = len;

    fclose(fp);
    return true;
}

// internal function
static bool charInString(char c, const char* s);
static void appendToken(unsigned int* tokens, int* tokCount, int start, int end);

void BLD_tokenize(PARSER_CTX* ctx){
    ctx->tokens = malloc(0);
    int tokenStart = ctx->tokenCount = 0;

    for(int i = 0; i < ctx->fileLength; i++){
        char c = ctx->filedata[i];
        if(c == STRING){
            appendToken(ctx->tokens, &ctx->tokenCount, tokenStart, i-1);
            tokenStart = i;
            while(ctx->filedata[++i] != STRING);
            appendToken(ctx->tokens, &ctx->tokenCount, tokenStart, i);
            tokenStart = i+1;
        } else if(charInString(c, TOKEN_BREAK) || charInString(c, SOLITARY)){
            appendToken(ctx->tokens, &ctx->tokenCount, tokenStart, i-1);
            if(charInString(c, SOLITARY))
                appendToken(ctx->tokens, &ctx->tokenCount, i, i);
            tokenStart = i+1;
        }
    }
}

void BLD_prune(PARSER_CTX* ctx){
   
}

// internal function definitions

static bool charInString(char c, const char* s){
    for(int i = 0; i < strlen(s); i++)
        if(c == s[i]) return true;
    return false;
}

static void appendToken(unsigned int* tokens, int* tokCount, int start, int end){
    tokens = realloc(tokens, (++*tokCount) * 2 * sizeof(int));
    tokens[(*tokCount*2) - 2] = start;
    tokens[(*tokCount*2) - 1] = end;
}