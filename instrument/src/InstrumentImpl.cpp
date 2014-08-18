#include <baciDB.h>
#include <InstrumentImpl.h>
#include <ACSErrTypeCommon.h>
#include <iostream>
#include <stdio.h>
#include <string.h>


ACE_RCSID(acsexmpl, Instrument, "$Id: InstrumentImpl.cpp,v 1.3 2010-11-19 18:00:10 instrument Exp $")

using namespace baci;

Instrument::Instrument(
			       const ACE_CString name,
	       maci::ContainerServices * containerServices):
	ACSComponentImpl(name, containerServices)
	 
  {
    ACS_TRACE("::Instrument Imp::");
    
    m_camera_p = DEVCCD_MODULE::DevCCD::_nil();
}

Instrument::~Instrument()
{
    ACS_TRACE("::Instrument::~Instrument");
    ACS_DEBUG_PARAM("::Instrument Impl::~Instrument Impl", "Destroying %s...", name());
}

void Instrument::cameraOn()
{
    ACS_TRACE("::Camera On...::");
    ACS_SHORT_LOG((LM_INFO, "Getting component DEVCCD" ));

    m_camera_p = DEVCCD_MODULE::DevCCD::_nil();

    m_camera_p = getContainerServices()->getComponent<DEVCCD_MODULE::DevCCD>("DEVCCD");

    if (CORBA::is_nil(m_camera_p.in()))
	{
		ACS_SHORT_LOG((LM_WARNING, "Could not get reference to DEVCCD" ));
		throw acsErrTypeLifeCycle::LifeCycleExImpl(__FILE__, __LINE__, "::Building::execute");		
    }else{
    	ACS_TRACE("::DevCCDRef obtained...::");
    }
    
    m_camera_p->on();
    
}

void Instrument::cameraOff()
{
    ACS_TRACE("::Camera Off...::");
}

void Instrument::setRGB(const TYPES::RGB&) {
}

void Instrument::setPixelBias(CORBA::Long bias) {
}

void Instrument::setResetLevel(CORBA::Long resetLevel) {
}

TYPES::ImageType* Instrument::takeImage(CORBA::Long exposureTime) {
ACS_TRACE("::Camera takeImage...::");
    ACS_SHORT_LOG((LM_INFO, "Getting component DEVCCD" ));

    m_camera_p = DEVCCD_MODULE::DevCCD::_nil();

    m_camera_p = getContainerServices()->getComponent<DEVCCD_MODULE::DevCCD>("DEVCCD");

    if (CORBA::is_nil(m_camera_p.in()))
        {
                ACS_SHORT_LOG((LM_WARNING, "Could not get reference to DEVCCD" ));
                throw acsErrTypeLifeCycle::LifeCycleExImpl(__FILE__, __LINE__, "::Building::execute");
    }else{
        ACS_TRACE("::DevCCDRef obtained...::");
    }

    return m_camera_p->image(0);

}

/* --------------- [ MACI DLL support functions ] -----------------*/
#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(Instrument)
/* ----------------------------------------------------------------*/

