#include "linked_list.h"
#include <stdlib.h>

PLINKED_LIST linked_list_create(DATA_PRINT data_print, DATA_COMPARE data_compare, DATA_DESTROY data_destroy)
{
	PLINKED_LIST linked_list = (PLINKED_LIST)malloc(sizeof(LINKED_LIST));

	linked_list->head = NULL;

	linked_list->data_print = data_print;
	linked_list->data_compare = data_compare;
	linked_list->data_destroy = data_destroy;

	return linked_list;
}

void linked_list_destroy(PLINKED_LIST linked_list)
{
	linked_list_clear(linked_list);

	free(linked_list);
}

void linked_list_add(PLINKED_LIST linked_list, PNODE node)
{
	PNODE cur = linked_list->head;

	if (NULL == cur)
	{
		linked_list->head = node;
	}
	else
	{
		PNODE prev = NULL;
		while (cur)
		{
			prev = cur;
			cur = cur->next;
		}
		// prev points to last node in the list
		prev->next = node;		
	}
}

void linked_list_print(PLINKED_LIST linked_list)
{
	PNODE cur = linked_list->head;

	while (cur)
	{
		if (linked_list->data_print)
			linked_list->data_print(cur->data);
		cur = cur->next;
	}
}

PNODE linked_list_find(PLINKED_LIST linked_list, PNODE node)
{
	PNODE cur = NULL;

	if (linked_list->data_compare)
	{
		cur = linked_list->head;

		while (cur)
		{
			if (linked_list->data_compare(cur->data, node->data))
			{
				break;
			}
			cur = cur->next;
		}
	}
	return cur;
}

void linked_list_delete(PLINKED_LIST linked_list, PNODE node)
{
	if (linked_list->data_compare)
	{
		PNODE prev = NULL, cur = linked_list->head;

		while (cur)
		{
			if (linked_list->data_compare(cur->data, node->data))
			{
				if (NULL == prev)
				{
					linked_list->head = cur->next;
				}
				else
				{
					prev->next = cur->next;
				}
				node_destroy(cur);
				break;
			}
			prev = cur;
			cur = cur->next;
		}
	}
}

void linked_list_clear(PLINKED_LIST linked_list)
{
	PNODE cur = linked_list->head;
	PNODE prev = NULL;

	while (cur)
	{
		prev = cur;
		cur = cur->next;
		if (linked_list->data_destroy)
			linked_list->data_destroy(prev->data);
		node_destroy(prev);
	}
	linked_list->head = NULL;
}

