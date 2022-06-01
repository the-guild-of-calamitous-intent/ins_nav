#ifndef __PORT_OPS_H__
#define __PORT_OPS_H__

#include <sgtty.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <termios.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define IN_BUF    0
#define OUT_BUF   1
#define BOTH_BUF  2

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* milli seconds, aka mS */
#define SEC 1e3

typedef int port_t ;  /* port type */
#ifndef __STATUS
#define __STATUS
typedef enum {Func_Ok, Func_Error } status_t ;
#endif

typedef enum { COM1=0, COM2=1, COM3=2, COM4=3 } serial_t ;
typedef enum { PARALLEL=0, PRINTER=1, PTR0=2, PTYP1=3, 
               ENCODER0=4 } parallel_t ;

/* Functions Prototypes */
int      bytesInInputBuffer( port_t portDescriptor ) ;
int      bytesInOutputBuffer( port_t portDescriptor ) ;
void     clearToSend( port_t portDescriptor) ;
int      closePort( port_t portDescriptor) ;
status_t findStartMsg( port_t portDescriptor, u_char startOfMsg ) ;
int      flushBuf( port_t portDescriptor, int buf ) ;
port_t   initParallelPort( parallel_t whichPort ) ;
port_t   initPort( int whichPort, int baud, char *control ) ;
int      readBuf( port_t portDescriptor, void *buf, int nBytes ) ;
status_t waitForData( port_t portDescriptor, int waitTime, int *nChar ) ;
status_t waitForNdata( port_t portDescriptor, int waitTime, int nChar ) ;
status_t waitForResponse( port_t portDescriptor, u_char waitChar, int time ) ;
void     waiting( int mSec ) ;
int      writeBuf( port_t portDescriptor, void *buf, int nBytes ) ;

#endif
