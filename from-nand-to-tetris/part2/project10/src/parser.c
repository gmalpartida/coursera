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

void parser_raiseError(PTOKEN token)
{
	printf("Error parsing program.\n");
    token_print(token);
    printf("\n");
    exit(EXIT_FAILURE);
}

void parser_parseTerm(PPARSER parser)
{

	return;
}

bool parser_type(PPARSER parser)
{
	bool raiseError = false;

	PTOKEN token = lexer_read(parser->lexer);

	if (KEYWORD == token->type && (!strcmp("int", token->text) || !strcmp("boolean", token->text) || !strcmp("char", token->text)))
	{
		token_print(token);
	}
	else
		raiseError = true;

	if (raiseError)
		parser_raiseError(token);

	return !raiseError;
}

void parser_classVarDec(PPARSER parser)
{
	// ( static | field ) type varName (, varName )*;
	bool raiseError = false;
	
	PTOKEN token = lexer_read(parser->lexer);

	if (parser_keyword(parser, "static") || parser_keyword(parser, "field"))
	{
		token_print(token);

		parser_type(parser);

		parser_identifier(parser);

		token = lexer_peek(parser->lexer);
		while ((SYMBOL == token->type) && !strcmp(",", token->text))
		{
			token = lexer_read(parser->lexer);
			token_print(token);

			parser_identifier(parser);

			token = lexer_peek(parser->lexer);
		}

	}
	else
		raiseError = true;

	if (raiseError)
		parser_raiseError(token);
}

void parser_parameterList(PPARSER parser)
{
	bool raiseError = false;

	PTOKEN token = lexer_peek(parser->lexer);

	while (KEYWORD == token->type)
	{

		parser_type(parser);
	
		parser_identifier(parser);

	}

	if (raiseError)
		parser_raiseError(token);
}

void parser_subroutineDec(PPARSER parser)
{
	// ( constructor | function | method ) (void | type) subroutineName ( parameterList ) subroutineBody
	
	bool raiseError = false;
	PTOKEN token = lexer_read(parser->lexer);

	token_print(token);

	token = lexer_peek(parser->lexer);

	if (KEYWORD == token->type)
	{
		token = lexer_read(parser->lexer);
		if (!strcmp("void", token->text))
		{
			token_print(token);

			token = lexer_peek(parser->lexer);

			if (IDENTIFIER == token->type)
			{
				token = lexer_read(parser->lexer);
				token_print(token);

				token = lexer_peek(parser->lexer);

				if (SYMBOL == token->type && !strcmp("(", token->text))
				{
					token = lexer_read(parser->lexer);
					token_print(token);
					parser_parameterList(parser);
					
					token = lexer_read(parser->lexer);
					if (SYMBOL == token->type && !strcmp(")", token->text))
						token_print(token);
					else
						raiseError = true;

				}
				else
					raiseError = true;

			}
			else
				raiseError = true;
		}
		else
			parser_type(parser);
	}
	else
		raiseError = true;

	if (raiseError)
		parser_raiseError(token);
}

bool parser_identifier(PPARSER parser)
{
	bool raiseError = false;
	PTOKEN token = lexer_read(parser->lexer);

	if (IDENTIFIER == token->type)
	{
		token_print(token);
	}
	else
		raiseError = true;

	if (raiseError)
		parser_raiseError(token);

	return !raiseError;
}

bool parser_keyword(PPARSER parser, char * keyword)
{
	bool raiseError = false;
	PTOKEN token = lexer_read(parser->lexer);

	if (KEYWORD == token->type && !strcmp(keyword, token->text))
	{
		token_print(token);
	}
	else
		raiseError = true;

	if (raiseError)
		parser_raiseError(token);

	return !raiseError;
}

bool parser_symbol(PPARSER parser, char * symbol)
{
	bool raiseError = false;

	PTOKEN token = lexer_read(parser->lexer);

	if (SYMBOL == token->type && !strcmp(symbol, token->text))
	{
		token_print(token);
	}
	else
		raiseError = true;

	if (raiseError)
		parser_raiseError(token);

	return !raiseError;
}

void parser_parseClass(PPARSER parser)
{
	bool raiseError = false;
	// class className { classVarDec* subroutineDec* }
	
	printf("<class>\n");

	// consume the keyword token "class"
	PTOKEN token = lexer_peek(parser->lexer);
	parser_keyword(parser, "class");

	parser_identifier(parser);

	parser_symbol(parser, "{");

	token = lexer_peek(parser->lexer);
	while (KEYWORD == token->type && (!strcmp(token->text, "static") || !strcmp(token->text, "field")))
	{
		parser_classVarDec(parser);
	}

	token = lexer_peek(parser->lexer);
	while (KEYWORD == token->type && (!strcmp(token->text, "constructor") || !strcmp(token->text, "function") || !strcmp(token->text, "method")))
	{
		parser_subroutineDec(parser);
	}

	parser_symbol(parser, "}");

	printf("</class>\n");


	if (raiseError)
		parser_raiseError(token);
}

void parser_parse(PPARSER parser)
{
	/*
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
		else if (KEYWORD == token->type)
		{
			if (!strcmp(token->text, "class"))
			{
				parser_parseClass(parser);
			}
		}
        else
        {
            token = lexer_read(parser->lexer);
			token_print(token);
			printf("\n");
       }
   }
*/

	parser_parseClass(parser);
}

