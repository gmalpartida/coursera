#include "token.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


char * token_type_map[] = {"KEYWORD", "SYMBOL", "INTCONSTANT", "STRCONSTANT", "IDENTIFIER", "ERROR", "EOE"};
int token_type_map_size = 7;

PTOKEN token_create(char * text, int position, TOKEN_TYPE type)
{
    PTOKEN token = (PTOKEN)malloc(sizeof(TOKEN));

    token->text = (char*)malloc(sizeof(char) * (1 + strlen(text)));
    strcpy(token->text, text);
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

char * token_map_type_to_text(TOKEN_TYPE type)
{
    char * text = NULL;
    if (type >= KEYWORD && type <= EOE)
    {
        text = (char*)malloc(sizeof(char) * strlen(token_type_map[type])+1);
        strcpy(text, token_type_map[type]);
    }   

    return text;
}

TOKEN_TYPE token_map_text_to_type(char * text)
{
    TOKEN_TYPE type = ERROR;
    for (int i = KEYWORD; i < EOE; i++)
    {
        if (0 == strcmp(token_type_map[i], text) )
        {
            type = i;
            break;
        }
    }
    return type;    
}

void token_print(PTOKEN token)
{
//    printf("{text: %s, position: %d, type: %s}", token->text, token->position, token_map_type_to_text( token->type));
	if (token->type == KEYWORD)
	{
		printf("<keyword>%s</keyword>", token->text);
	}
	else if (token->type == SYMBOL)
	{
		printf("<symbol>%s</symbol>", token->text);
	}
	else if (token->type == INTCONSTANT)
	{
		printf("<integerConstant>%s</integerConstant>", token->text);
	}
	else if (token->type == STRCONSTANT)
	{
		printf("<stringConstant>%s</stringConstant>", token->text);
	}
	else if(token->type == IDENTIFIER)
	{
		printf("<identifier>%s</identifier>", token->text);
	}
}




