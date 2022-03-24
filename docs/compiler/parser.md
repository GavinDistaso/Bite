# Parser

> The parser source code is in [parser.c](../../src/parser/parser.c)

## Contents

* [Prerequisites](#prerequisites)
    * [Parser CTX](#parser-ctx)

* [Tokenizer](#tokenizer)
    * [Process](#process)
        * [Strings](#strings)
        * [Separating Garbage](#separating-garbage)

* [Pruning](#pruning)

## Prerequisites

First im going to define some prerequisites used by the parser. 

There are 7 variables that define `tokens` and there different jobs to the parser. The different `tokens` are as follows, note some `tokens` are in multiple different groups.

| var name | description | type | value |
| - | - | - | - |
| TOKEN_BREAK | These are chars that the parser uses to end a meta-token (note: this token will be later split into multiple different tokens) | <nobr>`const char*`</nobr> | <nobr>`"[space]\n\t\v\f;,()"`</nobr> |
| NO_INFO_SKIP | These chars have no importance to the parser, so they can be skipped (unless in a string) | <nobr>`const char *`</nobr> | <nobr>`"[space]\n\t\v\f;"`</nobr> |
| TOKEN_SIGNIFIERS | These signify different key terms, example: `!>`, `++`, `^`. Now the order these are listed __do__ mater, because some signifiers share some chars. e.g `jim += bob` should be `jim` `+=` `bob` __not__ `jim` `+` `=` `bob` | <nobr>`const char **`</nobr> | See [TOKEN_SIGNIFIERS initial value](#tokensignifiers-initial-value) |

</br>

#### TOKEN_SIGNIFIERS initial value
```c
{
    "!>", "!*", "*!", // comments
    
    "->", // declare return type

    /* == operators == */
    // -- multi char
    "//","**", "--", "++",
    "==", "!=", ">=", "<=",
    "&&", "||",
    "<<", ">>",
    "+=", "-=", "/=", "*=", "%=","&=", "|=", "!=", "^=",
    // -- single char
    "+","-","/","*","%",
    ">", "<", "!", "=",
    "&", "|", "~", "^",

    /* misc */
    "\n", ",", // segmenting
    "(", ")", "{", "}" // grouping
}
``` 

### Parser CTX

`PARSER_CTX` (defined in [parser.h](../../src/parser/parser.h)) is a `struct` holding the unparsed `filedata` and the `tokens` array. The token array is stored as so: `[token 1 start][token 1 length] [token 2 start][token 2 length] ...`. This struct is used by functions as an interface with the main code ([main.c](../../src/main.c)).

# Tokenizer

The tokenizer source code is defined in [parser.c](../../src/parser/parser.c).

### Process

The tokenizer works by looping through every character in the input file. Then grouping some characters that are important to the interpretation of the file, and ignoring others. 

These groups of characters are appended to the `tokens` component of [PARSER_CTX](#parser-ctx), for later use. Also note `PARSER_CTX.tokenCount` is the number of tokens after tokenizing.

</br>

#### Strings

When looping through the file the first thing the compiler looks for is strings. String decelerations are single character start and stop points that define the bounds of the string, the start and stop have to be self similar. These characters are defined as `STRING` in [parser.c](../../src/parser/parser.c).
> note: strings can contain newlines

When a string is found it is appended to the tokens with the string deceleration. Example: ```Bob said "Hey Jim!" Jim replied with `Hello Bob`.``` The string tokens would be `"Hey Jim!"` and ``` `Hello Bob` ```.

> Note: all [No Info Skip](#prerequisites) chars after are skipped

</br>

#### Separating Garbage

If no strings are found the next step is to wait for a [Token Break (explained here)](#prerequisites). When a token break is found assume all the characters between the current character and the start of the current token (defined as `tokenStart`) are in token. Now we want to split the token by any [Token Signifier (explained here)](#prerequisites), but we also want to put the signifier in its own token. 

Heres an example, lets say we just got this token `f(x)=2x+1**3`, if `=`, `+`, and `**` are signifiers we will end up with `f(x)`, `=`, `2x`, `+`, `**`, and `3` as our new tokens.

After all that we append our new tokens.

> Note: all [No Info Skip](#prerequisites) chars after are skipped

# Pruning

All that pruning is as of 3/24/22 is a method of removing the comments after tokenization. Comments are in defined in [parser.c](../../src/parser/parser.c) as `LINE_COMMENT`, `BLOCK_START`, and `BLOCK_END`.

This is best shown with an example, lets say we have these tokens: `!*`, `oogady`, `boogady`, `\n`, `do`, `*!`, `\n`, `\n`, `!>`, `liner`, `liner`, `\n`, `some`, `other`, `stuff`. The pruner would look for the start of either comment, which in this case would be `!*` or `!>`. After ones found the pruner will remove all tokens after it until its respected closing came, which for `!*` would be `*!` and `!>` would be `\n` (because its a line comment).

So our result would be `some`, `other`, and `stuff` because everything else was in a comment.

> Note: pruner might be used for optimization in the future