/*************************************************************************
cMLError.cpp - error class.
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

/////////////////////////////////////////////////////////////
/// Constructor cError holds the type of error and a string 
/// to identify what the error was.
/// \param a type of error
/// \param msg string holding
/////////////////////////////////////////////////////////////
cMLError::cMLError(int a, const char *msg){
	type = a;

	if (msg == NULL){
		strcpy(errMsg," -- no error message -- ");
	}
	else {
		strcpy(errMsg,msg);
	}
}

/////////////////////////////////////////////////////////////
/// Destructor. This does nothing.
/////////////////////////////////////////////////////////////
cMLError::~cMLError(void){
	;
}


/////////////////////////////////////////////////////////////
/// Prints error to stdout in a format that displays the type
/// of error and the error message.
/////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream &s, cMLError &e){
	s<<"ERROR(";
	switch(e.type){
		case cMLError::FATAL:
			s<<"FATAL";
			break;
		case cMLError::NONFATAL:
			s<<"WARNING";
			break;
	}
	s<<"): "<<e.errMsg<<std::endl;
	return s;
}

/////////////////////////////////////////////////////////////
/// Concatonate two errors together. The highest error type
/// is always kept and concatonates the error strings together.
/////////////////////////////////////////////////////////////
void cMLError::operator+=(cMLError &e){
	type = type > e.type ? type : e.type;
	strcat(errMsg,"\n");
	strcat(errMsg,e.errMsg);
}

/////////////////////////////////////////////////////////////
/// Concatonates the error strings together.
/////////////////////////////////////////////////////////////
void cMLError::operator+=(char *e){
	strcat(errMsg,e);
}
