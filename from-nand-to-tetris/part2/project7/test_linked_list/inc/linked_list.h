#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "linked_list_node.h"
#include "node_data.h"

typedef struct LinkedList * PLINKEDLIST;

typedef struct LinkedList
{
    uint16_t size;
    PLINKEDLISTNODE head;
} LINKEDLIST, *PLINKEDLIST;

PLINKEDLIST linked_list_create();
void linked_list_print(PLINKEDLIST linked_list);
void linked_list_destroy(PLINKEDLIST linked_list);
void linked_list_add(PLINKEDLIST linked_list, PLINKEDLISTNODE linked_list_node);
bool linked_list_find(PLINKEDLIST linked_list, PNODEDATA node_data);
PLINKEDLISTNODE linked_list_get(PLINKEDLIST linked_list, PNODEDATA node_data);
bool linked_list_is_empty(PLINKEDLIST linked_list);

#endif
