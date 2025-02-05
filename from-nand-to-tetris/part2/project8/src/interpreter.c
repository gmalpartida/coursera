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

char * interpreter_translate_segment(char * segment)
{
    if (!strcmp(segment, "local"))
        return "LCL";
    else if (!strcmp(segment, "argument"))
        return "ARG";
    else if (!strcmp(segment, "this"))
        return "THIS";
    else if (!strcmp(segment, "that"))
        return "THAT";
    else if (!strcmp(segment, "0"))
        return "THIS";
    else if (!strcmp(segment, "1"))
        return "THAT";
    return "";
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
            char * format_str = "// %s %s %s\n"
                                "@%s\n"
                                "D=A\n"
                                "@SP\n"
                                "A=M\n"
                                "M=D\n"
                                "@SP\n"
                                "M=M+1\n";

            sprintf(assembly_code, format_str, astnode->op->text, astnode->operand1->text, astnode->operand2->text, astnode->operand2->text);
        }
        else if (!strcmp(astnode->operand1->text, "local") || !strcmp(astnode->operand1->text, "argument") ||
                !strcmp(astnode->operand1->text, "this") || !strcmp(astnode->operand1->text, "that"))
        {
            // push segment i
             char * format_str = "// %s %s %s\n"
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

            sprintf(assembly_code, format_str, astnode->op->text, astnode->operand1->text, astnode->operand2->text, atoi(astnode->operand2->text), interpreter_translate_segment(astnode->operand1->text));
        }
        else if (!strcmp(astnode->operand1->text, "static"))
        {

            char * format_str =  "// %s %s %s\n"
                                 "@%s        //@file.i\n"
                                 "D=M        // D=RAM[var_name]\n"
                                 "@SP\n"
                                 "A=M        // jump to location of SP\n"
                                 "M=D        // RAM[*SP] = D\n"
                                 "@SP\n"
                                 "M=M+1      // SP++\n";

            char * static_var_name = interpreter_get_static_variable_name(filename, astnode->operand2->text);
            sprintf(assembly_code, format_str, astnode->op->text, astnode->operand1->text, astnode->operand2->text, static_var_name);
 
            free(static_var_name);
        }
        else if (!strcmp(astnode->operand1->text, "temp"))
        {
            char * format_str = "// %s %s %s\n"
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

            sprintf(assembly_code, format_str, astnode->op->text, astnode->operand1->text, astnode->operand2->text, astnode->operand2->text);
 
        }
        else if (!strcmp(astnode->operand1->text, "pointer"))
        {
            char * format_str = "// %s %s %s\n"
                                "@%s\n"
                                "A=M\n"
                                "D=A\n"
                                "@SP\n"
                                "A=M\n"
                                "M=D\n"
                                "@SP\n"
                                "M=M+1\n";
            sprintf(assembly_code, format_str, astnode->op->text, astnode->operand1->text, astnode->operand2->text, interpreter_translate_segment(astnode->operand2->text));
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
            char * format_str = "// %s %s %s\n"
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

            sprintf(assembly_code, format_str, astnode->op->text, astnode->operand1->text, astnode->operand2->text, interpreter_translate_segment(astnode->operand1->text), atoi(astnode->operand2->text));
        }
        else if (!strcmp(astnode->operand1->text, "static"))
        {
            char * format_str = "// %s %s %s\n"
                                "@SP\n"
                                "AM=M-1\n"
                                "D=M\n"
                                "@%s\n"
                                "M=D\n";

            char * static_var_name = interpreter_get_static_variable_name(filename, astnode->operand2->text);
            sprintf(assembly_code, format_str, astnode->op->text, astnode->operand1->text, 
                                                                astnode->operand2->text, static_var_name);
            
            free(static_var_name);
        }
        else if (!strcmp(astnode->operand1->text, "temp"))
        {
            char * format_str = "// %s %s %s\n"
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

            sprintf(assembly_code, format_str, astnode->op->text, astnode->operand1->text, astnode->operand2->text, astnode->operand2->text );
        }
        else if (!strcmp(astnode->operand1->text, "pointer"))
        {
            char * format_str = "// %s %s %s\n"
                                "@SP\n"
                                "M=M-1\n"
                                "A=M\n"
                                "D=M\n"
                                "@%s\n"
                                "M=D\n";
            sprintf(assembly_code, format_str, astnode->op->text, astnode->operand1->text, astnode->operand2->text, interpreter_translate_segment(astnode->operand2->text));
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
                            "M=D+M\n";
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
                            "A=M-1\n"
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
	static uint16_t label_index = 0;
    char * assembly_code = (char*)malloc(sizeof(char) * 256);

    if (!strcmp(astnode->op->text, "eq") || !strcmp(astnode->op->text, "gt") || !strcmp(astnode->op->text, "lt")) 
    {
		label_index++;
        char * format_str = "// %s\n"
                      "@SP\n"
                      "M=M-1\n"
                      "A=M\n"
                      "D=M\n"
                      "A=A-1\n"
                      "D=M-D\n"
                      "@SP\n"
                      "M=M-1\n"
                      "@EQUAL%d\n"
                      "D;%s\n"
                      "@SP\n"
                      "A=M\n"
                      "M=-1\n"
                      "@END_EQUAL%d\n"
                      "0;JMP\n"
                      "(EQUAL%d)\n"
                      "@SP\n"
                      "A=M\n"
                      "M=0\n"
                      "(END_EQUAL%d)\n"
                      "@SP\n"
                      "M=M+1\n";

        char eq_gt_lt[4];
        if (!strcmp(astnode->op->text, "eq"))
            strcpy(eq_gt_lt, "JNE");
        else if (!strcmp(astnode->op->text, "gt"))
            strcpy(eq_gt_lt, "JLE");
        else if (!strcmp(astnode->op->text, "lt"))
            strcpy(eq_gt_lt, "JGE");
        else
        {
            free(assembly_code);
            exit(EXIT_FAILURE);
        }
        sprintf(assembly_code, format_str, astnode->op->text, label_index, eq_gt_lt, label_index, label_index, label_index);
    }
    else if (!strcmp(astnode->op->text, "and") || !strcmp(astnode->op->text, "or"))
    {
        char * format_str = "// %s\n"
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
        sprintf(assembly_code, format_str, astnode->op->text, and_or);
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

char * interpreter_branch_op(PASTNODE astnode)
{
    char * assembly_code = (char*)malloc(sizeof(char) * 256);
    if (!strcmp(astnode->op->text, "label"))
    {
        char * format_str = "// %s %s\n"
                            "(%s)\n";
        sprintf(assembly_code, format_str, astnode->op->text, astnode->operand1->text, astnode->operand1->text);
    }
    else if (!strcmp(astnode->op->text, "goto"))
    {
        char * format_str = "// %s %s\n"
                            "@%s\n"
                            "0;JMP\n";
        sprintf(assembly_code, format_str, astnode->op->text, astnode->operand1->text, astnode->operand1->text);
    }
    else if (!strcmp(astnode->op->text, "if-goto"))
    {
        char * format_str = "// %s %s\n"
                            "@SP\n"
                            "AM=M-1\n"
                            "D=M\n"
                            "@%s\n"
                            "D;JNE\n";
        sprintf(assembly_code, format_str, astnode->op->text, astnode->operand1->text, astnode->operand1->text);
    }
    else
    {
        free(assembly_code);
        astnode_print(astnode);
        exit(EXIT_FAILURE);
    }
    return assembly_code;
}

char * interpreter_function_op(char * filename, PASTNODE astnode)
{
	static uint16_t return_index = 0;
	static uint16_t function_label_index = 0;
    char * assembly_code = (char*)malloc(sizeof(char) * 256);

    if (!strcmp(astnode->op->text, "function"))
    {
		char * format_str = "// %s %s %s\n"
							"(%s.%s)\n"
							"@%s\n"
							"D=A\n"
							"(loop_%d)\n"
							"@loop_end_%d\n"
							"D;JEQ\n"
							"@SP\n"
							"A=M\n"
							"M=0\n"
							"@SP\n"
							"M=M+1\n"
							"D=D-1\n"
							"@loop_%d\n"
							"0;JMP\n"
							"(loop_end_%d)\n";

		sprintf(assembly_code, format_str, astnode->op->text, astnode->operand1->text, astnode->operand2->text,
				filename, astnode->operand1->text, astnode->operand2->text, function_label_index, function_label_index,
				function_label_index, function_label_index);
    }
    else if (!strcmp(astnode->op->text, "call"))
    {
		return_index++;
		char * format_str = "// %s %s %s\n"
							"@%s$ret.%d\n"
							"D=A\n"
							"@SP\n"
							"A=M\n"
							"M=D\n"
							"@SP\n"
							"M=M+1\n"
							"@LCL\n"
							"D=M\n"
							"@SP\n"
							"A=M\n"
							"M=D\n"
							"@ARG\n"
							"D=M\n"
							"@SP\n"
							"A=M\n"
							"M=D\n"
							"@THIS\n"
							"D=M\n"
							"@SP\n"
							"A=M\n"
							"M=D\n"
							"@THAT\n"
							"D=M\n"
							"@SP\n"
							"A=M\n"
							"M=D\n"
							"@SP\n"
							"D=M\n"
							"@5\n"
							"D=D-A\n"
							"@%s\n"
							"D=D-A\n"
							"@ARG\n"
							"M=D\n"
							"@SP\n"
							"D=M\n"
							"@LCL\n"
							"M=D\n"
							"@%s\n"
							"0;JMP\n"
							"(%s$ret.%d)\n";
	sprintf(assembly_code, format_str, astnode->op->text, astnode->operand1->text, astnode->operand2->text,
				astnode->operand1->text, return_index, astnode->operand1->text, astnode->operand2->text, astnode->operand1->text, return_index);
    }
    else if (!strcmp(astnode->op->text, "return"))
    {
		char * format_str = "// %s\n"
                            "@LCL\n"
                            "D=M\n"
                            "@endFrame\n"
                            "M=D\n"
                            "D=M\n"
                            "@5\n"
                            "D=D-A\n"
                            "@retAddr\n"
                            "M=D\n"
                            "@SP\n"
                            "AM=M-1\n"
                            "D=M\n"
                            "@ARG\n"
                            "M=D\n"
                            "D=A\n"
                            "D=D+1\n"
                            "@SP\n"
                            "M=D\n"
                            "@endFrame\n"
                            "D=A\n"
                            "D=D-1\n"
                            "@THAT\n"
                            "M=D\n"
                            "D=D-1\n"
                            "@THIS\n"
                            "M=D\n"
                            "D=D-1\n"
                            "@ARG\n"
                            "M=D\n"
                            "D=D-1\n"
                            "@LCL\n"
                            "M=D\n"
                            "@retAddr\n"
                            "0;JMP\n";

		sprintf(assembly_code, format_str, astnode->op->text);
		return_index--;
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
    char * filename = (char*)malloc(sizeof(char) * strlen(interpreter->parser->lexer->scanner->filename) + 1);
    strcpy(filename, interpreter->parser->lexer->scanner->filename);

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
        else if (astnode->op->type == BRANCH_OP)
        {
            interpreter->assembly_code[interpreter->assembly_code_size++] = interpreter_branch_op(astnode);
        }
        else if (astnode->op->type == FUNCTION_OP)
        {
            interpreter->assembly_code[interpreter->assembly_code_size++] = interpreter_function_op(interpreter->parser->lexer->scanner->filename, astnode);
        }
    }
}


void interpreter_save_to_file(PINTERPRETER interpreter, char * filename)
{
    FILE *fptr = fopen(filename, "a");

    if (fptr)
    {
        for (uint16_t i = 0; i < interpreter->assembly_code_size; i++)        
        {
            fputs(interpreter->assembly_code[i], fptr);
        }
    }
    fclose(fptr);
}



