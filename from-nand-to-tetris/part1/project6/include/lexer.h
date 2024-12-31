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


#endif
