
#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

#include <Instrument1.h>


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
    		ACSComponentImpl(name, containerServices),
		Camera(CAMERA_MODULE::Camera::_nil())
{
	status = 0; //CAMERA Logically OFF
	try
	{
		Camera = getContainerServices()->getComponent<CAMERA_MODULE::Camera>("CAMERA");
	}
	catch(int e)
	{
		ACS_SHORT_LOG((LM_INFO, "Unable to get CAMERA component"));
	}

}
    
   /**
     * Destructor
     */
	Instrument1::~Instrument1(){}
    
   /* --------------------- [ CORBA interface ] ----------------------*/    
    
	void Instrument1::cameraOn()
	{
		if(status == 0)
		{
			status = 1;
			ACS_SHORT_LOG((LM_INFO, "Camera is ON"));
		}
		else
		{
			ACS_SHORT_LOG((LM_INFO, "Camera already ON"));
		}

	}

	void Instrument1::cameraOff ()
	{
		if(status == 1)
		{
			status = 0;
			ACS_SHORT_LOG((LM_INFO, "Camera is OFF"));
		}
		else
		{
			ACS_SHORT_LOG((LM_INFO, "Camera already OFF"));
		}
	}

	TYPES::ImageType* Instrument1::takeImage(CORBA::Long exposureTime)
	{	
		std::ostringstream oss;
		oss << exposureTime;
		const char* expTime = oss.str().c_str();
		
		const char* iso = "400";		

		TYPES::ImageType* Image = new TYPES::ImageType;
		
		if (status == 1)
		{
			try
			{
				Image = Camera->takeImage(expTime,iso);	
				ACS_SHORT_LOG((LM_INFO, "TakeImage is Ready"));
			}
		
			catch(int e)
			{
				ACS_SHORT_LOG((LM_INFO, "Problem detected for takeImage"));
			}
		}
		else
		{
			SYSTEMErr::CameraIsOffExImpl ex (__FILE__, __LINE__, "Instrument1::takeImage");
			throw ex.getCameraIsOffEx();
		}
		
		return Image;	
	}

	void Instrument1::setRGB (const TYPES::RGB & rgbConfig)
	{
		if(status == 0)
		{
			SYSTEMErr::CameraIsOffExImpl ex (__FILE__, __LINE__, "Instrument1::setRGB");
			throw ex.getCameraIsOffEx();
		}
		else
		{
			ACS_SHORT_LOG((LM_INFO, "setRGB is Ready"));
		}
	}

	void Instrument1::setPixelBias (CORBA::Long bias)
	{
		if(status == 0)
		{
			SYSTEMErr::CameraIsOffExImpl ex (__FILE__, __LINE__, "Instrument1::setPixelBias");
			throw ex.getCameraIsOffEx();
		}
		else
		{
			ACS_SHORT_LOG((LM_INFO, "setPixelBias is Ready"));
		}
	}

	void Instrument1::setResetLevel (CORBA::Long resetLevel)
	{
		if(status == 0)
		{
			SYSTEMErr::CameraIsOffExImpl ex (__FILE__, __LINE__, "Instrument1::setResetLevel");
			throw ex.getCameraIsOffEx();
		}
		else
		{
			ACS_SHORT_LOG((LM_INFO, "setResetLevel is Ready"));
		}
	}
	
/*--------------- [ MACI DLL support functions] -------------------*/
#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(Instrument1)
/*-----------------------------------------------------------------*/
	 
/*\@}*/
/*\@}*/

