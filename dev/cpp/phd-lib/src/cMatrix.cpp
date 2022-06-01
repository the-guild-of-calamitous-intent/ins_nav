/*************************************************************************
cMatrix.cpp - matrix class.
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
/// Constructor. If no data array is specified, then all elements
/// of the matrix is set to zero.
/// \param rows 
/// \param cols
/// \param n string containing the name of matrix
/// \param d an array of ml_data to fill matrix with
/////////////////////////////////////////////////////////////////
cMatrix::cMatrix(int rows, int cols, char *n, const ml_data *d){
  r=rows;
  c=cols;
  int range=r*c;
  type = ML_MATRIX;
	
  p = NULL;
  p=new ml_data[range];

#ifdef ML_CHECK
  if(p == NULL){
    sprintf(errMsg,"%s could not allocate memory for data %s\n",__PRETTY_FUNCTION__,n);
    throw cMLError(cMLError::FATAL,errMsg);
  }
#endif
	
	
  if(d == NULL){
    memset(p,0,sizeof(ml_data)*range);
  }
  else{
    memcpy(p,d,sizeof(ml_data)*range);
  }

  setName(n);
}


/////////////////////////////////////////////////////////////////
/// Copy constructor
/////////////////////////////////////////////////////////////////
cMatrix::cMatrix(const cMatrix &mat) : cBaseMath() {
  int range = r*c;
  r=mat.r;
  c=mat.c;
  if(p) delete[] p;
  p = NULL;
  p=new ml_data[range];
#ifdef ML_CHECK
  if(p == NULL){
    sprintf(errMsg,"%s could not allocate memory for data %s\n",__PRETTY_FUNCTION__,name);
    throw cMLError(cMLError::FATAL,errMsg);
  }
#endif
	
  type = ML_MATRIX;
  memcpy(p,mat.p,sizeof(ml_data)*range);
  mat.releaseTmp();
}


/////////////////////////////////////////////////////////////////
/// Destructor. Does nothing.
/////////////////////////////////////////////////////////////////
cMatrix::~cMatrix(){
}

/////////////////////////////////////////////////////////////////
/// Resizes a matrix to a x b.
/// \param a new row size.
/// \param b new column size.
/////////////////////////////////////////////////////////////////
void cMatrix::resize(int a, int b){
  if(p) delete[] p;
  p = NULL;
  r = a;
  c = b;
  type = ML_MATRIX;

  if (r == 0 || c == 0) return;

  p=new ml_data[r*c];
#ifdef ML_CHECK
  if( p == NULL){
    sprintf(errMsg,"%s couldn't create enough memory when resizing matrix %s",__PRETTY_FUNCTION__,name);
    throw cMLError(cMLError::FATAL,errMsg);
  }
#endif
  memset(p,0,sizeof(ml_data)*r*c);
}

//--- math ------------------------------------------

/////////////////////////////////////////////////////////////////
///  This function makes the matrix an identity matrix.
///  \return pointer to this matrix.
/////////////////////////////////////////////////////////////////
cMatrix& cMatrix::eye(void){
  int i;

	if (r != c){
		sprintf(errMsg,"%s r != c",__PRETTY_FUNCTION__);
		throw cMLError(cMLError::FATAL,errMsg);
	}

  memset(p,0,c*r*sizeof(ml_data)); // set all to zero
  for(i=0; i<r; i++){
    p[i+i*c] = 1.0;
  }

  return *this;
}

/////////////////////////////////////////////////////////////////
///  This function makes all elements 1.
///  \return pointer to this matrix.
/////////////////////////////////////////////////////////////////
cMatrix& cMatrix::ones(void){
  int i;
  int range = r*c;

  for(i=0; i<range; i++){
    p[i] = 1.0;
  }

  return *this;
}


/*!
  \param size of a square matrix: eye(3)=cMatrix(3,3)
  \return cMatrix(size,size)
 */
/////////////////////////////////////////////////////////////////
/// Returns a square identity matrix.
/// \param size of a square matrix: eye(3)=cMatrix(3,3)
/// \return cMatrix(size,size)
/////////////////////////////////////////////////////////////////
cMatrix& eye(int size){
  int i;

  cMatrix *ans = cBaseMath::getTmp(size,size); //ans->getTmp(size,size);
  memset(ans->p,0,size*size*sizeof(ml_data)); // set all to zero
  for(i=0; i<size; i++){
    ans->p[i+i*size] = 1.0;
  }

  return *ans;
}


/////////////////////////////////////////////////////////////////
/// Returns a matrix composed all of ones
/////////////////////////////////////////////////////////////////
cMatrix& ones(int r, int c){
  int i;

  cMatrix *ans = cBaseMath::getTmp(r,c); //ans->getTmp(size,size);
  memset(ans->p,0,r*c*sizeof(ml_data)); // set all to zero
  for(i=0; i<r*c; i++){
    ans->p[i] = 1.0;
  }

  return *ans;
}


/////////////////////////////////////////////////////////////////
/// Returns a matrix composed all of zeros
/////////////////////////////////////////////////////////////////
cMatrix& zeros(int r, int c){
  cMatrix *ans = cBaseMath::getTmp(r,c); //ans->getTmp(size,size);
  memset(ans->p,0,r*c*sizeof(ml_data)); // set all to zero
	
  return *ans;
}

/////////////////////////////////////////////////////////////////
/// This function creates a diagional square matrix. 
/// \return pointer to this matrix.
/////////////////////////////////////////////////////////////////
cMatrix& cMatrix::diag(const ml_data *dat){
  int i;

  memset(p,0,r*c*sizeof(ml_data)); // set all to zero
  for(i=0; i<r; i++){
    p[i+i*c] = dat[i];
  }
  return *this;
}

/////////////////////////////////////////////////////////////////
/// This function multiplies a matrix times a vector and returns a vector.
/// The function checks the size of the input and output vectors and the
/// matrix.
///
/// \f$ ans = a * b \f$
/////////////////////////////////////////////////////////////////
cVector&  operator*( const cMatrix &a,  const cVector &b){
  ml_data sum;
  int i,k;

#ifdef ML_CHECK
  if (a.c != b.size){
    sprintf(errMsg,"%s can't multiply matrix and vector %s(%d,%d) * %s(%d)",__PRETTY_FUNCTION__,a.name,a.r,a.c,b.name,b.size);
    throw cMLError(cMLError::FATAL,errMsg);
  }
#endif

  cVector *ans = b.getTmp(a.r);

  for(i=0; i<a.r; i++){
    sum=0;
    for(k=0; k<a.c; k++){
      sum+=a.p[k+i*a.c]*b.p[k];
    }
    ans->p[i]=sum;
  }

  a.releaseTmp();
  b.releaseTmp();

  return *ans;
}


/////////////////////////////////////////////////////////////////
/// Equates two matrices together
/////////////////////////////////////////////////////////////////
cMatrix& cMatrix::operator=(const cMatrix &mat){

#ifdef ML_CHECK
  if(mat.r!=r || mat.c!=c){
    sprintf(errMsg,"%s operator= matrix sizes do not match %s(%d,%d)=%s(%d,%d)\n",__PRETTY_FUNCTION__,name,r,c,mat.name,mat.r,mat.c);
    throw cMLError(cMLError::FATAL,errMsg);
  }
#endif

  memcpy(p,mat.p,sizeof(ml_data)*r*c);
  mat.releaseTmp();
	
  return *this;
}


/////////////////////////////////////////////////////////////////
/// Fills a matrix from an array and returns a reference to it.
/////////////////////////////////////////////////////////////////
cMatrix& cMatrix::operator=(const ml_data *d){

  memcpy(p,d,sizeof(ml_data)*r*c);
 
  return *this;
}


/////////////////////////////////////////////////////////////////
/// Divides a matrix by a scalar, and returns a reference to
/// this one.
/////////////////////////////////////////////////////////////////
cMatrix & cMatrix::operator/=(ml_data s){
	for(int i=0;i<r*c;i++){
		p[i]/=s;
	}

	return *this;
}


/////////////////////////////////////////////////////////////////
/// Multiplies a matrix by a scalar, and returns a reference to
/// this one.
/////////////////////////////////////////////////////////////////
cMatrix& cMatrix::operator*=(ml_data s){
	for(int i=0;i<r*c;i++){
		p[i]*=s;
	}

	return *this;
}


/////////////////////////////////////////////////////////////////
/// Subtracts a matrix from this one, and returns a reference to
/// this one.
/////////////////////////////////////////////////////////////////
cMatrix& cMatrix::operator-=(const cMatrix &mat){
#ifdef ML_CHECK
	if(mat.r!=r || mat.c!=c){
		sprintf(errMsg,"%s operator= matrix sizes do not match %s(%d,%d)=%s(%d,%d)\n",__PRETTY_FUNCTION__,name,r,c,mat.name,mat.r,mat.c);
		throw cMLError(cMLError::FATAL,errMsg);
	}
#endif

	for(int i=0;i<r*c;i++){
		p[i]-=mat.p[i];
	}
	mat.releaseTmp();

	return *this;
}

/////////////////////////////////////////////////////////////////
/// Adds a matrix from this one, and returns a reference to
/// this one.
/////////////////////////////////////////////////////////////////
cMatrix& cMatrix::operator+=(const cMatrix &mat){
#ifdef ML_CHECK
	if(mat.r!=r || mat.c!=c){
		sprintf(errMsg,"%s operator= matrix sizes do not match %s(%d,%d)=%s(%d,%d)\n",__PRETTY_FUNCTION__,name,r,c,mat.name,mat.r,mat.c);
		throw cMLError(cMLError::FATAL,errMsg);
	}
#endif

	for(int i=0;i<r*c;i++){
		p[i]+=mat.p[i];
	}
	mat.releaseTmp();

	return *this;
}

/////////////////////////////////////////////////////////////////
/// Equates two matrices together
/////////////////////////////////////////////////////////////////
cMatrix& cMatrix::operator=(const cMatrix *mat){
#ifdef ML_CHECK
  if(mat->r!=r || mat->c!=c){
    sprintf(errMsg,"%s operator= matrix sizes do not match %s(%d,%d)=%s(%d,%d)\n",__PRETTY_FUNCTION__,name,r,c,mat->name,mat->r,mat->c);
    throw cMLError(cMLError::FATAL,errMsg);
  }
#endif

  memcpy(p,mat->p,sizeof(ml_data)*r*c);
  mat->releaseTmp();
	
  return *this;
}



/////////////////////////////////////////////////////////////////
/// Adds two matrices together
/////////////////////////////////////////////////////////////////
cMatrix& operator+(const cMatrix &m1, const cMatrix &m2){
  int i,j;

#ifdef ML_CHECK
  //printf("checking\n");
  if(m1.r!=m2.r || m1.c!=m2.c){
    sprintf(errMsg,"%s matrix sizes do not match %s(%d,%d)+%s(%d,%d)\n",__PRETTY_FUNCTION__,m1.name,m1.r,m1.c,m2.name,m2.r,m2.c);
    throw cMLError(cMLError::FATAL,errMsg);
  }
#endif

  cMatrix *ans = m1.getTmp(m1.r,m1.c);

  for(i=0;i<m1.r;i++){
    for(j=0;j<m1.c;j++){
      //ans->operator()(i,j)=m1(i,j)+m2(i,j);
      ans->p[j+i*ans->c] = m1.p[j+i*m1.c] + m2.p[j+i*m2.c];
    }
  }

  m1.releaseTmp();
  m2.releaseTmp();

  return *ans; 
}



/////////////////////////////////////////////////////////////////
/// Subtracts two matrices
/////////////////////////////////////////////////////////////////
cMatrix& operator-( const cMatrix &left, const cMatrix &right ){
  int i;

#ifdef ML_CHECK
  if(left.r != right.r || left.c != right.c){
    sprintf(errMsg,"%s unable to subtract matricies %s - %s\n",__PRETTY_FUNCTION__,left.name,right.name);
    throw cMLError(cMLError::FATAL,errMsg);
  }
#endif

  cMatrix *ans = left.getTmp(left.r,left.c);

  for(i=0;i<left.r*left.c;i++)
    ans->p[i] = left.p[i] - right.p[i];

  left.releaseTmp();
  right.releaseTmp();

  return *ans;
}


/////////////////////////////////////////////////////////////////
/// Returns the negitive of a matrix
/////////////////////////////////////////////////////////////////
cMatrix& operator-( const cMatrix &a){
  int i;

  cMatrix *ans = a.getTmp(a.r,a.c);

  for(i=0;i<a.r*a.c;i++)
    ans->p[i] =  - a.p[i];

  a.releaseTmp();

  return *ans;
}


/////////////////////////////////////////////////////////////////
/// Multiplies two matrices together
/////////////////////////////////////////////////////////////////
cMatrix& operator*( const cMatrix &a,  const cMatrix &b){
  ml_data sum;
  int i,j,k;

#ifdef ML_CHECK
  if (a.c != b.r ){
    sprintf(errMsg,"%s can't multiply matricies %s(%d,%d) * %s(%d,%d)\n",__PRETTY_FUNCTION__,a.name,a.r,a.c,b.name,b.r,b.c);
    //cout<<errMsg;
    throw cMLError(cMLError::FATAL,errMsg);
  }
#endif

  cMatrix *ans = a.getTmp(a.r,b.c);

  for(i=0; i<a.r; i++){
    for(j=0; j<b.c; j++){
      sum=0;
      for(k=0; k<a.c; k++){
	sum+=a.p[k+i*a.c]*b.p[j+k*b.c];
      }
      ans->p[j+i*ans->c]=sum;
    }
  }

  a.releaseTmp();
  b.releaseTmp();

  return *ans;
}

/////////////////////////////////////////////////////////////////
/// Returns the matrix multiplied by a scalar
/////////////////////////////////////////////////////////////////
cMatrix& operator*( ml_data scalar, const cMatrix &a ){
  return operator*(a,scalar);
}

/////////////////////////////////////////////////////////////////
/// Returns the matrix multiplied by a scalar
/////////////////////////////////////////////////////////////////
cMatrix& operator*( const cMatrix &a, ml_data scalar ){
  int i;

  cMatrix *ans = cBaseMath::getTmp(a.r,a.c);

  for(i=0;i<a.r*a.c;i++)
    ans->p[i]=scalar*a.p[i];

  a.releaseTmp();

  return *ans;
}

/*!
  Perform scalar division on a matrix.
  \n ans = in / scalar
  \param in a pointer to a cMatrix.
  \param scalar a scalar value of type ml_data.
  \param ans a pointer to a cMatrix which contains the return value.
  \return pointer to a cMatrix.
*/
cMatrix& operator/( const cMatrix &a, ml_data scalar){
  int i;

  cMatrix *ans = cBaseMath::getTmp(a.r,a.c);

  for(i=0;i<a.r*a.c;i++)
    ans->p[i]=a.p[i]/scalar;

  a.releaseTmp();

  return *ans;
}


/*!
  This function transposes a matrix.
  \n b = transpose(a).
  \return pointer to a cMatrix.
*/
cMatrix& cMatrix::trans(void){
  int i,j;

  cMatrix *ans = cBaseMath::getTmp(c,r);

  for(i=0;i<r;i++)
    for(j=0;j<c;j++)
      ans->p[i+j*r] = p[j+i*c];

  return *ans;
}


/////////////////////////////////////////////////////////////////
/// Return the inverse of a matrix.
/////////////////////////////////////////////////////////////////
cMatrix& inv(const cMatrix& a){
	cMatrix *ans = cBaseMath::getTmp(a.c,a.r);
	//ans->operator=( a );
	memcpy(ans->p,a.p,sizeof(ml_data)*a.r*a.c);
	ans->inv();
	a.releaseTmp();
	return *ans;
}

/*!
  Inverse Function
 
  \f$ a = inv(a) \f$
  \return inverse of a matrix.
  \todo make this handle 1x1, 2x2, 3x3, etc in a more efficient way.
 \todo this doesn't effect the current data, it makes a new matrix
*/
cMatrix& cMatrix::inv( void ){ 

  int *indxc, *indxr, *ipiv;
  cArray *iindxc, *iindxr, *iipiv;
  int i, icol, irow, j, k, l,ll;
  //ml_data c = 0.0, d = 0.0;
  ml_data big, dum, pivinv;//, temp = 0.0;
  int n=c;
  int m=r;

#ifdef ML_CHECK
  if(r != c){
   
    sprintf(errMsg,"%s unable to invert matrix %s[%i x %i], must be square\n",__PRETTY_FUNCTION__,name,r,c);
    throw cMLError(cMLError::FATAL,errMsg);
  }
#endif

  cMatrix *a = this; //getTmp(r,c);
  //memcpy(a->p,p,sizeof(ml_data)*r*c);
  cMatrix *b = getTmp(r,c);

  iipiv = getTmpArray(n);
  iindxr = getTmpArray(n);
  iindxc = getTmpArray(n);
	
  ipiv = iipiv->array;
  indxr = iindxr->array;
  indxc = iindxc->array;
  
  for (j=0; j<n; j++) ipiv[j]=0;
  for (i=0; i<n; i++) {		// main loop over the c to be reduced
    big=0.0;
    for (j=0; j<n; j++)		// outer loop of the search for a pivot
      if (ipiv[j] != 1)	// element
	for (k=0; k<n; k++) {
	  if (ipiv[k] == 0) {
	    if (fabs(a->p[k+j*a->c]) >= big) {
	      big=fabs(a->p[k+j*a->c]);
	      irow=j;
	      icol=k;
	    }
	  }
	  else if (ipiv[k] > 1) {
	    sprintf(errMsg,"%s gaussj: Singular Matrix-1 %s\n\t\tDisregard solution.\n\n",__PRETTY_FUNCTION__,name);
	    throw cMLError(cMLError::FATAL,errMsg);
	  }
	}
    ++(ipiv[icol]);
    if (irow != icol) {
      for (l=0;l<n;l++) std::swap(a->p[l+irow*a->c],a->p[l+icol*a->c]);
      for (l=0;l<m;l++) std::swap(b->p[l+irow*b->c],b->p[l+icol*b->c]);
    }
    indxr[i]=irow;  // This is where the division takes place
    indxc[i]=icol;  // Pivot row gets divided by pivot value
    if (a->p[icol+icol*a->c] == 0.0) {
      sprintf(errMsg,"%s gaussj: Singular Matrix-2 %s\n\t\tDisregard solution.\n\n",__PRETTY_FUNCTION__,name);
      throw cMLError(cMLError::FATAL,errMsg);
    }
    pivinv=1.0/a->p[icol+icol*a->c];
    a->p[icol+icol*a->c]=1.0;
    for (l=0;l<n;l++) a->p[l+icol*a->c] *= pivinv;
    for (l=0;l<n;l++) b->p[l+icol*b->c] *= pivinv;
    for (ll=0;ll<n;ll++)
      if (ll != icol){
	dum=a->p[icol+ll*a->c];
	a->p[icol+ll*a->c]=0.0;
	for (l=0;l<n;l++) a->p[l+ll*a->c] -= a->p[l+icol*a->c]*dum;
	for (l=0;l<n;l++) b->p[l+ll*b->c] -= b->p[l+icol*b->c]*dum;
      }
  }
  for (l=n-1;l>=0;l--) {
    if (indxr[1] != indxc[l])
      for (k=0; k<n;k++)
	std::swap(a->p[indxr[l]+k*a->c],a->p[indxc[l]+k*a->c]);
  }

  iipiv->releaseTmp();
  iindxr->releaseTmp();
  iindxc->releaseTmp();
	 
  b->releaseTmp();
	
  return *a;
}


/*!
  This creates a skew matrix of size 3x3.
  \param a cMatrix[3x3] pointer.
  \param b an array of size 3.
  \return pointer to a cMatrix.
  \note the cMatrix must already exist.
*/
cMatrix& cMatrix::skew( const ml_data *b){

  switch(c){
  case 3:
#ifdef ML_CHECK
    if(r != 3 || c != 3 ){
      sprintf(errMsg,"%s %s(%d,%d) MUST be a semetric cMatrix",__PRETTY_FUNCTION__,name,r,c);
      throw cMLError(cMLError::FATAL,errMsg);
    }
#endif
    p[0] = 0.0;
    p[1] = -b[2];
    p[2] = b[1];
    p[3] = b[2];
    p[4] = 0.0;
    p[5] = -b[0];
    p[6] = -b[1];
    p[7] = b[0];
    p[8] = 0.0;
    break;
  case 4:
#ifdef ML_CHECK
    if(r != 4 || c != 4){
      sprintf(errMsg,"%s %s(%d,%d) MUST be a semetric cMatrix",__PRETTY_FUNCTION__,name,r,c);
      throw cMLError(cMLError::FATAL,errMsg);
    }
#endif
    p[0] = 0.0;
    p[1] = b[2];
    p[2] = -b[1];
    p[3] = b[0];

    p[4] = -b[2];
    p[5] = 0.0;
    p[6] = b[0];
    p[7] = b[1];

    p[8] = b[1];
    p[9] = -b[0];
    p[10] = 0.0;
    p[11] = b[2];

    p[12] = -b[0];
    p[13] = -b[1];
    p[14] = -b[2];
    p[15] = 0.0;
    break;
  default:
#ifdef ML_CHECK
    sprintf(errMsg,"%s %s.skew(%d,%d) can only do size 3 and 4\n",__PRETTY_FUNCTION__,name,r,c);
    throw cMLError(cMLError::FATAL,errMsg);
#endif
    break;
  }

  return *this;
}


/////////////////////////////////////////////////////////////////
/// Create a skew matrix from an array.
/////////////////////////////////////////////////////////////////
cMatrix& skew(const ml_data *b, int size){

  cMatrix *ans = cBaseMath::getTmp(size,size);

  switch(ans->c){
  case 3:
    ans->p[0] = 0.0;
    ans->p[1] = -b[2];
    ans->p[2] = b[1];
    ans->p[3] = b[2];
    ans->p[4] = 0.0;
    ans->p[5] = -b[0];
    ans->p[6] = -b[1];
    ans->p[7] = b[0];
    ans->p[8] = 0.0;
    break;
  case 4:
    ans->p[0] = 0.0;
    ans->p[1] = b[2];
    ans->p[2] = -b[1];
    ans->p[3] = b[0];

    ans->p[4] = -b[2];
    ans->p[5] = 0.0;
    ans->p[6] = b[0];
    ans->p[7] = b[1];

    ans->p[8] = b[1];
    ans->p[9] = -b[0];
    ans->p[10] = 0.0;
    ans->p[11] = b[2];

    ans->p[12] = -b[0];
    ans->p[13] = -b[1];
    ans->p[14] = -b[2];
    ans->p[15] = 0.0;
    break;
  default:
    sprintf(errMsg,"%s skew(%d,%d) can only do size 3 and 4\n",__PRETTY_FUNCTION__,ans->r,ans->c);
    throw cMLError(cMLError::FATAL,errMsg);
  }	
	
  return *ans;
}


//--- special ----------------------------------

/*! 
  Index to access elements of a matrix. This function
  is one based, thus the first element in the matrix is 
  matrix(1,1).  
*/
ml_data & cMatrix::operator()(int i, int j)const{
#ifdef ML_CHECK
  if(i*j<=0 || i>r || j>c){
    sprintf(errMsg,"%s matrix(%d,%d) is out of bounds\n",__PRETTY_FUNCTION__,i,j);
    throw cMLError(cMLError::FATAL,errMsg);
  }
#endif

  return *(p+(i-1)*c+(j-1));
}

/*!
  Set a row of a matrix.
  \param a ml_data array to fill row with.
  \param row to be filled.
*/
void cMatrix::setR(const ml_data *a, int row){
  int i;
  for (i = 0; i < c; i++)
    p[i+row*c] = a[i];
}

/*!
  Set a column of a matrix.
  \param a ml_data array to fill column with.
  \param col to be filled.
*/
void cMatrix::setC(const ml_data *a, int col){
  int i;
  for (i = 0; i < c; i++)
    p[col+i*c] = a[i];
}

/*!
  Set a part of a matrix to some value.
  \param row starting row
  \param col starting column
  \param dr row offset
  \param dc column offset
*/
void cMatrix::set(const ml_data *a, int row, int col, int dr, int dc){
  int i,j,k;
  k=0;
  for (i = row; i < row+dr; i++)
    for (j = col; j< col+dc; j++)
      p[j+i*c] = a[k++];
}

/////////////////////////////////////////////////////////////////
/// Output a matrix to the terminal.
/////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream &s,const cMatrix &mat){
  printf("--- Matrix %s(%d,%d) --------\n",mat.getName(),mat.getRow(),mat.getCol());
  for (int i=1;i<mat.r+1;i++){
    for (int j=1;j<mat.c+1;j++){
      s<<mat(i,j)<<"\t";
    }
    s<<"\n";
  }
  s<<"\n";
  mat.releaseTmp();
  return s;
}

/////////////////////////////////////////////////////////////////
/// Fill part of a matrix from an array.
/// \param dat a pointer to a data array.
/// \param mat a pionter to a cMatrix.
/// \param r starting row
/// \param c starting column
/// \param rw row length
/// \param cw column length
/// \return pointer to a cMatrix.
/////////////////////////////////////////////////////////////////
cMatrix& cMatrix::fill(int ir, int ic, int rw, int cw, const ml_data *dat){
  int i,j,k=0;

  for(i=ir;i<ir+rw;i++)
    for(j=ic;j<ic+cw;j++)
      p[j+i*c] = dat[k++];

  return *this;
}

/////////////////////////////////////////////////////////////////
/// Calculates the trace of a matrix, which is the sum
/// of the diagonal elements squared.
/////////////////////////////////////////////////////////////////
ml_data cMatrix::trace(void){
  int i;
  ml_data ans=0;
	
  for(i=0;i<r;i++) ans+=p[i+i*r];

  return ans;
}

