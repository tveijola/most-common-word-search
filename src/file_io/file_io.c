#include "file_io.h"

FILE* openFile (char *filename)
{
	// Try to open specified file
	FILE *filepointer;
	char fullpath[strlen("input_files\\") + strlen(filename)];
	strcpy(fullpath, "input_files\\");
	strcat(fullpath, filename);
	
	filepointer = fopen(fullpath, "r");
	if (filepointer == NULL)
	{
		perror("Could not open file");
		return NULL;
	}
	return filepointer;
}

char* selectFile(char *filename)
{
	printf("\nChoose file.\n");
	
	char* path = "input_files";
	struct dirent *entry;
	char *entries[BUFFER_SIZE];
	
	DIR *dir;
	dir	= opendir(path);
	if (dir == NULL)
	{
		printf("Could not open dir");
		return NULL;
	}
	printf("----- Files -----\n");
	int i = 0;
	
	while((entry = readdir(dir)))
	{
		if (entry->d_type == 16) continue;
		if (strstr(entry->d_name, ".txt") == NULL) continue;
		
		char* file = (char*) calloc (1, sizeof(entry->d_name));
		strcpy(file, entry->d_name);
		entries[i] = file;
		printf("  %d. %s\n", i+1, entry->d_name);
		i++;
	}
	
	closedir(dir);
	
	char choice[BUFFER_SIZE];
	
	printf("\nSelect the file number and press ENTER\n> ");
	scanf("%s", choice);
	fflush(stdin);
	int selection = atoi(choice);
	
	while (selection == 0 || selection > i)
	{
		printf("Invalid input!\n> ");
		scanf("%s", choice);
		fflush(stdin);
		selection = atoi(choice);
	}
	printf("Selection: %d\n", selection);
	strcpy(filename, entries[selection-1]);
	
	// Free allocated memory
	for (int j = 0; j < i; j++)
	{
		free(entries[j]);
	}
	
	return filename;
}