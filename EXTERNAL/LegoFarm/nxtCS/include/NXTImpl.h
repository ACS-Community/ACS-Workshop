#ifndef NXTImpl_h
#define NXTImpl_h
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
* "@(#) $Id: NXTImpl.h,v 1.1 2009/11/09 03:01:39 almamgr Exp $"
*
* who       when      what
* --------  --------  ----------------------------------------------
* jslopez  2008-10-26 Initial implementation
*
*/

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

#include <acscomponentImpl.h>
#include "NXTS.h"
#include "TypesC.h"
#include "DevNXTS.h"
#include "AstarS.h"

using namespace acscomponent;

class NXTImpl: public virtual ACSComponentImpl,
	public virtual POA_NXT_MODULE::NXT
{
	public:
		NXTImpl(const ACE_CString &name, maci::ContainerServices *containerServices);

		virtual ~NXTImpl();

		virtual void initialize() throw (acsErrTypeLifeCycle::LifeCycleExImpl);
		
		virtual void cleanUp();

		virtual void associate(const char *componentName)
			throw(CORBA::SystemException);

		virtual void setObjective(const TYPES::Positions & positions,
				CORBA::Long legoNumber)
			throw(CORBA::SystemException);

		virtual void runVehicle() throw(CORBA::SystemException);

		void getRoad(const TYPES::Grid & gr);

		void forward();
		
		void turnRight();

		void turnLeft();

		void spin();

	private:
		DEVNXT_MODULE::DevNXT_var devnxt_comp;
		ASTAR_MODULE::Astar_var astar_comp;
		TYPES::Solution road;
		TYPES::Grid individualGrid;
		bool associatedToDevNxt;
		/**
		 * ALMA C++ coding standards state copy operators should be disabled.
		 */
		void operator=(const NXTImpl&);

};

#endif
