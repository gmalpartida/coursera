#ifndef TOKEN_H
#define TOKEN_H

typedef enum {D, A, M, IDENTIFIER,             
                R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15, 
                EQUAL, PLUS,SEMI_COLON,
                ERROR, AMPERSAND, AT, OPEN_PAREN, CLOSE_PAREN, ADDRESS, EOE} TOKEN_TYPE;

typedef struct Token
{
    char * text;
    int position;
    TOKEN_TYPE type;
} TOKEN, *PTOKEN;

PTOKEN token_create(char * text, int position, TOKEN_TYPE type);

char * token_map_type(TOKEN_TYPE type);

void token_print();

#endif
