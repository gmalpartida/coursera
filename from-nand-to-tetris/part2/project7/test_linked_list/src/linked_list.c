#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>

PLINKEDLIST linked_list_create()
{
    PLINKEDLIST linked_list = (PLINKEDLIST)malloc(sizeof(LINKEDLIST));

    linked_list->head = NULL;
    linked_list->size = 0;

    return linked_list;
}    

void linked_list_print(PLINKEDLIST linked_list)
{
    PLINKEDLISTNODE cur = linked_list->head;
    while(cur)
    {
        linked_list_node_print(cur);
        printf("\n");
        cur = cur->next;
    }
}

void linked_list_destroy(PLINKEDLIST linked_list)
{
    PLINKEDLISTNODE cur = NULL;
    
    if (linked_list->head)
    {
        cur = linked_list->head;
        PLINKEDLISTNODE prev = NULL;
        while (cur)
        {
            prev = cur;
            cur = cur->next;
            linked_list_node_destroy(prev);
        }
    }
    free(linked_list);    
}

void linked_list_add(PLINKEDLIST linked_list, PLINKEDLISTNODE linked_list_node)
{
    linked_list_node->next = NULL;
    if (NULL == linked_list->head)
        linked_list->head = linked_list_node;
    else
    {
        PLINKEDLISTNODE cur = linked_list->head;
        while(cur)
        {
            if (!cur->next)
            {
                cur->next = linked_list_node;
                break;
            }
            cur = cur->next;
        }
    }
}

bool linked_list_find(PLINKEDLIST linked_list, PNODEDATA node_data)
{
    bool result = false;
    PLINKEDLISTNODE cur = linked_list->head;
    while(cur)
    {
        if (node_data_is_equal(cur->data, node_data))
        {
            result = true;
            break;
        }
        cur = cur->next;
    }
    return result;
}


PLINKEDLISTNODE linked_list_get(PLINKEDLIST linked_list, PNODEDATA node_data)
{
    PLINKEDLISTNODE found = NULL;
    PLINKEDLISTNODE cur = linked_list->head;
    while(cur)
    {
        if (node_data_is_equal(cur->data, node_data))
        {
            found = cur;
            break;
        }
        cur = cur->next;
    }
    return found;
}

bool linked_list_is_empty(PLINKEDLIST linked_list)
{
    return linked_list->head == NULL;
}


