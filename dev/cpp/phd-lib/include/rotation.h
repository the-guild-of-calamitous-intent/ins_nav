#ifndef KEVINS_ROTATION_H
#define KEVINS_ROTATION_H

#include "matrix.h"
#include "navigation.h"
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

void setR_iv(Matrix *a, float lng, float lat, float time);
void setR_ev(Matrix *a, float lng, float lat);
void setR123_Euler(Matrix *a, float roll, float pitch, float yaw);
void setR_cv(Matrix *a,float pn, float pu);
void setR_ne(Matrix *a, double lng, double lat);

#ifdef __cplusplus
};
#endif

#endif


