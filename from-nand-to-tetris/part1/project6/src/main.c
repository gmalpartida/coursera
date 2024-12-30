#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"

int main(int argc, char * argv[])
{

    if (2 == argc)
    {
        char * filename = argv[1];
        scanner_create(filename);
        scanner_print();

        while (!scanner_at_end()){
            char c = scanner_get_next();
            printf("%c", c);
        }
    }
    else
    {
        printf("usage: assembler filename\n");
    }
    return EXIT_SUCCESS;
}


