#include "symbol_table.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

PSYMBOL_TABLE symbol_table_create()
{
    PSYMBOL_TABLE symbol_table = (PSYMBOL_TABLE)malloc(sizeof(SYMBOL_TABLE));
    symbol_table->symbol_table = (PSYMBOL)malloc(sizeof(SYMBOL)*256);
    symbol_table->size = 0;
    symbol_table->next_variable_index = 16;
    return symbol_table;
}

void symbol_table_print(PSYMBOL_TABLE symbol_table)
{
    for (int i = 0; i < symbol_table->size; i++)
    {
        printf("(symbol: %s, value: %d\n", symbol_table->symbol_table[i].symbol, symbol_table->symbol_table[i].value);
    }
}

void symbol_table_add(PSYMBOL_TABLE symbol_table,  PSYMBOL symbol)
{
    symbol_table->symbol_table[(symbol_table->size)++] = *symbol;

}

void symbol_table_add_variable(PSYMBOL_TABLE symbol_table, char * text)
{
    PSYMBOL symbol = (PSYMBOL)malloc(sizeof(SYMBOL));
    symbol->symbol = (char*)malloc(sizeof(char)*strlen(text)+1);
    strcpy(symbol->symbol, text);
    symbol->value = symbol_table->next_variable_index++;
    symbol_table_add(symbol_table, symbol);
}

int symbol_table_find(PSYMBOL_TABLE symbol_table, char * symbol_text)
{
    int result = false;

    for (int i = 0; i < symbol_table->size; i++)
    {
        if (0 == strcmp(symbol_table->symbol_table[i].symbol, symbol_text))
        {
            result = true;
            break;
        }
    }
    return result;
}

PSYMBOL  symbol_table_get(PSYMBOL_TABLE symbol_table, char * text)
{
    PSYMBOL symbol = NULL;
    for (int i = 0; i < symbol_table->size; i++)
    {
        if (strcmp(symbol_table->symbol_table[i].symbol, text) == 0)
        {
            symbol = symbol_duplicate(&(symbol_table->symbol_table[i]));
            break;
        }
    }
    return symbol;
}

PSYMBOL symbol_copy(PSYMBOL dest_symbol, PSYMBOL src_symbol)
{
    size_t len = strlen(src_symbol->symbol) + 1;
    dest_symbol->symbol = (char*)malloc(sizeof(char) * len);
    
    strcpy(dest_symbol->symbol, src_symbol->symbol);
    dest_symbol->value = src_symbol->value;

    return dest_symbol;
}

PSYMBOL symbol_duplicate(PSYMBOL symbol)
{
    PSYMBOL new_symbol = (PSYMBOL)malloc(sizeof(SYMBOL));

    symbol_copy(new_symbol, symbol);

    return new_symbol;
}




