#ifndef TOKEN_H
#define TOKEN_H

typedef enum {KEYWORD, SYMBOL, INTCONSTANT, STRCONSTANT, IDENTIFIER, ERROR, EOE} TOKEN_TYPE;

typedef struct Token
{
    char * text;
    int position;
    TOKEN_TYPE type;
} TOKEN, *PTOKEN;

PTOKEN token_create(char * text, int position, TOKEN_TYPE type);

char * token_map_type(TOKEN_TYPE type);

void token_print();

void token_destroy(PTOKEN token);

#endif
