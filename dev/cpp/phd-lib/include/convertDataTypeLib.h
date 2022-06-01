#ifndef __convertDataTypeLib_h__
#define __convertDataTypeLib_h__

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef enum { BIG_END, LITTLE_END } memory_t ;
#ifndef __STATUS
#define __STATUS
typedef enum { OK, ERROR } status ;
#endif

#include "textFileLib.h"

float htonf(float);
float ntohf(float);
double htond(double);
double ntohd(double);

void     convertDataFrom( void *convert, int cnvSize, void *value, int size,
                          memory_t memory ) ; 
void     convertDataTo( void *convert, int cnvSize, void *value, int size, 
                        memory_t memory ) ;
memory_t determineMemoryType( void ) ;
void     packData( char *dataString, int *index, void *data, int size ) ;
void     parseStatus( char *statusStr, unsigned char status[], 
                      int statusSize ) ;
void     unpackData( char *dataString, int *index, void *data, int size ) ;

#endif
