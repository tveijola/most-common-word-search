#include "hash_table.h"

// Global variables
unsigned int g_conflicts = 0;
unsigned int g_word_count = 0;
unsigned int g_unique_words = 0;
unsigned int g_empty_buckets = TABLE_SIZE;

int g_words = 0;
int g_elements = 0;

int hash(const char * str)
{
	// Hash function from:
	// http://www.cse.yorku.ca/~oz/hash.html
	
	unsigned long value = 5381;
	for (int i = 0; i < strlen(str); i++)
	{
		value = ((value << 5) + value) + str[i];
	}
	// Return calculated index
	return value % TABLE_SIZE;
}

HashTableElement** initializeHashTable()
{
	// Create empty hashtable and return pointer to it
	HashTableElement **hashTable = (HashTableElement**) calloc(TABLE_SIZE, sizeof(HashTableElement*));
	return hashTable;
}

int addElementToHashTable (HashTableElement **hashTable, char *str)
{
	g_word_count++;
	int index = hash(str);
	
	if (hashTable[index] == NULL)
	{	
		/* New word, table is empty at index */
		char *new_word = (char*) calloc (1, (strlen(str)+1)*sizeof(char));
		strcpy(new_word, str);
		
		HashTableElement* el = (HashTableElement*) calloc (1, sizeof(HashTableElement));
		el->count = 1;
		el->word  = new_word;
		
		hashTable[index] = el;
		g_unique_words++; g_words++;
		g_empty_buckets--; g_elements++;
		return 1;
	}
	
	/* If got to here, table is not empty at index */
	HashTableElement* current = hashTable[index];
	
	/* If the input word matches the word at table index, increment count */
	if (strcmp(current->word, str) == 0)
	{
		current->count++;
		return 1;
	}
	
	/* The input word does not match the word at table index 
	 * Go through the linked list to find the word, or if last
	 * element is NULL, add new element
	 */
	else 
	{
		/* Get the next element */
		HashTableElement* link = current->next;
		
		/* Go through the linked list */
		while(link != NULL)
		{
			current = link;
			if (strcmp(current->word, str) == 0)
			{
				current->count++;
				return 1;
			}
			/* If the words don't match, keep looking*/
			link = current->next;
		}
		
		/* Here we have reached the end of the linked list without
		 * finding a match for the input word. Therefore, create
		 * a new element and add it to the end
		 */
		
		char *new_word = (char*) calloc (1, (strlen(str)+1)*sizeof(char));
		strcpy(new_word, str);
	
		HashTableElement* el = (HashTableElement*) calloc (1, sizeof(HashTableElement));
		el->count = 1;
		el->word  = new_word;
		
		/* Set the element to be next from current */
		current->next = el;
		g_conflicts++; g_words++;
		g_unique_words++; g_elements++;
		return 1;
	}
	return 0;
}

/*
 * Return values:
 *	-1 -> element1 < element2
 *	 0 -> element1 = element2
 *   1 -> element1 > element2
 *	-9 -> Error
 */
int compareElements(HashTableElement *element1, HashTableElement *element2)
{
	if (element1->count < element2->count) return -1;
	else if (element1->count > element2->count) return 1;
	else if (element1->count == element2->count) return 0;
	return -9;
}

void printFillInfo (HashTableElement **hashTable)
{
	printf("Word count:   %d\n", g_word_count);
	printf("Unique words: %d\n", g_unique_words);
	printf("\nNumber of conflicts: %d\n", g_conflicts);
	printf("Number of empty buckets: %d\n", g_empty_buckets);
	
	float load_factor = (float)(TABLE_SIZE - g_empty_buckets) / (float)TABLE_SIZE;
	printf("\nLoad factor: %.2f\n", load_factor);
}

void printHashTable(HashTableElement **hashTable)
{
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		if (hashTable[i] == NULL)
		{
			printf("%d. EMPTY\n", i);
		}
		else
		{
			HashTableElement* current = hashTable[i];
			HashTableElement* link    = current->next;
			
			printf("%d. %s[%d]", i, current->word, current->count);
			while(link != NULL)
			{
				printf(" -> %s[%d]", link->word, link->count);
				current = link;
				link = link->next;
			}
			printf("\n");
		}	
	}
}

void destroyHashTable(HashTableElement **hashTable)
{
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		if (hashTable[i] == NULL) continue;
		
		HashTableElement* previous = hashTable[i];
		HashTableElement* current  = previous->next;
		
		while(current != NULL)
		{
			free(previous->word);
			free(previous);
			g_words--;
			g_elements--;
			previous = current;
			current  = current->next;
		}
		free(previous->word);
		free(previous);
		g_words--;
		g_elements--;
	}
	free(hashTable);
	
	g_conflicts = 0;
	g_word_count = 0;
	g_unique_words = 0;
	g_empty_buckets = TABLE_SIZE;
}

int getWordCount()
{
	return g_word_count;
}
int getUniqueWordCount()
{
	return g_unique_words;
}