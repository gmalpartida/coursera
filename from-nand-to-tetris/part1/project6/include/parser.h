#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef struct Symbol
{
    char * symbol;
    int value;
} SYMBOL, *PSYMBOL;


typedef struct Parser
{
    PLEXER lexer;
    SYMBOL symbol_table[256];
} PARSER, *PPARSER;


PPARSER parser_create(PLEXER lexer);
void parser_destroy(PPARSER parser);
void parser_build_symbol_table(PPARSER parser);

#endif
