#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ioctl.h>

#ifndef ESC
#define ESC 0x1B
#endif

typedef enum _escKey {
  KEY_ESC,
  F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
  Up_Arrow, Down_Arrow, Right_Arrow, Left_Arrow,
  KEY_1, KEY_3, KEY_5, KEY_7, KEY_9,
  KEY_UNKN
} escKey_t ;

escKey_t decodeESC( int choice ) ;
escKey_t decodeKeybad( int choice ) ;
void     getInput( char *string, int size ) ;
int      readKeyboard( void ) ;

#endif
