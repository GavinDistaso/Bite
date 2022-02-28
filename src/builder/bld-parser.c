// SPDX-FileCopyrightText: 2022 Gavin Distaso
// SPDX-License-Identifier: MIT License

#include "bld-parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ===== Format Defines ===== */
const char* WHITESPACE      = " \t\r\n\v\f",

*OPPERATORS[] = {
    "+","-","/","*","%","//","**", "--", "++"       // Arithmetic
    "==", "!=", ">=", "<=", ">", "<",               // Relational
    "&&", "||", "!",                                // Logical 
    "&", "|", "~", "<<", ">>", "^"                  // binary
};
const int OPP_LEN = sizeof(OPPERATORS) / sizeof(char*);

const char* STRING          = "\"\"", // [start][end]
          * END_STATEMENTS  = ";",
          * LINE_COMMENT    = "!>",
          * BLOCK_START     = "!*",
          * BLOCK_END       = "*!";
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

/* Tokenizing & related */

static bool charInString(char c, const char* s);
static bool stringInArray(const char* s, const char** arr, const int arrLen);
static void appendToken(int* tokens, int* elemCount, int* start, int end);
static bool skip(PARSER_CTX* ctx, int* curIndex, const char* start, const char* end);

void BLD_tokenize(PARSER_CTX* ctx){
    int elemCount = 0;
    int* tokens = malloc(0);

    // split into segments of spaces and new lines 
    // newlines put a \n char in a seperate token
    int start = 0;
    for(int i = 0; i < ctx->fileLength; i++){
        char c = ctx->filedata[i];
        if(c == '\n'){
            appendToken(tokens, &elemCount, &start, i-1);
            appendToken(tokens, &elemCount, &start, i);
        }else if(charInString(c, WHITESPACE)){
            appendToken(tokens, &elemCount, &start, i-1);
            for(int j = i; j < ctx->fileLength; j++)
                if(charInString(ctx->filedata[i], WHITESPACE))
                    start = ++i;
        }
    }

    ctx->tokenCount = elemCount >> 1;
    ctx->tokens = tokens;
}

void BLD_prune(PARSER_CTX* ctx){
    int elemCount = 0;
    int* tokens = malloc(0);

    for(int i = 0; i < ctx->tokenCount * 2; i+=2){
        int start   = ctx->tokens[i],
            end     = ctx->tokens[i+1],
            len     = end-start + 1;
        
        bool act = 
            skip(ctx, &i, LINE_COMMENT, "\n") &
            skip(ctx, &i, BLOCK_START, BLOCK_END);
        
        if(act)
            appendToken(tokens, &elemCount, &start, end);
        
    }

    // replace old tokens
    free(ctx->tokens);

    ctx->tokenCount = elemCount >> 1;
    ctx->tokens = tokens;
}

/* ===== internal functions ===== */

static bool charInString(char c, const char* s){
    for(int i = 0; i < strlen(s); i++)
        if(s[i] == c) return true;
    return false;
}

static bool stringInArray(const char* s, const char** arr, const int arrLen){
    for(int i = 0; i < arrLen; i++)
        if(!strcmp(s, arr[i])) return true;
    return false;
}

static void appendToken(int* tokens, int* elemCount, int* start, int end){
    tokens = realloc(tokens, (*elemCount+=2) * sizeof(int));
    tokens[*elemCount-2] = *start;
    tokens[*elemCount-1] = end;
    *start = end+1;
}

static void skipUntil(PARSER_CTX* ctx, int* startIndex, const char* end){
    for(int j = *startIndex; j < ctx->tokenCount * 2; j+=2){
        if(memcmp(ctx->filedata + ctx->tokens[j], end, strlen(end)) == 0){
            *startIndex = j;
            break;
        }
    }
}

static bool memStrCmp(const char* memStart, const char* s){
    return !memcmp(memStart, s, strlen(s));
}

static bool skip(PARSER_CTX* ctx, int* curIndex, const char* start, const char* end){
    if(memStrCmp(ctx->filedata + ctx->tokens[*curIndex], start))
        skipUntil(ctx, curIndex, end);
    else
        return 1;
    return 0;
}