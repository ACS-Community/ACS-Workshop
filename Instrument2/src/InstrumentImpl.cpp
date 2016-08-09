#include <baciDB.h>
#include <InstrumentS.h>
#include <iostream>
#include <InstrumentImpl.h>

using namespace baci;

int LOCALCOUNT = 0;

ACE_Log_Priority LOCAL_LOGGING_LEVEL = LM_INFO;

const static int MAX_LOGS = 5;

Instrument::Instrument(const ACE_CString &name, maci::ContainerServices * containerServices) :
                ACSComponentImpl(name, containerServices),

                on(false)
{}

void Instrument::cameraOn()
{
	on = true;
	
	ACS_SHORT_LOG((LOCAL_LOGGING_LEVEL, "Instrument on."));
}

void Instrument::cameraOff()
{
	on = false;

	ACS_SHORT_LOG((LOCAL_LOGGING_LEVEL, "Instrument off."));
}

::TYPES::ImageType * Instrument::takeImage (::CORBA::Long exposureTime)
{
	if (on) {
		std::cout << "Instrument::takeImage(" << exposureTime << ");" << std::endl;	
	} else {
		std::cout << "Instrument::takeImage failed! Camera is off." << std::endl;
	}

	ACS_SHORT_LOG((LOCAL_LOGGING_LEVEL, "takeImage"));
	
	return NULL;
}

void Instrument::setRGB (const ::TYPES::RGB & rgbConfig)
{
	std::cout << "Instrument::setRGB(...);" << std::endl;
}
	
void Instrument::setPixelBias(::CORBA::Long bias)
{
	std::cout << "Instrument::setPixelBias(" << bias << ");" << std::endl;
}

void Instrument::setResetLevel(::CORBA::Long resetLevel)
{
	std::cout << "Instrument::setResetLevel(" << resetLevel << ");" << std::endl;
}

void Instrument::initialize() throw (acsErrTypeLifeCycle::acsErrTypeLifeCycleExImpl)
{
}

void Instrument::execute() throw (acsErrTypeLifeCycle::acsErrTypeLifeCycleExImpl)
{
}

void Instrument::cleanUp()
{
}

void Instrument::aboutToAbort()
{
}


#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(Instrument)

