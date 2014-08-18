#ifndef H3EImpl_h
#define H3EImpl_h

/*
 * "@(#) $Id: MOUNT2.h,v 1.3 2010-11-17 20:42:26 its Exp $"
 *
 * $Log: MOUNT2.h,v $
 * Revision 1.3  2010-11-17 20:42:26  its
 * Adding Characteristics Component implementation for AltAz External Module
 *
 * Revision 1.2  2008/11/27 17:32:34  almamgr
 * Asynchronous -> Synchronous calls
 *
 * Revision 1.1.1.1  2008/11/24 00:37:29  almamgr
 * Initial import
 *
 * Revision 1.2  2007/07/10 18:50:28  wg2
 *  - First DevIO functional implementation (some TODO)
 *
 * Revision 1.1  2007/07/09 20:30:15  wg2
 *  - Included tests and several more files
 *
 * Revision 1.1.1.1  2007/07/09 12:47:39  wg8
 * Repository Setup
 *
 *
 */

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

#include <baciCharacteristicComponentImpl.h>
#include <baciSmartPropertyPointer.h>


#include <baciRWdouble.h>
#include <baciRWpattern.h>
#include <baciROdouble.h>

#include <H3ES.h>

using namespace baci;


class MOUNT2Impl : public virtual CharacteristicComponentImpl,
                        public virtual POA_H3E::TelescopeControl
{
	public:
		/**
	 	 * Constructor
	   * @param containerServices ContainerServices which are needed for 
		 * various component related methods.
	   * @param name component name
	   */
		MOUNT2Impl(const ACE_CString& name, 
				            maci::ContainerServices* containerServices);

		/**
	 	 * Destructor
	 	 */
		virtual ~MOUNT2Impl();

		
/* ------------------- [ Lifecycle START interface ] --------------------- */

		/**
	 	 * Lifecycle method called at component initialization state.
	 	 * Overwrite it in case you need it.
	 	 */
		virtual void initialize(void) throw (acsErrTypeLifeCycle::acsErrTypeLifeCycleExImpl);

		/**
	 	 * Lifecycle method called at component execute state.
	 	 * Overwrite it in case you need it.
	 	 */
		virtual void execute(void) throw (acsErrTypeLifeCycle::acsErrTypeLifeCycleExImpl);

		/**
	 	 * Lifecycle method called at component clean up state.
	 	 * Overwrite it in case you need it.
	 	 */
		virtual void cleanUp(void);

		/**
	   * Lifecycle method called at component abort state.
	   * Overwrite it in case you need it.
	   */
		virtual void aboutToAbort(void);

/* ------------------- [ Lifecycle END interface ] --------------- */

/* --------------------- [ CORBA START interface ] ----------------*/

	virtual void setTo(CORBA::Double altitude, CORBA::Double azimuth) throw(CORBA::SystemException);

	virtual void offSet(CORBA::Double altOffset, CORBA::Double azOffset) throw(CORBA::SystemException);

	virtual void zenith() throw(CORBA::SystemException);

	virtual void park() throw(CORBA::SystemException);

	virtual void setUncalibrated() throw(CORBA::SystemException);

	virtual void calibrateEncoders() throw(CORBA::SystemException);

	virtual ACS::RWdouble_ptr commandedAltitude() throw(CORBA::SystemException);

	virtual ACS::RWdouble_ptr commandedAzimuth() throw(CORBA::SystemException);

	virtual ACS::ROdouble_ptr actualAltitude() throw(CORBA::SystemException);

	virtual ACS::ROdouble_ptr actualAzimuth() throw(CORBA::SystemException);

	virtual ACS::RWpattern_ptr status() throw(CORBA::SystemException);


/* --------------------- [ CORBA END interface ] ----------------*/

/* ----------------------------------------------------------------*/

	private:
		/**
		 * Copy constructor is not allowed following the ACS desgin rules.
		 */
		MOUNT2Impl(const MOUNT2Impl&);

		/**
	 	 * Assignment operator is not allowed due to ACS design rules.
	 	 */
		void operator=(const MOUNT2Impl&);


/* --------------------- [ Constants START ] ----------------------*/
		const static int setTo_ACTION = 0;
		const static int offSet_ACTION = 1;
		const static int zenith_ACTION = 2;
		const static int park_ACTION = 3;

/* --------------------- [ Constants END ] ------------------------*/

/* --------------------- [ Properties START ] ----------------------*/

	SmartPropertyPointer<RWdouble> commandedAltitude_m;
	SmartPropertyPointer<RWdouble> commandedAzimuth_m;
	SmartPropertyPointer<ROdouble> actualAltitude_m;
	SmartPropertyPointer<ROdouble> actualAzimuth_m;
	SmartPropertyPointer<RWpattern> status_m;

/* --------------------- [ Properties END ] ------------------------*/

/* --------------------- [ Local properties START ] ----------------*/
//#include <H3ELocalProperties.h>
	bool         component_running;
	std::string  component_name;
/* --------------------- [ Local properties END ] ------------------*/
};


#endif /* H3EImpl_h */

