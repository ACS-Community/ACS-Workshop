#ifndef LegoNXTImpl_h
#define LegoNXTImpl_h
/*******************************************************************************
*    ALMA - Atacama Large Millimiter Array
*    (c) European Southern Observatory, 2002
*    Copyright by ESO (in the framework of the ALMA collaboration)
*    and Cosylab 2002, All rights reserved
*
*    This library is free software; you can redistribute it and/or
*    modify it under the terms of the GNU Lesser General Public
*    License as published by the Free Software Foundation; either
*    version 2.1 of the License, or (at your option) any later version.
*
*    This library is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*    Lesser General Public License for more details.
*
*    You should have received a copy of the GNU Lesser General Public
*    License along with this library; if not, write to the Free Software
*    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*
*
* "@(#) $Id: LegoNXTImpl.h,v 1.1 2009/11/09 03:01:39 almamgr Exp $"
*
* who       when      what
* --------  --------  ----------------------------------------------
* jslopez  2008-10-18 Refurbish of an old component
*
*/

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

#include <baciCharacteristicComponentImpl.h>
#include <baciSmartPropertyPointer.h>

#include <baciROstring.h>

#include "DevNXTS.h"
#include "TypesC.h"
#include <bluecomm.h>

using namespace baci;

class LegoNXTImpl : public virtual CharacteristicComponentImpl,  //Standard component superclass
                     public virtual POA_DEVNXT_MODULE::DevNXT     //CORBA servant stub
{

	public:
		LegoNXTImpl(const ACE_CString& name,
				maci::ContainerServices* containerServices);

		virtual ~LegoNXTImpl();

		/* --------------------- [ Life cycle methods ] -------------------- */
		virtual void initialize()
			throw (acsErrTypeLifeCycle::acsErrTypeLifeCycleExImpl);

		virtual void cleanUp();


		/* --------------------- [ CORBA interface ] ----------------------*/   
		virtual void 
			setMotorState(const TYPES::MotorState& mState) 
			throw(CORBA::SystemException);

		virtual TYPES::MotorState
			getMotorState(CORBA::Long port)
			throw(CORBA::SystemException);

		virtual void 
			setSensorState(const TYPES::SensorState& sState)
			throw(CORBA::SystemException);

		virtual CORBA::Long
			getSensorState(CORBA::Long port)
			throw(CORBA::SystemException);

		virtual ACS::ROstring_ptr 
			macAddress()
			throw(CORBA::SystemException);

	private:

		/**
		 * macAddress property pointer
		 */
		SmartPropertyPointer<ROstring> m_macAddress_sp;

		/**
		 * NXT Brick reference
		 */
		BlueComm *nxt;

		/**
		 * Current mode of the robot
		 */
		int currentMode;

		bool connectedToLego;

		std::string  component_name;

		/**
		 * Copy constructor is not allowed following the ACS desgin rules.
		 */
		LegoNXTImpl(const LegoNXTImpl&);

		/**
		 * Assignment operator is not allowed due to ACS design rules.
		 */
		void operator=(const LegoNXTImpl&);
};

#endif 
