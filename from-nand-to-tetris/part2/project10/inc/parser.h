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
void parser_execute(PPARSER parser);
bool parser_keyword(PPARSER parser, char * keyword);
bool parser_symbol(PPARSER parser, char * symbol);
bool parser_identifier(PPARSER parser);
bool parser_type(PPARSER parser);
void parser_expression(PPARSER parser);

#endif
