#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "node.h"
#include <stdbool.h>

typedef void(*DATA_PRINT)(void * data);
typedef bool(*DATA_COMPARE)(void * from_data, void * to_data);
typedef void(*DATA_DESTROY)(void * data);

typedef struct _LINKED_LIST
{
	PNODE head;

	DATA_PRINT data_print;
	DATA_COMPARE data_compare;
	DATA_DESTROY data_destroy;
} LINKED_LIST, *PLINKED_LIST;

PLINKED_LIST linked_list_create(DATA_PRINT data_print, DATA_COMPARE data_compare, DATA_DESTROY data_destroy);

void linked_list_destroy(PLINKED_LIST linked_list);

void linked_list_add(PLINKED_LIST linked_list, PNODE node);

void linked_list_print(PLINKED_LIST linked_list);

PNODE linked_list_find(PLINKED_LIST linked_list, PNODE node);

void linked_list_delete(PLINKED_LIST linked_list, PNODE node);

void linked_list_clear(PLINKED_LIST linked_list);

#endif
