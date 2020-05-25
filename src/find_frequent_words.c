/*
 * Program to find the 100 most common words that appear
 * in a large text file.
 *
 * Tommi Veijola
 * 27.11.2019
 *
 */
 
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <dirent.h>
#include <stdlib.h>
#include "hash_table/hash_table.h"
#include "word_mod/word_mod.h"
#include "linked_list/linked_list.h"
#include "file_io/file_io.h"

#define BUFFER_SIZE 128

int showMenu(char *filename);
Node* findMostCommonWords(HashTableElement **hashTable);
void fillHashTable (HashTableElement **hashTable, FILE *filepointer);
int outputMostCommonWords(FILE *fp_out, Node *head);

int main (int argc, char **argv)
{	
	char filename[BUFFER_SIZE] = "";
	
	// If a command line argument was given, use that
	if (argv[1] != NULL) strcpy(filename, argv[1]);
	int execute = 0;
	
	while(1)
	{
		while (!execute)
		{
			// Show menu
			execute = showMenu(filename);
		}
		
		// Get start time
		clock_t t = clock();
		
		// Open the specified text file
		FILE *filepointer = openFile(filename);
		
		// If the file could not be found/opened
		if (filepointer == NULL)
		{
			execute = 0;
			// Clear the filename
			strcpy(filename, "");
			continue;
		}			
		
		// Initialize hash table
		HashTableElement** hashTable = initializeHashTable();
		
		// Reads the file and stores words and occurances into hashtable
		fillHashTable(hashTable, filepointer);
		
		printf("----- RESULTS -----\n");
		printFillInfo(hashTable);
		
		// Find the 100 most common words from the hash table
		Node* head = findMostCommonWords(hashTable);
		
		// Time difference
		t = clock() - t;
		
		// Time in seconds
		double cpu_time_used = ((double) t) / CLOCKS_PER_SEC;
		
		printf("Program took %f seconds to execute.\n", cpu_time_used);
		
		// Create output file
		char filename_out[strlen(filename) + strlen("output_")];
		strcpy(filename_out, "output_");
		strcat(filename_out, filename);
		FILE *fp_out = fopen(filename_out, "w");
		
		// Write information to file
		fprintf(fp_out, "Execution time: %.3f seconds.\n", cpu_time_used);
		fprintf(fp_out, "Total word count: %d.\n", getWordCount());
		fprintf(fp_out, "Unique word count: %d.\n\n", getUniqueWordCount());
		fprintf(fp_out, "The 100 most common words:\n\n");
		
		// Write most common words to file
		outputMostCommonWords(fp_out, head);
		
		// Close file stream
		fclose(fp_out);
		
		// Free memory
		destroyHashTable(hashTable);
		destroyList(head);
		
		execute = 0;
	}
	printf("Press any key to exit program...\n> ");
	getch();
	return 0;
}

int showMenu(char *filename)
{
	int haveFile = 0;
	int selection = 0;
	
	// If filename is empty
	if (strcmp(filename, "") == 0)
	{
		printf("No filename provided.\n");
		selection = 2;
	}
	else
	{
		haveFile = 1;
		printf("\n+++++++++++++++++++++++++++++++++++++++++++++\n");
		printf("+CURRENT FILENAME: %s.\n", filename);
		printf("+++++++++++++++++++++++++++++++++++++++++++++\n\n");
	}
	
	// If have file, get choice from user
	if (haveFile)
	{
		printf("Choose option.\n  1. Proceed with current file.\n  2. Choose another file.\n");
		printf("  3. Exit program.\n> ");
		char choice = getch(); printf("%c\n", choice);
		while (choice > 51 || choice < 49)
		{
			printf("Invalid input! Input one number only. [1 - 3]\n> ");
			choice = getch(); printf("%c\n", choice);
		}
		selection = choice - '0';
	}
	
	switch(selection)
	{
		// Proceed with current file
		case 1:
			return 1;
			break;
		// Choose another file
		case 2:
			filename = selectFile(filename);
			return 0;
			break;
		// Exit
		case 3:
			exit(0);
			break;
		default:
			perror("Selection invalid.");
			break;
	}
	return 0;
}

int appendBackslash(char *str)
{
	size_t len = strlen(str);
	// If last char is not backslash, append one
	if (str[len-1] != '\\') strcat(str, "\\");
	return 1;
}

void fillHashTable (HashTableElement** hashTable, FILE *filepointer)
{
	// Buffer to read a word to
	char buffer[BUFFER_SIZE];
	char delims[] = "-";
	while (fscanf(filepointer, "%s", buffer) == 1)
	{
		// Split word
		char *token = strtok(buffer, delims);
		while( token != NULL ) {
			// Word to lower case
			toLowerCase(token);
			// Remove extra characters from word
			removeExtraCharacters(token);
			// If we have something left after operations, add word to hashtable
			if (token[0] != 0)
				addElementToHashTable(hashTable, token);
			// Get next token
			token = strtok(NULL, delims);
		}
	}
	// Close the file
	fclose(filepointer);
}

Node* findMostCommonWords(HashTableElement **hashTable)
{
	// Head of linked list
	Node *head = NULL;
	
	// For each slot in hash table
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		// If slot is empty, go to next one
		if (hashTable[i] == NULL) 
		{
			continue;
		}
		
		// Get the element from slot
		HashTableElement* current = hashTable[i];
		
		// Add the element to linked list and update head if needed
		head = addToList(current, head);
		
		// Get the next linked element
		HashTableElement* link = current->next;
		
		// While there is an element linked, add to linked list
		while(link != NULL)
		{
			current = link;
			head = addToList(current, head);
			link = current->next;
		}
	}
	
	// Print 100 most common words
	printList(head);
	return head;
}

int outputMostCommonWords(FILE *fp_out, Node *head)
{
	// If head of list is null, return
	if (head == NULL)
	{
		return 0;
	}
	
	// Index of word
	int n = 1;
	// String representation of index
	char n_str[8];
	sprintf(n_str, "%d.", n);
	
	// Print head info
	fprintf(fp_out, "%-5s | %-10s | %s\n", "N", "WORD", "COUNT");
	fprintf(fp_out, "--------------------------\n");
	fprintf(fp_out, "%-5s | %-10s | %d\n", n_str, head->element->word, head->element->count);
	
	n++;
	Node *curr = head->next;
	
	// While there is a next node in list and index <= 100
	while(curr != NULL && n <= 100)
	{
		// Print info
		sprintf(n_str, "%d.", n);
		fprintf(fp_out, "%-5s | %-10s | %d\n", n_str, curr->element->word, curr->element->count);
		curr = curr->next;
		n++;
	}
	
	return 1;
}