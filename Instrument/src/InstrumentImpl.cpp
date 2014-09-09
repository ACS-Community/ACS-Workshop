#include <InstrumentImpl.h>



InstrumentImpl::InstrumentImpl(
	     const ACE_CString &name,
	     maci::ContainerServices *& containerServices):
	acscomponent::ACSComponentImpl(name, containerServices),
	cameraState(false)
{

}

InstrumentImpl::~InstrumentImpl()
{

}

void InstrumentImpl::execute()
{
//m_door_p = getContainerServices()->getComponent<acsexmplBuilding::Door>(frontDoorCobName.c_str()); 
   camera = getContainerServices()->getComponent<INSTRUMENT_MODULE::Camera> ("CAMERA");

}

void InstrumentImpl::cleanUp()
{
    getContainerServices()->releaseComponent("CAMERA");
}

void InstrumentImpl::cameraOn()
{
   cameraState = true;   // Camera ON
}

void InstrumentImpl::cameraOff()
{
   cameraState = false;   // Camera OFF
}


TYPES::ImageType *InstrumentImpl::takeImage(CORBA::Long exposureTime)
{

  return NULL;
}

void InstrumentImpl::setRGB(const TYPES::RGB& rgbConfig)
{

}

void InstrumentImpl::setPixelBias(CORBA::Long bias)
{

}


void InstrumentImpl::setResetLevel(CORBA::Long resetLevel)
{

}


#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(InstrumentImpl)
