
#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

#include <Instrument1.h>
#include <ACSErrTypeCommon.h>
#include <loggingACEMACROS.h>
#include "SYSTEMErr.h"
#include "CameraC.h"

    /**
     * Constructor
     * @param poa Poa which will activate this and also all other components. Developers need
     * not be concerned with what a PortableServer does...just pass it to the superclass's
     * constructor.
     * @param name component's name. All components have a name associated with them so other 
     * components and clients can access them.
     */
    Instrument1::Instrument1(
	       const ACE_CString& name,
	       maci::ContainerServices * containerServices):
    ACSComponentImpl(name, containerServices)
{
	//ACS_TRACE("::Instrument1::Instrument1");
	//camara = CAMERA_MODULE::Camera::_nil();
	//camara = getContainerServices()->getComponent<CAMERA_MODULE::Camera>("CAMERA")
}
    
   /**
     * Destructor
     */
	Instrument1::~Instrument1(){}
    
   /* --------------------- [ CORBA interface ] ----------------------*/    
    
	void Instrument1::cameraOn()
	{
	std::cout << "cameraOn OK" << std::endl;
	
	//	throw ACSErrTypeCommon::CouldntAccessComponentExImpl
	//			   __FILE__, __LINE__,
	//			   "ClientErrorComponent::testTypeException");	
	}

	void Instrument1::cameraOff ()
	{
		std::cout << "cameraOff OK" << std::endl;
		ACS_SHORT_LOG((LM_INFO, "camera is off"));
	}

	TYPES::ImageType* Instrument1::takeImage(CORBA::Long exposureTime)
	{	
		//const char* expTime = (std::string("1/")+std::string(exposureTime)).c_str();
		//const char* iso = "1/10";
		//const char* iso = "100";	
		
		TYPES::ImageType* Image = new TYPES::ImageType(10);
		/*CAMERA_MODULE::Camera_var Camera_component;

		Camera_component = getContainerServices()->getComponent<CAMERA_MODULE::Camera>("CAMERA");
		
		Image = Camera_component->takeImage("1/10","100");*/
		std::cout << "takeImage OK" << std::endl;
		return Image;	
	}

	void Instrument1::setRGB (const TYPES::RGB & rgbConfig){}

	void Instrument1::setPixelBias (CORBA::Long bias){}

	void Instrument1::setResetLevel (CORBA::Long resetLevel){}

	
/*--------------- [ MACI DLL support functions] -------------------*/
#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(Instrument1)
/*-----------------------------------------------------------------*/
	 
/*\@}*/
/*\@}*/

