#include "parser.h"
#include <stdlib.h>
#include "lexer.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

PPARSER parser_create(PLEXER lexer)
{
    PPARSER parser = (PPARSER)malloc(sizeof(PARSER));

    parser->lexer = lexer;

    scanner_reset(lexer->scanner);

    parser->ast = ast_create();

    return parser;
}

void parser_destroy(PPARSER parser)
{
    free(parser->lexer);\
    free(parser);
    parser = NULL;
}

void parser_print(PPARSER parser)
{
    ast_print(parser->ast);
}

void parser_parse(PPARSER parser)
{
    PTOKEN token = NULL;
    while (EOE != (token = lexer_peek(parser->lexer))->type)
    {
        if (ERROR == token->type)
        {
            printf("Error parsing program.\n");
            token_print(token);
            printf("\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            token = lexer_read(parser->lexer);
            if (token->type == STACK_OP)
            {
                PASTNODE astnode = astnode_create(token);
                
                astnode->operand1 = lexer_read(parser->lexer);
                astnode->operand2 = lexer_read(parser->lexer);
                ast_add(parser->ast, astnode);
            }
            else if (token->type == ARITHMETIC_OP)
            {
                PASTNODE astnode = astnode_create(token);
                
                ast_add(parser->ast, astnode);
            }
            else if (token->type == LOGICAL_OP)
            {
                PASTNODE astnode = astnode_create(token);
                
                ast_add(parser->ast, astnode);
            }
            else if (token->type == BRANCH_OP)
            {
                PASTNODE astnode = astnode_create(token);
                astnode->operand1 = lexer_read(parser->lexer);
                ast_add(parser->ast, astnode);
            }
            else if (token->type == FUNCTION_OP)
            {
                PASTNODE astnode = astnode_create(token);
                if (strcmp(token->text, "return"))
                {
                    astnode->operand1 = lexer_read(parser->lexer);
                
                    astnode->operand2 = lexer_read(parser->lexer);
                }
				astnode_print(astnode);
				ast_add(parser->ast, astnode);
            }
        }
   }

}

