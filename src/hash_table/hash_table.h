#ifndef __HASH_TABLE_H
#define __HASH_TABLE_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define TABLE_SIZE 40009

typedef struct HashTableElement_struct HashTableElement;

struct HashTableElement_struct
{
	char *word;
	unsigned int count;
	HashTableElement *next;
};

int hash(const char * str);

HashTableElement** initializeHashTable();
int addElementToHashTable (HashTableElement **hashTable, char *str);

int compareElements(HashTableElement *element1, HashTableElement *element2);

void printFillInfo   (HashTableElement **hashTable);
void printHashTable  (HashTableElement **hashTable);
void destroyHashTable(HashTableElement **hashTable);

int getWordCount();
int getUniqueWordCount();

#endif // HASH_TABLE_H