/*************************************************************************
cQuaternion.cpp - quaternion class.
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
#include "cMathlib.h"


static char errMsg[ERROR_STRING_SIZE];


/////////////////////////////////////////////////////////////////
/// Contructor for the quaternion class. The initial data values
/// are set to (0,0,0,1).
/// \param n name for the quaternion (\e optional)
/////////////////////////////////////////////////////////////////
cQuaternion::cQuaternion(char *n){
	setName(n);
	type = cBaseMath::ML_QUATERNION;

	p = NULL;
	p = new ml_data[4];

	if ( p == NULL ){
		throw cMLError( cMLError::FATAL, "cQuaternion() couldn't create memory");
	}
	
	clear();
}

/////////////////////////////////////////////////////////////////
/// Copy constructor for the quaternion class.
/// \param q quaternion to be copied.
/////////////////////////////////////////////////////////////////
cQuaternion::cQuaternion(const cQuaternion &q){
	type = cBaseMath::ML_QUATERNION;

	p = NULL;
	p = new ml_data[4];

	if ( p == NULL ){
		throw cMLError( cMLError::FATAL, "cQuaternion() couldn't create memory");
	}

	p[0] = q.p[0];
	p[1] = q.p[1];
	p[2] = q.p[2];
	p[3] = q.p[3];
}


/////////////////////////////////////////////////////////////////
/// Returns the axis of rotation.
/////////////////////////////////////////////////////////////////
ml_data* cQuaternion::getAxis(){
	ml_data angle = acos(p[3]);
	ml_data d = sin(angle);
	ml_data *axis = new ml_data[3];

	if(fabs(d)<.000001){
		axis[0]=axis[1]=0.0;
		axis[2]=1.0;
	}
	else{
		axis[0]=p[0]/d;
		axis[1]=p[1]/d;
		axis[2]=p[2]/d;
	}
	return axis;
}

/////////////////////////////////////////////////////////////////
/// Returns the angle of rotation.
/////////////////////////////////////////////////////////////////
ml_data cQuaternion::getAngle(){
	ml_data angle = 2.0*acos(p[3]);
	return angle;
}

/////////////////////////////////////////////////////////////////
/// Clears a quaternion to zero rotation (i.e. (0,0,0,1) )
/////////////////////////////////////////////////////////////////
void cQuaternion::clear(void){
	memset(p,0,sizeof(ml_data)*3);
	p[3] = 1.0;
}


/////////////////////////////////////////////////////////////////
/// Makes the magnitude of the quaternion 1, which is required
/// for unit quaternions that represent rotations.
/////////////////////////////////////////////////////////////////
void cQuaternion::normalize(void){
	ml_data temp = sqrt(p[0]*p[0]+p[1]*p[1]+p[2]*p[2]+p[3]*p[3]);
	p[0]/=temp;
	p[1]/=temp;
	p[2]/=temp;
	p[3]/=temp;
}

/////////////////////////////////////////////////////////////////
/// Returns the magnitude of the quaternion. For a unit
/// quaternion, the returned value should be 1.
/////////////////////////////////////////////////////////////////
ml_data cQuaternion::norm(void){
	return sqrt(p[0]*p[0]+p[1]*p[1]+p[2]*p[2]+p[3]*p[3]);
}

/////////////////////////////////////////////////////////////////
/// Returns the inverse of a quaternion or (-x,-y,-z,r)
/////////////////////////////////////////////////////////////////
cQuaternion& cQuaternion::inv(void){
	cQuaternion *q = cBaseMath::getTmp();

	q->p[0] = -p[0];
	q->p[1] = -p[1];
	q->p[2] = -p[2];
	q->p[3] =  p[3];

	return *q;
	
}


/////////////////////////////////////////////////////////////////
/// Returns the conjugate of a quaternion or (-x,-y,-z,r)
/////////////////////////////////////////////////////////////////
cQuaternion& cQuaternion::conj(void){
	cQuaternion *q = cBaseMath::getTmp();

	q->p[0] = -p[0];
	q->p[1] = -p[1];
	q->p[2] = -p[2];
	q->p[3] =  p[3];

	return *q;
}



/////////////////////////////////////////////////////////////////
/// Prints a quaternion to stdout.
/////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream &s, cQuaternion &q){
	s<<"cQuaternion["<<q.name<<"]: "<<q[0]<<q[1]<<q[2]<<q[3]<<std::endl;
	q.releaseTmp();
	return s;
}


/////////////////////////////////////////////////////////////////
/// Transforms a quaternion to an euler angle.
/// \param r roll (radians)
/// \param p pitch (radians)
/// \param y yaw (radians)
/// \param flag which euler representation to use.
/// The input are:
/// \n flag = 1 for 1-2-1
/// \n = 2 for 2-3-2
/// \n = 3 for 3-1-3
/// \n = 4 for 1-3-1
/// \n = 5 for 2-1-2
/// \n = 6 for 3-2-3
/// \n = 7 for 1-2-3
/// \n = 8 for 2-3-1
/// \n = 9 for 3-1-2
/// \n = 10 for 1-3-2
/// \n = 11 for 2-1-3
/// \n = 12 for 3-2-1
/// 
/// written by John L. Crassidis 4/24/95
/////////////////////////////////////////////////////////////////
void cQuaternion::e2q(ml_data r, ml_data p, ml_data y, int flag){
	ml_data q1;
	ml_data q2;
	ml_data q3;
	ml_data q4;

	if (flag==1 || flag==2 || flag==3 || flag==4 || flag==5 || flag ==6)
		q4=cos(p/2)*cos((r+y)/2);
	else if (flag==7 || flag==8 || flag==9)
		q4=cos(r/2)*cos(p/2)*cos(y/2)-sin(r/2)*sin(p/2)*sin(y/2);
	else if (flag==10 || flag==11 || flag==12)
		q4=cos(r/2)*cos(p/2)*cos(y/2)+sin(r/2)*sin(p/2)*sin(y/2);
	else
		throw cMLError(cMLError::FATAL,"e2q() incorrect flag");

	switch(flag){
		case 1:
			q1=cos(p/2)*sin((r+y)/2);
			q2=sin(p/2)*cos((r-y)/2);
			q3=sin(p/2)*sin((r-y)/2);
			break;
		case 2:
			q1=sin(p/2)*sin((r-y)/2);
			q2=cos(p/2)*sin((r+y)/2);
			q3=sin(p/2)*cos((r-y)/2);
			break;
		case 3:
			q1=sin(p/2)*cos((r-y)/2);
			q2=sin(p/2)*sin((r-y)/2);
			q3=cos(p/2)*sin((r+y)/2);
			break;
		case 4:
			q1=cos(p/2)*sin((r+y)/2);
			q2=sin(p/2)*sin((y-r)/2);
			q3=sin(p/2)*cos((y-r)/2);
			break;
		case 5:
			q1=sin(p/2)*cos((y-r)/2);
			q2=cos(p/2)*sin((y+r)/2);
			q3=sin(p/2)*sin((y-r)/2);
			break;
		case 6:
			q1=sin(p/2)*sin((y-r)/2);
			q2=sin(p/2)*cos((y-r)/2);
			q3=cos(p/2)*sin((y+r)/2);
			break;
		case 7:
			q1=sin(r/2)*cos(p/2)*cos(y/2)+cos(r/2)*sin(p/2)*sin(y/2);
			q2=cos(r/2)*sin(p/2)*cos(y/2)-sin(r/2)*cos(p/2)*sin(y/2);
			q3=cos(r/2)*cos(p/2)*sin(y/2)+sin(r/2)*sin(p/2)*cos(y/2);
			break;
		case 8:
			q1=cos(r/2)*cos(p/2)*sin(y/2)+sin(r/2)*sin(p/2)*cos(y/2);
			q2=sin(r/2)*cos(p/2)*cos(y/2)+cos(r/2)*sin(p/2)*sin(y/2);
			q3=cos(r/2)*sin(p/2)*cos(y/2)-sin(r/2)*cos(p/2)*sin(y/2);
			break;
		case 9:
			q1=cos(r/2)*sin(p/2)*cos(y/2)-sin(r/2)*cos(p/2)*sin(y/2);
			q2=cos(r/2)*cos(p/2)*sin(y/2)+sin(r/2)*sin(p/2)*cos(y/2);
			q3=sin(r/2)*cos(p/2)*cos(y/2)+cos(r/2)*sin(p/2)*sin(y/2);
			break;
		case 10:
			q1=sin(r/2)*cos(p/2)*cos(y/2)-cos(r/2)*sin(p/2)*sin(y/2);
			q2=cos(r/2)*cos(p/2)*sin(y/2)-sin(r/2)*sin(p/2)*cos(y/2);
			q3=cos(r/2)*sin(p/2)*cos(y/2)+sin(r/2)*cos(p/2)*sin(y/2);
			break;
		case 11:
			q1=cos(r/2)*sin(p/2)*cos(y/2)+sin(r/2)*cos(p/2)*sin(y/2);
			q2=sin(r/2)*cos(p/2)*cos(y/2)-cos(r/2)*sin(p/2)*sin(y/2);
			q3=cos(r/2)*cos(p/2)*sin(y/2)-sin(r/2)*sin(p/2)*cos(y/2);
			break;
		case 12:
			q1=cos(r/2)*cos(p/2)*sin(y/2)-sin(r/2)*sin(p/2)*cos(y/2);
			q2=cos(r/2)*sin(p/2)*cos(y/2)+sin(r/2)*cos(p/2)*sin(y/2);
			q3=sin(r/2)*cos(p/2)*cos(y/2)-cos(r/2)*sin(p/2)*sin(y/2);
			break;
	}

	this->p[0] = q1;
	this->p[1] = q2;
	this->p[2] = q3;
	this->p[3] = q4;

}

/////////////////////////////////////////////////////////////////
/// Transform a quaternion into an euler representation.
/// \param flag which euler representation. The default is 7
/// which is a 1-2-3 or x-y-z rotation used in aerospace.
/// \sa e2q
/////////////////////////////////////////////////////////////////
ml_data* cQuaternion::q2e(int flag ){
	ml_data q1=p[0],q2=p[1],q3=p[2],q4=p[3];
	ml_data angle = 2.0*acos(q4)*180.0/M_PI;
	ml_data r,p,y,cp;
	ml_data a11,a21,a31,a32,a33;
	ml_data *euler = new ml_data[3];

	switch(flag){
		case 7:
			a21=2*(q1*q2-q3*q4);
			a11=q1*q1-q2*q2-q3*q3+q4*q4;
			a31=2*(q1*q3+q2*q4);
			a32=2*(q2*q3-q1*q4);
			a33=-q1*q1-q2*q2+q3*q3+q4*q4;

			p = asin(a31);
			cp = cos(p);
			y = atan2(-a21/cp,a11/cp);
			r = atan2(-a32/cp,a33/cp);

			break;
		default:
			throw cMLError(cMLError::FATAL,"q2e() incorrect flag");
	}
	
	euler[0] = r;
	euler[1] = p;
	euler[2] = y;

	return euler;
}		


/////////////////////////////////////////////////////////////////
/// Calculate the error between two quaternions.
/// \f$error = inv(current)*desired = [desired]*inv(current)\f$
/// \param a rigid bodies current orientation
/// \param b desired orientation
/// \return out error
/////////////////////////////////////////////////////////////////
cQuaternion& error(cQuaternion &qc, cQuaternion &qd){
	ml_data *a = qc.p;
	ml_data *b = qd.p;
	cQuaternion *qe = cBaseMath::getTmp();
	ml_data *out = qe->p;
	
	out[0] = -a[0]*b[3]-a[1]*b[2]+a[2]*b[1]+a[3]*b[0];
	out[1] =  a[0]*b[2]-a[1]*b[3]-a[2]*b[0]+a[3]*b[1];
	out[2] = -a[0]*b[1]+a[1]*b[0]-a[2]*b[3]+a[3]*b[2];
	out[3] =  a[0]*b[0]+a[1]*b[1]+a[2]*b[2]+a[3]*b[3];

	qc.releaseTmp();
	qd.releaseTmp();

	return *qe;
}


/////////////////////////////////////////////////////////////////
/// This function will interpolate from quaternion a to
/// quaternion b. The quaternion return is a function of
/// h. For h=0, returns a, for h=1, returns b, and for
/// 0<h<1 returns a quaternion between a and b.
/// \param qstart start quaternion
/// \param qstop end quaternion
/// \param h amount of interpolation
/// \param out answer
/////////////////////////////////////////////////////////////////
cQuaternion& slerp(cQuaternion &qstart, cQuaternion &qstop, ml_data h){
	ml_data *a = qstart.p;
	ml_data *b = qstop.p;
	cQuaternion *qs = cBaseMath::getTmp();
	ml_data *out = qs->p;
	ml_data angle = acos(a[0]*b[0]+a[1]*b[1]+a[2]*b[2]+a[3]*b[3]);
	ml_data aa = sin((1.0-h)*angle)/sin(angle);
	ml_data bb = sin(angle*h)/sin(angle);

	//qprint("a",a);
	//qprint("b",b);
	//printf("angle: %f aa: %f  bb: %f\n",angle,aa,bb);
	out[0] = a[0]*aa + b[0]*bb;
	out[1] = a[1]*aa + b[1]*bb;
	out[2] = a[2]*aa + b[2]*bb;
	out[3] = a[3]*aa + b[3]*bb;

	qstart.releaseTmp();
	qstop.releaseTmp();

	return *qs;
}


/////////////////////////////////////////////////////////////////
/// This function multiplies two quaternions together, which
/// is equivelent to multiplying two rotation matricies. Note
/// that the order of the quaternions is switched for the
/// multiplication.
/// together.
///
/// \f$ out = a * b = [b] * a \f$
///
/// \param a cQuaternion
/// \param b cQuaternion
/// \param out
/////////////////////////////////////////////////////////////////
cQuaternion& operator*(cQuaternion &qa, cQuaternion &qb){
	ml_data *a = qa.p;
	ml_data *b = qb.p;
	cQuaternion *qout = cBaseMath::getTmp();
	ml_data *out = qout->p;
	printf("quatMult(), WARNING, this maybe wrong!!!\n");
	out[0] =  a[0]*b[3]+a[1]*b[2]-a[2]*b[1]+a[3]*b[0];
	out[1] = -a[0]*b[2]+a[1]*b[3]+a[2]*b[0]+a[3]*b[1];
	out[2] =  a[0]*b[1]-a[1]*b[0]+a[2]*b[3]+a[3]*b[2];
	out[3] = -a[0]*b[0]-a[1]*b[1]-a[2]*b[2]+a[3]*b[3];

	qa.releaseTmp();
	qb.releaseTmp();
	
	return *qout;
}

/////////////////////////////////////////////////////////////////
/// This function converts a quaternion to a rotation
/// matrix.
/// \param q a pointer to an array containing the quaternion.
/////////////////////////////////////////////////////////////////
ml_data* cQuaternion::q2R(int flag){
	ml_data q1,q2,q3,q4;
	ml_data *rot = new ml_data[9];
	q1 = p[0];
	q2 = p[1];
	q3 = p[2];
	q4 = p[3];

	if(/*a->rows != 3 || a->cols != 3 */ 0){
		//printError("skew semetric double array"," ",' ',a->name,"data array");
		exit(1);
	}

	switch(flag){
		case 0:
			rot[0] = q1*q1-q2*q2-q3*q3+q4*q4;
			rot[1] = 2.0*(q1*q2+q3*q4);
			rot[2] = 2.0*(q1*q3-q2*q4);
			rot[3] = 2.0*(q1*q2-q3*q4);
			rot[4] = -q1*q1+q2*q2-q3*q3+q4*q4;
			rot[5] = 2.0*(q2*q3+q1*q4);
			rot[6] = 2.0*(q1*q3+q2*q4);
			rot[7] = 2.0*(q2*q3-q1*q4);
			rot[8] = -q1*q1-q2*q2+q3*q3+q4*q4;
		case 12:
			//--- quaternion 3-2-1 -------------
			rot[0] = 2.0*q4*q4-1.0+2.0*q1*q1;
			rot[1] = 2.0*q1*q2+2.0*q4*q3;
			rot[2] = 2.0*q1*q3-2.0*q4*q2;
			rot[3] = 2.0*q1*q2-2.0*q4*q3;
			rot[4] = 2.0*q4*q4-1.0+2.0*q2*q2;
			rot[5] = 2.0*q2*q3+2.0*q4*q1;
			rot[6] = 2.0*q1*q3+2.0*q4*q2;
			rot[7] = 2.0*q2*q3-2.0*q4*q1;
			rot[8] = 2.0*q4*q4-1.0+2.0*q3*q3;
		default:
			throw cMLError(cMLError::FATAL,"q2R() invalid flag");
	}
}

/////////////////////////////////////////////////////////////////
/// Equate two quaternions
/////////////////////////////////////////////////////////////////
void cQuaternion::operator=(cQuaternion &q){
	p[0] = q.p[0];
	p[1] = q.p[1];
	p[2] = q.p[2];
	p[3] = q.p[3];
	q.releaseTmp();
}


/////////////////////////////////////////////////////////////////
/// Equate two quaternions
/////////////////////////////////////////////////////////////////
void cQuaternion::operator=(cQuaternion *q){
	if(q == NULL)
		throw cMLError(cMLError::FATAL,"cQuaternion::operator=() is passed a NULL");
	p[0] = q->p[0];
	p[1] = q->p[1];
	p[2] = q->p[2];
	p[3] = q->p[3];

	q->releaseTmp();
}


/////////////////////////////////////////////////////////////////
/// Fill a quaternion with an array of data.
/////////////////////////////////////////////////////////////////
void cQuaternion::operator=(ml_data *q){
	if(q == NULL)
		throw cMLError(cMLError::FATAL,"cQuaternion::operator=() is passed a NULL");	
	p[0] = q[0];
	p[1] = q[1];
	p[2] = q[2];
	p[3] = q[3];
}

/*
void operator=(ml_data *p, cQuaternion &q){
	p[0] = q.p[0];
	p[1] = q.p[1];
	p[2] = q.p[2];
	p[3] = q.p[3];
}
*/

/*!
copy
 \param q pointer to data.
 */
/////////////////////////////////////////////////////////////////
/// Copy quaternion data into an array.
/////////////////////////////////////////////////////////////////
void cQuaternion::copy(ml_data *q){
	if(q == NULL)
		throw cMLError(cMLError::FATAL,"cQuaternion::copy() is passed a NULL");
	memcpy(q,p,4*sizeof(ml_data));
}




