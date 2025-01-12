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
    int next_variable_index;
} SYMBOL_TABLE, *PSYMBOL_TABLE;

PSYMBOL_TABLE symbol_table_create();
void symbol_table_print(PSYMBOL_TABLE symbol_table);
void symbol_table_add(PSYMBOL_TABLE symbol_table, PSYMBOL symbol);
int symbol_table_find(PSYMBOL_TABLE symbol_table, char * symbol_text);
PSYMBOL symbol_table_get(PSYMBOL_TABLE symbol_table, char * text);
PSYMBOL symbol_duplicate(PSYMBOL symbol);
PSYMBOL symbol_copy(PSYMBOL dest_symbol, PSYMBOL src_symbol);
void symbol_table_add_variable(PSYMBOL_TABLE symbol_table, char * text);
void symbol_print(PSYMBOL symbol);

#endif
