#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H


typedef struct Symbol
{
    char * symbol;
    int value;
} SYMBOL, *PSYMBOL;

typedef struct SYMBOL_TABLE
{
    PSYMBOL symbol_table;
    int size;
} SYMBOL_TABLE, *PSYMBOL_TABLE;

PSYMBOL_TABLE symbol_table_create();
void symbol_table_print(PSYMBOL_TABLE symbol_table);
void symbol_table_add(PSYMBOL_TABLE symbol_table, PSYMBOL symbol);
int symbol_table_find(PSYMBOL_TABLE symbol_table, char * symbol_text);
PSYMBOL symbol_table_get(PSYMBOL_TABLE symbol_table, char * text);

#endif
