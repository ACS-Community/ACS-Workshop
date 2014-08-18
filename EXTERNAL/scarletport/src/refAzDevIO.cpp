/*******************************************************************************
* CSRG - UTFSM Computer Systems Research Group
* ACS-UTFSM Team
*
*
*    This library is free software; you can redistribute it and/or
*    modify it under the terms of the GNU Lesser General Public
*    License as published by the Free Software Foundation; either
*    version 2.1 of the License, or (at your option) any later version.
*
*    This library is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*    Lesser General Public License for more details.
*
*    You should have received a copy of the GNU Lesser General Public
*    License along with this library; if not, write to the Free Software
*    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*
*/

#include <refAzDevIO.h>
#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

using namespace baci;


refAzDevIO::refAzDevIO() {
};

refAzDevIO::~refAzDevIO() {
};

bool refAzDevIO::initializeValue()
{ 
	std::cout<<"Intialized"<<std::endl;
	return true; }

CORBA::Double refAzDevIO::read(unsigned long long& timestamp)
{
   //Method is so simple no error can occur.
   std::cout<<"read"<<std::endl;

   timestamp = ::baci::getTimeStamp();
   double value;
   int link = openLego("/dev/legousbtower0");
   value=getVariable(link,TARGET_AZIMUTH);
   return value;
}

void refAzDevIO::write(const CORBA::Double &value, unsigned long long& timestamp)
{
   //Method is so simple no error can occur.
//   std::cout<<"write"<<std::endl;

   timestamp = ::baci::getTimeStamp();
   int link = openLego("/dev/legousbtower0");
//    std::cout<<"Dev. opened"<<std::endl;
   setVariable(link,TARGET_AZIMUTH,(int)value);
//   std::cout<<"Write donw"<<std::endl;
}


