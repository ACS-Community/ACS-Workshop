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
*/

#include <refAltDevIO.h>

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

refAltDevIO::refAltDevIO() {
};

refAltDevIO::~refAltDevIO() {
};

bool refAltDevIO::initializeValue()
{ return true; }

CORBA::Double refAltDevIO::read(unsigned long long& timestamp)
{
   //Method is so simple no error can occur.

   timestamp = ::baci::getTimeStamp();
   double value;
   int link = openLego("/dev/legousbtower0");
   value=getVariable(link,TARGET_ELEVATION);
   return value;
}

void refAltDevIO::write(const CORBA::Double &value, unsigned long long& timestamp)
{
   //Method is so simple no error can occur.

   timestamp = ::baci::getTimeStamp();
   int link = openLego("/dev/legousbtower0");
   setVariable(link,TARGET_ELEVATION,(int)value);

}

