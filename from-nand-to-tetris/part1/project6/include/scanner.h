#ifndef SCANNER_H
#define SCANNER_H


typedef struct Scanner{
    char * buffer;
    char * tmp_buffer;
    size_t buffer_size;
} SCANNER, *PSCANNER;


int scanner_create(char * filename);
char scanner_get_next();
int scanner_at_end();
void scanner_print();

#endif
