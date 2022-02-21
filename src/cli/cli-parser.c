// SPDX-FileCopyrightText: 2022 Gavin Distaso
// SPDX-License-Identifier: MIT License

#include "cli-parser.h"

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

bool CLI_getKeyPair(CLI_CTX* ctx, const char* key, char** output){
    for(int i = 1; i < ctx->argc - 1; i++){
        int valLen = strlen(ctx->argv[i+1]);
        if(ctx->argv[i][0] == '-' && strcmp(ctx->argv[i], key) == 0){
            *output = calloc(valLen+1, 1);
            memcpy(*output, ctx->argv[i+1], valLen);
            return true;
        }
    }

    *output = NULL;
    return false;
}

int CLI_findTag(CLI_CTX* ctx, const char* tag){
    for(int i = 1; i < ctx->argc; i++)
        if(ctx->argv[i][0] == '-' && strcmp(ctx->argv[i], tag) == 0)
            return i;
    
    return -1;
}

int CLI_findUnknownTags(CLI_CTX* ctx, const char** tags, const int taglen){
    for(int i = 1; i < ctx->argc; i++){
        bool flag = ctx->argv[i][0] != '-';
        for(int j = 0; j < taglen & !flag; j++)
            flag = !strcmp(tags[j], ctx->argv[i]);
        if(!flag) return i;
    }
    return -1;
}
