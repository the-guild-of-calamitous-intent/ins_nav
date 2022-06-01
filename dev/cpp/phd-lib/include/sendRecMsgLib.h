#ifndef __SEND_REC_MSG_H__
#define __SEND_REC_MSG_H__

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "portOpsLib.h"
#include "convertDataTypeLib.h"

#define ASCII_MSG

typedef enum { ASCII, HEX } dispType ;

#if !( defined(ASCII_MSG) || defined(BINARY_MSG) )
#error ASCII_MSG or BINARY_MSG must be defined to compile
#endif

#if ( defined(ASCII_MSG) && defined(BINARY_MSG) )
#error Only define ASCII_MSG or BINARY_MSG, not both
#endif

/* Maximum ammount of data to transmit in one packet. */
#define DATA_OVERFLOW 255

/* A maximum value */
#define HEADER_SIZE 40

/* A minimum value */
#define HEADER_MINIMUM 11

#ifdef ASCII_MSG
typedef struct _message {
  u_int   MsgID ;       /* in Hex                  */
  char    Dest[4] ;     /* Destination designation */
  char    Source[4] ;   /* Source designation      */
  char    Vehicle[4] ;  /* Vehicle designation     */
  int     dataStatus ;  /* Status of data packet   */
  u_int   dataSize ;    /* Number of bytes of data */
  char   *data ;        /* actual data             */
} message_t ;

typedef struct _packetData {
  char *data ;
  int totalDataSize ;
  int currentDataSize ;
  FILE *file ;
} packetData_t ;

#define START_MSG 0x02
#define END_MSG   0x03

#define HEADER_COMMA_COUNT 6

#endif  /* end of #ifdef ASCII_MSG */

#ifdef BINARY_MSG
typedef struct _message {
  u_int    MsgID ;      /* in Hex                      */
  u_char   Dest ;       /* Hex Destination designation */
  u_char   Source ;     /* Hex Source designation      */
  u_char   Vehicle ;    /* Hex Vehicle designation     */
  u_short  Reserved ;   /* Reserved                    */
  u_char   dataStatus ; /* Status of data packet       */
  u_int    dataSize ;   /* Number of bytes of data     */
  void    *data ;       /* actual data                 */
} message_t ;

typedef struct _packetData {
  void *data ;
  int totalDataSize ;
  int currentDataSize ;
  FILE *file ;
} packetData_t ;

#define START_MSG1 0x55
#define START_MSG2 0xAA

#endif  /* end of #ifdef BINARY_MSG */

#define FULL_DATA  0
#define START_DATA 1
#define CONT_DATA  2
#define END_DATA   3

typedef struct _messageString {
  u_char *message ;
  u_int   numMessage ;
  u_char  head[HEADER_SIZE+1] ;
  u_char *data ;
} messageString_t ;


/* Functions Prototypes */
u_char   calculateByteChecksum( void *data, int dataSize ) ;
void     clearMessage( message_t *message ) ;
void     clearMsgString( messageString_t *message ) ;
status_t copyData( messageString_t *message, u_char *from, int size ) ;
int      delimiterCount( char *s1, char delimiter, int count, int *index ) ; 
void     initMessage( message_t *message ) ;
void     initMsgString( messageString_t *messageStr ) ;
void     printMAXmsg( message_t *messgePtr, dispType format ) ;
status_t recASCIImsg( port_t portDescriptor, messageString_t *messageStr,
                      char start_of_message, char end_of_message ) ;
status_t recMAXmsg( port_t portDescriptor, message_t *message ) ;
status_t sendMAXmsg( port_t portDescriptor, message_t *messagePtr ) ;

#endif
