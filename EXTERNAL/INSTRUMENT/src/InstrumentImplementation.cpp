/*******************************************************************************
*    ALMA - Atacama Large Millimiter Array
*    (c) European Southern Observatory, 2002
*    Copyright by ESO (in the framework of the ALMA collaboration)
*    and Cosylab 2002, All rights reserved
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
*
*
* "@(#) $Id: acsexmplLampImpl.cpp,v 1.101 2008/10/01 04:30:47 cparedes Exp $"
*
* who       when      what
* --------  --------  ----------------------------------------------
* acaproni 2004-04-06 Use of smart pointer for properties
* david 2002-08-08 changed defines to const static int's
* david 2002-06-16 added a few comments and deleted RESET_ACTION define
* almamgr 2002-04-07 Removed poa parameter from call to ConstructorEpilogue()
* blopez   2002-04-05 Modified for ACSDO usage. Header removed.
* almamgr 2002-01-22 Replaced old include files with new axsexmpl... files
* msekoran 2001-07-06 improved error handling
* msekoran 2001-03-10 integrated with new BACI; ALMA coding convention used
* gchiozzi 2001-02-15 Added real implementation for method descriptor()
* gchiozzi 2001-02-15 Added body of get_interface() method for Object Explorer
* gchiozzi 2001-02-15 created standard header 
*/


#include <InstrumentImplementation.h>
#include <logging.h>
#include <SORTErr.h>


InstrumentInt::InstrumentInt(
	   const ACE_CString &name,
	   maci::ContainerServices * containerServices) :
	   acscomponent::ACSComponentImpl(name, containerServices)
{
   CameraStatus=false;
}

InstrumentInt::~InstrumentInt()
{
}

/* --------------------- [ CORBA interface ] ----------------------*/

void
InstrumentInt::cameraOn ()
{
  CameraStatus = true;
  ACS_SHORT_LOG((LM_INFO, "Camera is ON"));
}

void
InstrumentInt::cameraOff ()
{
    //registers the action to the BACI and returns control immediately
    //getComponent()->registerAction(BACIValue::type_null, cb, desc, this, OFF_ACTION);
	CameraStatus = false;
	ACS_SHORT_LOG((LM_INFO, "Camera is OFF"));
}


	TYPES::ImageType* InstrumentInt::takeImage(CORBA::Long exposureTime)
{
    //registers the action to the BACI and returns control immediately
    //getComponent()->registerAction(BACIValue::type_null, cb, desc, this, OFF_ACTION);


	TYPES::ImageType_var image(new TYPES::ImageType);

	if (CameraStatus == true){
		image->length(100L);
		int ii;
	    for (ii=0;ii<100;ii++){
	    		image[ii]=ii;
	    }
	    ACS_SHORT_LOG((LM_INFO, "Image is ready."));
	}
	else
	{
		SORTErr::CameraIsOffExImpl ex(__FILE__, __LINE__, __PRETTY_FUNCTION__);
		ex.log();
		throw ex.getCameraIsOffEx();
	}

	return image._retn();
}

void
InstrumentInt::setRGB(const TYPES::RGB& rgbConfig)
{
	ACS_SHORT_LOG((LM_INFO, "Metod not implemented"));
}

void
InstrumentInt::setPixelBias(CORBA::Long bias)
{
	ACS_SHORT_LOG((LM_INFO, "Metod not implemented"));
}

void
InstrumentInt::setResetLevel(CORBA::Long resetLevel)
{
	ACS_SHORT_LOG((LM_INFO, "Metod not implemented"));
}


/* --------------- [ MACI DLL support functions ] -----------------*/
#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(InstrumentInt)
/* ----------------------------------------------------------------*/

