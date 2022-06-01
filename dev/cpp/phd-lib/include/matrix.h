/*************************************************************************
matrix.h -- matrix and vector functions
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
#ifndef KEVINS_MATRIX_FUNCTIONS
#define KEVINS_MATRIX_FUNCTIONS

#include <stdio.h>
#include <math.h>

/*!
This macro swaps two things
*/
#define SWAP(a,b) {temp=(a);(a)=(b);(b)=temp;}

/*!
This macro returns the minimum of two items
*/
#define	min(A, B)	((A) < (B) ? (A) : (B))

/*!
CHECK allows you to turn off the capability to determine if boundry
checking is performed for matrix and vector functions.  The idea for
this, is that initially it will be defined so errors are not made.
Eventually though, once the code is completely checked for errors, it
can be turned off to increase performance by not having to check the
boundries.
*/
#define CHECK 1

/*!
This is the C (poor) solution to C++ templates (AFAIK).  At compile time,
the matrix and vector routines can have thier data types set to
double, float, int, char, etc.
*/
#define TYPE double

/*!
\ingroup Matrix
blah .. main discription.
\par ----------------------------------------------------------------
\author   Kevin J. Walchko (walchko@ufl.edu)
\version  .2
\date     Fall 2000
\date     Updated by KJW [11 March 2001]
*/
typedef struct{
  TYPE **p;   /*!< pointer to a pointer containing the data  */
  int rows;   /*!< number of rows */
  int cols;   /*!< number of columns */
  char *name; /*!< name of the matrix (used for debugging and printing) */
} Matrix;



/*!
blah .. main discription.
\par ----------------------------------------------------------------
\author   Kevin J. Walchko
\version  .2
\date     Fall 2000
\date     Updated by KJW [11 March 2001]
*/
typedef struct{
  TYPE *p;    /*!< pointer containing the data */
  int size;   /*!< size of the vector */
  char *name; /*!< name of the matrix (used for debugging and printing) */
} Vector;


#ifdef __cplusplus
extern "C" {
#endif

//!\name Matrix
//@{
/********************* Matrix Functions *************************/
Matrix* initMatrix(int,int,char*);
Matrix* initMatrixEye(int);
Matrix* initMatrixDiag(int,TYPE*);
Matrix* initMatrixZero(int,int);
void freeMatrix( Matrix*);
void matrixAdd( Matrix*, Matrix*, Matrix*);
void matrixSub( Matrix*, Matrix*, Matrix*);
void matrixInv( Matrix*, Matrix*);
void matrixMult( Matrix*, Matrix*, Matrix*);
void matrixMultS( Matrix*,TYPE, Matrix*);
void matrixDivS( Matrix*,TYPE, Matrix*);
void matrixInv( Matrix*, Matrix*);
void matrixEye(Matrix*);
void matrixClear( Matrix*);
void printMatrix( Matrix*);
void matrixFill( Matrix*,TYPE*);
void matrixFillColV( Matrix*,int, Vector*);
void matrixFillRowV( Matrix*,int, Vector*);
void matrixFillColA( Matrix*,int,TYPE*);
void matrixFillRowA( Matrix*,int,TYPE*);
void matrixFillSkewD(Matrix *a, TYPE *b);
void matrixFillSkewV(Matrix *a,Vector *b);
void matrixFillSkew4(Matrix *a,TYPE *b);
void subMatrixFill(Matrix *a, Matrix *b, int start_row, int start_col);
void matrixCopy( Matrix*, Matrix*);
void matrixTrans( Matrix*, Matrix*);
void setNameMatrix( Matrix*,char*,int);
//@}

//!\name Vector
//@{
/********************** Vector Functions **************************/
Vector* initVector(int,char*);
Vector* initVectorPointer(int,char*);
void freeVector( Vector*);
void vectorAdd( Vector*, Vector*, Vector*);
void vectorSub( Vector*, Vector*, Vector*);
void vectorMult( Matrix*, Vector*, Vector*);
void vectorMultS( Vector*,TYPE, Vector*);
void vectorDivS( Vector*,TYPE, Vector*);
TYPE vectorDot(Vector*,Vector*); 
void vectorFill( Vector*, TYPE*);
void vectorCopy( Vector*, Vector*);
void subVectorCopy(Vector*,Vector*,int,int,int);
void printVector( Vector*);
void fprintVector(Vector*,FILE*);
void vectorClear( Vector*);
void printErrorV(char*, Vector*, Vector*);
void setNameVector( Vector*,char*,int);
//@}

void printError(char*,char*,char,char*,char*);

#ifdef __cplusplus
};
#endif

#endif
