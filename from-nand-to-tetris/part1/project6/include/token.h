#ifndef TOKEN_H
#define TOKEN_H

typedef enum {IDENTIFIER,             
                REGISTER, VREGISTER, EQUAL, PLUS,SEMI_COLON, VALUE, NOT, OPERATOR,
                ERROR, AMPERSAND, AT, OPEN_PAREN, CLOSE_PAREN, ADDRESS, MINUS, EOE} TOKEN_TYPE;

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
