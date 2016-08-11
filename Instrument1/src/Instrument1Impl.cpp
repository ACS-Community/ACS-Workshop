
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
		Camera = getContainerServices()->getDefaultComponent<CAMERA_MODULE::Camera>("IDL:acsws/CAMERA_MODULE/Camera:1.0");
		//Camera = getContainerServices()->getComponent<CAMERA_MODULE::Camera>("CAMERA");
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

//const char* shutterSpeedsValues[]{
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
		
		//std::string temp = shutterSpeedValues[exposureTime];
			
		//std::ostringstream oss;
		//oss << temp;
		//const char* expTime = oss.str().c_str();
		const char* expTime="1";
		const char* iso = "400";
		int flag=0;
	
		// Maps shutter speeds to microseconds equivalents
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
        	for (int i = 0; i < 52; i++) 
		{
                	diff = exposureTime - shutterSpeedMap[i];
                	if (diff == 0) 
			{
                        	flag=1;
				expTime =  shutterSpeedValues[i].c_str();		
			}
		}
		
		if (flag ==0)
			ACS_SHORT_LOG((LM_WARNING, "Exposure time not exact. Default Exposure Time = 1 seconds"));

		TYPES::ImageType* Image = new TYPES::ImageType;
		
		if (status == 1)
		{
			try
			{
				Image = Camera->takeImage(expTime,iso);	
				ACS_SHORT_LOG((LM_INFO, "TakeImage is Ready, Exposure Time = %s seconds",expTime));
			}
		
			catch(int e)
			{
				ACS_SHORT_LOG((LM_INFO, "Problem detected for takeImage"));
			}
		}
		else
		{
			ACS_SHORT_LOG((LM_INFO, "Problem detected, Verify if Camera ON"));
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

