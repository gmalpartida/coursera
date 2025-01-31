#include <stdlib.h>
#include "linked_list.h"
#include <stdio.h>

int main(void)
{
	char * test_names[] = {"gino", "john", "michael", "james", "rose", "walter", "ruby",
							"jeanette", "antoine", "mary", "oscar", "cesar", "christina",
							"lilian", "veronique", "peter", "jeremy", "jesus", "mildred", "ruth"};
    PLINKEDLIST linked_list = linked_list_create();

	for (uint16_t i = 0; i < 20; i++)
	{
	    PNODEDATA node_data = node_data_create(i, test_names[i]);

    	PLINKEDLISTNODE list_node = linked_list_node_create(node_data);

    	linked_list_add(linked_list, list_node);
	}

    linked_list_print(linked_list);
	PNODEDATA node_data = node_data_create(0, "gino");
	if (linked_list_find(linked_list, node_data))
	{
		node_data_print(node_data);
		printf(" was found.\n");
	}
	else
	{
		node_data_print(node_data);
		printf(" was not found.\n");
	}

	
	if (linked_list_delete(linked_list, node_data_create(4, "rose")))
		printf("rose was deleted.\n");
	else
		printf("rose was not deleted.\n");

	if (linked_list_delete(linked_list, node_data_create(19, "ruth")))
		printf("ruth was deleted.\n");
	else
		printf("ruth was not deleted.\n");

	linked_list_add(linked_list, linked_list_node_create(node_data_create(20, "george")));
	linked_list_print(linked_list);

	PLINKEDLISTNODE node = linked_list_get(linked_list, node_data_create(18, "mildred"));

	if (node)
	{
		printf("mildred was found.\n");
	}
	else
		printf("mildred was not found.\n");

	linked_list_delete(linked_list, node_data_create(20, "george"));
	linked_list_print(linked_list);
	uint16_t i = 0;
	while(!linked_list_is_empty(linked_list))
	{
		linked_list_delete(linked_list, node_data_create(i, test_names[i]));
		i++;
		if (i> 20)
			break;
	}

	linked_list_print(linked_list);
	if (linked_list_is_empty(linked_list))
		printf("linked list is empty.\n");

    return EXIT_SUCCESS;
}
