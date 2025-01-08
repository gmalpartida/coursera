#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"

int main(int argc, char * argv[])
{

    if (2 == argc)
    {
        char * filename = argv[1];
        PSCANNER scanner =  scanner_create(filename);

        if (NULL != scanner)
        {
            PLEXER lexer = lexer_create(scanner);
            if (NULL != lexer)
            {
                PPARSER parser = parser_create(lexer);

                parser_print(parser);
                if (NULL != parser)
                {
                    PTOKEN token = NULL;
                    while (EOE != (token = lexer_peek(parser->lexer))->type)
                    {
                        token_print(token);
                        if (AMPERSAND == token->type)
                        {
                            parser_parse_A_instruction(parser);
                        }
                        else if (OPEN_PAREN == token->type)
                        {
                            parser_parse_label(parser);
                        }
                        else
                        {
                            parser_parse_C_instruction(parser);
                        }
                    }
                    parser_destroy(parser);
                }
                
            }
            scanner_destroy(scanner);
        }
    }
    else
    {
        printf("usage: assembler filename\n");
    }
    return EXIT_SUCCESS;
}


