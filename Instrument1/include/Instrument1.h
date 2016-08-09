#ifndef _INSTRUMENT1_H
#define _INSTRUMENT1_H

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

#include <acscomponentImpl.h>
#include <InstrumentS.h>
#include <TypesC.h>
 
class Instrument1: public virtual acscomponent::ACSComponentImpl,     //Component superclass
		  public POA_INSTRUMENT_MODULE::Instrument    //CORBA servant stub
{    
  public:
    /**
     * Constructor
     * @param poa Poa which will activate this and also all other components. Developers need
     * not be concerned with what a PortableServer does...just pass it to the superclass's
     * constructor.
     * @param name component's name. All components have a name associated with them so other 
     * components and clients can access them.
     */
    Instrument1(
	       const ACE_CString& name,
	       maci::ContainerServices * containerServices);
    
    /**
     * Destructor
     */
    virtual ~Instrument1();
    
    /* --------------------- [ CORBA interface ] ----------------------*/    
    /**
	 * Turns the Instrument camera on.
	 *
	 * @return None
	 */
	virtual void cameraOn (void) = 0;

	/**
	 * Turns the Instrument off.
	 * A NULL string as the target identifier indicates that no image file
	 * should be saved.
	 *
	 * @todo Function should be refactored so that only one operation is performed.
	 *
	 * @return None
	 */
	virtual void cameraOff (void) = 0;

	/**
	 * Retrieve image from the Instrument. Raises an exception if the 
	 * camera is not on.
	 *
	 * @return array of longs containing the image pixels
	 */
	 virtual TYPES::ImageType * takeImage (CORBA::Long exposureTime) = 0;
 
};
/*\@}*/
/*\@}*/

#endif
