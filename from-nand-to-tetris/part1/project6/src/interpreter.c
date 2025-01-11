#include "interpreter.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

PINTERPRETER interpreter_create(PPARSER parser)
{
    PINTERPRETER interpreter = (PINTERPRETER)malloc(sizeof(INTERPRETER));

    interpreter->parser = parser;

    interpreter->op_codes = (char**)malloc(sizeof(char*) * 256);
    interpreter->size = 0;
    return interpreter;
}

void interpreter_destroy(PPARSER parser)
{
    free(parser);
    parser = NULL;
}

void interpreter_reverse(char str[], int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
}

char* interpreter_itoa(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;
 
    /* Handle 0 explicitly, otherwise empty string is
     * printed for 0 */
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    // In standard itoa(), negative numbers are handled
    // only with base 10. Otherwise numbers are
    // considered unsigned.
    if (num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    interpreter_reverse(str, i);
 
    return str;
}

void interpreter_interpret(PINTERPRETER interpreter)
{
    for (int i = 0; i < interpreter->parser->ast->size; i++)
    {
        PASTNODE astnode = interpreter->parser->ast->ast_node[i];

        char op_code[17] = {0};
        if (astnode->A_instruction)
        {
            interpreter_itoa(astnode->A_instruction->value, op_code, 2);
            op_code[16] = '\0';

            interpreter->op_codes[interpreter->size]= (char*)malloc(sizeof(char) * 17);
            strcpy(interpreter->op_codes[interpreter->size], op_code);
            interpreter->size++;
        }
        else
        {
            char a = '1';
            char prefix[4];         strcpy(prefix, "111");
            char dest[4];           strcpy(dest, "000");
            char jump[4];           strcpy(jump, "000");
            char comp[7];           strcpy(comp, "000000");
            if (astnode->C_instruction->dest)
            {
                if (!strcmp(astnode->C_instruction->dest, "M"))
                    strcpy(dest, "001");
                else if (!strcmp(astnode->C_instruction->dest, "D"))
                    strcpy(dest, "010");
                else if (!strcmp(astnode->C_instruction->dest, "MD"))
                    strcpy(dest, "011");
                else if (!strcmp(astnode->C_instruction->dest, "A"))
                    strcpy(dest, "100");
                else if (!strcmp(astnode->C_instruction->dest, "AM"))
                    strcpy(dest, "101");
                else if (!strcmp(astnode->C_instruction->dest, "AD"))
                    strcpy(dest, "110");
                else if (!strcmp(astnode->C_instruction->dest, "AMD"))
                    strcpy(dest, "111");
            }
            else if (astnode->C_instruction->comp)
            {
                if (!strcmp(astnode->C_instruction->comp, "0"))
                {
                    strcpy(comp, "101010");
                    a = '0';
                }
                else if (!strcmp(astnode->C_instruction->comp, "1"))
                {
                    strcpy(comp, "111111");
                    a = '0';
                }
                else if (!strcmp(astnode->C_instruction->comp, "-1"))
                {
                    strcpy(comp, "111010");
                    a = '0';
                }
                else if (!strcmp(astnode->C_instruction->comp, "D"))
                {
                    strcpy(comp, "001100");
                    a = '0';
                }
                else if (!strcmp(astnode->C_instruction->comp, "A"))
                {
                    strcpy(comp, "110000");
                    a = '0';
                }
                else if (!strcmp(astnode->C_instruction->comp, "!D"))
                {
                    strcpy(comp, "001101");
                    a = '0';
                }
                else if (!strcmp(astnode->C_instruction->comp, "!A"))
                {
                    strcpy(comp, "110001");
                    a = '0';
                }
                else if (!strcmp(astnode->C_instruction->comp, "-D"))
                {
                    strcpy(comp, "001111");
                    a = '0';
                }
                else if (!strcmp(astnode->C_instruction->comp, "-A"))
                {
                    strcpy(comp, "110011");
                    a = '0';
                }
                else if (!strcmp(astnode->C_instruction->comp, "D+1"))
                {
                    strcpy(comp, "011111");
                    a = '0';
                }
                else if (!strcmp(astnode->C_instruction->comp, "A+1"))
                {
                    strcpy(comp, "110111");
                    a = '0';
                }
                else if (!strcmp(astnode->C_instruction->comp, "D-1"))
                {
                    strcpy(comp, "001110");
                    a = '0';
                }
                else if (!strcmp(astnode->C_instruction->comp, "A-1"))
                {
                    strcpy(comp, "110010");
                    a = '0';
                }
                else if (!strcmp(astnode->C_instruction->comp, "D+A"))
                {
                    strcpy(comp, "000010");
                    a = '0';
                }
                else if (!strcmp(astnode->C_instruction->comp, "D-A"))
                {
                    strcpy(comp, "010011");
                    a = '0';
                }
                else if (!strcmp(astnode->C_instruction->comp, "A-D"))
                {
                    strcpy(comp, "000111");
                    a = '0';
                }
                else if (!strcmp(astnode->C_instruction->comp, "D&A"))
                {
                    strcpy(comp, "000000");
                    a = '0';
                }
                else if (!strcmp(astnode->C_instruction->comp, "D|A"))
                {
                    strcpy(comp, "010101");
                    a = '0';
                }
                else if (!strcmp(astnode->C_instruction->comp, "M"))
                {
                    strcpy(comp, "110000");
                    a = '1';
                }
                else if (!strcmp(astnode->C_instruction->comp, "!M"))
                {
                    strcpy(comp, "110001");
                    a = '1';
                }
                else if (!strcmp(astnode->C_instruction->comp, "-M"))
                {
                    strcpy(comp, "110011");
                    a = '1';
                }
                else if (!strcmp(astnode->C_instruction->comp, "M+1"))
                {
                    strcpy(comp, "110111");
                    a = '1';
                }
                else if (!strcmp(astnode->C_instruction->comp, "M-1"))
                {
                    strcpy(comp, "110010");
                    a = '1';
                }
                else if (!strcmp(astnode->C_instruction->comp, "D+M"))
                {
                    strcpy(comp, "000010");
                    a = '1';
                }
                else if (!strcmp(astnode->C_instruction->comp, "D-M"))
                {
                    strcpy(comp, "010011");
                    a = '1';
                }
                else if (!strcmp(astnode->C_instruction->comp, "M-D"))
                {
                    strcpy(comp, "000111");
                    a = '1';
                }
                else if (!strcmp(astnode->C_instruction->comp, "D&M"))
                {
                    strcpy(comp, "000000");
                    a = '1';
                }
                else if (!strcmp(astnode->C_instruction->comp, "D|M"))
                {
                    strcpy(comp, "010101");
                    a = '1';
                }
                    
            }
            else if (astnode->C_instruction->jump)
            {
                if (!strcmp(astnode->C_instruction->jump, "JGT"))
                    strcpy(jump, "001");
                else if (!strcmp(astnode->C_instruction->jump, "JEQ"))
                    strcpy(jump, "010");
                else if (!strcmp(astnode->C_instruction->jump, "JGE"))
                    strcpy(jump, "011");
                else if (!strcmp(astnode->C_instruction->jump, "JLT"))
                    strcpy(jump, "100");
                else if (!strcmp(astnode->C_instruction->jump, "JNE"))
                    strcpy(jump, "101");
                else if (!strcmp(astnode->C_instruction->jump, "JLE"))
                    strcpy(jump, "110");
                else if (!strcmp(astnode->C_instruction->jump, "JMP"))
                    strcpy(jump, "111");
            }
            interpreter->op_codes[interpreter->size] = (char*)malloc(sizeof(char)*17);
            sprintf(interpreter->op_codes[interpreter->size], "%s%c%s%s%s", prefix, a, dest, comp, jump);
            interpreter->size++;
        }
    }
}
    
void interpreter_save_to_file(PINTERPRETER interpreter, char * filename)
{
    FILE *fptr = fopen(filename, "w");
    for (int i = 0; i < interpreter->size; i++)
    {
        fprintf(fptr, "%s\n", interpreter->op_codes[i]);
    }
    fclose(fptr);
}




