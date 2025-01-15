#include "lexer.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

PLEXER lexer_create(PSCANNER scanner)
{
    PLEXER lexer = (PLEXER)malloc(sizeof(LEXER));
    lexer->scanner = scanner;

    return lexer;
}

void lexer_ignore_whitespace(PLEXER lexer)
{
    char c = scanner_peek_next(lexer->scanner);
    while (' ' == c || '\t' == c || '\n' == c)
    {
        c = scanner_get_next(lexer->scanner);
        c = scanner_peek_next(lexer->scanner);
    }
}

void lexer_ignore_comment(PLEXER lexer)
{
   char c = scanner_peek_next(lexer->scanner);
   if ('/' == c)
   {
       c = scanner_get_next(lexer->scanner);
       c = scanner_peek_next(lexer->scanner);
       if ('/' == c)
       {    // this is a comment
           c = scanner_get_next(lexer->scanner);
           c = scanner_peek_next(lexer->scanner);
           while ('\n' != c)
           {
               c = scanner_get_next(lexer->scanner);
               c = scanner_peek_next(lexer->scanner);
           }
           // read EOL
           if ('\n' == c)
               c = scanner_get_next(lexer->scanner);
       }
   }
}

PTOKEN lexer_read(PLEXER lexer)
{
    PTOKEN token = NULL;
    while (lexer_is_whitespace_or_comment(lexer))
    {
        lexer_ignore_whitespace(lexer);
        lexer_ignore_comment(lexer);
    }

    if (!scanner_at_end(lexer->scanner))
    {
        char c = scanner_get_next(lexer->scanner);
        char d = scanner_peek_next(lexer->scanner);
        if (isalpha(c) && (isalpha(d) || '_' == d))
        {
            char text[256];
            int i = 0;
            text[i] = c;
        }
        else
        {
            token = token_create("ERROR", scanner_position(lexer->scanner), ERROR);
        }
    }
    else
        token = token_create("EOE", -1, EOE);

    return token;
}

PTOKEN lexer_peek(PLEXER lexer)
{
    int position = scanner_position(lexer->scanner);
    PTOKEN token = lexer_read(lexer);
    scanner_set_position(lexer->scanner, position);
    return token;
}

void lexer_print(PLEXER lexer)
{
   scanner_print(lexer->scanner);
}

int lexer_is_whitespace_or_comment(PLEXER lexer)
{
    int result = 0;
    char c = scanner_peek_next(lexer->scanner);
    if ('/' == c || ' ' == c || '\t' == c || '\n' == c)
        result = 1;
    return result;
}


