/*************************************************************************
cVector.cpp - vector class.
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


/*!
Creates a vector with all elements zero or filled with user defined
 data.
 \param s size of vector
 \param new_name is the name of the vector used for debugging
 purposes (optional)
 \param a an array of data to fill the vector with (optional)
 */
cVector::cVector(int s, char *new_name, ml_data *a){
	type = ML_VECTOR;
	key = 0;
	size = s;
	name = NULL;
	p = NULL;
	p = new ml_data[s];

#ifdef ML_CHECK
	if(p == NULL){
		sprintf(errMsg,"%s could not allocate memory for data %s\n",__PRETTY_FUNCTION__,new_name);
		throw cMLError(cMLError::FATAL,errMsg);
	}
#endif

	setName(new_name);

	if( a == NULL ){
		memset(p,0,size*sizeof(ml_data)); // set all to zero	
	}
	else{
		memcpy(p,a,size*sizeof(ml_data)); // set all to zero
	}

}

/*!
	This function copies the data from one vector to another vector.
	It does not copy the name, only the data.
	\n b = a
*/
cVector::cVector( const cVector &a ) : cBaseMath() {
	name = NULL;
	p = NULL;
	type = ML_VECTOR;
	key = 0;
	size = a.size;
	if(p) delete[] p;
	p = NULL;
	p = new ml_data[size]; 
#ifdef ML_CHECK
	if(p == NULL){
		sprintf(errMsg,"%s could not allocate memory for data %s\n",__PRETTY_FUNCTION__, name);
		throw cMLError(cMLError::FATAL,errMsg);
	}
#endif
	memcpy(p,a.p,a.size*sizeof(ml_data));
	a.releaseTmp();
}

/////////////////////////////////////////////////////////////////
/// Destructor. Does nothing.
/////////////////////////////////////////////////////////////////
cVector::~cVector(){
}

/////////////////////////////////////////////////////////////////
/// Resizes the vector to size a.
/// \param a new size
/////////////////////////////////////////////////////////////////
void cVector::resize(int a){
	if(p) delete[] p;
	size = a;
	type = ML_VECTOR;
	key = 0;
	p = NULL;
	p = new ml_data[size];
#ifdef ML_CHECK
	if(p == NULL){
		sprintf(errMsg,"%s could not allocate memory for data %s\n",__PRETTY_FUNCTION__,name);
		throw cMLError(cMLError::FATAL,errMsg);
	}
#endif
	
	memset(p,0,size*sizeof(ml_data)); // set all to zero	
}

//--- math --------------------------------------------------------


/////////////////////////////////////////////////////////////////
/// Divides the vector by a scalar value and returns this vector
/////////////////////////////////////////////////////////////////
cVector& cVector::operator/=(ml_data s){
	for(int i=0;i<size;i++) p[i]/=s;
	return *this;
}

/////////////////////////////////////////////////////////////////
/// Multiplies the vector by a scalar value and returns this vector
/////////////////////////////////////////////////////////////////
cVector& cVector::operator*=(ml_data s){
	for(int i=0;i<size;i++) p[i]*=s;
	return *this;
}

/////////////////////////////////////////////////////////////////
/// Subtracts a vector from this one, and returns this one.
/////////////////////////////////////////////////////////////////
cVector& cVector::operator-=(const cVector &v){

#ifdef ML_CHECK
	if(v.size != size){
		sprintf(errMsg,"%s vector sizes do not match %s=%s\n",__PRETTY_FUNCTION__,name,v.name);
		throw cMLError(cMLError::FATAL,errMsg);
	}
#endif

	for(int i=0;i<size;i++) p[i]-=v.p[i];

	v.releaseTmp();

	return *this;
}

/////////////////////////////////////////////////////////////////
/// Adds a vector from this one, and returns this one.
/////////////////////////////////////////////////////////////////
cVector& cVector::operator+=(const cVector &v){

#ifdef ML_CHECK
	if(v.size != size){
		sprintf(errMsg,"%s vector sizes do not match %s=%s\n",__PRETTY_FUNCTION__,name,v.name);
		throw cMLError(cMLError::FATAL,errMsg);
	}
#endif

	for(int i=0;i<size;i++) p[i]+=v.p[i];

	v.releaseTmp();

	return *this;
}

/////////////////////////////////////////////////////////////////
/// Equates two vectors
/////////////////////////////////////////////////////////////////
cVector& cVector::operator=(const cVector &v){

#ifdef ML_CHECK
	if(v.size != size){
		sprintf(errMsg,"%s vector sizes do not match %s=%s\n",__PRETTY_FUNCTION__,name,v.name);
		throw cMLError(cMLError::FATAL,errMsg);
	}
#endif

	memcpy(p,v.p,sizeof(ml_data)*size);

	v.releaseTmp();

	return *this;
}

/////////////////////////////////////////////////////////////////
/// Equates two vectors
/////////////////////////////////////////////////////////////////
cVector& cVector::operator=(const cVector *v){

#ifdef ML_CHECK
	if(v->size != size){
		sprintf(errMsg,"%s vector sizes do not match %s=%s\n",__PRETTY_FUNCTION__,name,v->name);
		throw cMLError(cMLError::FATAL,errMsg);
	}
#endif

	memcpy(p,v->p,sizeof(ml_data)*size);

	v->releaseTmp();

	return *this;
}

/*!
Fill a vector with data.
 \param d a ml_data array of data of length size.
 \warning the ml_data array MUST contain enough data to fill
 the cVector's data or the program will segfault.
 */
cVector& cVector::operator=(const ml_data *d){

	memcpy(p,d,sizeof(ml_data)*size);

	return *this;
}

//------------- math ------------------------------

/*!
	Add two vectors together.
 
	\f$ out = left + right \f$
*/
cVector& operator+( const cVector &left, const cVector &right){
	int i;

	#ifdef ML_CHECK 
	if( left.size != right.size ){
		sprintf(errMsg,"%s can't add vectors %s + %s",__PRETTY_FUNCTION__,left.name,right.name);
	throw cMLError(cMLError::FATAL,errMsg);
	}
	#endif

	cVector *out = left.getTmp(left.size);

	for(i=0;i<out->size;i++) out->p[i] = left.p[i]+right.p[i];
	
	left.releaseTmp();
	right.releaseTmp();

	return *out;
}

/*!
	This function subtracts a two vectors.  It checks the two input vector
	sizes and the output vector size.
	\n out = left - right
*/
cVector& operator-( const cVector &left, const cVector &right){
	int i;

	#ifdef ML_CHECK
	if(left.size != right.size ){
		sprintf(errMsg,"%s can't subtract vectors %s - %s",__PRETTY_FUNCTION__,left.name,right.name);
	throw cMLError(cMLError::FATAL,errMsg);
	}
	#endif

	cVector *out = left.getTmp(left.size);

	for(i=0;i<out->size;i++)
		out->p[i] = left.p[i]-right.p[i];

	left.releaseTmp();
	right.releaseTmp();

	return *out;
}

/*!
\return the negative of the vector.
 */
cVector& operator-( const cVector &a){
	int i;

	cVector *out = a.getTmp(a.size);

	for(i=0;i<out->size;i++)
		out->p[i] = -a.p[i];

	a.releaseTmp();

	return *out;
}


/*!
	Allows a vector to be multiplied by a scalar number.  The function
	checks the size of the input and output vectors.
	\n out = scalar * in
*/
cVector& operator*( const ml_data scalar, const cVector &in ){
	return operator*(in,scalar);
}

/*!
Multiplies a cVector by a scalar.
 \n out = in*scalar
 */
cVector& operator*( const cVector &in, const ml_data scalar ){
	int i;

	cVector *out = in.getTmp(in.size);

	for(i=0;i<out->size;i++) out->p[i] = scalar*in.p[i];

	in.releaseTmp();

	return *out;
}

/*!
	Allows a vector to be divided by a scalar number.  The function
	checks the size of the input and output vectors.
	\n out = in / scalar
*/
cVector& operator/( const cVector &in, const ml_data scalar){
	int i;
	
	cVector *out = in.getTmp(in.size);

	for(i=0;i<out->size;i++) out->p[i] = in.p[i]/scalar;

	in.releaseTmp();

	return *out;
}

/*!
	Performs the dot product on two Vectos.
*/
ml_data dot(const cVector &a, const cVector &b){
	int i;
	ml_data out = 0;

#ifdef ML_CHECK
	if(a.size != b.size){
		sprintf(errMsg,"%s MUST be the same size, out = dot(%s(%d),%s(%d))",__PRETTY_FUNCTION__,a.name,a.size,b.name,b.size);
		throw cMLError(cMLError::FATAL,errMsg);
	}
#endif 

	for(i=0;i<a.size;i++)
		out += a.p[i]*b.p[i];

	a.releaseTmp();
	b.releaseTmp();

	return out;
}

/*!
Performs the cross product of two vectors. Both cVectors MUST be of
 size 3.
 \return the cross product
 */
cVector& cross( const cVector &a, const cVector &b ){

#ifdef ML_CHECK
	if(a.size != 3 ||  b.size != 3){
		sprintf(errMsg,"%s vectors MUST be of size 3, out = dot(%s(%d),%s(%d))",__PRETTY_FUNCTION__,a.name,a.size,b.name,b.size);
		throw cMLError(cMLError::FATAL,errMsg);
	}
#endif

	cVector *c = a.getTmp(3);

	c->p[0] = a.p[1]*b.p[2] - b.p[1]*a.p[2];
	c->p[1] = -(a.p[0]*b.p[2] - b.p[0]*a.p[2]);
	c->p[2] = a.p[0]*b.p[1] - b.p[0]*a.p[1];

	a.releaseTmp();
	b.releaseTmp();

	return *c;
}

/*!
Performs the outer product.
 \returns a cMatrix = a*Transpose(b)
 */
cMatrix& outer( const cVector &a, const cVector &b ){
	int i,j;

#ifdef ML_CHECK
	if(a.size != b.size ){
		sprintf(errMsg,"%s %s(%d) * Transpose(%s(%d)) \n",__PRETTY_FUNCTION__,a.name,a.size,b.name,b.size);
		throw cMLError(cMLError::FATAL,errMsg);
	}
#endif

	cMatrix *c = a.getTmp(a.size,a.size);

	for(i=0;i<a.size;i++){
		for(j=0;j<b.size;j++){
			c->p[i*a.size+j] = a.p[i]*b.p[j];
		}
	}

	a.releaseTmp();
	b.releaseTmp();

	return *c;
}

//------------- special ----------------------------


/*!
Output a matrix to the terminal
 */
std::ostream& operator<<(std::ostream &s,const cVector &v){
	printf("--- Vector %s(%d) --------\n",v.getName(),v.getSize());

	for (int j=1;j<=v.getSize();j++){
		s<<v(j)<<"\t";
	}
	s<<"\n\n";
	
	//s<<"\n";

	v.releaseTmp();

	return s;
}


/*!
Index to access elements of a matrix
 \warning this function assumes the vector is one based. This means
 that the first element of the vector is v.p[0] is access by v(1).
*/
ml_data& cVector::operator()(int i)const{
#ifdef ML_CHECK
	if(i<=0 || i>size){
		sprintf(errMsg,"%s %s(%d) is out of bounds",__PRETTY_FUNCTION__,name,i);
		throw cMLError(cMLError::FATAL,errMsg);
	}
#endif
	return *(p+i-1);
}

/*!
Sets all values of a vector to 1.
 */
cVector& cVector::ones(void){
	int i;
	for(i=0;i<size;i++)
		p[i] = 1.0;

	return *this;
}
