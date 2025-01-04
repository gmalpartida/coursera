#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "token.h"
#include "lexer.h"
#include "parser.h"


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
                lexer_print(lexer);
                PPARSER parser = parser_create(lexer);

                if (NULL != parser)
                {
        
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


