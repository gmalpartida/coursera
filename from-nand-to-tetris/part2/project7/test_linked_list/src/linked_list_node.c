#include "linked_list_node.h"
#include "node_data.h"
#include <stdlib.h>

PLINKEDLISTNODE linked_list_node_create(PNODEDATA node_data)
{
    PLINKEDLISTNODE new_node = (PLINKEDLISTNODE)malloc(sizeof(LINKEDLISTNODE));

    new_node->data = node_data_create(node_data->number, node_data->text);

    return new_node;
}

void linked_list_node_print(PLINKEDLISTNODE linked_list_node)
{
    node_data_print(linked_list_node->data);
}

void linked_list_node_destroy(PLINKEDLISTNODE linked_list_node)
{
    node_data_destroy(linked_list_node->data);
    free(linked_list_node);
}
