#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "token.h"
#include "lexer.h"
#include "token.h"
#include "parser.h"
#include "interpreter.h"
#include <string.h>

int main(int argc, char * argv[])
{
    if (2 == argc)
    {
        char * filename = argv[1];
        PSCANNER scanner =  scanner_create(filename);

        char * out_filename = (char*)malloc(sizeof(char) * strlen(filename) + 1);
        strcpy(out_filename, filename);
        char *p = strrchr(out_filename, '.');
        strcpy(p, ".asm");

        if (NULL != scanner)
        {
            PLEXER lexer = lexer_create(scanner);
            if (NULL != lexer)
            {
                PPARSER parser = parser_create(lexer);
                if (NULL != parser)
                {
                    PINTERPRETER interpreter = interpreter_create(parser);
                    if (NULL != interpreter)
                    {
                        interpreter_interpret(interpreter);
                        interpreter_save_to_file(interpreter, out_filename);
                        interpreter_destroy(interpreter);
                    }
                    parser_destroy(parser);
                }
            }
            scanner_destroy(scanner);
        }
        free(out_filename);
    }
    else
    {
        printf("usage: VMTranslator filename\n");
    }
    return EXIT_SUCCESS;
}


