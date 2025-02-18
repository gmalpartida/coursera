#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <stdint.h>
#include <stdbool.h>
#include "ast.h"

typedef struct Parser
{
    PLEXER lexer;
    PAST ast;
} PARSER, *PPARSER;

PPARSER parser_create(PLEXER lexer);
void parser_destroy(PPARSER parser);
void parser_print(PPARSER parser);
void parser_print_ast(PPARSER parser);
void parser_parse(PPARSER parser);

#endif
