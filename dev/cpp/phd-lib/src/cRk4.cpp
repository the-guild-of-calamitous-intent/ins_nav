/*************************************************************************
cRk4.cpp - Rung-Kutta numerical integration class.
**************************************************************************
Copyright (C) 2003  Kevin J. Walchko <walchko@mac.com>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**************************************************************************/

#include "cRk4.h"

static char errMsg[ERROR_STRING_SIZE];

/*!
  This function initializes cRK4.
  \param imodel a pointer to the dynamic equations.
  \param idt time step of each iteration.
  \param x_size size of the state vector.
  \param u_size size of the control effort.
*/
cRK4::cRK4(
	   cVector& (*imodel)( cVector&,
			       cVector&,
			       cVector&), 
	   ml_data idt, 
	   int x_size, 
	   int u_size
	   ){

  x.resize(x_size);
  xx.resize(x_size);
  u.resize(u_size);
  k1x.resize(x_size);
  k2x.resize(x_size);
  k3x.resize(x_size);
  k4x.resize(x_size);
  dist.resize(x_size);


  x.setName("cRK4::x");
  xx.setName("cRK4::xx");
  u.setName("cRK4::u");
  k1x.setName("cRK4::k1x");
  k2x.setName("cRK4::k2x");
  k3x.setName("cRK4::k3x");
  k4x.setName("cRK4::k4x");
  dist.setName("cRK4::dist");

  model=imodel;
  dt=idt;
  time=0;
}

/*!
  This function performs the integration on the system
  using a fourth order, fixed time step runge-kutta
  integrateion. The results can be obtained by
  getting the cVector x.

  \todo properly calculate the change in time. Should be something like
  model(x,u,time,dist)
*/
cVector& cRK4::integrate(void){
  try {
    k1x=dt*model(x,u,dist);
    xx = x+.5*k1x;
    k2x=dt*model(xx,u,dist);
    xx = x+.5*k2x;
    k3x=dt*model(xx,u,dist);
    xx = x+k3x;
    k4x=dt*model(xx,u,dist);
    x = x +(1.0/6.0)*(k1x + 2.0*k2x + 2.0*k3x + k4x );

    time += dt;
  }
	RK4_CATCH_ERROR(FATAL);
	
  return x;
}

/*!
  Destructor. Does nothing.
*/
cRK4::~cRK4(void){
}
