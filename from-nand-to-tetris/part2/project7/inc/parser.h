#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "symbol_table.h"
#include <stdint.h>
#include <stdbool.h>

ypedef struct Parser
{
    PLEXER lexer;
    PSYMBOL_TABLE symbol_table;
    PAST ast;
} PARSER, *PPARSER;

PPARSER parser_create(PLEXER lexer);
void parser_destroy(PPARSER parser);
void parser_print(PPARSER parser);

#endif
