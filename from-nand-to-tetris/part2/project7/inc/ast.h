#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct ASTNode
{
    PTOKEN  token;
} ASTNODE, *PASTNODE;

typedef struct ASTNodeNumber
{
    PASTNODE astnode;
};

typedef struct ASTNodeStackOp
{
    PASTNODE astnode;
};


typedef struct Ast
{
    PASTNODE * ast_node;
    uint32_t size;
} AST, *PAST;


void parser_ast_add(PPARSER parser, PASTNODE astnode);
void parser_print_astnode(PASTNODE astnode);
void parser_parse(PPARSER parser);

#endif
