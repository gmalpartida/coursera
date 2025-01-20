#ifndef AST_H
#define AST_H

#include <stdint.h>
#include <stdbool.h>
#include "token.h"

typedef struct ASTNode
{
    PTOKEN op;
    PTOKEN operand1;
    PTOKEN operand2;
  
} ASTNODE, *PASTNODE;

typedef struct Ast
{
    PASTNODE * astnode;
    uint32_t size;
} AST, *PAST;

PAST ast_create();
void ast_destroy(PAST ast);
void ast_print(PAST ast);
void ast_add(PAST ast, PASTNODE astnode);
void astnode_print(PASTNODE astnode);
PASTNODE astnode_create(PTOKEN op);

#endif
