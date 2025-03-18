#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char * symbol_kind_desc(SYMBOL_KIND kind)
{
	switch(kind)
	{
		case THIS:
			return "this";
		case STATIC:
			return "static";
		case LOCAL:
			return "local";
		case ARGUMENT:
			return "argument";
	}
	return "";
}

char * symbol_op_desc(char * op)
{
	if (!strcmp(op, "+"))
		return "add";
	else if (!strcmp(op, "-"))
		return "sub";
	else if (!strcmp(op, "*"))
		return "call Math.multiply 2";
	else if (!strcmp(op, "/"))
		return "call Math.divide 2";
	else if (!strcmp(op, "<"))
		return "lt";
	else if (!strcmp(op, ">"))
		return "gt";
	else if (!strcmp(op, "&"))
		return "and";
	else if (!strcmp(op, "|"))
		return "or";
	else if (!strcmp(op, "="))
		return "eq";
	else
		return op;

	return "";
}

void symbol_print(void * data)
{
	printf("name: %s, type: %s, kind: %s, index: %d\n", ((PSYMBOL_REC)data)->name, ((PSYMBOL_REC)data)->type,
														symbol_kind_desc(((PSYMBOL_REC)data)->kind), ((PSYMBOL_REC)data)->nbr);
}

bool symbol_compare(void * from_data, void * to_data)
{
	bool result = false;

	if (!strcmp(((PSYMBOL_REC)from_data)->name, ((PSYMBOL_REC)to_data)->name))
		result = true;

	return result;
}

void symbol_destroy(void * data)
{
	free(((PSYMBOL_REC)data)->name);
	free(((PSYMBOL_REC)data)->type);
	free(data);
}

PSYMBOL_TABLE symbol_table_create()
{
	PSYMBOL_TABLE symbol_table = (PSYMBOL_TABLE)malloc(sizeof(SYMBOL_TABLE));

	symbol_table->linked_list = linked_list_create(symbol_print, symbol_compare, symbol_destroy);

	return symbol_table;
}

void symbol_table_destroy(PSYMBOL_TABLE symbol_table)
{
	linked_list_destroy(symbol_table->linked_list);
	free(symbol_table);
}

void symbol_table_add(PSYMBOL_TABLE symbol_table, PSYMBOL_REC symbol_rec)
{
	PNODE node = node_create(symbol_rec, NULL);
	linked_list_add(symbol_table->linked_list, node);
}

void symbol_table_print(PSYMBOL_TABLE symbol_table)
{
	linked_list_print(symbol_table->linked_list);
}

PSYMBOL_REC symbol_table_find(PSYMBOL_TABLE symbol_table, char * symbol_name)
{
	PSYMBOL_REC symbol_to_find = (PSYMBOL_REC)malloc(sizeof(SYMBOL_REC));

	symbol_to_find->name = symbol_name;

	PNODE node_to_find = node_create(symbol_to_find, NULL); 

	PNODE cur = linked_list_find(symbol_table->linked_list, node_to_find);

	PSYMBOL_REC symbol = cur ? cur->data : NULL;

	free(symbol_to_find);

	return symbol;
}



