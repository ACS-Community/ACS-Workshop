#ifndef Telescope_H
#define Telescope_H

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

///Contains the defintion of the standard superclass for C++ components
#include <baciCharacteristicComponentImpl.h>

///CORBA generated servant stub
#include <TelescopeS.h>
#include <H3EC.h>
#include <InstrumentC.h>

///Includes for each BACI property used in this example
#include <baciROdouble.h>
#include <baciRWdouble.h>
#include <baciROlong.h>
#include <baciROstring.h>

///Include the smart prointer for the properties
#include <baciSmartPropertyPointer.h>

///Include the acs thread header
#include <acsThread.h>

#include <iostream>

using namespace baci;

class TelescopeImpl : public acscomponent::ACSComponentImpl,
		public virtual POA_TELESCOPE_MODULE::Telescope    //CORBA servant stub
{
public:

	/**
	* Constructor
	* @param poa poa which will activate this and also all other components
	* @param name component name
	*/
	TelescopeImpl(
		const ACE_CString& name,
		maci::ContainerServices * containerServices);
	
	/**
	* Destructor
	*/
	virtual ~TelescopeImpl();

	//initialize
	virtual void initialize()
		throw (acsErrTypeLifeCycle::acsErrTypeLifeCycleExImpl);
		
	virtual void execute()
		throw (acsErrTypeLifeCycle::acsErrTypeLifeCycleExImpl);
		
	virtual void cleanUp();
	
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
	 TYPES::ImageType* observe(const ::TYPES::Position & coordinates,
        				::CORBA::Long exposureTime);
	//raises(UOSErr::PositionOutOfLimitsEx);
	
	
	/** 
	 * Commands the telescope to move to the given position. Raises
	 * an exception if the requested position is out of limits.
	 *
	 * @param coordinates Requested telescope position
	 * @return None
	 */
	void moveTo(const ::TYPES::Position & coordinates);
	//raises(UOSErr::PositionOutOfLimitsEx);

	/**
	 * Get the current telescope position.
	 *
	 * @return current telescope position
	 */
	TYPES::Position getCurrentPosition();

private:
	maci::ContainerServices * m_containerServices;
	H3E::TelescopeControl_var m_nexstar_p;
	INSTRUMENT_MODULE::Instrument_var m_instrument_p;
	TYPES::ImageType* m_image_p;

}; // class Telescope

#endif // Telescope_H
