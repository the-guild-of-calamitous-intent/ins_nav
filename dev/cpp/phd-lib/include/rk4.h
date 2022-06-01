/*************************************************************************
rk4.h -- Runge-Kutta 4th order integration functions
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


#ifndef KEVINS_RUNGE_KUTTA
#define KEVINS_RUNGE_KUTTA

#include<math.h>
#include "matrix.h"

//! Runge Kutta data structure
typedef struct {
	 Vector *x; //!< state vector
	 Vector *u; //!< control effort
	 Vector *k1x;
	 Vector *k2x;
	 Vector *k3x;
	 Vector *k4x;
	 Vector *tmp;
	 Vector *tmp2;
	void (*model)( Vector*,
								 Vector*,
								 Vector*); //!< model of the dynamics
	double dt; //!< time step
	int size; //!< size of the state vector
} Runge_Kutta;


#ifdef __cplusplus
extern "C" {
#endif

//! setup
 Runge_Kutta* createRK4(
							 void (*model)(
							 		 Vector*,
							 		 Vector*,
							 		 Vector*),
							 double dt,
							 int x_size,
							 int u_size
							 );		
//! perform integration					
void integrateRK4( Runge_Kutta*);
//! free memory
void freeRK4( Runge_Kutta*);


#ifdef __cplusplus
}
#endif

#endif

