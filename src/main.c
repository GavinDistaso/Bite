// SPDX-FileCopyrightText: 2022 Gavin Distaso
// SPDX-License-Identifier: MIT License

#define UNICODE
#define _UNICODE 

#include <main.h>

#include <cli/parser.h>
#include <cli/logger.h>

#include <parser/parser.h>

#include <stdbool.h>

/*
* The command line arguments are as follows
*                                   [Description]
* --- FLAGS ---
* `-I`                       -->    Sets to interperet mode
* --- KEYS ---
* `-o [relative filepath]`   -->    specifies output filepath (required)
* `-c [language]`            -->    Sets to convert mode for specific language
* `-i` [filepath]            -->    specifies input filepath (required)
*/
const char* TAGS[] = {"-I", "-o", "-c", "-i"};
const int TAGS_LEN = sizeof(TAGS) / sizeof(char*);

static inline void retriveBuildInfo(CLI_CTX*,SETTINGS_CTX*,char**,char**);

int main(int argc, char** argv){
    CLI_CTX cli = {argv, argc};
    SETTINGS_CTX settings = SETTINGS_CTX();

    CLI_rawPrint("===== Bite Lang Compiler =====\n");

    // check for unknown tags
    int tagI;
    if((tagI = CLI_findUnknownTags(&cli, TAGS, TAGS_LEN)) != -1)
        CLI_logStatus(STATUS_FATAL, "unknown tag '%s'", argv[tagI]);
    
    char* outputFile, *inputFile;
    
    retriveBuildInfo(&cli, &settings, &outputFile, &inputFile);
    
    /* ===== Tokenize File ===== */

    PARSER_CTX parser;
    if(!PRS_readSource(&parser, inputFile))
        CLI_logStatus(STATUS_FATAL, "input file not found ['%s']", inputFile);

    PRS_tokenize(&parser);

    PRS_prune(&parser);

    for(int i = 0; i < parser.tokenCount * 2; i+=2){
        int start   = parser.tokens[i],
            len     = parser.tokens[i+1];
        CLI_logStatus(STATUS_LOG, "|%.*s|", len, parser.filedata + start);
    }

    PRS_freeParser(&parser);
}

static inline void retriveBuildInfo(
    CLI_CTX* cli, 
    SETTINGS_CTX* settings,
    char** outputFile,
    char** inputFile
){
    // get build mode

    bool 
        interpret = CLI_findTag(cli, "-I") != -1,
        convert = CLI_findTag(cli, "-c") != -1,
        compile = 1 & !interpret & !convert;

    if(interpret && convert){
        CLI_logStatus(STATUS_FATAL, "can only convert OR interpret");
    } else if(interpret && !!(settings->buildMode = 1)){
        CLI_logStatus(STATUS_LOG, "setting build mode to interpret");
    } else if(convert && !!(settings->buildMode = 2)){
        CLI_logStatus(STATUS_FATAL, "convert isnt ready yet!");
    } else if(compile)
        CLI_logStatus(STATUS_FATAL, "compile isnt ready yet!");

    // get build file locations

    if(!CLI_getKeyPair(cli, "-o", outputFile) && !interpret)
        CLI_logStatus(STATUS_FATAL, "output file not specified! try `-o [filepath]`");

    if(!CLI_getKeyPair(cli, "-i", inputFile))
        CLI_logStatus(STATUS_FATAL, "input file not specified! try `-i [filepath]`");
}