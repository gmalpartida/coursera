#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"

typedef struct Interpreter
{
    char ** op_codes;
    int size;
    PPARSER parser;
} INTERPRETER, *PINTERPRETER;

PINTERPRETER interpreter_create(PPARSER parser);

void interpreter_destroy(PPARSER parser);

void interpreter_print(PINTERPRETER interpreter);

void interpreter_interpret(PINTERPRETER interpreter);

void interpreter_save_to_file(PINTERPRETER interpreter, char * filename);

void interpreter_print(PINTERPRETER interpreter);

#endif
