#include "lexer.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>

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
        char c = scanner_peek_next(lexer->scanner);
        if (isalpha(c))
        {
            char text[20];
            uint8_t i = 0;
            uint16_t pos = scanner_position(lexer->scanner);
            while (isalpha(c))
            {
                //consume the character
                c = scanner_get_next(lexer->scanner);

                text[i] = c;
                i++;

                // peek at next character
                c = scanner_peek_next(lexer->scanner);
            }
            text[i] = '\0';
            TOKEN_TYPE type = ERROR;
            
            if (0 == strcmp(text, "push") || 0 == strcmp(text, "pop"))
                type = STACK_OP;
            else if (0 == strcmp(text, "add") || 0 == strcmp(text, "sub") || 0 == strcmp(text, "neg"))
                type = ARITHMETIC_OP;
            else if (0 == strcmp(text, "eq") || 0 == strcmp(text, "gt") || 0 == strcmp(text, "lt") ||
                    0 == strcmp(text, "and") || 0 == strcmp(text, "or") || 0 == strcmp(text, "not"))
                type = LOGICAL_OP;
            else if (0 == strcmp(text, "local") || 0 == strcmp(text, "argument") || 0 == strcmp(text, "static") ||
                    0 == strcmp(text, "constant") || 0 == strcmp(text, "this") || 0 == strcmp(text, "that") ||
                    0 == strcmp(text, "temp") || 0 == strcmp(text, "pointer"))
                type = MEMORY_SEGMENT;

            token = token_create(text, pos, type);
        }
        else if (isdigit(c))
        {
            char text[20];
            uint8_t i = 0;
            uint16_t pos = scanner_position(lexer->scanner);
            while (isdigit(c))
            {
                // consume character
                c = scanner_get_next(lexer->scanner);

                text[i] = c;
                i++;

                // peek at next character
                c = scanner_peek_next(lexer->scanner);
            }
            text[i] = '\0';

            token = token_create(text, pos, NUMBER);
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


