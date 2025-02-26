#include "parser.h"
#include <stdlib.h>
#include "lexer.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define parser_raiseError(token) {printf("Syntax error in file %s, function %s at line %d: compiling token ", __FILE__, __FUNCTION__, __LINE__); token_print(token); printf("\n");	exit(EXIT_FAILURE);}

PPARSER parser_create(PLEXER lexer)
{
    PPARSER parser = (PPARSER)malloc(sizeof(PARSER));

    parser->lexer = lexer;

    scanner_reset(lexer->scanner);

    return parser;
}

void parser_destroy(PPARSER parser)
{
    free(parser);
    parser = NULL;
}

void parser_print(PPARSER parser)
{
	lexer_read(parser->lexer);
}

bool parser_isOp(PTOKEN token)
{
	return SYMBOL == token->type && (!strcmp(token->text, "+") || !strcmp(token->text, "-") || !strcmp(token->text, "*") ||
			!strcmp(token->text, "/") || !strcmp(token->text, "&") || !strcmp(token->text, "|") ||
			!strcmp(token->text, "<") || !strcmp(token->text, ">") || !strcmp(token->text, "="));
}

void parser_expressionList(PPARSER parser)
{
	// ( expression (, expression)* )?
	
	fprintf(parser->fptr, "<expressionList>\n");

	PTOKEN token = lexer_peek(parser->lexer);

	if (SYMBOL != token->type || strcmp(token->text, ")"))
	{
		parser_expression(parser);

		token = lexer_peek(parser->lexer);

		while (SYMBOL == token->type && !strcmp(token->text, ","))
		{
			parser_symbol(parser, ",");

			parser_expression(parser);

			token = lexer_peek(parser->lexer);
		}
	}
	fprintf(parser->fptr, "</expressionList>\n");
}

void parser_subroutineCall(PPARSER parser)
{
	// subroutineName ( expressionList ) | (className | varName).subroutineName ( expressionList )
	//
	parser_identifier(parser);

	PTOKEN token = lexer_peek(parser->lexer);

	if (SYMBOL == token->type && !strcmp(token->text, "("))
	{
		parser_symbol(parser, "(");

		parser_expressionList(parser);

		parser_symbol(parser, ")");
	}
	else
	{
		parser_symbol(parser, ".");

		parser_identifier(parser);

		parser_symbol(parser, "(");

		parser_expressionList(parser);

		parser_symbol(parser, ")");
	}
}

void parser_integerConstant(PPARSER parser)
{
	PTOKEN token = lexer_read(parser->lexer);
	fprintf(parser->fptr, "<integerConstant>%s</integerConstant>\n", token->text);
}

void parser_stringConstant(PPARSER parser)
{
	PTOKEN token = lexer_read(parser->lexer);
	fprintf(parser->fptr, "<stringConstant>%s</stringConstant>\n", token->text);
}

void parser_term(PPARSER parser)
{
	// integerConstant | stringConstant | keywordConstant | varName | varName[ expression ] | 
	// subroutineCall | ( expression ) | unaryOp term
	bool raiseError = false;

	fprintf(parser->fptr, "<term>\n");

	PTOKEN token = lexer_peek(parser->lexer);

	if (INTCONSTANT == token->type)
		parser_integerConstant(parser);
	else if (STRCONSTANT == token->type)
		parser_stringConstant(parser);
	else if (KEYWORD == token->type && (!strcmp(token->text, "true") || !strcmp(token->text, "false") ||
				!strcmp(token->text, "null") || !strcmp(token->text, "this")))
		parser_keyword(parser, token->text);
	else if (IDENTIFIER == token->type)
	{
		PTOKEN lookAhead = lexer_peek2(parser->lexer);
		if (SYMBOL == lookAhead->type && !strcmp(lookAhead->text, "["))
		{
			parser_identifier(parser);
			parser_symbol(parser, "[");
			parser_expression(parser);
			parser_symbol(parser, "]");
		}
		else if (SYMBOL == lookAhead->type && (!strcmp(lookAhead->text, "(") || !strcmp(lookAhead->text, ".")))
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
			parser_symbol(parser, "(");

			parser_expression(parser);

			parser_symbol(parser, ")");
		}
		else if (!strcmp(token->text, "-") || !strcmp(token->text, "~"))
		{
			parser_symbol(parser, token->text);

			parser_term(parser);
		}
	}

	fprintf(parser->fptr, "</term>\n");

	if (raiseError)
		parser_raiseError(token);
	
}

void parser_expression(PPARSER parser)
{
	// term ( op term )*
	fprintf(parser->fptr, "<expression>\n");

	parser_term(parser);

	PTOKEN token = lexer_peek(parser->lexer);

	while (parser_isOp(token))
	{
		parser_symbol(parser, token->text);
		parser_term(parser);
		token = lexer_peek(parser->lexer);
	}

	fprintf(parser->fptr, "</expression>\n");
}

bool parser_type(PPARSER parser)
{
	bool raiseError = false;

	PTOKEN token = lexer_peek(parser->lexer);

	if (KEYWORD == token->type && (!strcmp("int", token->text) || !strcmp("boolean", token->text) || !strcmp("char", token->text)))
		parser_keyword(parser, token->text);
	else if (IDENTIFIER == token->type)
		parser_identifier(parser);
	else
		raiseError = true;

	if (raiseError)
		parser_raiseError(token);

	return !raiseError;
}

void parser_classVarDec(PPARSER parser)
{
	// ( static | field ) type varName (, varName )*;
	

	fprintf(parser->fptr, "<classVarDec>\n");

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

	fprintf(parser->fptr, "</classVarDec>\n");
}

bool parser_isType(PTOKEN token)
{
	return ((KEYWORD == token->type && (!strcmp(token->text, "int") || !strcmp(token->text, "boolean") || !strcmp(token->text, "char"))) || (IDENTIFIER == token->type));
}

void parser_returnStatement(PPARSER parser)
{
	// return expression?

	fprintf(parser->fptr, "<returnStatement>\n");

	parser_keyword(parser, "return");

	PTOKEN token = lexer_peek(parser->lexer);

	if (SYMBOL != token->type || strcmp(token->text, ";"))
	{
		parser_expression(parser);
	}
	parser_symbol(parser, ";");

	fprintf(parser->fptr, "</returnStatement>\n");
}

void parser_letStatement(PPARSER parser)
{
	// let varName ( [ expression ] )? = expression ;
	
	fprintf(parser->fptr, "<letStatement>\n");

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

	fprintf(parser->fptr, "</letStatement>\n");
}

void parser_whileStatement(PPARSER parser)
{
	// while ( expression ) { statements }
	
	fprintf(parser->fptr, "<whileStatement>\n");

	parser_keyword(parser, "while");

	parser_symbol(parser, "(");

	parser_expression(parser);

	parser_symbol(parser, ")");

	parser_symbol(parser, "{");

	parser_statements(parser);

	parser_symbol(parser, "}");

	fprintf(parser->fptr, "</whileStatement>\n");
}

void parser_doStatement(PPARSER parser)
{
	// do subroutineCall ;
	
	fprintf(parser->fptr, "<doStatement>\n");

	parser_keyword(parser, "do");

	parser_subroutineCall(parser);

	parser_symbol(parser, ";");

	fprintf(parser->fptr, "</doStatement>\n");
}

void parser_ifStatement(PPARSER parser)
{
	// if ( expression ) { statements } ( else { statements } )?

	fprintf(parser->fptr, "<ifStatement>\n");

	parser_keyword(parser, "if");

	parser_symbol(parser, "(");

	parser_expression(parser);

	parser_symbol(parser, ")");

	parser_symbol(parser, "{");

	parser_statements(parser);

	parser_symbol(parser, "}");

	PTOKEN token = lexer_peek(parser->lexer);

	if (KEYWORD == token->type && (!strcmp(token->text, "else")))
	{
		parser_keyword(parser, "else");

		parser_symbol(parser, "{");

		parser_statements(parser);

		parser_symbol(parser, "}");
	}

	fprintf(parser->fptr, "</ifStatement>\n");
}

void parser_statements(PPARSER parser)
{
	// statement*
	
	fprintf(parser->fptr, "<statements>\n");

	PTOKEN token = NULL;
	while (parser_isStatement(parser))
	{

		token = lexer_peek(parser->lexer);
		if (!strcmp(token->text, "return"))
			parser_returnStatement(parser);
		else if (!strcmp(token->text, "let"))
			parser_letStatement(parser);
		else if (!strcmp(token->text, "while"))
			parser_whileStatement(parser);
		else if (!strcmp(token->text, "do"))
			parser_doStatement(parser);
		else if (!strcmp(token->text, "if"))
			parser_ifStatement(parser);

	}

	fprintf(parser->fptr, "</statements>\n");
}

void parser_parameterList(PPARSER parser)
{
	// ( ( type varName ) (, type varName)*)?
	
	bool raiseError = false;

	fprintf(parser->fptr, "<parameterList>\n");

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

	fprintf(parser->fptr, "</parameterList>\n");

	if (raiseError)
		parser_raiseError(token);
}

void parser_varDec(PPARSER parser)
{
	// var type varName (, varName )* ;
	//
	bool raiseError = false;

	fprintf(parser->fptr, "<varDec>\n");

	PTOKEN token = lexer_peek(parser->lexer);

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

	fprintf(parser->fptr, "</varDec>\n");

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

	fprintf(parser->fptr, "<subroutineBody>\n");

	parser_symbol(parser, "{");

	PTOKEN token = lexer_peek(parser->lexer);

	while (KEYWORD == token->type && !strcmp(token->text, "var"))
	{
		parser_varDec(parser);
		token = lexer_peek(parser->lexer);
	}

	parser_statements(parser);

	parser_symbol(parser, "}");

	fprintf(parser->fptr, "</subroutineBody>\n");

	if (raiseError)
		parser_raiseError(token);
}

void parser_subroutineDec(PPARSER parser)
{
	// ( constructor | function | method ) (void | type) subroutineName ( parameterList ) subroutineBody
	

	fprintf(parser->fptr, "<subroutineDec>\n");
	bool raiseError = false;
	PTOKEN token = lexer_peek(parser->lexer);

	raiseError = (token->type != KEYWORD || (strcmp(token->text, "constructor") && strcmp(token->text, "function") && strcmp(token->text, "method")));

	if (!raiseError)
	{
		parser_keyword(parser, token->text);

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

	fprintf(parser->fptr, "</subroutineDec>\n");

}

bool parser_identifier(PPARSER parser)
{
	bool raiseError = false;
	PTOKEN token = lexer_read(parser->lexer);

	if (IDENTIFIER == token->type)
		fprintf(parser->fptr, "<identifier>%s</identifier>\n", token->text);
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
		fprintf(parser->fptr, "<keyword>%s</keyword>\n", token->text);
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
		fprintf(parser->fptr, "<symbol>");
		if (!strcmp(token->text, "<"))
			fprintf(parser->fptr, "&lt;");
		else if (!strcmp(token->text, ">"))
			fprintf(parser->fptr, "&gt;");
		else if (!strcmp(token->text, "\""))
			fprintf(parser->fptr, "&quot;");
		else if (!strcmp(token->text, "&"))
			fprintf(parser->fptr, "&amp;");
		else
			fprintf(parser->fptr, "%s", token->text);

		fprintf(parser->fptr, "</symbol>\n");
	}
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
	
	fprintf(parser->fptr, "<class>\n");

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

	fprintf(parser->fptr, "</class>\n");

	if (raiseError)
		parser_raiseError(token);
}

void parser_execute(PPARSER parser, char * out_filename)
{
	parser->out_filename = out_filename;
	parser->fptr = fopen(out_filename, "w");;

	if (parser->fptr)
	{
		parser_class(parser);

		fclose(parser->fptr);
	}
	else
	{
		printf("Unable to open file %s\n", out_filename);
		exit(EXIT_FAILURE);
	}
}

