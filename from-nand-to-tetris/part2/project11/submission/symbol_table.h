#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "linked_list.h"
#include <stdint.h>

typedef enum  {THIS, STATIC, LOCAL, ARGUMENT} SYMBOL_KIND;

typedef struct _SYMBOL_REC
{
	char * name;
	char * type;
	SYMBOL_KIND kind;
	uint8_t nbr;
} SYMBOL_REC, *PSYMBOL_REC;

typedef struct _SYMBOL_TABLE
{
	PLINKED_LIST linked_list;

} SYMBOL_TABLE, *PSYMBOL_TABLE;

PSYMBOL_TABLE symbol_table_create();
void symbol_table_destroy(PSYMBOL_TABLE symbol_table);
void symbol_table_add(PSYMBOL_TABLE symbol_table, PSYMBOL_REC symbol_rec);
void symbol_table_print(PSYMBOL_TABLE symbol_table);
PSYMBOL_REC symbol_table_find(PSYMBOL_TABLE symbol_table, char * symbol_name);
char * symbol_kind_desc(SYMBOL_KIND kind);
char * symbol_op_desc(char * op);


#endif
