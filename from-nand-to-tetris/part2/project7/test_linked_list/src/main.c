#include <stdlib.h>
#include "linked_list.h"

int main(void)
{
    PLINKEDLIST linked_list = linked_list_create();

    PNODEDATA node_data = node_data_create(1, "gino");

    PLINKEDLISTNODE list_node = linked_list_node_create(node_data);

    linked_list_add(linked_list, list_node);


    linked_list_print(linked_list);


    return EXIT_SUCCESS;
}
