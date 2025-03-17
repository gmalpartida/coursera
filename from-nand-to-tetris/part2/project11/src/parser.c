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

	parser->class_name = NULL;

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

PSYMBOL_REC parser_find_symbol(PPARSER parser, PTOKEN token)
{
	PSYMBOL_REC symbol_to_find = symbol_table_find(parser->function_symbol_table, token->text);
	if (NULL == symbol_to_find)
	{
		symbol_to_find = symbol_table_find(parser->class_symbol_table, token->text);
	}
	return symbol_to_find;
}

char * parser_unique_label(char * prefix)
{
	static uint16_t unique_label_index = 0;

	char * label = (char*)malloc(sizeof(char) * 255 + 1);

	sprintf(label, "%s%d", prefix, unique_label_index++);

	return label;
}

bool parser_isOp(PTOKEN token)
{
	return SYMBOL == token->type && (!strcmp(token->text, "+") || !strcmp(token->text, "-") || !strcmp(token->text, "*") ||
			!strcmp(token->text, "/") || !strcmp(token->text, "&") || !strcmp(token->text, "|") ||
			!strcmp(token->text, "<") || !strcmp(token->text, ">") || !strcmp(token->text, "="));
}

uint8_t parser_expressionList(PPARSER parser, uint8_t tab_count)
{
	// ( expression (, expression)* )?
	
	uint8_t arg_count = 0;

	//fprintf(parser->fptr, "%s<expressionList>\n", parser_makeTabs(tab_count));

	PTOKEN token = lexer_peek(parser->lexer);

	if (SYMBOL != token->type || strcmp(token->text, ")"))
	{
		arg_count++;
		parser_expression(parser, tab_count + 1);

		token = lexer_peek(parser->lexer);

		while (SYMBOL == token->type && !strcmp(token->text, ","))
		{
			token = lexer_read(parser->lexer);
			token_destroy(token);
			//parser_symbol(parser, ",", tab_count + 1);

			arg_count++;
			parser_expression(parser, tab_count+1);

			token = lexer_peek(parser->lexer);
		}
	}
	//fprintf(parser->fptr, "%s</expressionList>\n", parser_makeTabs(tab_count));

	return arg_count;
}

void parser_subroutineCall(PPARSER parser, uint8_t tab_count)
{
	// subroutineName ( expressionList ) | (className | varName).subroutineName ( expressionList )
	//
	uint8_t arg_count = 0;

	PTOKEN token_class_or_function = lexer_read(parser->lexer);
	char * class_or_subroutine_name = duplicate_text(token_class_or_function->text);

	PTOKEN token = lexer_peek(parser->lexer);

	if (SYMBOL == token->type && !strcmp(token->text, "("))
	{
		token = lexer_read(parser->lexer);
		token_destroy(token);
		//parser_symbol(parser, "(", tab_count+1);

		fprintf(parser->fptr, "push pointer 0\n");
		arg_count += parser_expressionList(parser, tab_count+1);

		token = lexer_read(parser->lexer);
		token_destroy(token);
		//parser_symbol(parser, ")", tab_count+1);

		fprintf(parser->fptr, "call %s.%s %d\n", parser->class_name, class_or_subroutine_name, arg_count+1);
	}
	else
	{
		token = lexer_read(parser->lexer);
		token_destroy(token);
		//parser_symbol(parser, ".", tab_count+1);

		token = lexer_read(parser->lexer);
		char * subroutine_name = duplicate_text(token->text);
		token_destroy(token);
		//parser_identifier(parser, tab_count+1);
		//
		token = lexer_read(parser->lexer);
		token_destroy(token);
		//parser_symbol(parser, "(", tab_count+1);
		PSYMBOL_REC symbol_rec = parser_find_symbol(parser, token_class_or_function);

		if (symbol_rec)
		{
			fprintf(parser->fptr, "push %s %d\n", symbol_kind_desc( symbol_rec->kind), symbol_rec->nbr);
			arg_count++;
			free(class_or_subroutine_name);
			class_or_subroutine_name = duplicate_text(symbol_rec->type);
		}

		arg_count += parser_expressionList(parser, tab_count+1);

		token = lexer_read(parser->lexer);
		token_destroy(token);
		//parser_symbol(parser, ")", tab_count+1);
		//
		fprintf(parser->fptr, "call %s.%s %d\n", class_or_subroutine_name, subroutine_name, arg_count);

		free(subroutine_name);
	}
	free(class_or_subroutine_name);
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

	//fprintf(parser->fptr, "%s<term>\n", parser_makeTabs(tab_count));

	PTOKEN token = lexer_peek(parser->lexer);

	if (INTCONSTANT == token->type)
	{
		token = lexer_read(parser->lexer);
		fprintf(parser->fptr, "push constant %s\n", token->text);
		//parser_integerConstant(parser, tab_count+1);
	}
	else if (STRCONSTANT == token->type)
	{
		token = lexer_read(parser->lexer);
		uint8_t len = strlen(token->text);

		fprintf(parser->fptr, "push constant %d\n", len);

		fprintf(parser->fptr, "call String.new 1\n");

		for (uint8_t i = 0; i < len; i++)
		{
			fprintf(parser->fptr, "push constant %d\n", token->text[i]);
			fprintf(parser->fptr, "call String.appendChar 2\n");
		}

		token_destroy(token);
		//parser_stringConstant(parser, tab_count+1);
	}
	else if (KEYWORD == token->type && (!strcmp(token->text, "true") || !strcmp(token->text, "false") ||
				!strcmp(token->text, "null") || !strcmp(token->text, "this")))
	{
		//parser_keyword(parser, token->text, tab_count+1);
		token = lexer_read(parser->lexer);
		if (!strcmp(token->text, "true"))
			fprintf(parser->fptr, "push constant 1\nneg\n");
		else if (!strcmp(token->text, "this"))
			fprintf(parser->fptr, "push pointer 0\n");
		else
			fprintf(parser->fptr, "push constant 0\n");
	}
	else if (IDENTIFIER == token->type)
	{
		PTOKEN lookAhead = lexer_peek2(parser->lexer);
		if (SYMBOL == lookAhead->type && !strcmp(lookAhead->text, "["))
		{
			token = lexer_read(parser->lexer);
			PSYMBOL_REC symbol_rec = parser_find_symbol(parser, token);
			if (symbol_rec)
			{
				fprintf(parser->fptr, "push %s %d\n", symbol_kind_desc(symbol_rec->kind), symbol_rec->nbr);
			}
			else
				parser_raiseError();
			token_destroy(token);
			//parser_identifier(parser, tab_count+1);

			token = lexer_read(parser->lexer);
			token_destroy(token);
			//parser_symbol(parser, "[", tab_count+1);
			
			parser_expression(parser, tab_count+1);

			fprintf(parser->fptr, "add\n");
			fprintf(parser->fptr, "pop pointer 1\n");
			fprintf(parser->fptr, "push that 0\n");

			token = lexer_read(parser->lexer);
			token_destroy(token);
			//parser_symbol(parser, "]", tab_count+1);
		}
		else if (SYMBOL == lookAhead->type && (!strcmp(lookAhead->text, "(") || !strcmp(lookAhead->text, ".")))
		{
			parser_subroutineCall(parser, tab_count);
		}
		else
		{
			token = lexer_read(parser->lexer);
			PSYMBOL_REC symbol_rec = parser_find_symbol(parser, token);

			fprintf(parser->fptr, "push %s %d\n", symbol_kind_desc(symbol_rec->kind), symbol_rec->nbr);

			//parser_identifier(parser, tab_count+1);
		}
	}
	else if (SYMBOL == token->type)
	{
		if (!strcmp(token->text, "("))
		{
			token_destroy(token);

			token = lexer_read(parser->lexer);
			token_destroy(token);
			//parser_symbol(parser, "(", tab_count+1);

			parser_expression(parser, tab_count+1);

			token = lexer_read(parser->lexer);
			token_destroy(token);
			//parser_symbol(parser, ")", tab_count+1);
		}
		else if (!strcmp(token->text, "-") || !strcmp(token->text, "~"))
		{
			token_destroy(token);

			token = lexer_read(parser->lexer);
			//parser_symbol(parser, token->text, tab_count+1);

			parser_term(parser, tab_count+1);

			if (!strcmp(token->text, "-"))
				fprintf(parser->fptr, "neg\n");
			else
				fprintf(parser->fptr, "not\n");

			token_destroy(token);
		}
	}
	else
		raiseError = true;

	//fprintf(parser->fptr, "%s</term>\n", parser_makeTabs(tab_count));

	if (raiseError)
		parser_raiseError(token);
	
}

void parser_expression(PPARSER parser, uint8_t tab_count)
{
	// term ( op term )*
	//fprintf(parser->fptr, "%s<expression>\n", parser_makeTabs(tab_count));

	parser_term(parser, tab_count+1);

	PTOKEN token = lexer_peek(parser->lexer);

	while (parser_isOp(token))
	{
		token_destroy(token);
		//parser_symbol(parser, token->text, tab_count+1);
		token = lexer_read(parser->lexer);
		char * op_text = duplicate_text(token->text);
		token_destroy(token);
		parser_term(parser, tab_count+1);
		fprintf(parser->fptr, "%s\n", symbol_op_desc(op_text));
		free(op_text);
		token = lexer_peek(parser->lexer);
	}

	//fprintf(parser->fptr, "%s</expression>\n", parser_makeTabs(tab_count));
}

bool parser_type(PPARSER parser, uint8_t tab_count)
{
	bool raiseError = false;

	PTOKEN token = lexer_peek(parser->lexer);

	if (KEYWORD == token->type && (!strcmp("int", token->text) || !strcmp("boolean", token->text) || !strcmp("char", token->text)))
	{
		token = lexer_read(parser->lexer);
		token_destroy(token);
		//parser_keyword(parser, token->text, tab_count);
	}
	else if (IDENTIFIER == token->type)
	{
		token = lexer_read(parser->lexer);
		token_destroy(token);
		//parser_identifier(parser, tab_count);
	}
	else
		raiseError = true;

	if (raiseError)
		parser_raiseError(token);

	return !raiseError;
}

uint8_t parser_classVarDec(PPARSER parser, uint8_t tab_count)
{
	// ( static | field ) type varName (, varName )*;
	
	//fprintf(parser->fptr, "%s<classVarDec>\n", parser_makeTabs(tab_count));

	bool raiseError = false;
	uint8_t field_count = 0;

	PSYMBOL_REC symbol_rec = (PSYMBOL_REC)malloc(sizeof(SYMBOL_REC));		

	PTOKEN token = lexer_peek(parser->lexer);
		
	// store kind of symbol
	if (!strcmp(token->text, "static"))
	{
		symbol_rec->kind = STATIC;
		symbol_rec->nbr = parser->class_static_index++;

		token = lexer_read(parser->lexer);
		token_destroy(token);
		//parser_keyword(parser, "static", tab_count+1);
	}
	else if (!strcmp(token->text, "field"))
	{
		symbol_rec->kind = THIS;
		symbol_rec->nbr = parser->class_this_index++;

		token = lexer_read(parser->lexer);
		token_destroy(token);
		//parser_keyword(parser, "field", tab_count+1);
		field_count++;
	}
	else
		raiseError = true;

	if (!raiseError)
	{
		token = lexer_peek(parser->lexer);
		
		// type of symbol
		symbol_rec->type = duplicate_text(token->text);
	
		if (IDENTIFIER == token->type)
		{
			token = lexer_read(parser->lexer);
			token_destroy(token);
			//parser_identifier(parser, tab_count+1);
		}
		else
			parser_type(parser, tab_count+1);

		// name of symbol
		token = lexer_read(parser->lexer);
		symbol_rec->name = duplicate_text(token->text);

		token_destroy(token);
		//parser_identifier(parser, tab_count+1);

		symbol_table_add( parser->class_symbol_table, symbol_rec);

		token = lexer_peek(parser->lexer);
		while ((SYMBOL == token->type) && !strcmp(",", token->text))
		{
			token = lexer_read(parser->lexer);
			token_destroy(token);
			//parser_symbol(parser, ",", tab_count+1);

			// add next symbol, type and kind are the same
			// different name and increase index

			PSYMBOL_REC new_symbol_rec = (PSYMBOL_REC)malloc(sizeof(SYMBOL_REC));
			new_symbol_rec->type = duplicate_text(symbol_rec->type);
			new_symbol_rec->kind = symbol_rec->kind;

			token = lexer_read(parser->lexer);
			new_symbol_rec->name = duplicate_text(token->text);
			
			token_destroy(token);

			if (new_symbol_rec->kind == STATIC)
				new_symbol_rec->nbr = parser->class_static_index++;
			else
				new_symbol_rec->nbr = parser->class_this_index++;

			symbol_table_add( parser->class_symbol_table, new_symbol_rec);

			if (field_count > 0)
				field_count++;

			token = lexer_peek(parser->lexer);
		}

		token = lexer_read(parser->lexer);
		token_destroy(token);
		//parser_symbol(parser, ";", tab_count+1);
	}

	if (raiseError)
		parser_raiseError(token);

	//fprintf(parser->fptr, "%s</classVarDec>\n", parser_makeTabs(tab_count));
	//
	return field_count;
}

bool parser_isType(PTOKEN token)
{
	return ((KEYWORD == token->type && (!strcmp(token->text, "int") || !strcmp(token->text, "boolean") || !strcmp(token->text, "char"))) || (IDENTIFIER == token->type));
}

void parser_returnStatement(PPARSER parser, uint8_t tab_count)
{
	// return expression?

	//fprintf(parser->fptr, "%s<returnStatement>\n", parser_makeTabs(tab_count));

	PTOKEN token = lexer_read(parser->lexer);
	//parser_keyword(parser, "return", tab_count+1);

	token = lexer_peek(parser->lexer);

	if (SYMBOL != token->type || strcmp(token->text, ";"))
	{
		parser_expression(parser, tab_count+1);
	}
	else
	{
		fprintf(parser->fptr, "push constant 0\n");
	}

	token = lexer_read(parser->lexer);
	//parser_symbol(parser, ";", tab_count+1);

	fprintf(parser->fptr, "return\n");
	//fprintf(parser->fptr, "%s</returnStatement>\n", parser_makeTabs(tab_count));
}

void parser_letStatement(PPARSER parser, uint8_t tab_count)
{
	// let varName ( [ expression ] )? = expression ;
	
	//fprintf(parser->fptr, "%s<letStatement>\n", parser_makeTabs(tab_count));

	PTOKEN token = lexer_read(parser->lexer);
	//parser_keyword(parser, "let", tab_count+1);

	PTOKEN var_token = lexer_read(parser->lexer);
	//parser_identifier(parser, tab_count+1);
	PSYMBOL_REC symbol_rec = parser_find_symbol(parser, var_token);
	if (NULL == symbol_rec)
		parser_raiseError(var_token);

	token = lexer_peek(parser->lexer);

	if (SYMBOL == token->type && !strcmp(token->text, "["))
	{
		// assignment to array
		token = lexer_read(parser->lexer);
		token_destroy(token);
		//parser_symbol(parser, "[", tab_count+1);
		
		fprintf(parser->fptr, "push %s %d\n", symbol_kind_desc(symbol_rec->kind), symbol_rec->nbr);
		
		parser_expression(parser, tab_count+1);

		fprintf(parser->fptr, "add\n");

		token = lexer_read(parser->lexer);
		token_destroy(token);
		//parser_symbol(parser, "]", tab_count+1);
		//
		// read = symbol
		token = lexer_read(parser->lexer);
		token_destroy(token);

		parser_expression(parser, tab_count+1);

		fprintf(parser->fptr, "pop that 0\n");

		token = lexer_read(parser->lexer);

		token_destroy(token);
	}
	else
	{
		// assignment to variable
		token = lexer_read(parser->lexer);
		//parser_symbol(parser, "=", tab_count+1);

		parser_expression(parser, tab_count+1);

		token = lexer_read(parser->lexer);
		//parser_symbol(parser, ";", tab_count+1);

		fprintf(parser->fptr, "pop %s %d\n", symbol_kind_desc(symbol_rec->kind), symbol_rec->nbr);
		//fprintf(parser->fptr, "%s</letStatement>\n", parser_makeTabs(tab_count));
	
	}
}

void parser_whileStatement(PPARSER parser, uint8_t tab_count)
{
	// while ( expression ) { statements }
	
	//fprintf(parser->fptr, "%s<whileStatement>\n", parser_makeTabs(tab_count));

	PTOKEN token = lexer_read(parser->lexer);
	token_destroy(token);
	//parser_keyword(parser, "while", tab_count+1);

	char * label1 = parser_unique_label("Label");
	char * label2 = parser_unique_label("Label");

	token = lexer_read(parser->lexer);
	token_destroy(token);
	//parser_symbol(parser, "(", tab_count+1);

	fprintf(parser->fptr, "label %s\n", label1);
	parser_expression(parser, tab_count+1);

	fprintf(parser->fptr, "not\nif-goto %s\n", label2);

	token = lexer_read(parser->lexer);
	token_destroy(token);
	//parser_symbol(parser, ")", tab_count+1);

	token = lexer_read(parser->lexer);
	token_destroy(token);
	//parser_symbol(parser, "{", tab_count+1);

	parser_statements(parser, tab_count+1);

	fprintf(parser->fptr, "goto %s\n", label1);

	token = lexer_read(parser->lexer);
	token_destroy(token);
	//parser_symbol(parser, "}", tab_count+1);

	//fprintf(parser->fptr, "%s</whileStatement>\n", parser_makeTabs(tab_count));
	//
	fprintf(parser->fptr, "label %s\n", label2);
}

void parser_doStatement(PPARSER parser, uint8_t tab_count)
{
	// do subroutineCall ;
	
	//fprintf(parser->fptr, "%s<doStatement>\n", parser_makeTabs(tab_count));

	PTOKEN token = lexer_read(parser->lexer);
	//parser_keyword(parser, "do", tab_count+1);

	token_destroy(token);
	parser_subroutineCall(parser, tab_count+1);

	token = lexer_read(parser->lexer);
	token_destroy(token);
	//parser_symbol(parser, ";", tab_count+1);
	fprintf(parser->fptr, "pop temp 0\n");

	//fprintf(parser->fptr, "%s</doStatement>\n", parser_makeTabs(tab_count));
}

void parser_ifStatement(PPARSER parser, uint8_t tab_count)
{
	// if ( expression ) { statements } ( else { statements } )?

	//fprintf(parser->fptr, "%s<ifStatement>\n", parser_makeTabs(tab_count));

	PTOKEN token = lexer_read(parser->lexer);
	//parser_keyword(parser, "if", tab_count+1);

	token = lexer_read(parser->lexer);
	//parser_symbol(parser, "(", tab_count+1);

	parser_expression(parser, tab_count+1);
	fprintf(parser->fptr, "not\n");

	char * label1 = parser_unique_label("Label");
	char * label2 = parser_unique_label("Label");

	token = lexer_read(parser->lexer);
	//parser_symbol(parser, ")", tab_count+1);

	token = lexer_read(parser->lexer);
	//parser_symbol(parser, "{", tab_count+1);

	fprintf(parser->fptr, "if-goto %s\n", label1);
	parser_statements(parser, tab_count+1);
	fprintf(parser->fptr, "goto %s\n", label2);

	token = lexer_read(parser->lexer);
	//parser_symbol(parser, "}", tab_count+1);

	fprintf(parser->fptr, "label %s\n", label1);
	token = lexer_peek(parser->lexer);

	if (KEYWORD == token->type && (!strcmp(token->text, "else")))
	{
		token = lexer_read(parser->lexer);
		//parser_keyword(parser, "else", tab_count+1);

		token = lexer_read(parser->lexer);
		//parser_symbol(parser, "{", tab_count+1);

		parser_statements(parser, tab_count+1);

		token = lexer_read(parser->lexer);
		//parser_symbol(parser, "}", tab_count+1);
	}

	fprintf(parser->fptr, "label %s\n", label2);
	//fprintf(parser->fptr, "%s</ifStatement>\n", parser_makeTabs(tab_count));
}

void parser_statements(PPARSER parser, uint8_t tab_count)
{
	// statement*
	
	//fprintf(parser->fptr, "%s<statements>\n", parser_makeTabs(tab_count));

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

	//fprintf(parser->fptr, "%s</statements>\n", parser_makeTabs(tab_count));
}

void parser_parameterList(PPARSER parser, uint8_t tab_count)
{
	// ( ( type varName ) (, type varName)*)?
	
	bool raiseError = false;

	//fprintf(parser->fptr, "%s<parameterList>\n", parser_makeTabs(tab_count));

	PTOKEN token = lexer_peek(parser->lexer);

	while (parser_isType(token))
	{
		PSYMBOL_REC symbol_rec = (PSYMBOL_REC)malloc(sizeof(SYMBOL_REC));

		symbol_rec->kind = ARGUMENT;

		symbol_rec->type = duplicate_text(token->text);

		parser_type(parser, tab_count+1);
	
		token = lexer_peek(parser->lexer);

		symbol_rec->name = duplicate_text(token->text);

		token = lexer_read(parser->lexer);
		token_destroy(token);
		//parser_identifier(parser, tab_count+1);

		symbol_rec->nbr = parser->var_argument_index++;

		symbol_table_add(parser->function_symbol_table, symbol_rec);

		token = lexer_peek(parser->lexer);

		if (token->type != SYMBOL || strcmp(token->text, ","))
			break;
		else
		{
			token = lexer_read(parser->lexer);
			token_destroy(token);
			//parser_symbol(parser, ",", tab_count+1);
			token = lexer_peek(parser->lexer);
		}

	}

	//fprintf(parser->fptr, "%s</parameterList>\n", parser_makeTabs(tab_count));

	if (raiseError)
		parser_raiseError(token);
}

uint8_t parser_varDec(PPARSER parser, uint8_t tab_count)
{
	// var type varName (, varName )* ;
	//
	bool raiseError = false;
	uint8_t var_count = 0;

	PSYMBOL_REC symbol_rec = (PSYMBOL_REC)malloc(sizeof(SYMBOL_REC));
	symbol_rec->kind = LOCAL;

	//fprintf(parser->fptr, "%s<varDec>\n", parser_makeTabs(tab_count));

	PTOKEN token = lexer_read(parser->lexer);
	token_destroy(token);

	//parser_keyword(parser, "var", tab_count+1);

	token = lexer_peek(parser->lexer);
	symbol_rec->type = duplicate_text(token->text);

	token = lexer_read(parser->lexer);
	token_destroy(token);
	//parser_type(parser, tab_count+1);

	token = lexer_peek(parser->lexer);
	symbol_rec->name = duplicate_text(token->text);
	var_count++;

	token = lexer_read(parser->lexer);
	token_destroy(token);
	//parser_identifier(parser, tab_count+1);

	symbol_rec->nbr = parser->var_local_index++;

	symbol_table_add( parser->function_symbol_table, symbol_rec);

	token = lexer_peek(parser->lexer);

	while (SYMBOL == token->type && !strcmp(token->text, ","))
	{
		var_count++;
		PSYMBOL_REC new_symbol_rec = (PSYMBOL_REC)malloc(sizeof(SYMBOL_REC));
		new_symbol_rec->type = duplicate_text(symbol_rec->type);
		new_symbol_rec->kind = symbol_rec->kind;

		token = lexer_read(parser->lexer);
		token_destroy(token);
		//parser_symbol(parser, ",", tab_count+1);
		
		token = lexer_peek(parser->lexer);
		new_symbol_rec->name = duplicate_text(token->text);
		new_symbol_rec->nbr = parser->var_local_index++;

		symbol_table_add(parser->function_symbol_table, new_symbol_rec);

		token = lexer_read(parser->lexer);
		token_destroy(token);

		token = lexer_peek(parser->lexer);
	}

	token = lexer_read(parser->lexer);
	token_destroy(token);
	//parser_symbol(parser, ";", tab_count+1);

	//fprintf(parser->fptr, "%s</varDec>\n", parser_makeTabs(tab_count));

	if (raiseError)
		parser_raiseError(token);

	return var_count;
}

bool parser_isStatement(PPARSER parser)
{
	PTOKEN token = lexer_peek(parser->lexer);

	return (KEYWORD == token->type && (!strcmp(token->text, "let") || !strcmp(token->text, "while") ||
				!strcmp(token->text, "if") || !strcmp(token->text, "do") || !strcmp(token->text, "return")));
}

void parser_subroutineBody(PPARSER parser, uint8_t tab_count, bool is_constructor, bool is_function, char * function_name, uint8_t field_count)
{
	// { varDec* statements}
	//
	bool raiseError = false;
	uint8_t var_count = 0;

	//fprintf(parser->fptr, "%s<subroutineBody>\n", parser_makeTabs(tab_count));

	PTOKEN token = lexer_read(parser->lexer);
	token_destroy(token);
	//parser_symbol(parser, "{", tab_count+1);

	token = lexer_peek(parser->lexer);

	while (KEYWORD == token->type && !strcmp(token->text, "var"))
	{
		var_count += parser_varDec(parser, tab_count+1);
		token = lexer_peek(parser->lexer);
	}
	
	if (is_function)
		fprintf(parser->fptr, "function %s.%s %d\n", parser->class_name, function_name, var_count);
	else
		fprintf(parser->fptr, "function %s.%s %d\n", parser->class_name, function_name, var_count);

	if (is_constructor)
	{
		fprintf(parser->fptr, "push constant %d\n", field_count);
		fprintf(parser->fptr, "call Memory.alloc 1\n");
		fprintf(parser->fptr, "pop pointer 0\n");
	}

	if (!is_constructor && !is_function)
		fprintf(parser->fptr, "push argument 0\npop pointer 0\n");

	parser_statements(parser, tab_count+1);

	token = lexer_read(parser->lexer);
	token_destroy(token);
	//parser_symbol(parser, "}", tab_count+1);

	//fprintf(parser->fptr, "%s</subroutineBody>\n", parser_makeTabs(tab_count));

	if (raiseError)
		parser_raiseError(token);

}

void parser_subroutineDec(PPARSER parser, uint8_t tab_count, uint8_t field_count)
{
	// ( constructor | function | method ) (void | type) subroutineName ( parameterList ) subroutineBody
	

	//fprintf(parser->fptr, "%s<subroutineDec>\n", parser_makeTabs(tab_count));
	bool raiseError = false;
	PTOKEN token = lexer_peek(parser->lexer);

	raiseError = (token->type != KEYWORD || (strcmp(token->text, "constructor") && strcmp(token->text, "function") && strcmp(token->text, "method")));

	if (!raiseError)
	{		
		bool is_constructor = !strcmp("constructor", token->text);
		bool is_function = !strcmp("function", token->text);

		linked_list_clear(parser->function_symbol_table->linked_list);
		parser->var_argument_index = 0;
		parser->var_local_index = 0;
		if (!strcmp(token->text, "method"))
		{
			// add 'this' variable to symbol table.
			PSYMBOL_REC symbol_rec = (PSYMBOL_REC)malloc(sizeof(SYMBOL_REC));
			symbol_rec->kind = ARGUMENT;
			symbol_rec->name = duplicate_text("this");
			symbol_rec->type = duplicate_text(parser->class_name);
			symbol_rec->nbr = parser->var_argument_index++;

			symbol_table_add(parser->function_symbol_table, symbol_rec);
		}
		token_destroy(token);
		token = lexer_read(parser->lexer);
		token_destroy(token);
		//parser_keyword(parser, token->text, tab_count+1);

		token = lexer_peek(parser->lexer);

		if (KEYWORD == token->type && !strcmp(token->text, "void"))
		{
			token = lexer_read(parser->lexer);
			token_destroy(token);
			//parser_keyword(parser, "void", tab_count+1);
		}
		else
			parser_type(parser, tab_count+1);

		token = lexer_read(parser->lexer);
		char * function_name = duplicate_text(token->text);
		token_destroy(token);
		//parser_identifier(parser, tab_count+1);

		token = lexer_read(parser->lexer);
		token_destroy(token);
		//parser_symbol(parser, "(", tab_count+1);

		parser_parameterList(parser, tab_count+1);

		token = lexer_read(parser->lexer);
		token_destroy(token);
		//parser_symbol(parser, ")", tab_count+1);

		parser_subroutineBody(parser, tab_count+1, is_constructor, is_function, function_name, field_count);

	}
	
	if (raiseError)
		parser_raiseError(token);

	//fprintf(parser->fptr, "%s</subroutineDec>\n", parser_makeTabs(tab_count));

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
	uint8_t field_count = 0;
	
	//fprintf(parser->fptr, "%s<class>\n", parser_makeTabs(tab_count));

	// consume the keyword token "class"
	PTOKEN token = lexer_read(parser->lexer);
	token_destroy(token);
	//parser_keyword(parser, "class", tab_count+1);

	// save class name
	token = lexer_read(parser->lexer);
	parser->class_name = duplicate_text(token->text);
	
	token_destroy(token);
	//parser_identifier(parser, tab_count+1);

	token = lexer_read(parser->lexer);
	token_destroy(token);
	//parser_symbol(parser, "{", tab_count+1);

	token = lexer_peek(parser->lexer);
	while (KEYWORD == token->type && (!strcmp(token->text, "static") || !strcmp(token->text, "field")))
	{
		field_count += parser_classVarDec(parser, tab_count+1);
		token = lexer_peek(parser->lexer);
	}

	token = lexer_peek(parser->lexer);
	while (KEYWORD == token->type && (!strcmp(token->text, "constructor") || !strcmp(token->text, "function") || !strcmp(token->text, "method")))
	{
		parser_subroutineDec(parser, tab_count+1, field_count);
		token = lexer_peek(parser->lexer);
	}

	token = lexer_read(parser->lexer);
	token_destroy(token);
	//parser_symbol(parser, "}", tab_count+1);

	//fprintf(parser->fptr, "%s</class>\n", parser_makeTabs(tab_count));

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

