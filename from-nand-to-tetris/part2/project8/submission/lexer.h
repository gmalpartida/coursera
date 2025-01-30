#ifndef LEXER_H
#define LEXER_H

#include "scanner.h"
#include "token.h"

typedef struct Lexer
{
    PSCANNER scanner;
} LEXER, *PLEXER;


PLEXER lexer_create(PSCANNER scanner);
PTOKEN lexer_read(PLEXER lexer); 
void lexer_print(PLEXER lexer);
void lexer_ignore_whitespace(PLEXER lexer);
void lexer_ignore_comment(PLEXER lexer);
int lexer_is_whitespace_or_comment(PLEXER lexer);
PTOKEN lexer_peek(PLEXER lexer);

#endif
