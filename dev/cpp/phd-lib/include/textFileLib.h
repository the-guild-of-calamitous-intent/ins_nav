#ifndef __textFileLib_h__
#define __textFileLib_h__

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef __STATUS
#define __STATUS
typedef enum { Func_Ok, Func_Error } status_t ;
#endif
/* Function Prototypes */
int    delimiterCount( char *s1, char delimiter, int count, 
                       int *index ) ;
void   readLine( FILE *fp, char *string ) ;
void   readComments( FILE *fp, char *line, char comment, int *lineCount ) ;

#endif
