/*************************************************************************
cRand.cpp -- random number generator
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

#include "cRand.h"


static char errMsg[ERROR_STRING_SIZE];


/////////////////////////////////////////////////////////////////
/// Constructor. Sets up the random number generator.
/// \todo figure out how to include /dev/urandom here.
/////////////////////////////////////////////////////////////////
cRand::cRand(int bs){
	//srandomdev(); // seed rand function
	seed = new unsigned long[bs];
	//initstate(12345,state,256);
	//setstate(state);
	bufferSize = bs;
	file = NULL;
	index = 0;

	initSeed();

	max = (ml_data)RAND_MAX;
	
}

/////////////////////////////////////////////////////////////////
/// Destructor. Deletes the seed that was created.
/////////////////////////////////////////////////////////////////
cRand::~cRand(void){
	delete[] seed;
	//fclose(file);
}


/////////////////////////////////////////////////////////////////
/// Returns the current random number which is between 0 and 1.
/////////////////////////////////////////////////////////////////
ml_data cRand::get(void){
	srandom(seed[index]);
	//srandom(2);
	index = ++index > bufferSize ? -1 : index;
	if(index == -1){
		initSeed();
		index = 0;
	}
	return (ml_data)random()/max;
}


/////////////////////////////////////////////////////////////////
/// Open the random number generator file either "/dev/random"
/// or "/dev/urandom".
/////////////////////////////////////////////////////////////////
void cRand::openRandDev(void){
#if 1
	if((file = fopen("/dev/urandom","r")) == NULL){
		printf("Unable to open /dev/urandom!\n\n");
		throw cMLError(cMLError::FATAL,errMsg);
	}
#else
	if((file = fopen("/dev/random","r")) == NULL){
		printf("Unable to open /dev/random!\n\n");
		throw cMLError(cMLError::FATAL,errMsg);
	}
#endif
}


/////////////////////////////////////////////////////////////////
/// Open a user specified random number generator source file.
/////////////////////////////////////////////////////////////////
void cRand::openRandDev(char *filename){
	if((file = fopen(filename,"r")) == NULL){
		sprintf(errMsg,"Unable to open %s!",filename);
		throw cMLError(cMLError::FATAL,errMsg);
	}

}


/////////////////////////////////////////////////////////////////
/// Sets up the seed so the random number generator is random.
/////////////////////////////////////////////////////////////////
void cRand::initSeed(void){
	int size;

	if ( file != NULL ){
		fclose(file);
		file = NULL;
	}
	
	openRandDev();

	if( (size = fread(seed,sizeof(unsigned long),bufferSize,file) ) != bufferSize ){
		sprintf(errMsg,"Unable to read rand file!");
		throw cMLError(cMLError::FATAL,errMsg);
	}
	
	fclose(file);
	file = NULL;
}

