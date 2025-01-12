#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "interpreter.h"
#include <string.h>

int main(int argc, char * argv[])
{

    if (2 == argc)
    {
        char * filename = argv[1];
        PSCANNER scanner =  scanner_create(filename);
        char * out_filename = (char*)malloc(sizeof(char) * strlen(filename) + 2);
        strcpy(out_filename, filename);
        char *extension = strrchr(out_filename, '.');
        if (extension != NULL) 
            strcpy(extension, ".hack");

        if (NULL != scanner)
        {
            PLEXER lexer = lexer_create(scanner);
            if (NULL != lexer)
            {
                PPARSER parser = parser_create(lexer);

                if (NULL != parser)
                {
                    parser_parse(parser);
                    //parser_print(parser);

                    PINTERPRETER interpreter = interpreter_create(parser);

                    interpreter_interpret(interpreter);

                    interpreter_save_to_file(interpreter, out_filename);

                    parser_destroy(parser);
                }
                
            }
            scanner_destroy(scanner);
        }
        free(out_filename);
    }
    else
    {
        printf("usage: assembler filename\n");
    }
    return EXIT_SUCCESS;
}


