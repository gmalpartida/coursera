#include "symbol_table.h"
#include <stdlib.h>

PSYMBOL_TABLE symbol_table_create()
{
	PSYMBOL_TABLE symbol_table = (PSYMBOL_TABLE)malloc(sizeof(SYMBOL_TABLE));

	symbol_table->linked_list = linked_list_create(NULL, NULL, NULL);

	return symbol_table;
}

void symbol_table_destroy(PSYMBOL_TABLE symbol_table)
{
	linked_list_destroy(symbol_table->linked_list);
	free(symbol_table);
}


