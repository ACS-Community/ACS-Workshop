#ifndef _INSTRUMENT1_H
#define _INSTRUMENT1_H

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

#include <acscomponentImpl.h>
#include <InstrumentS.h>
#include <TypesC.h>
#include <SYSTEMErr.h>
#include <CameraC.h>
#include <ACSErrTypeCommon.h>
#include <loggingACEMACROS.h>
#include <stdlib.h>
#include <sstream>
 

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
    
	virtual void cameraOn (void);
		
	virtual void cameraOff (void);

	virtual TYPES::ImageType* takeImage (CORBA::Long exposureTime);
		//raises(SYSTEMErr::CameraIsOffEx);

	virtual void setRGB (const TYPES::RGB & rgbConfig);
		//raises(SYSTEMErr::CameraIsOffEx);		

	virtual void setPixelBias (CORBA::Long bias);
		//raises(SYSTEMErr::CameraIsOffEx);

	virtual void setResetLevel (CORBA::Long resetLevel);
		//raises(SYSTEMErr::CameraIsOffEx);
private:
	CAMERA_MODULE::Camera_var Camera;
	int status;
};
/*\@}*/
/*\@}*/

#endif
