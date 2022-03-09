// SPDX-FileCopyrightText: 2022 Gavin Distaso
// SPDX-License-Identifier: MIT License

#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ===== Format Defines ===== */

// these are delims for tokens
const char  *TOKEN_BREAK    = " \n\t\v\f;,()",
            *NO_INFO_SKIP   = " \n\t\v\f;",

// keywords that are important to the parser to keep individual
*TOKEN_SIGNIFIERS[] = {
    /* == comments == */
    "!>", "!*", "*!",
    
    /* == opperators == */
    // -- multi char
    "//","**", "--", "++",
    "==", "!=", ">=", "<=",
    "&&", "||",
    "<<", ">>",
    "+=", "-=", "/=", "*=", "%=","&=", "|=", "!=", "^=",
    // -- single char
    "+","-","/","*","%",
    ">", "<", "!", "=",
    "&", "|", "~", "^"

    // /* misc */
    "\n", ",", // segmenting

    "(", ")", "{", "}" // grouping
};
const int TOK_SIG_LEN = sizeof(TOKEN_SIGNIFIERS) / sizeof(char*);

const char* STRING          = "\"`",
          * LINE_COMMENT    = "!>",
          * BLOCK_START     = "!*",
          * BLOCK_END       = "*!";

/* ===== Function Definitions ===== */

void PRS_freeParser(PARSER_CTX* ctx){
    if(ctx != NULL){
        free(ctx->filedata);
        free(ctx->tokens);
        ctx = (void*)(ctx->tokens = (void*)(ctx->filedata = NULL));
    }
}

bool PRS_readSource(PARSER_CTX* ctx, const char* filepath){
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
static void skip(PARSER_CTX* ctx, int* index, int* tokenStart, const char* whitelist);

static bool memStrCmp(void* mem, int memLen, const char* s);

void PRS_tokenize(PARSER_CTX* ctx){
    ctx->tokens = malloc(0);
    int tokenStart = ctx->tokenCount = 0;

    for(int i = 0; i < ctx->fileLength; i++){
        char c = ctx->filedata[i];
        if(charInString(c, STRING)){
            appendToken(ctx->tokens, &ctx->tokenCount, tokenStart, i-1);
            tokenStart = i;
            while(!charInString(ctx->filedata[++i], STRING));
            appendToken(ctx->tokens, &ctx->tokenCount, tokenStart, i);
            skip(ctx, &i, &tokenStart, NO_INFO_SKIP);
        } else if(charInString(c, TOKEN_BREAK)){
            // append token to tokens, and remove all following useless chars
            appendToken(ctx->tokens, &ctx->tokenCount, tokenStart, i-1);
            if(charInString(c, "\n"))
                appendToken(ctx->tokens, &ctx->tokenCount, i, i);
            skip(ctx, &i, &tokenStart, NO_INFO_SKIP);
        }
    }
    appendToken(ctx->tokens, &ctx->tokenCount, tokenStart, ctx->fileLength-1);
}

void PRS_prune(PARSER_CTX* ctx){
    unsigned int* newTokens = malloc(0);
    int newTokenCount = 0;

    // 0 = not in comment, 1 = in line, 2 = in block
    int inComment = 0; 

    for(int i = 0; i < ctx->tokenCount * 2; i+=2){
        int start   = ctx->tokens[i],
            len     = ctx->tokens[i+1];
        
        /* comments */
        if(!inComment){
            inComment = 
                memStrCmp(ctx->filedata + start, len, LINE_COMMENT) |
                memStrCmp(ctx->filedata + start, len, BLOCK_START) * 2;
        }
        if(inComment){
            if(
                (inComment == 1 && memStrCmp(ctx->filedata + start, len, "\n")) ||
                (inComment == 2 && memStrCmp(ctx->filedata + start, len, BLOCK_END))
            )   inComment = 0;
            continue;
        }

        /* newlines */
        if(memStrCmp(ctx->filedata + start, len, "\n")) continue;

        appendToken(newTokens, &newTokenCount, start, start+len-1);
    }

    // update current tokens
    free(ctx->tokens);
    ctx->tokens = newTokens;
    ctx->tokenCount = newTokenCount;
}

// internal function definitions

static bool charInString(char c, const char* s){
    for(int i = 0; i < strlen(s); i++)
        if(c == s[i]) return true;
    return false;
}

static void appendToken(unsigned int* tokens, int* tokCount, int start, int end){
    if(end < start) return;
    tokens = realloc(tokens, (++*tokCount) * 2 * sizeof(int));
    tokens[(*tokCount*2) - 2] = start;
    tokens[(*tokCount*2) - 1] = end-start+1;
}

static void skip(PARSER_CTX* ctx, int* index, int* tokenStart, const char* whitelist){
    while(charInString(ctx->filedata[++(*index)], whitelist));
    *tokenStart = (*index)--;
}


static bool memStrCmp(void* mem, int memLen, const char* s){
    int sLen    = strlen(s);
    if(sLen != memLen) return false;
    return !memcmp(mem, s, sLen);
}