#include "InstrumentImpl.h"

InstrumentImpl::InstrumentImpl(const ACE_CString &name, maci::ContainerServices *cs) : ACSComponentImpl(name, cs) {
	std::cout << "constructor instrument" << std::endl;
	cameraState = false;
}

         
void InstrumentImpl::cameraOn (void) {
	STATIC_LOG_TO_DEVELOPER(LM_INFO, "camera is on");
	cameraState = true;
};
         
void InstrumentImpl::cameraOff (void) {
	STATIC_LOG_TO_DEVELOPER(LM_INFO, "camera is off");
	cameraState = false;
};
         
::TYPES::ImageType *InstrumentImpl::takeImage (::CORBA::Long exposureTime){ 
	TYPES::ImageType* img;
	img=NULL;
	STATIC_LOG_TO_DEVELOPER(LM_INFO, "take image begin");
	if(cameraState == false) {
		STATIC_LOG_TO_DEVELOPER(LM_INFO, "image taking impossible because the camera is off");
		SYSTEMErr::CameraIsOffEx ex;
		throw ex;
	} 
	if(exposureTime>5) {
		STATIC_LOG_TO_DEVELOPER(LM_INFO, "expose time is over the expected limit ");
		SYSTEMErr::CameraIsOffEx ex;
		throw ex;
	}
	try {
		sleep(exposureTime);
	
	        const ::CORBA::Long bufferlength = 20;
	        ::CORBA::ULong max = 1000;
	        ::CORBA::ULong length = bufferlength;
	        ::CORBA::Long* buffer = new ::CORBA::Long[bufferlength];
	        ::CORBA::Boolean release = false;
	        for(int i=0;i<bufferlength;i++)
	                buffer[i] = (::CORBA::Long)i;
	        img = new TYPES::ImageType(max, length,buffer,release);

		STATIC_LOG_TO_DEVELOPER(LM_INFO, "take image end");
        }
	catch(...) {
		STATIC_LOG_TO_DEVELOPER(LM_INFO, "exception camera is off");
		SYSTEMErr::CameraIsOffEx ex;
		throw ex;
	}

	return img;
};
        
void InstrumentImpl::setRGB (const ::TYPES::RGB & rgbConfig){};
        
void InstrumentImpl::setPixelBias ( ::CORBA::Long bias){};
        
void InstrumentImpl::setResetLevel ( ::CORBA::Long resetLevel){};


#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(InstrumentImpl)
