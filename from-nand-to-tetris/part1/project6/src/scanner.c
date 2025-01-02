#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "scanner.h"

PSCANNER scanner_create(char * filename)
{
    PSCANNER scanner = (PSCANNER)malloc(sizeof(SCANNER));
    
    if (NULL != scanner)
    {
        scanner->buffer = scanner->tmp_buffer = NULL;
        scanner->buffer_size = 0;
        char * file_content = NULL;
        FILE * f = fopen(filename, "r");
        if (NULL != f){
            fseek(f, 0, SEEK_END);
            long fsize = ftell(f);
            fseek(f, 0, SEEK_SET);

            file_content = malloc(fsize + 1);
            if (NULL != file_content){
                fread(file_content, fsize, 1, f);
                fclose(f);
                scanner->buffer_size = fsize;
                scanner->buffer = scanner->tmp_buffer = file_content;
            }
            else{
                free(scanner);
                scanner = NULL;
            }
        }
        else
        {
            free(scanner);
            scanner = NULL;
        }
    }
    return scanner;
}

char scanner_get_next(PSCANNER scanner)
{
    return *(scanner->tmp_buffer)++;
}

int scanner_at_end(PSCANNER scanner)
{
    int chars_read = scanner->tmp_buffer - scanner->buffer;
    return chars_read >= 0 && chars_read >= scanner->buffer_size;
}

void scanner_print(PSCANNER scanner)
{
    assert(scanner);
    printf("%s", scanner->buffer);
}

void scanner_destroy(PSCANNER scanner)
{
    free(scanner->buffer);
    free(scanner);
    scanner = NULL;
}

int scanner_position(PSCANNER scanner)
{
    return (scanner->tmp_buffer - scanner->buffer);
}

char scanner_peek_next(PSCANNER scanner)
{
    return *(scanner->tmp_buffer);
}

