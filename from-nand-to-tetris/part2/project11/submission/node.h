#ifndef NODE_H
#define NODE_H

typedef struct _NODE
{
	void* data;
	struct _NODE * next;

} NODE, *PNODE; 

PNODE node_create(void * data, PNODE next);

void node_destroy(PNODE node);

#endif
