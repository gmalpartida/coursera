#include "node.h"
#include <stddef.h>
#include <stdlib.h>

PNODE node_create(void * data, PNODE next)
{
	PNODE new_node = (PNODE)malloc(sizeof(NODE));

	new_node->data = data;
	new_node->next = next;

	return new_node;
}

void node_destroy(PNODE node)
{
	free(node);
}


