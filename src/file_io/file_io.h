#ifndef __FILE_IO_H
#define __FILE_IO_H

#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>

#define BUFFER_SIZE 128

FILE* openFile (char *filename);
char* selectFile(char *filename);

#endif // FILE_IO_H


