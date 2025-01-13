#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "token.h"
#include "lexer.h"
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


