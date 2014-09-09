#include <InstrumentImpl.h>



InstrumentImpl::InstrumentImpl(
	     const ACE_CString &name,
	     maci::ContainerServices *& containerServices):
	acscomponent::ACSComponentImpl(name, containerServices)
{

   cameraOn();

}

InstrumentImpl::~InstrumentImpl()
{

   cameraOff();

}


void InstrumentImpl::cameraOn()
{
   CameraState = true;   // Camera ON
}

void InstrumentImpl::cameraOff()
{
   CameraState = false;   // Camera OFF
}


TYPES::ImageType *InstrumentImpl::takeImage(CORBA::Long exposureTime)
{

  return NULL;
}

void InstrumentImpl::setRGB(TYPES::RGB rgbConfig)
{

}

void InstrumentImpl::setPixelBias(CORBA::Long bias)
{

}


void InstrumentImpl::setResetLevel(CORBA::Long resetLevel)
{

}



