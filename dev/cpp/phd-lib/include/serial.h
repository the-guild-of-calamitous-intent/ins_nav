/*  If GM had developed technology like Microsoft, we would all be  */
/*  driving  cars with the following characteristics:  */

/*  1.  For no reason whatsoever, your car would crash  twice a day.  */
/*  2.  Every time they repainted the lines in the road,  you would have */
/*          to buy a new car.  */
/*  3.  Occasionally your car would die on the freeway for  no reason. You */
/*          would have to pull over to the side of  the road, close all the  */
/*          windows, shut off the car,  restart it, and reopen the windows  */
/*          before you could continue. For some reason you would simply  */
/*          accept this.  */
/*  4.  Occasionally, executing a maneuver such as a left  turn would */
/*          cause your car to shut down and refuse to restart, in which  */
/*          case you would have to reinstall the engine.  */
/*  5.  Only one person at a time could use the car unless you bought  */
/*          "CarNT", but then you would have to buy more seats.  */
/*  6.  Macintosh would make a car that was powered by the sun, was  */
/*          reliable, five times as fast and twice as easy to drive - but  */
/*          would only run on five percent of  the roads.  */
/*  7.  The oil, water temperature, and alternator warning lights would */
/*          all be replaced by a single "General Protection Fault" warning light.  */
/*  8.  New seats would force everyone to have the same sized butt.  */
/*  9.  The airbag system would ask "are you sure?" before deploying.  */
/*  10. Occasionally, for no reason whatsoever, your car would lock you out */
/*          and refuse to let you in until you simultaneously lifted the door  */
/*          handle, turned the key and grabbed hold of the radio antenna.  */
/*  11. GM would require all car buyers to also purchase a  deluxe set of  */
/*          Rand McNally Road maps (Now a GM  subsidiary), even though they  */
/*          neither need nor want  them.  Attempting to delete this option  */
/*          would  immediately cause the cars performance to diminish by   */
/*          50% or more. Moreover, GM would become a target for investigation  */
/*          by the Justice Dept.  */
/*  12. Every time GM introduced a new car, car buyers would have to learn */
/*          to drive all over again because  none of the controls would operate  */
/*          it the same manner as the old car.  */
/*  13. You'd have to press the "Start" button to turn the engine off.  */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <termios.h>
//#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>


#ifndef SERIAL_H
#define SERIAL_H

#ifdef __cplusplus
extern "C" {
#endif

//enum {COM1,COM2,COM3,COM4,USB1,USB2,DevUSB1};

int open_serial_port(char*,int);
int print_speed(int fd);
int kbytesInInputBuffer( int portDescriptor );
int kbytesInOutputBuffer( int portDescriptor );

#ifdef __cplusplus
};
#endif

#endif
