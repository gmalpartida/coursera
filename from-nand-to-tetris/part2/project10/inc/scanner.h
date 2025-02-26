#ifndef SCANNER_H
#define SCANNER_H

#include <stddef.h>

typedef struct Scanner{
    char * buffer;
    char * tmp_buffer;
    size_t buffer_size;
    char * filename;
} SCANNER, *PSCANNER;


PSCANNER scanner_create(char * filename);
char scanner_get_next(PSCANNER scanner);
int scanner_at_end(PSCANNER scanner);
void scanner_print(PSCANNER scanner);
void scanner_destroy(PSCANNER scanner);
int scanner_position(PSCANNER scanner);
char scanner_peek_next(PSCANNER scanner);
void scanner_reset(PSCANNER scanner);
void scanner_set_position(PSCANNER scanner, int position);                
char scanner_peek2_next(PSCANNER scanner);

#endif
