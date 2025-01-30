#ifndef NODE_DATA_H
#define NODE_DATA_H

#include <stdint.h>
#include <stdbool.h>

typedef struct NodeData
{
    uint16_t number;
    char * text;
} NODEDATA, *PNODEDATA;

PNODEDATA node_data_create(uint16_t number, char * text);

void node_data_print(PNODEDATA node_data);

void node_data_copy(PNODEDATA node_data_target, PNODEDATA node_data_src);

bool node_data_is_equal(PNODEDATA node_data_target, PNODEDATA node_data_src);

void node_data_destroy(PNODEDATA node_data);

#endif
