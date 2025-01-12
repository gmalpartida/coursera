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
        if (isalpha(c) && (isalpha(d) || '_' == d || '.' == d || '$' == d) )
        {
            char text[256];
            int i = 0;
            text[i] = c;
            i++;
            while (isalpha(d) || '_' == d || isdigit(d) || '.' == d || '$' == d)
            {
                text[i] = d;
                d = scanner_get_next(lexer->scanner);
                d = scanner_peek_next(lexer->scanner);
                i++;
            }
            text[i] = '\0';
            token = token_create(text, scanner_position(lexer->scanner), IDENTIFIER);
            
        }
        else if ('A' == c)
        {
            token = token_create("A", scanner_position(lexer->scanner), A);
        }
        else if ('D' == c)
        {
            token = token_create("D", scanner_position(lexer->scanner), D);
        }
        else if ('M' == c)
        {
            token = token_create("M", scanner_position(lexer->scanner), M);
        }
        else if ('R' == c)
        {
            char text[256];
            int i = 0;
            text[i] = 'R';
            i++;
            c = scanner_peek_next(lexer->scanner);
            if (isdigit(c))
            {
                while (isdigit(c))
                {
                    text[i] = c;
                    i++;
                    c = scanner_get_next(lexer->scanner);
                    c = scanner_peek_next(lexer->scanner);
                }
                text[i] = '\0';
                token = token_create(text, scanner_position(lexer->scanner), REGISTER);
            }
        }
        else if ('@' == c)
        {
            token = token_create("@", scanner_position(lexer->scanner), AMPERSAND);
        }
        else if ('(' == c)
        {
            token = token_create("(", scanner_position(lexer->scanner), OPEN_PAREN);
        }
        else if (')' == c)
        {
            token = token_create(")", scanner_position(lexer->scanner), CLOSE_PAREN);
        }
        else if ('=' == c)
        {
            token = token_create("=", scanner_position(lexer->scanner), EQUAL);
        }
        else if ('+' == c)
        {
            token = token_create("+", scanner_position(lexer->scanner), PLUS);
        }
        else if (isdigit(c))
        {
            char text[256];
            int i = 0;
            text[i] = c;
            c = scanner_peek_next(lexer->scanner);
            i++;
            while (isdigit(c))
            {
                text[i] = c;
                c = scanner_get_next(lexer->scanner);
                c = scanner_peek_next(lexer->scanner);
                i++;
            }
            text[i] = '\0';
            token = token_create(text, scanner_position(lexer->scanner), VALUE);
        }
        else if (';' == c)
        {
            token = token_create(";", scanner_position(lexer->scanner), SEMI_COLON);
        }
        else if (isalpha(c))
        {
            char text[2];
            text[0] = c;
            text[1] = '\0';
            token = token_create(text, scanner_position(lexer->scanner), IDENTIFIER);
        }
        else if ('-' == c)
        {
            token = token_create("-", scanner_position(lexer->scanner), MINUS);
        }
        else if ('!' == c)
        {
            token = token_create("!", scanner_position(lexer->scanner), NOT);
        }
        else if ('&' == c)
        {
            token = token_create("&", scanner_position(lexer->scanner), OPERATOR);
        }
        else if ('|' == c)
        {
            token = token_create("|", scanner_position(lexer->scanner), OPERATOR);
        }
        else
        {
            char msg[21];
            sprintf(msg, "Unknown character: %c", c);
            token = token_create(msg, scanner_position(lexer->scanner), ERROR);
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


