#ifndef InstrumentImpl_h
#define InstrumentImpl_h

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

///Contains the defintion of the standard superclass for C++ components
#include <acscomponentImpl.h>
#include <TypesC.h>
#include <InstrumentS.h>



///CORBA generated servant stub
class InstrumentImpl: public acscomponent::ACSComponentImpl,     //Standard component superclass
	        public virtual POA_INSTRUMENT_MODULE::Instrument   //CORBA servant stub
{
  public:   
    /**
     * Constructor
     * @param poa Poa which will activate this and also all other components. 
     * @param name component's name. This is also the name that will be used to find the
     * configuration data for the component in the Configuration Database.
     */
    InstrumentImpl(
	     const ACE_CString &name,
	     maci::ContainerServices *& containerServices);
    
    /**
     * Destructor
     */
    virtual ~InstrumentImpl();
    
    /* --------------------- [ CORBA interface ] ----------------------*/
    /**
     * Camera of       *
         * Turns the Instrument camera on.
         *
         * @return None
         */
        virtual void cameraOn ();

        /**
         * Turns the Instrument off.
         * A NULL string as the target identifier indicates that no image file
         * should be saved.
         *
         * @todo  Function should be refactored so that only one operation is performed.
         *
         * @return None
         */
        virtual void cameraOff ();

 	/**
         * Retrieve image from the Instrument. Raises an exception if the
         * camera is not on.
         *
         * @return array of longs containing the image pixels
         */
        TYPES::ImageType *takeImage(CORBA::Long exposureTime);

        /* Camera settings */

        /**
         * Set the RGB configuration of the camera
         * @param rgbConfig the TYPES::RGB configuration
         */
        virtual void setRGB(const TYPES::RGB& rgbConfig);

        /**
         * Set the pixel bias configuration of the camera
         * @param bias the pixel bias configuration
         */
        virtual void setPixelBias(CORBA::Long bias);


     	/**
         * Set the reset level configuration of the camera
         * @param resetLevel the reset level configuration
         */
        virtual void setResetLevel(CORBA::Long resetLevel);





};
/*\@}*/
/*\@}*/

#endif

