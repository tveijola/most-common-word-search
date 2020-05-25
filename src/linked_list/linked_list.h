#ifndef __LINKED_LIST_H
#define __LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include "../hash_table/hash_table.h"

typedef struct Node_struct Node;

struct Node_struct
{
	HashTableElement* element;
	Node *next;
};

Node* addToList   (HashTableElement* element, Node *head);
void  destroyList (Node *head);
void  printList   (Node *head);

#endif // __LINKED_LIST_H