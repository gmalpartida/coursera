#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"

int main(int argc, char * argv[])
{

    if (2 == argc)
    {
        char * filename = argv[1];
        PSCANNER scanner =  scanner_create(filename);
        scanner_print(scanner);

        while (!scanner_at_end(scanner)){
            char c = scanner_get_next(scanner);
            printf("%c", c);
        }
        scanner_destroy(scanner);
    }
    else
    {
        printf("usage: assembler filename\n");
    }
    return EXIT_SUCCESS;
}


