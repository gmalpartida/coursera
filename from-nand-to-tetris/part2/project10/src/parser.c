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
	printf("Error near token ");
	token_print(token);
	printf("\n");
    exit(EXIT_FAILURE);
}

bool parser_isOp(PTOKEN token)
{
	return SYMBOL == token->type && (!strcmp(token->text, "+") || !strcmp(token->text, "-") || !strcmp(token->text, "*") ||
			!strcmp(token->text, "/") || !strcmp(token->text, "&") || !strcmp(token->text, "|") ||
			!strcmp(token->text, "<") || !strcmp(token->text, ">") || !strcmp(token->text, "="));
}

void parser_subroutineCall(PPARSER parser)
{
	// todo
}

void parser_term(PPARSER parser)
{
	bool raiseError = false;

	PTOKEN token = lexer_read(parser->lexer);

	if (INTCONSTANT == token->type)
		printf("<integerConstant>%s</integerConstant>", token->text);
	else if (STRCONSTANT == token->type)
		printf("<stringConstant>%s</stringConstant>", token->text);
	else if (KEYWORD == token->type && (!strcmp(token->text, "true") || !strcmp(token->text, "false") ||
				!strcmp(token->text, "null") || !strcmp(token->text, "this")))
		printf("<keyword>%s</keyword>", token->text);
	else if (IDENTIFIER == token->type)
	{
		PTOKEN lookAhead = lexer_peek(parser->lexer);
		if (SYMBOL == lookAhead->type && !strcmp(token->text, "["))
		{
			parser_symbol(parser, "[");
			parser_expression(parser);
			parser_symbol(parser, "]");
		}
		else if (SYMBOL == lookAhead->type && !strcmp(token->text, "("))
		{
			parser_subroutineCall(parser);
		}
		else
			parser_identifier(parser);
	}
	else if (SYMBOL == token->type)
	{
		if (!strcmp(token->text, "("))
		{
			parser_expression(parser);
			parser_symbol(parser, ")");
		}
		else if (!strcmp(token->text, "-") || !strcmp(token->text, "~"))
		{
			parser_symbol(parser, token->text);
		}
	}

	if (raiseError)
		parser_raiseError(token);
	
}

void parser_expression(PPARSER parser)
{
	parser_term(parser);

	PTOKEN token = lexer_peek(parser->lexer);

	while (parser_isOp(token))
	{
		token = lexer_read(parser->lexer);

		printf("<symbol>%s</symbol>", token->text);

		parser_term(parser);
	}

}

bool parser_type(PPARSER parser)
{
	bool raiseError = false;

	PTOKEN token = lexer_read(parser->lexer);

	if (KEYWORD == token->type && (!strcmp("int", token->text) || !strcmp("boolean", token->text) || !strcmp("char", token->text)))
		printf("<keyword>%s</keyword>", token->text);
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
	
	PTOKEN token = lexer_peek(parser->lexer);

	if (!strcmp(token->text, "static"))
		parser_keyword(parser, "static");
	else if (!strcmp(token->text, "field"))
		parser_keyword(parser, "field");
	else
		raiseError = true;

	if (!raiseError)
	{
		token = lexer_peek(parser->lexer);
		if (IDENTIFIER == token->type)
			parser_identifier(parser);
		else
			parser_type(parser);

		parser_identifier(parser);

		token = lexer_peek(parser->lexer);
		while ((SYMBOL == token->type) && !strcmp(",", token->text))
		{
			parser_symbol(parser, ",");

			parser_identifier(parser);

			token = lexer_peek(parser->lexer);
		}
		parser_symbol(parser, ";");
	}

	if (raiseError)
		parser_raiseError(token);
}

bool parser_isType(PTOKEN token)
{
	return ((KEYWORD == token->type && (!strcmp(token->text, "int") || !strcmp(token->text, "boolean") || !strcmp(token->text, "char"))) || (IDENTIFIER == token->type));
}

void parser_returnStatement(PPARSER parser)
{
	// return expression?

	parser_keyword(parser, "return");

	parser_expression(parser);

	parser_symbol(parser, ";");
}

void parser_letStatement(PPARSER parser)
{
	// let varName ( [ expression ] )? = expression ;
	parser_keyword(parser, "let");

	parser_identifier(parser);

	PTOKEN token = lexer_peek(parser->lexer);

	if (SYMBOL == token->type && !strcmp(token->text, "["))
	{
		parser_symbol(parser, "[");

		parser_expression(parser);

		parser_symbol(parser, "]");
	}

	parser_symbol(parser, "=");

	parser_expression(parser);

	parser_symbol(parser, ";");
}

void parser_parameterList(PPARSER parser)
{
	// ( ( type varName ) (, type varName)*)?
	
	bool raiseError = false;

	PTOKEN token = lexer_peek(parser->lexer);

	while (parser_isType(token))
	{

		parser_type(parser);
	
		parser_identifier(parser);

		token = lexer_peek(parser->lexer);

		if (token->type != SYMBOL || strcmp(token->text, ","))
			break;
		else
		{
			parser_symbol(parser, ",");
			token = lexer_peek(parser->lexer);
		}

	}

	if (raiseError)
		parser_raiseError(token);
}

void parser_varDec(PPARSER parser)
{
	// var type varName (, varName )* ;
	//
	bool raiseError = false;

	PTOKEN token = lexer_read(parser->lexer);

	parser_keyword(parser, "var");

	parser_type(parser);

	parser_identifier(parser);

	token = lexer_peek(parser->lexer);

	while (SYMBOL == token->type && !strcmp(token->text, ","))
	{
		parser_symbol(parser, ",");
		parser_identifier(parser);
		token = lexer_peek(parser->lexer);
	}

	parser_symbol(parser, ";");

	if (raiseError)
		parser_raiseError(token);
}

bool parser_isStatement(PPARSER parser)
{
	PTOKEN token = lexer_peek(parser->lexer);

	return (KEYWORD == token->type && (!strcmp(token->text, "let") || !strcmp(token->text, "while") ||
				!strcmp(token->text, "if") || !strcmp(token->text, "do") || !strcmp(token->text, "return")));
}

void parser_subroutineBody(PPARSER parser)
{
	// { varDec* statements}
	//
	bool raiseError = false;

	parser_symbol(parser, "{");

	PTOKEN token = lexer_peek(parser->lexer);

	while (KEYWORD == token->type && !strcmp(token->text, "var"))
	{
		parser_varDec(parser);
		token = lexer_peek(parser->lexer);
	}

	while (parser_isStatement(parser))
	{
		token = lexer_peek(parser->lexer);
		if (!strcmp(token->text, "return"))
			parser_returnStatement(parser);
		else if (!strcmp(token->text, "let"))
			parser_letStatement(parser);
	}

	parser_symbol(parser, "}");

	if (raiseError)
		parser_raiseError(token);
}

void parser_subroutineDec(PPARSER parser)
{
	// ( constructor | function | method ) (void | type) subroutineName ( parameterList ) subroutineBody
	
	bool raiseError = false;
	PTOKEN token = lexer_read(parser->lexer);

	raiseError = (token->type != KEYWORD || (strcmp(token->text, "constructor") && strcmp(token->text, "function") && strcmp(token->text, "method")));

	if (!raiseError)
	{
		printf("<keyword>%s</keyword>", token->text);

		token = lexer_peek(parser->lexer);

		if (KEYWORD == token->type && !strcmp(token->text, "void"))
			parser_keyword(parser, "void");
		else
			parser_type(parser);

		parser_identifier(parser);

		parser_symbol(parser, "(");

		parser_parameterList(parser);

		parser_symbol(parser, ")");

		parser_subroutineBody(parser);
	}
	if (raiseError)
		parser_raiseError(token);
}

bool parser_identifier(PPARSER parser)
{
	bool raiseError = false;
	PTOKEN token = lexer_read(parser->lexer);

	if (IDENTIFIER == token->type)
		printf("<identifier>%s</identifier>", token->text);
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
		printf("<keyword>%s</keyword>", token->text);
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
		printf("<symbol>%s</symbol>", token->text);
	else
		raiseError = true;

	if (raiseError)
		parser_raiseError(token);

	return !raiseError;
}

void parser_class(PPARSER parser)
{
	// class className { classVarDec* subroutineDec* }

	bool raiseError = false;
	
	printf("<class>\n");

	// consume the keyword token "class"
	parser_keyword(parser, "class");

	parser_identifier(parser);

	parser_symbol(parser, "{");

	PTOKEN token = lexer_peek(parser->lexer);
	while (KEYWORD == token->type && (!strcmp(token->text, "static") || !strcmp(token->text, "field")))
	{
		parser_classVarDec(parser);
		token = lexer_peek(parser->lexer);
	}

	token = lexer_peek(parser->lexer);
	while (KEYWORD == token->type && (!strcmp(token->text, "constructor") || !strcmp(token->text, "function") || !strcmp(token->text, "method")))
	{
		parser_subroutineDec(parser);
		token = lexer_peek(parser->lexer);
	}
	parser_symbol(parser, "}");

	printf("</class>\n");

	if (raiseError)
		parser_raiseError(token);
}

void parser_execute(PPARSER parser)
{
	parser_class(parser);
}

