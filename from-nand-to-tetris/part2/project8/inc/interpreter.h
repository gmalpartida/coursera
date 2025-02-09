#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"

typedef struct Interpreter
{
    char ** assembly_code;
    uint16_t assembly_code_size;
    PPARSER parser;
} INTERPRETER, *PINTERPRETER;

PINTERPRETER interpreter_create(PPARSER parser);
void interpreter_destroy(PINTERPRETER interpreter);
void interpreter_print(PINTERPRETER interpreter);
void interpreter_interpret(PINTERPRETER interpreter, bool do_bootstrap);
void interpreter_save_to_file(PINTERPRETER interpreter, char * filename);

#endif
