#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "scanner.h"

PSCANNER scanner;

int scanner_create(char * filename)
{
    scanner = (PSCANNER)malloc(sizeof(SCANNER));
    
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
    return NULL != scanner;
}

char scanner_get_next()
{
    return *(scanner->tmp_buffer)++;
}

int scanner_at_end()
{
    int chars_read = scanner->tmp_buffer - scanner->buffer;
    return chars_read >= 0 && chars_read >= scanner->buffer_size;
}

void scanner_print()
{
    assert(scanner);
    printf("%s", scanner->buffer);
}
