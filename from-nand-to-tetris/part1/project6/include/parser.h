#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "symbol_table.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct AInstruction
{
    uint16_t value;
} AINSTRUCTION, *PAINSTRUCTION;

typedef struct CInstruction
{
  char * dest;
  char * comp;
  char * jump;
} CINSTRUCTION, *PCINSTRUCTION;

typedef struct ASTNode
{
    PAINSTRUCTION A_instruction;
    PCINSTRUCTION C_instruction;
} ASTNODE, *PASTNODE;

typedef struct Ast
{
    PASTNODE * ast_node;
    uint32_t size;
} AST, *PAST;

typedef struct Parser
{
    PLEXER lexer;
    PSYMBOL_TABLE symbol_table;
    PAST ast;
} PARSER, *PPARSER;

PPARSER parser_create(PLEXER lexer);
void parser_destroy(PPARSER parser);
void parser_build_symbol_table(PPARSER parser);
PASTNODE parser_parse_A_instruction(PPARSER parser, bool process_variables);
PASTNODE parser_parse_C_instruction(PPARSER parser);
void parser_print(PPARSER parser);
void parser_parse_label(PPARSER parser);
void parser_ast_add(PPARSER parser, PASTNODE astnode);
void parser_print_astnode(PASTNODE astnode);
void parser_parse(PPARSER parser);

#endif
