#ifndef CameraImpl_h
#define CameraImpl_h

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

///Contains the defintion of the standard superclass for C++ components
#include <acscomponentImpl.h>
#include <TypesC.h>
#include <InstrumentS.h>



///CORBA generated servant stub
class CameraImpl: public acscomponent::ACSComponentImpl,     //Standard component superclass
	        public virtual POA_INSTRUMENT_MODULE::Camera   //CORBA servant stub
{
  public:   
    /**
     * Constructor
     * @param poa Poa which will activate this and also all other components. 
     * @param name component's name. This is also the name that will be used to find the
     * configuration data for the component in the Configuration Database.
     */
    CameraImpl(
	     const ACE_CString &name,
	     maci::ContainerServices *& containerServices);
    
    /**
     * Destructor
     */
    virtual ~CameraImpl();
    
    /* --------------------- [ CORBA interface ] ----------------------*/
    
    virtual TYPES::ImageType* getFrame(CORBA::Long exposure);




};
/*\@}*/
/*\@}*/

#endif

