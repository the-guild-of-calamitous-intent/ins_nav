#ifndef KEVINS_QUATERNION_H
#define KEVINS_QUATERNION_H

#include "matrix.h"
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif
	
void e2q3(double *q, double r, double p, double y);
void q2e3(double *q, double *r, double *p, double *y);
void e2q(double *e, double *q);
void q2R4(Matrix *a, double q1, double q2, double q3, double q4);
void q2R(Matrix *a, double *q);
void setR321_Quat(Matrix *a, double roll, double pitch, double yaw);
void quatMult(double*,double*,double*);
void normalize(double *a);
double quatMag(double *a);
void quatCopy(double *in, double *out);
double* initQuat(double axis[3], double angle);

#ifdef __cplusplus
};
#endif

#endif













