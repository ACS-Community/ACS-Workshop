
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
    ACSComponentImpl(name, containerServices)
{}
    
   /**
     * Destructor
     */
	Instrument1::~Instrument1(){}
    
   /* --------------------- [ CORBA interface ] ----------------------*/    
    
	void Instrument1::cameraOn()
	{
		std::cout << "cameraOn OK" << std::endl;
		
	}

	void Instrument1::cameraOff ()
	{
		std::cout << "cameraOff OK" << std::endl;	
	}

	TYPES::ImageType* Instrument1::takeImage(CORBA::Long exposureTime)
	{
		std::cout << "takeImage OK" << std::endl;
		return NULL;	
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

