// SPDX-FileCopyrightText: 2022 Gavin Distaso
// SPDX-License-Identifier: MIT License

#include "bld-lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

const char* WHITESPACE = " \t\r\n\v\f";

void BLD_freeLexer(LEXER_CTX* ctx){
    if(ctx != NULL){
        free(ctx->filedata);
        ctx = (void*)(ctx->filedata = NULL);
    }
}

bool BLD_readSource(LEXER_CTX* ctx, const char* filepath){
    FILE* fp = fopen(filepath, "r");
    if(fp == NULL) return false;

    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    ctx->filedata = calloc(len + 1, 1);

    fclose(fp);
    return true;
}