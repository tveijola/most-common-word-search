#include "linked_list.h"

// Global node count
int g_count = 0;
static const int max_size = 100;

Node* addToList (HashTableElement* element, Node *head)
{	
	g_count++;
	int node_num = 1;
	
	// Allocate memory for new Node
	Node *newNode = (Node*) calloc(1, sizeof(Node));
	// Set the element of the new node
	newNode->element = element;
	
	// If linked list is empty
	if (head == NULL)
	{
		// The created node is head
		newNode->next = head;
		head = newNode;
		return head;
	}
	
	// If new node should be placed first (newNode > head)
	if (compareElements(newNode->element, head->element) == 1)
	{
		// The created node is head
		newNode->next = head;
		head = newNode;
		return head;
	}
	
	// Start going through the list
	Node *prev = head;
	Node *node = head->next;
	while (node != NULL)
	{
		// If the word count is smaller than taht of the one hundreth word, return
		if (node_num >= max_size)
		{
			g_count--;
			free(newNode);
			return head;
		}
		// If the new Node count is greater than the current node in list, insert between
		if (compareElements(newNode->element, node->element) == 1)
		{
			newNode->next = node;
			prev->next = newNode;
			return head;
		}
		prev = node;
		node = node->next;
		node_num++;
	}
	
	// Reached end of list and node number is less than 100
	prev->next = newNode;
	
	return head;
}

void destroyList (Node *head)
{
	// Store the next node and free the previous
	Node *prev = head;
	Node *node = head->next;
	
	while(node != NULL)
	{
		free(prev);
		prev = node;
		node = node->next;
	}
	// Free the last node
	free(prev);
}

void printList (Node *head)
{
	int i = 2;
	//int max_words = 100000;
	if (head == NULL) return;
	printf("\n\tWORD  	   | OCCURRANCES\n");
	printf("\t------------------------\n");
	printf("1.\t%-10s | %d\n", head->element->word, head->element->count);
	
	Node *node = head->next;
	while (node != NULL && i <= max_size)
	{
		printf("%d.\t%-10s | %d\n", i, node->element->word, node->element->count);
		node = node->next;
		i++;
	}
	printf("\n");
}