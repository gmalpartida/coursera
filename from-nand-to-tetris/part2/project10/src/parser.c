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

//    parser->ast = ast_create();

    return parser;
}

void parser_destroy(PPARSER parser)
{
  //  ast_destroy(parser->ast);
    free(parser);
    parser = NULL;
}

void parser_print(PPARSER parser)
{
    //ast_print(parser->ast);
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
            if (token->type == KEYWORD)
            {
				printf("<keyword>");
				printf("%s", token->text);
				printf("</keyword>\n");
            }
            else if (token->type == SYMBOL)
            {
				printf("<symbol>");
				printf("%s", token->text);
				printf("</symbol>\n");
            }
            else if (token->type == INTCONSTANT)
            {
				printf("<integerConstant>");
				printf("%s", token->text);
				printf("</integerConstant>\n");
            }
            else if (token->type == STRCONSTANT)
            {
				printf("<stringConstant>");
				printf("%s", token->text);;
				printf("</stringConstant>\n");
            }
            else if (token->type == IDENTIFIER)
            {
				printf("<identifier>");
				printf("%s", token->text);
				printf("</identifier>\n");
            }
        }
   }

}

