#include "parser.h"
#include <stdlib.h>
#include "lexer.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "symbol_table.h"

PPARSER parser_create(PLEXER lexer)
{
    PPARSER parser = (PPARSER)malloc(sizeof(PARSER));

    parser->lexer = lexer;

    parser->symbol_table = symbol_table_create();

    parser_build_symbol_table(parser);
    
    scanner_reset(lexer->scanner);

    parser->ast = (PAST)malloc(sizeof(AST));

    parser->ast->size = 0;

    parser->ast->ast_node = (PASTNODE*)malloc(sizeof(ASTNODE) * 32 * 1024);

    return parser;
}

void parser_destroy(PPARSER parser)
{
    free(parser->lexer);\
    free(parser);
    parser = NULL;
}

void parser_build_symbol_table(PPARSER parser)
{
    PSYMBOL symbol = NULL;
    for (int i = 0; i < 16; i++)
    {
        char * buffer = (char*)malloc(3);
        sprintf(buffer, "R%d", i);

        symbol = (PSYMBOL)malloc(sizeof (SYMBOL));
        symbol->symbol = buffer;
        symbol->value = i;
        symbol_table_add(parser->symbol_table, symbol);        
    }

    symbol = (PSYMBOL)malloc(sizeof(SYMBOL));
    symbol->symbol = (char*)malloc(3);       sprintf(symbol->symbol, "SP");     symbol->value = 0;
    symbol_table_add(parser->symbol_table, symbol);

    symbol = (PSYMBOL)malloc(sizeof(SYMBOL));
    symbol->symbol = (char*)malloc(4);       sprintf(symbol->symbol, "LCL");     symbol->value = 1;
    symbol_table_add(parser->symbol_table, symbol);

    symbol = (PSYMBOL)malloc(sizeof(SYMBOL));
    symbol->symbol = (char*)malloc(4);       sprintf(symbol->symbol, "ARG");     symbol->value = 2;
    symbol_table_add(parser->symbol_table, symbol);

    symbol = (PSYMBOL)malloc(sizeof(SYMBOL));
    symbol->symbol = (char*)malloc(5);       sprintf(symbol->symbol, "THIS");     symbol->value = 3;
    symbol_table_add(parser->symbol_table, symbol);

    symbol = (PSYMBOL)malloc(sizeof(SYMBOL));
    symbol->symbol = (char*)malloc(5);       sprintf(symbol->symbol, "THAT");     symbol->value = 4;
    symbol_table_add(parser->symbol_table, symbol);

    symbol = (PSYMBOL)malloc(sizeof(SYMBOL));
    symbol->symbol = (char*)malloc(7);       sprintf(symbol->symbol, "SCREEN");     symbol->value = 16384;
    symbol_table_add(parser->symbol_table, symbol);

    symbol = (PSYMBOL)malloc(sizeof(SYMBOL));
    symbol->symbol = (char*)malloc(4);       sprintf(symbol->symbol, "KBD");     symbol->value = 24576;
    symbol_table_add(parser->symbol_table, symbol);


    PTOKEN token = NULL;
    int instruction_index = -1;

    while (EOE != (token = lexer_peek(parser->lexer))->type)
    {
        if (token->type == ERROR)
        {
            printf("Error building symbol table.\n");
            token_print(token);
            printf("\n");
            exit(EXIT_FAILURE);
        }
       else if (OPEN_PAREN == token->type)
        {
            // read open parenthesis
            lexer_read(parser->lexer);

            token = lexer_read(parser->lexer);
            if (false == symbol_table_find(parser->symbol_table, token->text))
            {
                PSYMBOL symbol = (PSYMBOL)malloc(sizeof(SYMBOL));
                symbol->symbol = (char*)malloc(sizeof(char) * (strlen(token->text) + 1));
                strcpy(symbol->symbol, token->text);
                symbol->value = instruction_index+1;
                symbol_table_add(parser->symbol_table, symbol);
            }

            // read close parenthesis
            lexer_read(parser->lexer);
        }
        else if (AMPERSAND == token->type)
        {
            PASTNODE astnode = parser_parse_A_instruction(parser, false);
            //parser_print_astnode(astnode);
            free(astnode);
            instruction_index++;
        }
        else
        {
            PASTNODE astnode = parser_parse_C_instruction(parser);
            //parser_print_astnode(astnode);
            free(astnode);
            instruction_index++;
        }
    }

}

PASTNODE parser_parse_A_instruction(PPARSER parser, bool process_variables)
{
    PASTNODE astnode = (PASTNODE)malloc(sizeof(ASTNODE));

    astnode->A_instruction = (PAINSTRUCTION)malloc(sizeof(AINSTRUCTION));
    astnode->C_instruction = NULL;

    PTOKEN token = NULL;

    // consume the @
    token = lexer_read(parser->lexer);
    // peek at next token
    token = lexer_peek(parser->lexer);
    if (token->type == VALUE)
    {
        // consume token
        token = lexer_read(parser->lexer);
        astnode->A_instruction->value = atoi(token->text);
    }
    else if (token->type == IDENTIFIER)
    {
        // consume token
        token = lexer_read(parser->lexer);
        if (process_variables)
        {
            PSYMBOL symbol = symbol_table_get(parser->symbol_table, token->text);
            if (NULL == symbol)
            {
                // add variable
                symbol_table_add_variable(parser->symbol_table, token->text);
                symbol = symbol_table_get(parser->symbol_table, token->text);
            }
            astnode->A_instruction->value = symbol->value;
        }
    }
    else if (token->type == VREGISTER)
    {
        // consume token
        token = lexer_read(parser->lexer);
        PSYMBOL symbol = symbol_table_get(parser->symbol_table, token->text);
        astnode->A_instruction->value = symbol->value;
    }
    return astnode;
}

PASTNODE parser_parse_C_instruction(PPARSER parser)
{
    PASTNODE astnode = (PASTNODE)malloc(sizeof(ASTNODE));
    astnode->C_instruction = (PCINSTRUCTION)malloc(sizeof(CINSTRUCTION));
    astnode->A_instruction = NULL;

    astnode->C_instruction->dest = NULL;
    astnode->C_instruction->comp = NULL;
    astnode->C_instruction->jump = NULL;

    PTOKEN token = lexer_read(parser->lexer);

    if (VALUE == token->type)
    {
        // 0; jmp
        PTOKEN token2 = lexer_peek(parser->lexer);
        astnode->C_instruction->comp = (char*)malloc(sizeof(char) * strlen(token->text) + 1);
        strcpy(astnode->C_instruction->comp, token->text);

        if (SEMI_COLON == token2->type)
        {
            // read semicolon
            lexer_read(parser->lexer);
            // read jump
            //
            token2 = lexer_read(parser->lexer);

            astnode->C_instruction->jump = (char*)malloc(sizeof(char) * strlen(token2->text) + 1);
            strcpy(astnode->C_instruction->jump, token2->text);   
        }
    }
    else if (token->type == REGISTER || (0 == strcmp(token->text, "MD"))
            || (0 == strcmp(token->text, "AM")) || (0 == strcmp(token->text, "AD")) ||
                (0 == strcmp(token->text, "AMD")))
    {
        // process C instruction
        PTOKEN token2 = lexer_peek(parser->lexer);
        if (token2->type == EQUAL)
        {
            //process Dest
            astnode->C_instruction->dest = (char*)malloc(sizeof(char) * strlen(token->text) + 1);
            strcpy(astnode->C_instruction->dest, token->text);

            //consume the = 
            lexer_read(parser->lexer);
            // M = 0
            token = lexer_peek(parser->lexer);
            if (token->type == VALUE)
            {
                lexer_read(parser->lexer);
                astnode->C_instruction->comp = (char*)malloc(sizeof(char) * strlen(token->text) + 1);
                strcpy(astnode->C_instruction->comp, token->text);
            }
            else if (token->type == REGISTER)
            {
                // M = D

                token = lexer_read(parser->lexer);
                token2 = lexer_peek(parser->lexer);
                if (token2->type == PLUS || token2->type == MINUS || token2->type == OPERATOR)
                {
                    // consume plus or minus
                    token2 = lexer_read(parser->lexer);

                    PTOKEN token3 = lexer_read(parser->lexer);
                    
                    astnode->C_instruction->comp = (char*)malloc(sizeof(char) * 10);
                    sprintf(astnode->C_instruction->comp, "%s%s%s", token->text, token2->text, token3->text);
                    
                }
                else
                {
                    astnode->C_instruction->comp = (char*)malloc(sizeof(char) * strlen(token->text) + 1);
                    strcpy(astnode->C_instruction->comp, token->text);
                }
            }
            else if (token->type == MINUS)
            {
                // consume -
                token = lexer_read(parser->lexer);
                // read value
                token = lexer_read(parser->lexer);
                astnode->C_instruction->comp = (char*)malloc(sizeof(char) * strlen(token->text) + 2);
                sprintf(astnode->C_instruction->comp, "-%s",token->text );

            }
            else if (token->type == NOT)
            {
                token = lexer_read(parser->lexer);
                // read value
                PTOKEN token2 = lexer_read(parser->lexer);
                astnode->C_instruction->comp = (char*)malloc(sizeof(char) * (strlen(token->text) + strlen(token2->text) + 1));                          sprintf(astnode->C_instruction->comp, "%s%s", token->text, token2->text);
            }
            
        }
        else
        {
            astnode->C_instruction->comp = (char*)malloc(sizeof(char) * strlen(token->text) + 1);
            strcpy(astnode->C_instruction->comp, token->text);
            // read semi-colon
            token = lexer_read(parser->lexer);
            // read jump
            token = lexer_read(parser->lexer);
            astnode->C_instruction->jump = (char*)malloc(sizeof(char) * strlen(token->text) + 1);
            strcpy(astnode->C_instruction->jump, token->text);
        }
    }

    return astnode;
}

void parser_parse_label(PPARSER parser)
{
    // read open parenthesis
    PTOKEN token = lexer_read(parser->lexer);
    token_destroy(token);

    // read identifier
    token = lexer_read(parser->lexer);
    token_destroy(token);

    // read close parenthesis
    token = lexer_read(parser->lexer);
    token_destroy(token);

}

void parser_ast_add(PPARSER parser, PASTNODE astnode)
{
    parser->ast->ast_node[parser->ast->size] = astnode;
    parser->ast->size++;
}

void parser_print_astnode(PASTNODE astnode)
{
    if (NULL != astnode->A_instruction)
    {
        printf("@%d\n", astnode->A_instruction->value);
    }
    else
    {
        if (NULL != astnode->C_instruction->dest)
            printf("%s=", astnode->C_instruction->dest);
        printf("%s", astnode->C_instruction->comp);
        if (NULL != astnode->C_instruction->jump)
            printf(";%s", astnode->C_instruction->jump);
        printf("\n");
    }
}

void parser_print_ast(PPARSER parser)
{
    for (int i = 0; i < parser->ast->size; i++)
    {
        parser_print_astnode(parser->ast->ast_node[i]);
        
    }
}

void parser_print(PPARSER parser)
{
    symbol_table_print(parser->symbol_table);
    parser_print_ast(parser);
}

void parser_parse(PPARSER parser)
{
    PTOKEN token = NULL;
    while (EOE != (token = lexer_peek(parser->lexer))->type)
    {
        if (ERROR == token->type)
        {
            printf("Error parsing program.\n");
            token_print(token);
            printf("\n");
            exit(EXIT_FAILURE);
        }
        else if (AMPERSAND == token->type)
        {
            PASTNODE astnode = parser_parse_A_instruction(parser, true);
            parser_ast_add(parser, astnode);
            //parser_print_astnode(astnode); 
        }
        else if (OPEN_PAREN == token->type)
        {
            parser_parse_label(parser);
        }
        else
        {
            PASTNODE astnode = parser_parse_C_instruction(parser);
            parser_ast_add(parser, astnode);
            //parser_print_astnode(astnode);
        }
    }

}

