#include <baciDB.h>
#include <InstrumentS.h>
#include <iostream>
#include <InstrumentImpl.h>
#include <SYSTEMErr.h>
#include <string>

using namespace baci;

int LOCALCOUNT = 0;

ACE_Log_Priority LOCAL_LOGGING_LEVEL = LM_INFO;

const static int MAX_LOGS = 5;

// Please forgive us
std::string shutterSpeedValues[52]= {
    "30",
    "25",
    "20",
    "15",
    "13",
    "10",
    "8",
    "6",
    "5",
    "4",
    "3.2",
    "2.5",
    "2",
    "1.6",
    "1.3",
    "1",
    "0.8",
    "0.6",
    "0.5",
    "0.4",
    "0.3",
    "1/4",
    "1/5",
    "1/6",
    "1/8",
    "1/10",
    "1/13",
    "1/15",
    "1/20",
    "1/25",
    "1/30",
    "1/40",
    "1/50",
    "1/60",
    "1/80",
    "1/100",
    "1/125",
    "1/160",
    "1/200",
    "1/250",
    "1/320",
    "1/400",
    "1/500",
    "1/640",
    "1/800",
    "1/1000",
    "1/1250",
    "1/1600",
    "1/2000",
    "1/2500",
    "1/3200",
    "1/4000"
};


std::string longExposureTransform(::CORBA::Long exposureTime)
{
	static ::CORBA::Long shutterSpeedMap[52] = {
		30000000, 25000000, 20000000, 15000000, 13000000, 10000000,
		8000000, 6000000, 5000000, 4000000, 3200000, 2500000,
		2000000, 1600000, 1300000, 1000000, 800000, 600000,
		500000, 400000, 300000, 250000, 200000, 166667,
		125000, 100000, 76923, 666667, 50000, 40000,
		33333, 25000, 20000, 16667, 12500, 10000,
		8000, 6250, 5000, 4000, 3125, 2500,
		2000, 1563, 1250, 1000, 800, 625,
		500, 400, 313, 250
	};

	::CORBA::Long diff;
	for (int i = 0; i < 52; i++) {
		diff = exposureTime - shutterSpeedMap[i];

		if (diff == 0) {
			return shutterSpeedValues[i];
		} else if (diff > 0) {
			ACS_SHORT_LOG((LM_WARNING, "Exposure time not exact. Fixed to %li.", (long)shutterSpeedMap[i]));
			return shutterSpeedValues[i];
		}
	}

	ACS_SHORT_LOG((LM_WARNING, "Exposure time below bound. Fixed to %li.", (long)shutterSpeedMap[51]));
	return shutterSpeedValues[51];	
}


Instrument::Instrument(const ACE_CString &name, maci::ContainerServices * containerServices) :
                ACSComponentImpl(name, containerServices),
                on(false),
		camera(CAMERA_MODULE::Camera::_nil())
{
}

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
		std::string tr = longExposureTransform(exposureTime);	
		ACS_SHORT_LOG((LOCAL_LOGGING_LEVEL, "TakeImage with exposureTime = %s", tr.c_str()));	
		return camera->takeImage(tr.c_str(), "400");
	}
	
	SYSTEMErr::CameraIsOffExImpl ex(__FILE__, __LINE__, "Instrument::takeImage");
	ex.log();

	throw ex.getCameraIsOffEx();
}

void Instrument::setRGB (const ::TYPES::RGB & rgbConfig)
{
	ACS_TRACE("Instrument::setRGB");
}
	
void Instrument::setPixelBias(::CORBA::Long bias)
{
	ACS_TRACE("Instrument::setPixelBias");
}

void Instrument::setResetLevel(::CORBA::Long resetLevel)
{
	ACS_TRACE("Instrument::setResetLevel");
}

void Instrument::initialize() throw (acsErrTypeLifeCycle::acsErrTypeLifeCycleExImpl)
{
	ACS_TRACE("Instrument::initialize");
}

void Instrument::execute() throw (acsErrTypeLifeCycle::acsErrTypeLifeCycleExImpl)
{
	ACS_TRACE("Instrument::execute");
	
	camera = getContainerServices()->getComponent<CAMERA_MODULE::Camera>("CAMERA");

	if (CORBA::is_nil(camera.in())) {
		throw acsErrTypeLifeCycle::LifeCycleExImpl(__FILE__, __LINE__, "::Building::execute");
	}
}

void Instrument::cleanUp()
{
	ACS_TRACE("Instrument::cleanUp");
}

void Instrument::aboutToAbort()
{
	ACS_TRACE("Instrument::aboutToAbort");
}


#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(Instrument)

