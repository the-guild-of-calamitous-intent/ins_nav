
/*!
\mainpage
\author Kevin J. Walchko

 \warning This is still a work in progress, so not all member functions
 are documented. Also not all member functions are completely tested,
 so make sure that any mathamatical code is double checked.

 <hr>

 cMathlib is a collection of mathamatical classes for vectors, matrices,
 and quaternions. There are additionally class provided for random number
 generation and numerical integration.

 <hr>

 <center> <h2> License (LGPL) </h2> </center>

 <center> Copyright (C) 2000  Kevin J. Walchko <walchko@mac.com> </center>

 <kbd>
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
 </kbd>
*/

#ifndef CMATHLIB_H
#define CMATHLIB_H

//--- C++ ---
#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <algorithm>

//--- C ---
#include <cmath>
// #include <cstring>
#include <cstdlib>

//--- defines ---
#define ML_CHECK
#define ERROR_STRING_SIZE 255
#define ml_data float

#define R2D (180.0/M_PI)
#define D2R (M_PI/180.0)

class cBaseMath;
class cMatrix;
class cQuaternion;
class cVector;
class cArray;


///////////////////////////////////////////////////////////////////
/// Any errors that occur use catch and throw. This class
/// is what is thrown and caught.
/// \code
/// #include "cMathlib.h"
/// void main(void){
///    cVector a(3),b(4),c(9);
///    try {
///       a = b + c;  // this won't work because of dimensions
///                   // an error will be thrown.
///    }
///    catch( cMLError &error ){
///       cout<<error;
///       exit(1);
///    }
/// }
/// \endcode
///////////////////////////////////////////////////////////////////
class cMLError {
 public:
  cMLError(int,const char*);
  ~cMLError(void);
  friend std::ostream &operator<<(std::ostream&,cMLError&);
  void operator+=(cMLError&);
  void operator+=(char*);

  enum {NONFATAL,FATAL};
  char errMsg[255];
  int type;
};


///////////////////////////////////////////////////////////////////
/// This is the base class for vectors, matrices and quaternions.
/// Basically, it handles all of the common functions between them
/// and handles the implementation of temporary vectors, matrices,
/// arrays, and quaternions. Whenever a cMathlib object requires a
/// temporary object, the cBaseMath provides either one that is in
/// its cache or creates a new one for use. When the object is no
/// longer needed, it is made available again in the cache, so that
/// the overhead of new is not required for subsequent requests for
/// a similar object.
/// \warning NEVER pass a temp matrix, vector, or
///  quaternion to or from a function. A
///  temp should ALWAYS be passed to a user
///  created matrix, vector, or quaternion.
///////////////////////////////////////////////////////////////////
class cBaseMath {
 public:
  cBaseMath(void);
  ~cBaseMath(void);
  static cQuaternion* getTmp(void);
  static cVector* getTmp(int);
  static cArray* getTmpArray(int);
  static cMatrix* getTmp(int,int);
  void releaseTmp(void) const;
  inline void setKey(int a){key = a;}
  inline int getKey(){return key;}
  void setName(char*);
  char* getName(void) const {return name;}
  inline bool valid(void){return p == NULL ? false : true;}
  inline int getType(void){return type;}
  inline ml_data& operator[](int i){return p[i];}

  //protected:
  enum {ML_VECTOR,ML_MATRIX,ML_QUATERNION,ML_ARRAY};
  static std::vector<cBaseMath*> cache; //!< cache
  int key;        //!< unique key used by temporary objects
  int type;       //!< type of object: array, vector, matrix, quaternion
  bool available; //!< used by temporary objects
  ml_data *p;        //!< pointer to data array
  char *name;     //!< name of cMathlib object
  // std::string name;
  static int keyNum; //!< current key number
};


///////////////////////////////////////////////
/// cArray is used internally to hold arrays of
/// integers for matirx inversion. There is no
/// reason to every use this class.
///////////////////////////////////////////////
class cArray : public cBaseMath {
 public:
  cArray(int);
  ~cArray(void);
  //inline int& operator[](int a){return array[a];}
  inline int getSize(void){return size;}
  //protected:
  int *array;
  int size;
};

///////////////////////////////////////////////

/*!
cVector represents a vector. This implementation contains all
 of the typically used mathamatical operators used by vectors.

 This class is derived from cBaseMath which is responcible
 for managing temporary variables, and contains common
 functios for creating data storage and object names.

 \code
#include "cMathlib.h"
#include <iostream.h>

 void main(void){
     ml_data p[] = {-3.4,0.3};
     cVector a(2);        // create vector
     cVector b(2,"bob");  // vector named "bob"
     cVector c(2,"c",p);  // vector named "c" initialized to -3.4 and 0.3.

     a = b+c;
     cout<<a;
 }
 \endcode
*/
class cVector : public cBaseMath {
  friend cVector& operator+(const cVector&, const cVector&);
  friend cVector& operator-(const cVector&, const cVector&);
  friend cVector& operator-(const cVector&);
  friend cVector& operator*(const cVector&, const ml_data);
  friend cVector& operator*(const ml_data, const cVector&);
  friend cVector& operator/(const cVector&, const ml_data);
  friend std::ostream& operator<<(std::ostream&,const cVector&);
  friend ml_data dot(const cVector&,const cVector&);
  friend cVector& cross(const cVector&,const cVector&);
  friend cMatrix& outer(const cVector&,const cVector&);

 public:
  cVector(int,char* =NULL,ml_data* =NULL);
  cVector(void){size=0;name=NULL;p=NULL;}
  cVector(const cVector&);
  ~cVector();
  cVector& operator=(const cVector&);
  cVector& operator=(const cVector*);
  cVector& operator=(const ml_data*);
  // the logicial operators may need to try to releaseTmp() on a.
  inline bool operator==(const cVector &a){return (size==a.size ? true : false);}
  inline bool operator!=(const cVector &a){return (size==a.size ? false : true);}
  cVector& operator/=(ml_data);
  cVector& operator*=(ml_data);
  cVector& operator+=(const cVector&);
  cVector& operator-=(const cVector&);

  ml_data& operator()(int)const;
  ml_data norm(void);
  cVector& ones(void);

  inline void set(const ml_data *d){memcpy(p,d,sizeof(ml_data)*size);}
  inline void set(ml_data a, ml_data b, ml_data c){p[0]=a;p[1]=b;p[2]=c;}
  inline void clear(void){memset(p,0,sizeof(ml_data)*size);}
  inline int getSize(void) const {return size;}
  void resize(int);

  //private:
  int size; //!< size of vector.
};

/*!
cMatrix represents a matrix. These matrices can be of size
 m x n. This implementation contains all of the typically
 used mathamatical operators used by matrices.

 This class is derived from cBaseMath which is responcible
 for managing temporary variables, and contains common
 functios for creating data storage and object names.

 \code
#include "cMathlib.h"
#include <iostream.h>

 void main(void){
     ml_data p[] = {1,2,3,4};
     cMatrix a(2,2);        // create matrix
     cMatrix b(2,2,"bob");  // matrix named "bob"
     cMatrix c(2,2,"c",p);  // matrix named "c" initialized to: {1,2,3,4}.

     b.ones();       // set all data in b to 1
     a = eye(2)+b+c; // add b and c to an identity matrix
     cout<<a;
 }
 \endcode
 */
class cMatrix : public cBaseMath {
  friend cMatrix& operator+(const cMatrix&,const cMatrix&);  //matrix addition
  friend cMatrix& operator-(const cMatrix&,const cMatrix&);  //matrix subtraction
  friend cMatrix& operator-(const cMatrix&);                 // negate a matrix
  friend cMatrix& operator*(const cMatrix&,const cMatrix&);  //matrix multiplication
  friend cMatrix& operator*(ml_data,const cMatrix&);         // scalar multiplication
  friend cMatrix& operator*(const cMatrix&,ml_data);         // scalar multiplication
  friend cVector& operator*(const cMatrix&,const cVector&);  //matrix addition
  friend cMatrix& operator/(const cMatrix&,ml_data);         // scalar division
  friend cMatrix& skew(const ml_data*,int);                // skew matrix
  friend cMatrix& eye(int);            // identity matrix
  friend cMatrix& zeros(int,int);      // a matrix of zeros
  friend cMatrix& ones(int,int);       // a matrix of ones
  friend cMatrix& inv(const cMatrix&); // a matrix inverse

  friend std::ostream &operator<<(std::ostream &s, const cMatrix &mat); // print matrix

 public:
  cMatrix(int,int,char* =NULL,const ml_data* =NULL);    // constructor
  cMatrix(void): r(0),c(0) {} //{r=c=0;name=NULL;p=NULL;}
  ~cMatrix();  // destructor
  cMatrix(const cMatrix &m);    // copy constructor

  cMatrix& eye(void);
  cMatrix& ones(void);
  cMatrix& diag(const ml_data*);
  cMatrix& inv(void);
  cMatrix& trans(void);
  cMatrix& skew(const ml_data*);
  ml_data trace(void);
  void resize(int,int);
  //inline void reset(int a, int b, char *n =NULL){resize(a,b);setName(n);}

  ml_data& operator()(int i, int j)const;    // index
  cMatrix& operator=(const cMatrix&);
  cMatrix& operator=(const cMatrix*);
  cMatrix& operator=(const ml_data*);
  cMatrix& fill(int,int,int,int,const ml_data*);
  inline bool operator==(const cMatrix &a){return (r==a.r && c==a.c ? true : false);}
  inline bool operator!=(const cMatrix &a){return (r==a.r && c==a.c ? false : true);}
  cMatrix& operator/=(ml_data); // scalar division
  cMatrix& operator*=(ml_data); // scalar multiplication
  cMatrix& operator*=(const cMatrix&); // matrix multiplication
  cMatrix& operator+=(const cMatrix&); // matrix addition
  cMatrix& operator-=(const cMatrix&); // matrix subtraction

  inline int getRow(void) const {return r;}
  inline int getCol(void) const {return c;}
  void setR(const ml_data*,int);
  void setC(const ml_data*,int);
  inline void set(const cMatrix &m,int a, int b){set(m.p,a,b,m.r,m.c);m.releaseTmp();}
  inline void set(const ml_data *d){memcpy(p,d,sizeof(ml_data)*r*c);}
  inline void set(const ml_data *d,int rr,int cc){set(d,rr,cc,r,c);}
  void set(const ml_data*,int,int,int,int);
  inline void clear(void){memset(p,0,sizeof(ml_data)*r*c);}

  //private:
  int r; //!< rows
  int c; //!< columns
};

/*!
cQuaternion represents a unit quaternion. A unit quaternion
 is used to represent an orientation in 3D space. Quaternions
 have several superior mathamatical properties over the commonly used
 euler angles. The two most important are: quaternions are less
 computational and they lack singularities (for most operations).
 The quaternion is arranged as [x,y,z,real], where the imaginary
 part of the quaternion is the first 3 elements [x,y,z].

 \code
#include "cMathlib.h"
#include <iostream.h>

 void main(void){
     ml_data p[] = {1,2,3,4};
     cQuaternion a, b;    // create quaternions
     cQuaternion c("c");  // quaternion named "c"

     b.set(p);       // set quaternions data.
     b.normalize();  // have to normalize the unit quaternion
                     // since the data wasn't already.
     c.set(4,3,2,1); // do the same for c.
     c.normalize();

     a = b*c;
     cout<<a;
 }
 \endcode
 */
class cQuaternion : public cBaseMath {
  friend cQuaternion& operator*(cQuaternion&,cQuaternion&);
  friend inline cQuaternion& inv(cQuaternion &q){return q.inv();}
  friend cQuaternion& slerp(cQuaternion&,cQuaternion&,ml_data);
  friend std::ostream &operator<<(std::ostream&, cQuaternion&); // print quaternion
  friend cQuaternion& error(cQuaternion&,cQuaternion&);
  //friend void operator=(ml_data*,cQuaternion&);

 public:
  cQuaternion(char*);
  cQuaternion(const cQuaternion&); // copy constructor

  // set data
  inline void set(ml_data a, ml_data b, ml_data c, ml_data d){ p[0]=a; p[1]=b; p[2]=c; p[3]=d; }
  inline void set(ml_data *a){set(a[0],a[1],a[2],a[3]);}
  inline ml_data& operator[](int i){return p[i];}   // 0 based
  inline ml_data& operator()(int i){return p[i-1];} // 1 based
  cQuaternion operator*=(cQuaternion&);
  void operator=(cQuaternion&);
  void operator=(cQuaternion*);
  void operator=(ml_data*);

  // get data
  inline ml_data& x(void){return p[0];}
  inline ml_data& y(void){return p[1];}
  inline ml_data& z(void){return p[2];}
  inline ml_data& r(void){return p[3];}
  inline ml_data* getImg(void){return p;}
  inline ml_data getReal(void){return p[3];}
  ml_data* getAxis(void);
  ml_data getAngle(void);
  ml_data norm(void);
  void copy(ml_data*);

  // misc
  cQuaternion& inv(void);
  cQuaternion& conj(void);
  void normalize(void);
  void clear(void);
  void error(cQuaternion&,cQuaternion&);

  // conversions
  void e2q(ml_data,ml_data,ml_data,int);
  inline void e2q(ml_data *e,int t){e2q(e[0],e[1],e[2],t);}
  ml_data* q2e(int=7);
  ml_data* q2R(int);

 private:

};


#endif
