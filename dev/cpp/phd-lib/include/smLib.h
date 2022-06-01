#if 0

#ifndef __SH_MEM_OPS_H__
#define __SH_MEM_OPS_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>  
#include <sys/shm.h> 
#include <errno.h>

typedef enum { REMORSE, NO_REMORSE } level ;

/* Function Prototypes */
void *shMemAttach( int shmid ) ;
int   shMemCheck( int size, int numAttach ) ;
int   shMemCreate( int size ) ;
int   shMemDestroy( int shmid, level remorse ) ;
int   shMemDetach( void * ) ;
int   shMemNumAttach( int ) ;

extern int errno ;

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#endif

#endif
