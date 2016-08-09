#include <baciDB.h>
#include <InstrumentS.h>
#include <iostream>
#include <InstrumentImpl.h>
#include <SYSTEMErr.h>

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
	ACS_TRACE("::Instrument::cameraOn");

	on = true;
}

void Instrument::cameraOff()
{
	ACS_TRACE("::Instrument::cameraOff");

	on = false;
}

::TYPES::ImageType * Instrument::takeImage (::CORBA::Long exposureTime)
{
	ACS_TRACE("::Instrument::takeImage");

	if (on) {
		ACS_SHORT_LOG((LOCAL_LOGGING_LEVEL, "TakeImage with exposureTime = %li", (long)exposureTime));	

		// chill, this is just a test.
		return new ::TYPES::ImageType();
	}
	
	SYSTEMErr::CameraIsOffExImpl ex(__FILE__, __LINE__, "Instrument::takeImage");
	ex.log();

	throw ex.getCameraIsOffEx();
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

