#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "token.h"
#include "lexer.h"

int main(int argc, char * argv[])
{

    if (2 == argc)
    {
        char * filename = argv[1];
        PSCANNER scanner =  scanner_create(filename);

        PLEXER lexer = lexer_create(scanner);
        PTOKEN token = NULL;
        
        lexer_print(lexer);
        printf("\n");

        while (EOE != (token = lexer_read(lexer))->type)
        {
            token_print(token);
            printf("\n");
            if (token->type == ERROR)
                break;
        }

        scanner_destroy(scanner);
    }
    else
    {
        printf("usage: assembler filename\n");
    }
    return EXIT_SUCCESS;
}


