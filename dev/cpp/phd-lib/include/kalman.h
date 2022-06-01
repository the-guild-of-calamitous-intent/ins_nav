/*************************************************************************
kalman.h -- kalman filter program
Copyright (C) 2000 Free Software Foundation, Inc.
Written by Kevin J Walchko <walchko@ufl.edu>
**************************************************************************
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
**************************************************************************/

#ifndef KEVINS_KALMAN_FILTER
#define KEVINS_KALMAN_FILTER

#include <math.h>
#include "matrix.h"

//!\name KalmanFilter
//@{

/*!
Kalman filter flags
*/
enum KF_Flags {	KF_LINEAR_MODEL = 1, /*!< Linear system model */
		KF_NONLINEAR_MODEL = 2, /*!< Nonlinear system model */
		KF_DISCRETE 	= 4, /*!< Discrete Kalman Filter */
		KF_CONTINUOUS	= 8, /*!< Continuous Kalman Filter */
		KF_DISC_MODEL	= 16, /*!< Discrete system model */
		KF_CONT_MODEL	= 32 /*!< Continuous system model */
		};



/*!
Kalman filter structure which contains all important variables and
temporary matricies.
*/
typedef struct {
  Matrix *Ac;  /*!< continuous state trans. matrix */
  Matrix *Ad;  /*!< discrete state trans. matrix */
  Matrix *Bc;  /*!< gain matrix */
  Matrix *Bd;  /*!< gain matrix */
  Matrix *H;  /*!< output matrix */
  Matrix *D;  /*!< direct transmition matrix */
  Vector *u;  /*!< control effort */

  Vector *z;  /*!< measured state (noisy) */
  Matrix *Q;  /*!< process noise covariance */
  Matrix *R;  /*!< measured noise covariance */
  Matrix *P;  /*!< error covariance */
  Matrix *k;  /*!< kalman gain */
  Vector *xk;  /*!< model state */
  Vector *x_est;  /*!< noiseless state estimate */

  float dt;  /*!< time step (cont. kalman filter)	 */
  int flags;  /*!< Kalman Filter Flags */
  int size;   /*!< size of state vector */
  int u_size;

  void (*model)( // model(cont. kalman filter)
		Vector*, // input state vector
		Vector*, // control effort
		Vector*); // output state vector

//---- misc. and temporary data	--------------------------
  Matrix *eye;  /*!< identity matrix */
  Vector *linstate;  /*!< used for linearization */
  Vector *ulinstate;
  Matrix *AcT; /*!< transpose of Ac */
  Matrix *AdT;  /*!< transpose of Ad */
  Matrix *HT;  /*!< transpose of H */
  Matrix *P2;  /*!< temp P */

  Vector *tmpu;
  Vector **tmpv; /*!< temp vector */
  Matrix **tmpm; /*!< temp matrix */
  Matrix **tmpB; /*!< temp matrix for B */
} Kalman_Filter;


#ifdef __cplusplus
extern "C" {
#endif
	
Kalman_Filter* createKalmanFilter(
       void (*model)(
		     Vector *input_state,
		     Vector *control_effort,
		     Vector *output_state), /* nonlinear continuous model dynamics */
       Matrix *A,          /* state matrix */
       Matrix *B,          /* input matrix */
       Matrix *H,          /* continuous output matrix */
       Matrix *D,          /* direct transfer matrix */
       Matrix *Q,          /* process noise */
       Matrix *R,          /* measurement noise */
       Vector *initState,  /* initial states for kalman filter */
       float dt,                  /* time step (for linearization) */
       int state_size,            /* size of state vector */
       int u_size,                /* size of control effort */
       int KF_Flags               /* Kalman Filter Flags */
       );

	
	
void freeKalmanFilter( Kalman_Filter*);
int kalman( Kalman_Filter*);
void continousKalmanFilter( Kalman_Filter*);
void discreteKalmanFilter( Kalman_Filter*);
void C2D( Kalman_Filter*);
void linearizeKalmanFilter( Kalman_Filter*);
void int_X(Kalman_Filter *kf);
void int_P(Kalman_Filter *kf);
void xmodel(Vector*,Kalman_Filter*,Vector*);
void pmodel(Matrix*,Kalman_Filter*,Matrix*);
	
//@}

#ifdef __cplusplus
};
#endif

#endif
