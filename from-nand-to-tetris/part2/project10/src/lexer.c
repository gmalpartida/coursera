#include "lexer.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

char symbols[] = {'{', '}', '(', ')', '[', ']', '.', ',', ';', '+', '-', '*', '/', '&', '|', '<', '>', '=', '~'};
char * keywords[] = {"class", "constructor", "function", "method", "field", "static", "var", "int", "char", "boolean",
					"void", "true", "false", "null", "this", "let", "do", "if", "else", "while", "return"};

const uint16_t keyword_cnt = 21;

PLEXER lexer_create(PSCANNER scanner)
{
    PLEXER lexer = (PLEXER)malloc(sizeof(LEXER));
    lexer->scanner = scanner;

    return lexer;
}

void lexer_destroy(PLEXER lexer)
{
    free(lexer);
}

bool lexer_ignore_whitespace(PLEXER lexer)
{
	bool result = false;
    char c = scanner_peek_next(lexer->scanner);
    while (' ' == c || '\t' == c || '\n' == c || '\r' == c )
    {
		result = true;
        c = scanner_get_next(lexer->scanner);
        c = scanner_peek_next(lexer->scanner);
    }
	return result;
}

bool lexer_ignore_comment(PLEXER lexer)
{
	bool result = false;
	char c = scanner_peek_next(lexer->scanner);
	char d = scanner_peek2_next(lexer->scanner);

	if ('/' == c && '/' == d)
	{
		result = true;
		    // this is a line comment
			while ('\n' != c && '\0' != c)
			{
				c = scanner_get_next(lexer->scanner);
				c = scanner_peek_next(lexer->scanner);
			}
			// read EOL
			if ('\n' == c)
				c = scanner_get_next(lexer->scanner);
	}
	else if ('/' == c && '*' == d)
	{
		result = true;
		c = scanner_get_next(lexer->scanner);
		d = scanner_get_next(lexer->scanner);

		// this is a block comment
		bool done = false;
		while (!done)
		{
			c = scanner_peek_next(lexer->scanner);

			if ('*' == c)
			{
				d = scanner_peek2_next(lexer->scanner);
				if ('/' == d)
				{
					d = scanner_get_next(lexer->scanner);
					done = true;

				}
			}

			c = scanner_get_next(lexer->scanner);
		}
	}
	return result;
}

bool is_symbol(char c)
{
	bool result = false;

	uint8_t len = sizeof(symbols) / sizeof(char);
	
	for (uint8_t index = 0; index < len; index++)
	{
		if (c == symbols[index])
		{
			result = true;
			break;
		}
	}

	return result;
}

bool is_keyword(char * text)
{
	bool result = false;

	for (uint8_t index = 0; index < keyword_cnt; index++)
	{
		if (!strcmp(text, keywords[index]))
		{
			result = true;
			break;
		}
	}

	return result;
}

PTOKEN lexer_read(PLEXER lexer)
{
    PTOKEN token = NULL;
	bool done = false;
    while (!done)
    {
        done = !(lexer_ignore_whitespace(lexer) || lexer_ignore_comment(lexer));
    }

   if (!scanner_at_end(lexer->scanner))
    {
		uint16_t pos = scanner_position(lexer->scanner);
        char c = scanner_peek_next(lexer->scanner);
		if (is_symbol(c))
		{
			uint16_t pos = 
			c = scanner_get_next(lexer->scanner);
			char * text = (char*)malloc(2);
			text[0] = c;
			text[1] = '\0';
			token = token_create(text, pos, SYMBOL);
		}
        else if (isdigit(c))
        {
            char text[20];
            uint8_t i = 0;
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

            token = token_create(text, pos, INTCONSTANT);
        }
		else if (isalpha(c) || ('_' == c))
        {
            char text[20];
            uint8_t i = 0;
            while (isalpha(c) || isdigit(c) || '_' == c)
            {
                //consume the character
                c = scanner_get_next(lexer->scanner);

                text[i] = c;
                i++;

                // peek at next character
                c = scanner_peek_next(lexer->scanner);
            }
            text[i] = '\0';

			TOKEN_TYPE type = IDENTIFIER;
			if (is_keyword(text))
			{
				type = KEYWORD;
			}

            token = token_create(text, pos, type);
        }
		else if ('"' == c)
		{
			char text[20];
			uint8_t i = 0;
			// consume character
			scanner_get_next(lexer->scanner);
			c = scanner_peek_next(lexer->scanner);
			while (c != '"')
			{
				c = scanner_get_next(lexer->scanner);
				text[i++] = c;
				c = scanner_peek_next(lexer->scanner);
			}
			text[i] = '\0';
			if (c == '"')
			{
				scanner_get_next(lexer->scanner);
				token = token_create(text, pos, STRCONSTANT);
			}
			else
				token = token_create("ERROR", pos, ERROR);
		}
		else
        {
            token = token_create("ERROR", pos, ERROR);
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

PTOKEN lexer_peek2(PLEXER lexer)
{
	int position = scanner_position(lexer->scanner);
    PTOKEN token = lexer_read(lexer);
	token = lexer_read(lexer);
    scanner_set_position(lexer->scanner, position);
    return token;
}

void lexer_print(PLEXER lexer)
{
   scanner_print(lexer->scanner);
}

