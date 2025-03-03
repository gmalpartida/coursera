#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "linked_list.h"

typedef struct _SYMBOL_TABLE_RECORD
{
	
} SYMBOL_TABLE_RECORD, *PSYMBOL_TABLE_RECORD;

typedef struct _SYMBOL_TABLE
{
	PLINKED_LIST linked_list;

} SYMBOL_TABLE, *PSYMBOL_TABLE;

PSYMBOL_TABLE symbol_table_create();
void symbol_table_destroy(PSYMBOL_TABLE symbol_table);

#endif
