#include <InstrumentImpl.h>
#include <SYSTEMErr.h>


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


    ACS_TRACE("InstrumentImpl::execute");

 
    camera = getContainerServices()->getComponent<INSTRUMENT_MODULE::Camera> ("CAMERA");
    if (camera == INSTRUMENT_MODULE::Camera::_nil() )
    {
        ACS_SHORT_LOG((LM_ERROR,"Building::execute: cant retrive component Camera"));
    }

/*
void a() {
    NetworkError ex = new NetworkError();
    throw ex;
}
    try {
           a();
	} catch (NetworkError ex) {
           ex.log();
           throw;
           NetworkError nex = new NetworkError(ex);
           throw nex;
	} catch (OutOfMemory ex) {
	}
*/
}

void InstrumentImpl::cleanUp()
{
     ACS_TRACE("InstrumentImpl::cleanUp");
    getContainerServices()->releaseComponent("CAMERA");
}

void InstrumentImpl::cameraOn()
{
   ACS_TRACE("InstrumentImpl::cameraOn");
   cameraState = true;   // Camera ON
}

void InstrumentImpl::cameraOff()
{
    ACS_TRACE("InstrumentImpl::cameraOff");
   cameraState = false;   // Camera OFF
}


TYPES::ImageType *InstrumentImpl::takeImage(CORBA::Long exposureTime)
{
     ACS_TRACE("InstrumentImpl::takeImage");
     if (cameraState == true)
	return camera->getFrame(exposureTime);
     else {
        SYSTEMErr::CameraIsOffExImpl ex(__FILE__, __LINE__, "InstrumentImpl::takeImage");
        ex.log();
        //ACS_SHORT_LOG((LM_ERROR,"InstrumentImpl::takeImage: can't take image from Camera"));
        throw ex.getCameraIsOffEx();
     }
        
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


bool InstrumentImpl::checkCameraState()
{
   return cameraState;
}

#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(InstrumentImpl)
