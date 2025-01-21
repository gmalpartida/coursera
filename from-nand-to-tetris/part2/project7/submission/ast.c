#include "ast.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


PAST ast_create()
{
    PAST ast = (PAST)malloc(sizeof(AST));
    ast->astnode = (PASTNODE*)malloc(sizeof(PASTNODE) * 1024);
    ast->size = 0;
    return ast;
}

void ast_destroy(PAST ast)
{
    free(ast->astnode);
    free(ast);
    ast = NULL;
}

void ast_print(PAST ast)
{
    for (uint16_t i = 0; i < ast->size; i++)
    {
        astnode_print(ast->astnode[i]);
    }
}

void ast_add(PAST ast, PASTNODE astnode)
{
    ast->astnode[ast->size++] = astnode;
}

PASTNODE astnode_create(PTOKEN op)
{
    PASTNODE astnode = (PASTNODE)malloc(sizeof(ASTNODE));
    astnode->op = op;
    astnode->operand1 = NULL;
    astnode->operand2 = NULL;
    return astnode;
}

void astnode_print(PASTNODE astnode)
{
    if (astnode->op)
        token_print(astnode->op);
    if (astnode->operand1)
        token_print(astnode->operand1);
    if (astnode->operand2)
        token_print(astnode->operand2);
    printf("\n");
}


