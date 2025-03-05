#include "parser.h"
#include <stdlib.h>
#include "lexer.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#define parser_raiseError(token) {printf("Syntax error in file %s, function %s at line %d: compiling token ", __FILE__, __FUNCTION__, __LINE__); token_print(token); printf("\n");	exit(EXIT_FAILURE);}

char * duplicate_text(char * text)
{
	char * new_text = (char*)malloc(sizeof(char) * strlen(text) + 1);
	strcpy(new_text, text);
	return new_text;
}

PPARSER parser_create(PLEXER lexer)
{
    PPARSER parser = (PPARSER)malloc(sizeof(PARSER));

    parser->lexer = lexer;

	parser->class_symbol_table = symbol_table_create();

	parser->function_symbol_table = symbol_table_create();

	parser->var_local_index = parser->var_argument_index = parser->class_this_index = parser->class_static_index = 0;

    scanner_reset(lexer->scanner);

    return parser;
}

void parser_destroy(PPARSER parser)
{
	symbol_table_destroy(parser->class_symbol_table);
	symbol_table_destroy(parser->function_symbol_table);
    free(parser);
    parser = NULL;
}

void parser_print(PPARSER parser)
{
	printf("Class Symbol Table:\n");
	symbol_table_print(parser->class_symbol_table);
	printf("Function Symbol Table:\n");
	symbol_table_print(parser->function_symbol_table);
}

bool parser_isOp(PTOKEN token)
{
	return SYMBOL == token->type && (!strcmp(token->text, "+") || !strcmp(token->text, "-") || !strcmp(token->text, "*") ||
			!strcmp(token->text, "/") || !strcmp(token->text, "&") || !strcmp(token->text, "|") ||
			!strcmp(token->text, "<") || !strcmp(token->text, ">") || !strcmp(token->text, "="));
}

void parser_expressionList(PPARSER parser, uint8_t tab_count)
{
	// ( expression (, expression)* )?
	
	fprintf(parser->fptr, "%s<expressionList>\n", parser_makeTabs(tab_count));

	PTOKEN token = lexer_peek(parser->lexer);

	if (SYMBOL != token->type || strcmp(token->text, ")"))
	{
		parser_expression(parser, tab_count + 1);

		token = lexer_peek(parser->lexer);

		while (SYMBOL == token->type && !strcmp(token->text, ","))
		{
			parser_symbol(parser, ",", tab_count + 1);

			parser_expression(parser, tab_count+1);

			token = lexer_peek(parser->lexer);
		}
	}
	fprintf(parser->fptr, "%s</expressionList>\n", parser_makeTabs(tab_count));
}

void parser_subroutineCall(PPARSER parser, uint8_t tab_count)
{
	// subroutineName ( expressionList ) | (className | varName).subroutineName ( expressionList )
	//
	parser_identifier(parser, tab_count+1);

	PTOKEN token = lexer_peek(parser->lexer);

	if (SYMBOL == token->type && !strcmp(token->text, "("))
	{
		parser_symbol(parser, "(", tab_count+1);

		parser_expressionList(parser, tab_count+1);

		parser_symbol(parser, ")", tab_count+1);
	}
	else
	{
		parser_symbol(parser, ".", tab_count+1);

		parser_identifier(parser, tab_count+1);

		parser_symbol(parser, "(", tab_count+1);

		parser_expressionList(parser, tab_count+1);

		parser_symbol(parser, ")", tab_count+1);
	}
}

void parser_integerConstant(PPARSER parser, uint8_t tab_count)
{
	PTOKEN token = lexer_read(parser->lexer);
	fprintf(parser->fptr, "%s<integerConstant>%s</integerConstant>\n", parser_makeTabs(tab_count), token->text);
}

void parser_stringConstant(PPARSER parser, uint8_t tab_count)
{
	PTOKEN token = lexer_read(parser->lexer);
	fprintf(parser->fptr, "%s<stringConstant>%s</stringConstant>\n", parser_makeTabs(tab_count), token->text);
}

void parser_term(PPARSER parser, uint8_t tab_count)
{
	// integerConstant | stringConstant | keywordConstant | varName | varName[ expression ] | 
	// subroutineCall | ( expression ) | unaryOp term
	bool raiseError = false;

	fprintf(parser->fptr, "%s<term>\n", parser_makeTabs(tab_count));

	PTOKEN token = lexer_peek(parser->lexer);

	if (INTCONSTANT == token->type)
		parser_integerConstant(parser, tab_count+1);
	else if (STRCONSTANT == token->type)
		parser_stringConstant(parser, tab_count+1);
	else if (KEYWORD == token->type && (!strcmp(token->text, "true") || !strcmp(token->text, "false") ||
				!strcmp(token->text, "null") || !strcmp(token->text, "this")))
		parser_keyword(parser, token->text, tab_count+1);
	else if (IDENTIFIER == token->type)
	{
		PTOKEN lookAhead = lexer_peek2(parser->lexer);
		if (SYMBOL == lookAhead->type && !strcmp(lookAhead->text, "["))
		{
			parser_identifier(parser, tab_count+1);
			parser_symbol(parser, "[", tab_count+1);
			parser_expression(parser, tab_count+1);
			parser_symbol(parser, "]", tab_count+1);
		}
		else if (SYMBOL == lookAhead->type && (!strcmp(lookAhead->text, "(") || !strcmp(lookAhead->text, ".")))
		{
			parser_subroutineCall(parser, tab_count);
		}
		else
			parser_identifier(parser, tab_count+1);
	}
	else if (SYMBOL == token->type)
	{
		if (!strcmp(token->text, "("))
		{
			parser_symbol(parser, "(", tab_count+1);

			parser_expression(parser, tab_count+1);

			parser_symbol(parser, ")", tab_count+1);
		}
		else if (!strcmp(token->text, "-") || !strcmp(token->text, "~"))
		{
			parser_symbol(parser, token->text, tab_count+1);

			parser_term(parser, tab_count+1);
		}
	}
	else
		raiseError = true;

	fprintf(parser->fptr, "%s</term>\n", parser_makeTabs(tab_count));

	if (raiseError)
		parser_raiseError(token);
	
}

void parser_expression(PPARSER parser, uint8_t tab_count)
{
	// term ( op term )*
	fprintf(parser->fptr, "%s<expression>\n", parser_makeTabs(tab_count));

	parser_term(parser, tab_count+1);

	PTOKEN token = lexer_peek(parser->lexer);

	while (parser_isOp(token))
	{
		parser_symbol(parser, token->text, tab_count+1);
		parser_term(parser, tab_count+1);
		token = lexer_peek(parser->lexer);
	}

	fprintf(parser->fptr, "%s</expression>\n", parser_makeTabs(tab_count));
}

bool parser_type(PPARSER parser, uint8_t tab_count)
{
	bool raiseError = false;

	PTOKEN token = lexer_peek(parser->lexer);

	if (KEYWORD == token->type && (!strcmp("int", token->text) || !strcmp("boolean", token->text) || !strcmp("char", token->text)))
		parser_keyword(parser, token->text, tab_count);
	else if (IDENTIFIER == token->type)
		parser_identifier(parser, tab_count);
	else
		raiseError = true;

	if (raiseError)
		parser_raiseError(token);

	return !raiseError;
}

void parser_classVarDec(PPARSER parser, uint8_t tab_count)
{
	// ( static | field ) type varName (, varName )*;
	
	fprintf(parser->fptr, "%s<classVarDec>\n", parser_makeTabs(tab_count));

	bool raiseError = false;

	PSYMBOL_REC symbol_rec = (PSYMBOL_REC)malloc(sizeof(SYMBOL_REC));		

	PTOKEN token = lexer_peek(parser->lexer);
		
	// store kind of symbol
	if (!strcmp(token->text, "static"))
	{
		symbol_rec->kind = STATIC;
		symbol_rec->nbr = parser->class_static_index++;
		parser_keyword(parser, "static", tab_count+1);
	}
	else if (!strcmp(token->text, "field"))
	{
		symbol_rec->kind = THIS;
		symbol_rec->nbr = parser->class_this_index++;
		parser_keyword(parser, "field", tab_count+1);
	}
	else
		raiseError = true;

	if (!raiseError)
	{
		token = lexer_peek(parser->lexer);
		
		// type of symbol
		symbol_rec->type = duplicate_text(token->text);
	
		if (IDENTIFIER == token->type)
			parser_identifier(parser, tab_count+1);
		else
			parser_type(parser, tab_count+1);

		// name of symbol
		token = lexer_peek(parser->lexer);
		symbol_rec->name = duplicate_text(token->text);

		parser_identifier(parser, tab_count+1);

		symbol_table_add( parser->class_symbol_table, symbol_rec);

		token = lexer_peek(parser->lexer);
		while ((SYMBOL == token->type) && !strcmp(",", token->text))
		{
			parser_symbol(parser, ",", tab_count+1);

			// add next symbol, type and kind are the same
			// different name and increase index

			PSYMBOL_REC new_symbol_rec = (PSYMBOL_REC)malloc(sizeof(SYMBOL_REC));
			new_symbol_rec->type = duplicate_text(symbol_rec->type);
			new_symbol_rec->kind = symbol_rec->kind;

			token = lexer_peek(parser->lexer);
			new_symbol_rec->name = duplicate_text(token->text);
			
			if (new_symbol_rec->kind == STATIC)
				new_symbol_rec->nbr = parser->class_static_index++;
			else
				new_symbol_rec->nbr = parser->class_this_index++;

			symbol_table_add( parser->class_symbol_table, new_symbol_rec);

			parser_identifier(parser, tab_count+1);

			token = lexer_peek(parser->lexer);
		}
		parser_symbol(parser, ";", tab_count+1);
	}

	if (raiseError)
		parser_raiseError(token);

	fprintf(parser->fptr, "%s</classVarDec>\n", parser_makeTabs(tab_count));
}

bool parser_isType(PTOKEN token)
{
	return ((KEYWORD == token->type && (!strcmp(token->text, "int") || !strcmp(token->text, "boolean") || !strcmp(token->text, "char"))) || (IDENTIFIER == token->type));
}

void parser_returnStatement(PPARSER parser, uint8_t tab_count)
{
	// return expression?

	fprintf(parser->fptr, "%s<returnStatement>\n", parser_makeTabs(tab_count));

	parser_keyword(parser, "return", tab_count+1);

	PTOKEN token = lexer_peek(parser->lexer);

	if (SYMBOL != token->type || strcmp(token->text, ";"))
	{
		parser_expression(parser, tab_count+1);
	}
	parser_symbol(parser, ";", tab_count+1);

	fprintf(parser->fptr, "%s</returnStatement>\n", parser_makeTabs(tab_count));
}

void parser_letStatement(PPARSER parser, uint8_t tab_count)
{
	// let varName ( [ expression ] )? = expression ;
	
	fprintf(parser->fptr, "%s<letStatement>\n", parser_makeTabs(tab_count));

	parser_keyword(parser, "let", tab_count+1);

	parser_identifier(parser, tab_count+1);

	PTOKEN token = lexer_peek(parser->lexer);

	if (SYMBOL == token->type && !strcmp(token->text, "["))
	{
		parser_symbol(parser, "[", tab_count+1);

		parser_expression(parser, tab_count+1);

		parser_symbol(parser, "]", tab_count+1);
	}

	parser_symbol(parser, "=", tab_count+1);

	parser_expression(parser, tab_count+1);

	parser_symbol(parser, ";", tab_count+1);

	fprintf(parser->fptr, "%s</letStatement>\n", parser_makeTabs(tab_count));
}

void parser_whileStatement(PPARSER parser, uint8_t tab_count)
{
	// while ( expression ) { statements }
	
	fprintf(parser->fptr, "%s<whileStatement>\n", parser_makeTabs(tab_count));

	parser_keyword(parser, "while", tab_count+1);

	parser_symbol(parser, "(", tab_count+1);

	parser_expression(parser, tab_count+1);

	parser_symbol(parser, ")", tab_count+1);

	parser_symbol(parser, "{", tab_count+1);

	parser_statements(parser, tab_count+1);

	parser_symbol(parser, "}", tab_count+1);

	fprintf(parser->fptr, "%s</whileStatement>\n", parser_makeTabs(tab_count));
}

void parser_doStatement(PPARSER parser, uint8_t tab_count)
{
	// do subroutineCall ;
	
	fprintf(parser->fptr, "%s<doStatement>\n", parser_makeTabs(tab_count));

	parser_keyword(parser, "do", tab_count+1);

	parser_subroutineCall(parser, tab_count+1);

	parser_symbol(parser, ";", tab_count+1);

	fprintf(parser->fptr, "%s</doStatement>\n", parser_makeTabs(tab_count));
}

void parser_ifStatement(PPARSER parser, uint8_t tab_count)
{
	// if ( expression ) { statements } ( else { statements } )?

	fprintf(parser->fptr, "%s<ifStatement>\n", parser_makeTabs(tab_count));

	parser_keyword(parser, "if", tab_count+1);

	parser_symbol(parser, "(", tab_count+1);

	parser_expression(parser, tab_count+1);

	parser_symbol(parser, ")", tab_count+1);

	parser_symbol(parser, "{", tab_count+1);

	parser_statements(parser, tab_count+1);

	parser_symbol(parser, "}", tab_count+1);

	PTOKEN token = lexer_peek(parser->lexer);

	if (KEYWORD == token->type && (!strcmp(token->text, "else")))
	{
		parser_keyword(parser, "else", tab_count+1);

		parser_symbol(parser, "{", tab_count+1);

		parser_statements(parser, tab_count+1);

		parser_symbol(parser, "}", tab_count+1);
	}

	fprintf(parser->fptr, "%s</ifStatement>\n", parser_makeTabs(tab_count));
}

void parser_statements(PPARSER parser, uint8_t tab_count)
{
	// statement*
	
	fprintf(parser->fptr, "%s<statements>\n", parser_makeTabs(tab_count));

	PTOKEN token = NULL;
	while (parser_isStatement(parser))
	{

		token = lexer_peek(parser->lexer);
		if (!strcmp(token->text, "return"))
			parser_returnStatement(parser, tab_count+1);
		else if (!strcmp(token->text, "let"))
			parser_letStatement(parser, tab_count+1);
		else if (!strcmp(token->text, "while"))
			parser_whileStatement(parser, tab_count+1);
		else if (!strcmp(token->text, "do"))
			parser_doStatement(parser, tab_count+1);
		else if (!strcmp(token->text, "if"))
			parser_ifStatement(parser, tab_count+1);

	}

	fprintf(parser->fptr, "%s</statements>\n", parser_makeTabs(tab_count));
}

void parser_parameterList(PPARSER parser, uint8_t tab_count)
{
	// ( ( type varName ) (, type varName)*)?
	
	bool raiseError = false;

	fprintf(parser->fptr, "%s<parameterList>\n", parser_makeTabs(tab_count));

	PTOKEN token = lexer_peek(parser->lexer);

	while (parser_isType(token))
	{
		PSYMBOL_REC symbol_rec = (PSYMBOL_REC)malloc(sizeof(SYMBOL_REC));

		symbol_rec->kind = ARGUMENT;

		symbol_rec->type = duplicate_text(token->text);

		parser_type(parser, tab_count+1);
	
		token = lexer_peek(parser->lexer);

		symbol_rec->name = duplicate_text(token->text);

		parser_identifier(parser, tab_count+1);

		symbol_rec->nbr = parser->var_argument_index++;

		symbol_table_add(parser->function_symbol_table, symbol_rec);

		token = lexer_peek(parser->lexer);

		if (token->type != SYMBOL || strcmp(token->text, ","))
			break;
		else
		{
			parser_symbol(parser, ",", tab_count+1);
			token = lexer_peek(parser->lexer);
		}

	}

	fprintf(parser->fptr, "%s</parameterList>\n", parser_makeTabs(tab_count));

	if (raiseError)
		parser_raiseError(token);
}

void parser_varDec(PPARSER parser, uint8_t tab_count)
{
	// var type varName (, varName )* ;
	//
	bool raiseError = false;

	PSYMBOL_REC symbol_rec = (PSYMBOL_REC)malloc(sizeof(SYMBOL_REC));
	symbol_rec->kind = LOCAL;

	fprintf(parser->fptr, "%s<varDec>\n", parser_makeTabs(tab_count));

	PTOKEN token = lexer_peek(parser->lexer);

	parser_keyword(parser, "var", tab_count+1);

	token = lexer_peek(parser->lexer);
	symbol_rec->type = duplicate_text(token->text);
	parser_type(parser, tab_count+1);

	token = lexer_peek(parser->lexer);
	symbol_rec->name = duplicate_text(token->text);
	parser_identifier(parser, tab_count+1);

	symbol_rec->nbr = parser->var_local_index++;

	symbol_table_add( parser->function_symbol_table, symbol_rec);

	token = lexer_peek(parser->lexer);

	while (SYMBOL == token->type && !strcmp(token->text, ","))
	{
		PSYMBOL_REC new_symbol_rec = (PSYMBOL_REC)malloc(sizeof(SYMBOL_REC));
		new_symbol_rec->type = duplicate_text(symbol_rec->type);
		new_symbol_rec->kind = symbol_rec->kind;

		parser_symbol(parser, ",", tab_count+1);
		
		token = lexer_peek(parser->lexer);
		new_symbol_rec->name = duplicate_text(token->text);
		new_symbol_rec->nbr = parser->var_local_index++;

		symbol_table_add(parser->function_symbol_table, new_symbol_rec);

		parser_identifier(parser, tab_count+1);
		token = lexer_peek(parser->lexer);
	}

	parser_symbol(parser, ";", tab_count+1);

	fprintf(parser->fptr, "%s</varDec>\n", parser_makeTabs(tab_count));

	if (raiseError)
		parser_raiseError(token);
}

bool parser_isStatement(PPARSER parser)
{
	PTOKEN token = lexer_peek(parser->lexer);

	return (KEYWORD == token->type && (!strcmp(token->text, "let") || !strcmp(token->text, "while") ||
				!strcmp(token->text, "if") || !strcmp(token->text, "do") || !strcmp(token->text, "return")));
}

void parser_subroutineBody(PPARSER parser, uint8_t tab_count)
{
	// { varDec* statements}
	//
	bool raiseError = false;

	fprintf(parser->fptr, "%s<subroutineBody>\n", parser_makeTabs(tab_count));

	parser_symbol(parser, "{", tab_count+1);

	PTOKEN token = lexer_peek(parser->lexer);

	while (KEYWORD == token->type && !strcmp(token->text, "var"))
	{
		parser_varDec(parser, tab_count+1);
		token = lexer_peek(parser->lexer);
	}

	parser_statements(parser, tab_count+1);

	parser_symbol(parser, "}", tab_count+1);

	fprintf(parser->fptr, "%s</subroutineBody>\n", parser_makeTabs(tab_count));

	if (raiseError)
		parser_raiseError(token);
}

void parser_subroutineDec(PPARSER parser, uint8_t tab_count)
{
	// ( constructor | function | method ) (void | type) subroutineName ( parameterList ) subroutineBody
	

	fprintf(parser->fptr, "%s<subroutineDec>\n", parser_makeTabs(tab_count));
	bool raiseError = false;
	PTOKEN token = lexer_peek(parser->lexer);

	raiseError = (token->type != KEYWORD || (strcmp(token->text, "constructor") && strcmp(token->text, "function") && strcmp(token->text, "method")));

	if (!raiseError)
	{
		parser_keyword(parser, token->text, tab_count+1);

		token = lexer_peek(parser->lexer);

		if (KEYWORD == token->type && !strcmp(token->text, "void"))
			parser_keyword(parser, "void", tab_count+1);
		else
			parser_type(parser, tab_count+1);

		parser_identifier(parser, tab_count+1);

		parser_symbol(parser, "(", tab_count+1);

		parser_parameterList(parser, tab_count+1);

		parser_symbol(parser, ")", tab_count+1);

		parser_subroutineBody(parser, tab_count+1);
	}
	
	if (raiseError)
		parser_raiseError(token);

	fprintf(parser->fptr, "%s</subroutineDec>\n", parser_makeTabs(tab_count));

}

bool parser_identifier(PPARSER parser, uint8_t tab_count)
{
	bool raiseError = false;
	PTOKEN token = lexer_read(parser->lexer);

	if (IDENTIFIER == token->type)
		fprintf(parser->fptr, "%s<identifier>%s</identifier>\n", parser_makeTabs(tab_count), token->text);
	else
		raiseError = true;

	if (raiseError)
		parser_raiseError(token);

	return !raiseError;
}

bool parser_keyword(PPARSER parser, char * keyword, uint8_t tab_count)
{
	bool raiseError = false;
	PTOKEN token = lexer_read(parser->lexer);

	if (KEYWORD == token->type && !strcmp(keyword, token->text))
		fprintf(parser->fptr, "%s<keyword>%s</keyword>\n", parser_makeTabs(tab_count), token->text);
	else
		raiseError = true;

	if (raiseError)
		parser_raiseError(token);

	return !raiseError;
}

bool parser_symbol(PPARSER parser, char * symbol, uint8_t tab_count)
{
	bool raiseError = false;

	PTOKEN token = lexer_read(parser->lexer);

	if (SYMBOL == token->type && !strcmp(symbol, token->text))
	{
		fprintf(parser->fptr, "%s<symbol>", parser_makeTabs(tab_count));
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

void parser_class(PPARSER parser, uint8_t tab_count)
{
	// class className { classVarDec* subroutineDec* }

	bool raiseError = false;
	
	fprintf(parser->fptr, "%s<class>\n", parser_makeTabs(tab_count));

	// consume the keyword token "class"
	parser_keyword(parser, "class", tab_count+1);

	parser_identifier(parser, tab_count+1);

	parser_symbol(parser, "{", tab_count+1);

	PTOKEN token = lexer_peek(parser->lexer);
	while (KEYWORD == token->type && (!strcmp(token->text, "static") || !strcmp(token->text, "field")))
	{
		parser_classVarDec(parser, tab_count+1);
		token = lexer_peek(parser->lexer);
	}

	token = lexer_peek(parser->lexer);
	while (KEYWORD == token->type && (!strcmp(token->text, "constructor") || !strcmp(token->text, "function") || !strcmp(token->text, "method")))
	{
		parser_subroutineDec(parser, tab_count+1);
		token = lexer_peek(parser->lexer);
	}
	parser_symbol(parser, "}", tab_count+1);

	fprintf(parser->fptr, "%s</class>\n", parser_makeTabs(tab_count));

	if (raiseError)
		parser_raiseError(token);
}

char * parser_makeTabs(uint8_t tab_count)
{
	int i = 0;
	char * tabs = (char*)malloc(sizeof(char) * tab_count + 1);
	for (i = 0; i < tab_count; i++)
	{
		tabs[i] = '\t';
	}
	tabs[i] = '\0';
	return tabs;
}

void parser_execute(PPARSER parser, char * out_filename)
{
	parser->out_filename = out_filename;
	parser->fptr = fopen(out_filename, "w");

	if (parser->fptr)
	{
		parser_class(parser, 0);

		fclose(parser->fptr);
	}
	else
	{
		printf("Unable to open file %s\n", out_filename);
		exit(EXIT_FAILURE);
	}

	parser_print(parser);
}

