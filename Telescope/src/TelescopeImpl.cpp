#include <TelescopeImpl.h>


TelescopeImpl::TelescopeImpl(
		const ACE_CString& name,
		maci::ContainerServices * containerServices) :
		acscomponent::ACSComponentImpl::ACSComponentImpl(name, containerServices)
{
	const char * __METHOD__ = "::TelescopeImpl::TelescopeImpl";
	ACS_TRACE(__METHOD__);
}

TelescopeImpl::~TelescopeImpl()
{
	const char * __METHOD__ = "::TelescopeImpl::~TelescopeImpl";
	ACS_TRACE(__METHOD__);
	if (m_image_p != NULL)
		delete (m_image_p);
	
}

void TelescopeImpl::initialize()
	throw (acsErrTypeLifeCycle::acsErrTypeLifeCycleExImpl)
{
	const char * __METHOD__ = "::TelescopeImpl::initialize";
	ACS_TRACE(__METHOD__);

}

void TelescopeImpl::execute()
	throw (acsErrTypeLifeCycle::acsErrTypeLifeCycleExImpl)
{
	const char * __METHOD__ = "::TelescopeImpl::execute";
	ACS_TRACE(__METHOD__);
	try{	
	m_nexstar_p = H3E::TelescopeControl::_nil();
	m_instrument_p = INSTRUMENT_MODULE::Instrument::_nil();

	m_nexstar_p = getContainerServices()->getDefaultComponent<H3E::TelescopeControl>("IDL:alma/H3E/TelescopeControl:1.0");
        m_instrument_p = getContainerServices()->getComponent<INSTRUMENT_MODULE::Instrument>("INSTRUMENT");	
	}catch(...){}
}
	
void TelescopeImpl::cleanUp()
{
	const char * __METHOD__ = "::TelescopeImpl::cleanUp";
	ACS_TRACE(__METHOD__);
	
	m_nexstar_p = H3E::TelescopeControl::_nil();
	m_instrument_p = INSTRUMENT_MODULE::Instrument::_nil();
}

/** 
* Moves to the given position and takes an exposure
* of length exposureTime (seconds). Raises and exception 
* if the requested position is out of limits.
*
* @param coordinates target coordinates
* @param exposureTime exposure time of the current observation
* @return Image 
* 
*/
TYPES::ImageType* TelescopeImpl::observe(const ::TYPES::Position & coordinates,
        					::CORBA::Long exposureTime)
{
	const char * __METHOD__ = "TelescopeImpl::observe";
	TYPES::ImageType* image;
	::CORBA::Long* buffer = new ::CORBA::Long(3);
	m_image_p = new TYPES::ImageType(1UL, 2UL, buffer, false);
	image = m_image_p;
	ACS_TRACE(__METHOD__);
	try{
		delete (buffer);
		delete (m_image_p);
		TelescopeImpl::moveTo(coordinates);
		
		image = (m_instrument_p->takeImage(exposureTime));
		ACS_LOG(LM_RUNTIME_CONTEXT, __METHOD__ ,
					(LM_DEBUG, "Image taken"));
		return image;
	}catch(...)
	{
		return m_image_p;
	}
}
//raises(UOSErr::PositionOutOfLimitsEx);

/** 
* Commands the telescope to move to the given position. Raises
* an exception if the requested position is out of limits.
*
* @param coordinates Requested telescope position
* @return None
*/
void TelescopeImpl::moveTo(const ::TYPES::Position & coordinates)
{
	const char * __METHOD__ = "TelescopeImpl::moveTo";
	
	ACS_TRACE(__METHOD__);
	try{	
		m_nexstar_p->setTo (
			coordinates.el,
			coordinates.az);
			ACS_LOG(LM_RUNTIME_CONTEXT, __METHOD__ ,
					(LM_DEBUG, "Position reached"));
	}catch(...){}
}
//raises(UOSErr::PositionOutOfLimitsEx);

/**
* Get the current telescope position.
*
* @return current telescope position
*/
TYPES::Position TelescopeImpl::getCurrentPosition()
{
	const char * __METHOD__ = "TelescopeImpl::getCurrentPosition";
	TYPES::Position currentPos;
	
	ACS_TRACE(__METHOD__);
	try{
		//obtenemos la posicion
		ACSErr::Completion_var compl;
		currentPos.el = m_nexstar_p->actualAltitude()->get_sync(compl.out());
		currentPos.az = m_nexstar_p->actualAzimuth()->get_sync(compl.out());
		ACS_LOG(LM_RUNTIME_CONTEXT, __METHOD__ ,
					(LM_DEBUG, "Coordinates taken"));
	}catch(...){}

	return currentPos;
}

/* --------------- [ MACI DLL support functions ] -----------------*/
#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS (TelescopeImpl)
/* ----------------------------------------------------------------*/

