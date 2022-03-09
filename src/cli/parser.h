// SPDX-FileCopyrightText: 2022 Gavin Distaso
// SPDX-License-Identifier: MIT License

#ifndef CLI_PARSER_H
#define CLI_PARSER_H

typedef struct CLI_CTX{
    char** argv;
    int argc;
} CLI_CTX;

// returns: (bool) if key is found
// NOTE: output is memory allocated, or NULL if key not found
_Bool CLI_getKeyPair(CLI_CTX* ctx, const char* key, char** output);

// returns: index of tag, if not found -1 is returned
int CLI_findTag(CLI_CTX* ctx, const char* tag);

// return: index of unknown tag, -1 if no unknowns are found
int CLI_findUnknownTags(CLI_CTX* ctx, const char** tags, const int taglen);

#endif // CLI_PARSER_H