#ifndef SM_H
#define SM_H

#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <glib.h>

/*
    Compass structure

    heading     - heading of the compass (and the vehicle,
                  assuming they are still attached). Units
                  of degrees.
    roll, pitch - roll and pitch of compass (and the
                  vehicle assuming they are still attached).
                  Units of degrees.
    errors      - various errors associated with the compass
*/
#if 0                     /* moved to current struct, DKN 7/16 */
typedef estruct {
  float heading,
        roll,
        pitch ;
  char  eepromError1,
        eepromError2,
        cmdParameterInvalid,
        cmdInvalid,
        magOutOfRange,
        incOutOfRange,
        magDistortion ;
} compass_t ;
#endif


/*!
	State of vehicle
*/
typedef struct {
  double heading, roll, pitch ;
  double x,y,z;
  double xVel,yVel,zVel;
	double speed;
	double steering;
	float lat,lon,alt;
} state_t ;

/*!
  This holds all important data for the
  imu.
*/
typedef struct{
  float  heading_rate,
         roll,
         roll_rate,
         pitch,
         pitch_rate,
         x_accel,
         y_accel,
         z_accel;
  float dt;
  float imu_temp;
  int imu_bad_chk_sum;
  int new_data;
} imu_t;

/*!
  The data structure for the GPS.
*/
typedef struct{
  unsigned char mode;
	double lat;
	double lon;
	double alt;
	int num_sats;
} gps_t;
	
/*!
  This data structure holds all important
  data for the navigation process.
*/
typedef struct{
  double heading,
         heading_rate,
         roll,
         roll_rate,
         pitch,
         pitch_rate,
         x,
         x_velocity,
         x_accel,
         y,
         y_velocity,
         y_accel,
         z,
         z_velocity,
         z_accel;
  double bias[3];
  double drift[3];
  double dpos[3]; // kalman position correction
  double dvel[3]; // kalman velocity correction
  double datt[3]; // kalman attitude correction
  double q[4]; // attitude quaternion
	double dist;
} navigation_t;

enum {MSG_NOT_SET,MSG_CMP,MSG_IMU,MSG_DAT,MSG_CAM,
			MSG_SRV,MSG_NAV,MSG_GPS,MSG_SER};

#define MSG_SIZE 70

typedef struct {
	char msg[MSG_SIZE];
	char key;
	char ref;
} msglog_data_t;

typedef struct {
	GSList *list;
	char size;
  pthread_mutex_t msg_mutex;
} msglog_t;

/*!
	Status of a single thread
*/
enum {
	TS_EXIT,   // exit thread loop
	TS_STOP,   // stop
	TS_RUN,    // run
	TS_DEBUG,  // print out debug stuff
	TS_RESET   // reset to initial conditions
};

typedef struct {
	char status;
	msglog_data_t msg;
} thread_state_t;

#define UPDATE_STATUS( a, b )  a = status_str[((b)++)%sizeof(status_str)]
#define UPDATE_STATUS2( a, b )  a = status_str2[((b)++)%sizeof(status_str2)]

static char status_str[] = {'.','*','o','0','O','0','o','*'};
static char status_str2[] = {'+','-','\\','|','/','-'};

/*!
	Status of all threads running in the system
*/
typedef struct {
	thread_state_t datalog;
	thread_state_t camera;
	thread_state_t imu;
	thread_state_t compass;
	thread_state_t server;
	thread_state_t gps;
	thread_state_t nav;
	thread_state_t kf;
	thread_state_t msglog;
	thread_state_t sim;
	char client;
} system_state_t;

enum {GREY,RGB};

/*!
	Image from the camera
*/
typedef struct {
	unsigned char *buf;
	int w;
	int h;
	int format;
} image_t;

#define IMAGE_WIDTH 320
#define IMAGE_HEIGHT 240

enum {NONE,JOYSTICK,THREADS,CLOSE_CONNECTION,IMAGE,STATE,GPS,IMU};
			
typedef struct {
	int packet_type;
	int size;
} ipacket_t;

/* Data logger */
typedef struct {
	int sample_rate;
	int sample_number;
	struct timeval file_time; /*!< time file was started */
	long time_limit;          /*!< time limit before new file started */
	float start_time;
	char file_name[30];
} datalog_t;

/* Shared memory */
typedef struct {
  imu_t          imu;           // totally unfiltered nav data from imu 
  navigation_t   nav;           // coarse navigation
  state_t        current ;
	state_t        desired;
	image_t        pic;
	gps_t          gps;
	datalog_t      datalog;
	system_state_t status;        // status of running threads for debug
  pthread_mutex_t pic_mutex;	
} sm_t;

extern sm_t *sm;


#endif






































































