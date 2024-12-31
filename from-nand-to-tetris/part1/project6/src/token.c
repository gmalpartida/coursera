#include "token.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


char * token_type_map[] = {"D", "A", "M", "Variable", "Label", "R0", "R1", "R2", "R3", "R4", "R5", "R6",
                        "R7", "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15", "Error", "EOF"};
int token_type_map_size = 23;

PTOKEN token_create(char * text, int position, TOKEN_TYPE type)
{
    PTOKEN token = (PTOKEN)malloc(sizeof(TOKEN));

    token->text = (char*)malloc(sizeof(char) * (1 + strlen(text)));
    strcpy(text, token->text);
    token->position = position;
    token->type = type;
    return token;
}

void token_destroy(PTOKEN token)
{
    free(token->text);
    free(token);
    token = NULL;
}

void token_print(PTOKEN token)
{
    printf("{text: %s, position: %d, type: %d}", token->text, token->position, token->type);
}

char * token_map_type_to_text(TOKEN_TYPE type)
{
    char * text = NULL;
    if (type >= D && type <= EOE)
    {
        text = (char*)malloc(sizeof(token_type_map[type])+1);
        strcpy(text, token_type_map[type]);
    }   

    return text;
}

TOKEN_TYPE token_map_text_to_type(char * text)
{
    TOKEN_TYPE type = Error;
    for (int i = D; i < EOE; i++)
    {
        if (0 == strcmp(token_type_map[i], text) )
        {
            type = i;
            break;
        }
    }
    return type;    
}


