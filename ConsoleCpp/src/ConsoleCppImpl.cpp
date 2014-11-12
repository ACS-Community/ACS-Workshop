#include "ConsoleCppImpl.h"

ConsoleCppImpl::ConsoleCppImpl(const ACE_CString &name, maci::ContainerServices *cs) : ACSComponentImpl(name, cs)
{
}
                        
ConsoleCppImpl::~ConsoleCppImpl()
{
}

void ConsoleCppImpl::execute()
{
	ACS_TRACE("ConsoleCppImpl::execute");
 	scheduler_p = getContainerServices()->getComponent<SCHEDULER_MODULE::Scheduler>("SCHEDULER");
 	instrument_p = getContainerServices()->getComponent<INSTRUMENT_MODULE::Instrument>("INSTRUMENT");
 	telescope_p = getContainerServices()->getComponent<TELESCOPE_MODULE::Telescope>("TELESCOPE");

 	m_mode = false;
 	//scheduler_p->stop();
}

void ConsoleCppImpl::cleanUp()
{
	ACS_TRACE("ConsoleCppImpl::cleanUp");

	if (scheduler_p != SCHEDULER_MODULE::Scheduler::_nil())
	{
		getContainerServices()->releaseComponent("SCHEDULER");
		scheduler_p = SCHEDULER_MODULE::Scheduler::_nil();
	}

	if (instrument_p != INSTRUMENT_MODULE::Instrument::_nil())
	{
		getContainerServices()->releaseComponent("INSTRUMENT");
		instrument_p = INSTRUMENT_MODULE::Instrument::_nil();
	}

	if (telescope_p != TELESCOPE_MODULE::Telescope::_nil())
	{
		getContainerServices()->releaseComponent("TELESCOPE");
		telescope_p = TELESCOPE_MODULE::Telescope::_nil();
	}
}

void ConsoleCppImpl::setMode(bool mode) throw (SYSTEMErr::AlreadyInAutomaticEx)
{	
	ACS_SHORT_LOG((LM_DEBUG, "ConsoleCppImpl::setMode"));

	if (mode) // automatic mode
	{
		if (m_mode) { // if the automatic mode is asked twice
			ACS_SHORT_LOG((LM_ERROR, "SYSTEMErr::AlreadyInAutomaticEx"));
			SYSTEMErr::AlreadyInAutomaticExImpl ex(__FILE__, __LINE__, "ConsoleCppImpl::setMode");
			throw ex.getAlreadyInAutomaticEx();
		} else {
			ACS_SHORT_LOG((LM_INFO, "Setting mode to automatic mode"));
			m_mode = true;
			scheduler_p->start();
		}
	}else{
		ACS_SHORT_LOG((LM_INFO, "Setting mode to manual mode"));
		m_mode = false;
		scheduler_p->stop();
	}		
}

bool ConsoleCppImpl::getMode()
{
	ACS_SHORT_LOG((LM_DEBUG, "ConsoleCppImpl::getMode"));
	return m_mode;
}

void ConsoleCppImpl::cameraOn() throw (SYSTEMErr::SystemInAutoModeEx)
{
	ACS_SHORT_LOG((LM_DEBUG, "ConsoleCppImpl::cameraOn"));

	if (!m_mode) { // manual mode
	    ACS_SHORT_LOG((LM_INFO, "Turning on camera for manual mode"));
		instrument_p->cameraOn();
	}else{
		ACS_SHORT_LOG((LM_INFO, "In automatic mode we do nothing"));

		ACS_SHORT_LOG((LM_ERROR, "SYSTEMErr::SystemInAutoModeEx"));
		SYSTEMErr::SystemInAutoModeExImpl ex(__FILE__, __LINE__, "ConsoleCppImpl::cameraOn");
		throw ex.getSystemInAutoModeEx();
	}
}

void ConsoleCppImpl::cameraOff() throw (SYSTEMErr::SystemInAutoModeEx)
{
	ACS_SHORT_LOG((LM_DEBUG, "ConsoleCppImpl::cameraOff"));

	if (!m_mode) { // manual mode
	    ACS_SHORT_LOG((LM_INFO, "Turning off camera for manual mode"));
		instrument_p->cameraOff();
	}else{
		ACS_SHORT_LOG((LM_INFO, "In automatic mode we do nothing"));

		ACS_SHORT_LOG((LM_ERROR, "SYSTEMErr::SystemInAutoModeEx"));
		SYSTEMErr::SystemInAutoModeExImpl ex(__FILE__, __LINE__, "ConsoleCppImpl::cameraOff");
		throw ex.getSystemInAutoModeEx();
	}
}

void  ConsoleCppImpl::moveTelescope(const ::TYPES::Position & coordinates)
	throw (SYSTEMErr::PositionOutOfLimitsEx, SYSTEMErr::SystemInAutoModeEx)
{    
	ACS_SHORT_LOG((LM_DEBUG, "ConsoleCppImpl::moveTelescope"));

	if (!m_mode) { // manual mode
	    ACS_SHORT_LOG((LM_INFO, "Moving Telescope for manual mode"));

	    try {
			telescope_p->moveTo(coordinates);
	    } catch (SYSTEMErr::PositionOutOfLimitsEx &ex) {
			ACS_SHORT_LOG((LM_ERROR, "PositionOutOfLimitsEx exception was raised by Telescopy Component"));
			throw ex;
	    }
	}else{
		ACS_SHORT_LOG((LM_INFO, "In automatic mode we do nothing"));

		ACS_SHORT_LOG((LM_ERROR, "SYSTEMErr::SystemInAutoModeEx"));
		SYSTEMErr::SystemInAutoModeExImpl ex(__FILE__, __LINE__, "ConsoleCppImpl::moveTelescope");
		throw ex.getSystemInAutoModeEx();
	}
}

TYPES::Position  ConsoleCppImpl::getTelescopePosition()
{
    ACS_SHORT_LOG((LM_DEBUG, "ConsoleCppImpl::getTelescopePosition"));
    
    ACS_SHORT_LOG((LM_INFO, "Getting Telescope Position"));
	return telescope_p->getCurrentPosition();
}

TYPES::ImageType* ConsoleCppImpl::getCameraImage()
	throw (SYSTEMErr::SystemInAutoModeEx, SYSTEMErr::CameraIsOffEx)
{
    ACS_SHORT_LOG((LM_DEBUG, "ConsoleCppImpl::getCameraImage"));
        
    if (!m_mode) { // manual mode
	    ACS_SHORT_LOG((LM_INFO, "Getting Camera Image for manual mode"));
		return instrument_p->takeImage(10); // 10 is just a random number
	} else { // auto mode
        ACS_SHORT_LOG((LM_INFO, "In automatic mode we do nothing"));

        ACS_SHORT_LOG((LM_ERROR, "SYSTEMErr::SystemInAutoModeEx"));
		SYSTEMErr::SystemInAutoModeExImpl ex(__FILE__, __LINE__, "ConsoleCppImpl::getCameraImage");
		throw ex.getSystemInAutoModeEx();
	}
}

void  ConsoleCppImpl::setRGB(const ::TYPES::RGB & rgbConfig)
	throw (SYSTEMErr::CameraIsOffEx)
{
    ACS_SHORT_LOG((LM_DEBUG, "ConsoleCppImpl::setRGB"));

    ACS_SHORT_LOG((LM_INFO, "Setting RGB for manual mode"));
	instrument_p->setRGB(rgbConfig);
}

void  ConsoleCppImpl::setPixelBias(::CORBA::Long bias)
	throw (SYSTEMErr::CameraIsOffEx)
{
    ACS_SHORT_LOG((LM_DEBUG, "ConsoleCppImpl::setPixelBias"));

	ACS_SHORT_LOG((LM_INFO, "Setting Pixel Bias for manual mode"));
	instrument_p->setPixelBias(bias);
}

void  ConsoleCppImpl::setResetLevel(::CORBA::Long resetLevel)
	throw (SYSTEMErr::CameraIsOffEx)
{
    ACS_SHORT_LOG((LM_DEBUG, "ConsoleCppImpl::setResetLevel"));

	ACS_SHORT_LOG((LM_INFO, "Setting Reset Level for manual mode"));
	instrument_p->setResetLevel(resetLevel);
}

#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(ConsoleCppImpl)
