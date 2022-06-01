#ifndef __signalLib_h__
#define __signalLib_h__

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>

/* Functions Prototypes */
int trapTerminate( void (*signalHandler)() ) ;
int sendSignal( pid_t pid, int signo ) ;
int setupAlarmSignal( float hertz ) ;
int setupSignalHandler( int signo, sigset_t mask, void (*signalHandler)() );
int clearMask( sigset_t *mask ) ;
int setMask( sigset_t *mask ) ;
int unmaskSignal( sigset_t *mask, int signo ) ;
int maskSignal( sigset_t *mask, int signo ) ;
int waitForSignal( int signo ) ;
int ignoreSignal( int signo ) ;
int killProcess( pid_t pid ) ;
int delay( time_t usecs ) ;


#endif
