#include "lexer.h"
#include <stdlib.h>

PLEXER lexer_create(PSCANNER scanner)
{
    PLEXER lexer = (PLEXER)malloc(sizeof(LEXER));

    return lexer;
}

PTOKEN lexer_read(PLEXER lexer)
{
    PTOKEN token = NULL;
    while (!scanner_at_end(lexer->scanner))
    {
        char c = scanner_get_next(lexer->scanner);
        if (c <= ' ')   // ignore whitespace
            continue;
    }
    return token;
}


