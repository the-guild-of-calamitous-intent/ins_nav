
#ifndef FILEIO_H
#define FILEIO_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif

	int readALine( FILE *fp, char *line, int size, char comment );
	FILE* openFile(char *file, char *mode);
	int write2File(char *name,int len);

#ifdef __cplusplus
};
#endif

#endif
