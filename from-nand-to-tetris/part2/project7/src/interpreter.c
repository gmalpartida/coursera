#include "interpreter.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

PINTERPRETER interpreter_create(PPARSER parser)
{
    PINTERPRETER interpreter = (PINTERPRETER)malloc(sizeof(INTERPRETER));

    interpreter->parser = parser;

    interpreter->assembly_code = (char**)malloc(sizeof(char*) * 1024);
    interpreter->assembly_code_size = 0;

    return interpreter;
}

void interpreter_destroy(PINTERPRETER interpreter)
{
    free(interpreter);
    interpreter = NULL;
}

void interpreter_print(PINTERPRETER interpreter)
{
    for (uint16_t i = 0; i < interpreter->assembly_code_size; i++)
    {
        printf("%s\n", interpreter->assembly_code[i]);
    }
}

char * interpreter_get_static_variable_name(char * filename, char * suffix)
{
    char * static_variable_name = (char*)malloc(sizeof(char) * (strlen(filename) + strlen(suffix) + 1));

    char * r = strrchr(filename, '/');
    if (!r)
        r = filename;
    else
        r = r+1;

    strcpy(static_variable_name, r);

    char * p = strchr(static_variable_name, '.');
    if (p)
        strcpy(p+1, suffix);
    else
    {
        strcat(static_variable_name, ".");
        strcat(static_variable_name, suffix);
    }
    return static_variable_name;
}

char * interpreter_stack_op(PASTNODE astnode, char * filename)
{
    char * assembly_code = (char*)malloc(sizeof(char) * 256);

    if (!strcmp(astnode->op->text, "push"))
    {
        if (!strcmp(astnode->operand1->text, "constant"))
        {
            char * format_str = "// push constant i\n"
                                "@%s\n"
                                "D=A\n"
                                "@SP\n"
                                "A=M\n"
                                "M=D\n"
                                "@SP\n"
                                "M=M+1\n";

            sprintf(assembly_code, format_str, astnode->operand2->text);
        }
        else if (!strcmp(astnode->operand1->text, "local") || !strcmp(astnode->operand1->text, "argument") ||
                !strcmp(astnode->operand1->text, "this") || !strcmp(astnode->operand1->text, "that"))
        {
            // push segment i
             char * format_str = "// push segment i\n"
                                 "@%d        //@i\n"
                                 "D=A        // #D=i\n"
                                 "@%s        // @segment\n"
                                 "A=D+M      // A=LCL+i jump to that memory\n"
                                 "D=M        // D=LCL[i]\n"
                                 "@SP\n"
                                 "A=M        // jump to location of SP\n"
                                 "M=D        // RAM[*SP] = D\n"
                                 "@SP\n"
                                 "M=M+1      // SP++\n";

            sprintf(assembly_code, format_str, atoi(astnode->operand2->text), astnode->operand1->text);
        }
        else if (!strcmp(astnode->operand1->text, "static"))
        {

            char * format_str =  "// push static i\n"
                                 "@%s        //@file.i\n"
                                 "D=M        // D=RAM[var_name]\n"
                                 "@SP\n"
                                 "A=M        // jump to location of SP\n"
                                 "M=D        // RAM[*SP] = D\n"
                                 "@SP\n"
                                 "M=M+1      // SP++\n";

            char * static_var_name = interpreter_get_static_variable_name(filename, astnode->operand2->text);
            sprintf(assembly_code, format_str, static_var_name);
 
            free(static_var_name);
        }
        else if (!strcmp(astnode->operand1->text, "temp"))
        {
            char * format_str = "// push temp i\n"
                                "@%s\n"
                                "D=A\n"
                                "@5\n"
                                "A=D+A\n"
                                "D=M\n"
                                "@SP\n"
                                "A=M\n"
                                "M=D\n"
                                "@SP\n"
                                "M=M+1\n";

            sprintf(assembly_code, format_str, astnode->operand2->text);
 
        }
        else if (!strcmp(astnode->operand1->text, "pointer"))
        {
            char * format_str = "push pointer 0/1\n"
                                "%s\n"
                                "A=M\n"
                                "D=M\n"
                                "@SP\n"
                                "A=M\n"
                                "M=D\n"
                                "@SP\n"
                                "M=M+1\n";
            char this_that[5];
            if (!strcmp(astnode->operand2->text, "0"))
                strcpy(this_that, "THIS");
            else
                strcpy(this_that, "THAT");
            sprintf(assembly_code, format_str, this_that);
        }
        else
        {
            astnode_print(astnode);
            exit(EXIT_FAILURE);
        }
    }
    else if (!strcmp(astnode->op->text, "pop"))
    {
        if (!strcmp(astnode->operand1->text, "local") || !strcmp(astnode->operand1->text, "argument") ||
                !strcmp(astnode->operand1->text, "this") || !strcmp(astnode->operand1->text, "that"))
        {
            // pop segment i
            char * format_str = "// pop segment i\n"
                                "@%s\n"
                                "D=M\n"
                                "@%d\n"
                                "A=D+A\n"
                                "D=A\n"
                                "@R13\n"
                                "M=D\n"
                                "@SP\n"
                                "AM=M-1\n"
                                "D=M\n"
                                "@R13\n"
                                "A=M\n"
                                "M=D\n";

            sprintf(assembly_code, format_str, astnode->operand1->text, atoi(astnode->operand2->text));
        }
        else if (!strcmp(astnode->operand1->text, "static"))
        {
            char * format_str = "// pop static i\n"
                                "@SP\n"
                                "AM=M-1\n"
                                "D=M\n"
                                "@%s\n"
                                "M=D\n";

            char * static_var_name = interpreter_get_static_variable_name(filename, astnode->operand2->text);
            sprintf(assembly_code, format_str, static_var_name);
            
            free(static_var_name);
        }
        else if (!strcmp(astnode->operand1->text, "temp"))
        {
            char * format_str = "// pop temp i\n"
                                "@5\n"
                                "D=A\n"
                                "@%s\n"
                                "D=D+A\n"
                                "@R15\n"
                                "M=D\n"
                                "@SP\n"
                                "AM=M-1\n"
                                "D=M\n"
                                "@R15\n"
                                "A=M\n"
                                "M=D\n";

            sprintf(assembly_code, format_str, astnode->operand2->text );
        }
        else if (!strcmp(astnode->operand1->text, "pointer"))
        {
            char * format_str = "pop pointer 0/1\n"
                                "@SP\n"
                                "A=M\n"
                                "D=M\n"
                                "@%s\n"
                                "A=M\n"
                                "M=D\n"
                                "@SP\n"
                                "M=M-1\n";
            char this_that[5];
            if (!strcmp(astnode->operand2->text, "0"))
                strcpy(this_that, "THIS");
            else
                strcpy(this_that, "THAT");
            sprintf(assembly_code, format_str, this_that);
        }
        else 
        {
            astnode_print(astnode);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        astnode_print(astnode);
        exit(EXIT_FAILURE);
    }

    return assembly_code;
}

char * interpreter_arithmetic_op(PASTNODE astnode)
{
    if (!strcmp(astnode->op->text, "add"))
    {
        char * format_str = "// add\n"
                            "@SP\n"
                            "AM=M-1\n"
                            "D=M\n"
                            "A=A-1\n"
                            "M=M+D\n";
        char * assembly_code = (char*)malloc(sizeof(char) * strlen(format_str) + 1);

        strcpy(assembly_code, format_str);

        return assembly_code;
    }
    else if (!strcmp(astnode->op->text, "sub"))
    {
        char * format_str = "// sub\n"
                            "@SP\n"
                            "AM=M-1\n"
                            "D=M\n"
                            "A=A-1\n"
                            "M=M-D\n";
        char * assembly_code = (char*)malloc(sizeof(char) * strlen(format_str) + 1);
        strcpy(assembly_code, format_str);
        return assembly_code;
    }
    else if (!strcmp(astnode->op->text, "neg"))
    {
        char * format_str = "// neg\n"
                            "@SP\n"
                            "AM=M-1\n"
                            "D=M\n"
                            "D=-D\n"
                            "M=D\n";
        char * assembly_code = (char*)malloc(sizeof(char) * strlen(format_str) + 1);
        strcpy(assembly_code, format_str);
        return assembly_code;
    }
    else
    {
        astnode_print(astnode);
        exit(EXIT_FAILURE);
    }
}

char * interpreter_logical_op(PASTNODE astnode)
{
    char * assembly_code = (char*)malloc(sizeof(char) * 256);

    if (!strcmp(astnode->op->text, "eq") || !strcmp(astnode->op->text, "gt") || !strcmp(astnode->op->text, "lt")) 
    {
        char * format_str = "// eq | gt | lt\n"
                      "@SP\n"
                      "M=M-1\n"
                      "A=M\n"
                      "D=M\n"
                      "A=A-1\n"
                      "D=D-M\n"
                      "@SP\n"
                      "M=M-1\n"
                      "@EQUAL\n"
                      "D;%s\n"
                      "@SP\n"
                      "A=M\n"
                      "M=0\n"
                      "@END_EQ\n"
                      "0;JMP\n"
                      "(EQUAL)\n"
                      "@SP\n"
                      "A=M\n"
                      "M=1\n"
                      "(END_EQUAL)\n"
                      "@SP\n"
                      "M=M+1\n";

        char eq_gt_lt[4];
        if (!strcmp(astnode->op->text, "eq"))
            strcpy(eq_gt_lt, "JEQ");
        else if (!strcmp(astnode->op->text, "gt"))
            strcpy(eq_gt_lt, "JGT");
        else if (!strcmp(astnode->op->text, "lt"))
            strcpy(eq_gt_lt, "JLT");
        else
        {
            free(assembly_code);
            exit(EXIT_FAILURE);
        }
        sprintf(assembly_code, format_str, eq_gt_lt);
    }
    else if (!strcmp(astnode->op->text, "and") || !strcmp(astnode->op->text, "or"))
    {
        char * format_str = "// and | or\n"
                            "@SP\n"
                            "M=M-1\n"
                            "A=M\n"
                            "D=M\n"
                            "@SP\n"
                            "M=M-1\n"
                            "A=M\n"
                            "A=M\n"
                            "D=D%sA\n"
                            "@SP\n"
                            "A=M\n"
                            "M=D\n"
                            "@SP\n"
                            "M=M+1\n";
        char and_or[2];
        if (!strcmp(astnode->op->text, "and"))
            strcpy(and_or, "&");
        else if (!strcmp(astnode->op->text, "or"))
            strcpy(and_or, "|");
        else
        {
            free(assembly_code);
            exit(EXIT_FAILURE);
        }
        sprintf(assembly_code, format_str, and_or);
    }
    else if (!strcmp(astnode->op->text, "not"))
    {
        char * format_str = "// not\n"
                            "@SP\n"
                            "M=M-1\n"
                            "A=M\n"
                            "A=M\n"
                            "D=!A\n"
                            "@SP\n"
                            "A=M\n"
                            "M=D\n"
                            "@SP\n"
                            "M=M+1\n";
        strcpy(assembly_code, format_str);
    }
    else
    {
        free(assembly_code);
        astnode_print(astnode);
        exit(EXIT_FAILURE);
    }

    return assembly_code;
}

void interpreter_interpret(PINTERPRETER interpreter)
{
    parser_parse(interpreter->parser);
    for (uint16_t i = 0; i < interpreter->parser->ast->size; i++)
    {
        PASTNODE astnode = interpreter->parser->ast->astnode[i];
        if (astnode->op->type == STACK_OP)
        {
           interpreter->assembly_code[interpreter->assembly_code_size++] = interpreter_stack_op(astnode,
                                                                                        interpreter->parser->lexer->scanner->filename);
        }
        else if (astnode->op->type == ARITHMETIC_OP)
        {
            interpreter->assembly_code[interpreter->assembly_code_size++] = interpreter_arithmetic_op(astnode);
        }
        else if (astnode->op->type == LOGICAL_OP)
        {
            interpreter->assembly_code[interpreter->assembly_code_size++] = interpreter_logical_op(astnode);
        }
    }
}



