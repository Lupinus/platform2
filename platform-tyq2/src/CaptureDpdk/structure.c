#include "structure.h"


 port_sort * creatNode(int speed, int port_id)
{
    port_sort * newnode=(struct port_sort*)malloc(sizeof(struct port_sort));
    newnode->Speed=speed;
	newnode->port_number = port_id;
    newnode->next=NULL;
    return newnode;
};

int addNode(port_sort * sort, int speed, int port_id) {
	port_sort * newnode = creatNode(speed, port_id);
	port_sort * dummy = creatNode(-1, -1);
	dummy->next = sort;
	port_sort * pre = dummy;
	port_sort * cur = sort;
	
	while(cur != NULL && cur->Speed > newnode->Speed) {
		cur = cur->next;
		pre = pre->next;
	}
	newnode->next = pre->next;
	pre->next = newnode;

	sort = dummy->next;

	return 0;
} 
