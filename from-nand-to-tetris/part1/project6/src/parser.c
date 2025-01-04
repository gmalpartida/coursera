#include "parser.h"
#include <stdlib.h>
#include "lexer.h"
#include "token.h"
#include <stdio.h>

PPARSER parser_create(PLEXER lexer)
{
    PPARSER parser = (PPARSER)malloc(sizeof(PARSER));

    parser->lexer = lexer;

    parser_build_symbol_table(parser);
    
    return parser;
}

void parser_destroy(PPARSER parser)
{
    free(parser->lexer);\
    free(parser);
    parser = NULL;
}

void parser_build_symbol_table(PPARSER parser)
{
    
    for (int i = 0; i < 16; i++)
    {
        char * buffer = (char*)malloc(3);
        sprintf(buffer, "R%d", i);

        parser->symbol_table[i].symbol = buffer;  parser->symbol_table[i].value = i;
    }

    char * buffer = (char*)malloc(3);       sprintf(buffer, "SP");
    parser->symbol_table[16].symbol = buffer;       parser->symbol_table[16].value = 0;
    buffer = (char*)malloc(4);              sprintf(buffer, "LCL");
    parser->symbol_table[17].symbol = buffer;       parser->symbol_table[17].value = 1;
    buffer = (char*)malloc(4);              sprintf(buffer, "ARG");
    parser->symbol_table[18].symbol = buffer;       parser->symbol_table[18].value = 2;
    buffer = (char*)malloc(5);              sprintf(buffer, "THIS");
    parser->symbol_table[19].symbol = buffer;       parser->symbol_table[19].value = 3;
    buffer = (char*)malloc(5);              sprintf(buffer, "THAT");
    parser->symbol_table[20].symbol = buffer;       parser->symbol_table[20].value = 4;

    buffer = (char*)malloc(7);              sprintf(buffer, "SCREEN");
    parser->symbol_table[21].symbol = buffer;       parser->symbol_table[21].value = 16384;

    buffer = (char*)malloc(4);              sprintf(buffer, "KBD");
    parser->symbol_table[22].symbol = buffer;       parser->symbol_table[22].value = 24576;
    
    PTOKEN token = NULL;

    while (EOE != (token = lexer_read(parser->lexer))->type)
    {
        if (token->type == OPEN_PAREN)
        {
            token_destroy(token);
            token = lexer_read(parser->lexer);
            
            if (token->type == IDENTIFIER)
            {
                // process label

                token_print(token);
                
                printf("\n");
            }
            token_destroy(token);
        }
        else
            token_destroy(token);
        if (token->type == ERROR)
            break;
    }


}




