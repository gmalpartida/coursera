#ifndef LINKED_LIST_NODE_H
#define LINKED_LIST_NODE

typedef struct NodeData * PNODEDATA;

typedef struct LinkedListNode * PLINKEDLISTNODE;

typedef struct LinkedListNode
{
    PNODEDATA data;
    PLINKEDLISTNODE next;
} LINKEDLISTNODE, *PLINKEDLISTNODE;

PLINKEDLISTNODE linked_list_node_create(PNODEDATA node_data);

void linked_list_node_print(PLINKEDLISTNODE linked_list_node);

void linked_list_node_destroy(PLINKEDLISTNODE linked_list_node);

#endif
