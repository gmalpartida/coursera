#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "symbol_table.h"

typedef struct Parser
{
    PLEXER lexer;
	char * out_filename;
	FILE* fptr;
	uint8_t tab_count;
	PSYMBOL_TABLE class_symbol_table;
	PSYMBOL_TABLE function_symbol_table;
	uint16_t var_local_index;
	uint16_t var_argument_index;
	uint16_t class_this_index;
	uint16_t class_static_index;
	char * class_name;

} PARSER, *PPARSER;

PPARSER parser_create(PLEXER lexer);
void parser_destroy(PPARSER parser);
void parser_print(PPARSER parser);
void parser_print_ast(PPARSER parser);
void parser_execute(PPARSER parser, char * out_filename);
bool parser_keyword(PPARSER parser, char * keyword, uint8_t tab_count);
bool parser_symbol(PPARSER parser, char * symbol, uint8_t tab_count);
bool parser_identifier(PPARSER parser, uint8_t tab_count);
bool parser_type(PPARSER parser, uint8_t tab_count);
void parser_expression(PPARSER parser, uint8_t tab_count);
void parser_statements(PPARSER parser, uint8_t tab_count);
void parser_letStatement(PPARSER parser, uint8_t tab_count);
void parser_whileStatement(PPARSER parser, uint8_t tab_count);
void parser_doStatement(PPARSER parser, uint8_t tab_count);
void parser_returnStatement(PPARSER parser, uint8_t tab_count); 
void parser_subroutineCall(PPARSER parser, uint8_t tab_count);
bool parser_isStatement(PPARSER parser);
char * parser_makeTabs(uint8_t tab_count);

#endif
