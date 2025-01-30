#include "node_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

PNODEDATA node_data_create(uint16_t number, char * text)
{
    PNODEDATA node_data = (PNODEDATA)malloc(sizeof(NODEDATA));

    node_data->number = number;
    node_data->text = (char*)malloc(sizeof(char) * strlen(text) + 1);
    strcpy(node_data->text, text);

    return node_data;
}

void node_data_print(PNODEDATA node_data)
{
    printf("number: %d, text: %s", node_data->number, node_data->text);
}

void node_data_copy(PNODEDATA node_data_target, PNODEDATA node_data_src)
{
    node_data_target->number = node_data_src->number;
    if (node_data_target->text)
        free(node_data_target->text);
    node_data_target->text = (char*)malloc(sizeof(char) * strlen(node_data_src->text) + 1);
    strcpy(node_data_target->text, node_data_src->text);
}

bool node_data_is_equal(PNODEDATA node_data_target, PNODEDATA node_data_src)
{
    return node_data_target->number == node_data_src->number &&
            !strcmp(node_data_target->text, node_data_src->text);
}

void node_data_destroy(PNODEDATA node_data)
{
    free(node_data->text);
    free(node_data);
}

