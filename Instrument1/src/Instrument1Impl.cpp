
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
    /**
	* Turns the Instrument camera on.
	*
	* @return None
	*/
	void Instrument1::cameraOn()
	{
		std::cout << "cameraOn OK" << std::endl;
		
	}

	/**
	 * Turns the Instrument off.
	 * A NULL string as the target identifier indicates that no image file
	 * should be saved.
	 *
	 * @todo Function should be refactored so that only one operation is performed.
	 *
	 * @return None
	 */
	void Instrument1::cameraOff ()
	{
		std::cout << "cameraOff OK" << std::endl;	
	}

	/**
	 * Retrieve image from the Instrument. Raises an exception if the 
	 * camera is not on.
	 *
	 * @return array of longs containing the image pixels
	 */
	TYPES::ImageType* Instrument1::takeImage(CORBA::Long exposureTime)
	{
		std::cout << "takeImage OK" << std::endl;
		return NULL;	
	}
	 
/*\@}*/
/*\@}*/

