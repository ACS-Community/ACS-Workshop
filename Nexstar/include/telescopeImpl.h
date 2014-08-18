#ifndef telescopeImpl_h
#define telescopeMountImpl_h


#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

#include <acscomponentImpl.h>
#include <ACSErrTypeCommon.h>

#include <TelescopeS.h>

#include <TypesS.h>
#include <H3EC.h>
#include <InstrumentC.h>
 

enum states{
	DOWN,ON
};
/*
 * ACSComponent, that control Nexstar telescope, making use of telescopeControl
 * characteristic component.
 */
class Telescope: public virtual acscomponent::ACSComponentImpl,
			     public POA_TELESCOPE_MODULE::Telescope
{
  public:
	/*
	 * Constructor
	 * @param name Name associated with the component
	 */
    Telescope(
	  const ACE_CString& name,
	  maci::ContainerServices * containerServices);
    
    /*
     * Destructor
     *
     */
    virtual ~Telescope();

    /*
     * cleanUp
     * Kill components started
     */
    void cleanUp();

    /*
     * execute
     * Call components needed
     */
    void execute();
    
    /*
     * Start telescopeControl Module
     *
     */
    void getTelescopeControl();

    /*
     *Start Instrument Module
     *
     */
    void getInstrumentControl();

    /* --------------- [ CORBAinterface ] -------------- */

      
    /**
     * Moves to the given position and takes an exposure
     * of length exposureTime (seconds). Raises and exception
     * if the requested position is out of limits.
     *
     * @param coordinates target coordinates
     * @param exposureTime exposure time of the current observation
     * @return Image
     *
     */
    void moveTo (const ::TYPES::Position & coordinates);
    
    /**
     * Get the current telescope position.
     *
     * @return current telescope position
     */

    TYPES::Position
	getCurrentPosition(); 

    /**
     * Moves to the given position and takes an exposure
     * of length exposureTime (seconds). Raises and exception
     * if the requested position is out of limits.
     *
     * @param coordinates target coordinates
     * @param exposureTime exposure time of the current observation
     * @return Image
     *
     */

    ::TYPES::ImageType *
    observe (const ::TYPES::Position & coordinates,
             ::CORBA::Long exposureTime);

    /*
     * Private attributes
     */
  private:
    /*
     * Needed components
     */
    H3E::TelescopeControl *telescopeControl;
    INSTRUMENT_MODULE::Instrument *instrumentControl;

    /*
     * Actual State of modules
     */
    states telescopeControlState;
    states instrumentControlState;

    void operator=(const Telescope&);
};
/*\@}*/
/*\@}*/

#endif /*!telescopeImpl_H*/



