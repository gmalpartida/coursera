#ifndef TOKEN_H
#define TOKEN_H

typedef enum {STACK_OP, ARITHMETIC_OP, LOGICAL_OP, MEMORY_SEGMENT, NUMBER, BRANCH_OP, FUNCTION_OP, IDENTIFIER, ERROR, EOE} TOKEN_TYPE;

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
