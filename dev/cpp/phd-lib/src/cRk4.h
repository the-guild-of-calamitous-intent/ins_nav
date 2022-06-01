/*************************************************************************
cRK4.h - Runge-Kutta numerical integration class.
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


#ifndef KEVINS_RUNGE_KUTTA
#define KEVINS_RUNGE_KUTTA

#include<cmath>
#include "cMathlib.h"

#define RK4_CATCH_ERROR(type)          \
catch( cMLError &e){                           \
    cMLError error(cMLError::type,__PRETTY_FUNCTION__);     \
    error+=e;                                   \
    throw error;                                \
}                                              \
catch ( ... ) {                                \
    cMLError error(cMLError::type,__PRETTY_FUNCTION__);     \
    error+="unknown error";                     \
    throw error;                                \
}


/////////////////////////////////////////////////////////////
/// cRK4 is a class for performing numerical integration
/// useing Runge-Kutta 4. The method assumes that the
/// equations of motion that are being integrated are first
/// order (i.e. like state space). They do not have to be linear
/// equations.
///
/// \f$ x' = A(x,u,dist) \f$
///
/// where x is the current state, u is an input or control
/// effort, and dist is and \e optional input or disturbance.
///
/// \code
/// #include "cMathlib.h"
/// #include "cRk4.h"
///
/// cVector& model(cVector &x, cVector &u, cVector &dist){
///     static cVector out(2,"out");
///     static ml_data adat[] = {1,2,3,4};
///     static cMatrix a(2,2,"a",adat);
///
///     out = a*x+u; // note dist is not used, it is optional
///
///     return out;
/// }
///
/// void main(void){
///     int i;
///     cRK4 rk(model,.01,2,2);
///     cVector z(2,"z");
///
///     for( i=0; i<100; i++){
///      rk.u = updateInput(); // this is some other function
///      rk.dist = updateDisturbance(); // some other function
///         z = rk.integrate();
///     }
/// }
/// \endcode
/////////////////////////////////////////////////////////////
class cRK4 {
public:
    cRK4(cVector& (*model)(
            cVector&,
            cVector&,
            cVector&),
        ml_data dt,
        int x_size,
        int u_size
        );

    ~cRK4(void);
    cVector& integrate(void);

    //protected:
    ml_data time,dt;
    cVector x,y,u,dist;
    cVector& (*model)(
            cVector&,
            cVector&,
            cVector&);
protected:
    cVector k1x,k2x,k3x,k4x,xx;
};


#endif
