#ifndef _TELESCOPE_CPP_H_
#define _TELESCOPE_CPP_H_
/*******************************************************************************
* ALMA - Atacama Large Millimiter Array
* Copyright (c) UNSPECIFIED - FILL IN, 2014 
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*
*
* who       when      what
* --------  --------  ----------------------------------------------
* almadev3  2014-11-04  created
*/

/************************************************************************
 *
 *----------------------------------------------------------------------
 */

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

#include <acscomponentImpl.h>
#include <InstrumentC.h>
#include <TelescopeS.h>
#include <TypesS.h>
#include <SYSTEMErrS.h>

class TelescopeCppImpl :
	public acscomponent::ACSComponentImpl,
	public POA_TELESCOPE_MODULE::Telescope {
public:
	TelescopeCppImpl(const ACE_CString &name, maci::ContainerServices *cs);
	virtual ~TelescopeCppImpl();


::TYPES::ImageType* observe(const ::TYPES::Position & coordinates,
	      ::CORBA::Long exposureTime)
                throw(SYSTEMErr::PositionOutOfLimitsEx);
void moveTo (const ::TYPES::Position & coordinates)
		throw(SYSTEMErr::PositionOutOfLimitsEx);


::TYPES::Position getCurrentPosition (void);
private:
TYPES::Position pos;
INSTRUMENT_MODULE::Instrument_var instrument_p;
 };
#endif
