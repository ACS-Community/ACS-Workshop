#ifndef RCBASEIMPL_H
#define RCBASEIMPL_H
/*******************************************************************************
* ALMA - Atacama Large Millimiter Array
* (c) UNSPECIFIED - FILL IN, 2009 
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*
* "@(#) $Id: RCBaseImpl.h,v 1.3 2009/11/13 15:25:36 almamgr Exp $"
*
* who       when      what
* --------  --------  ----------------------------------------------
* almadev  2009-11-11  created
*/

/************************************************************************
 *
 *----------------------------------------------------------------------
 */

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

#include <baciCharacteristicComponentImpl.h>
//#include <baci.h>
///CORBA generated servant stub
#include <RCBaseS.h>
#include "RobotControlC.h"

namespace MARS
{
	class RCBaseImpl: public virtual baci::CharacteristicComponentImpl, public POA_MARS::RCBase {
	
		public:
		
		RCBaseImpl(const ACE_CString &name,
				maci::ContainerServices *containerServices);
		~RCBaseImpl();
	
	 	void move(const pos_struct &pos);
		void executeCommand(CORBA::Long action);
		void park();
		robot_status_struct status();
		pos_struct position();
		CORBA::Long robotType();

		// Life cycle methods
		virtual void initialize();
		virtual void cleanUp();
		virtual void aboutToAbort();
		
		private:
		robot_status_struct myStatus;
		pos_struct parkPosition;
		std::string robotName;
		ROBOTCONTROL_MODULE::RobotControl * Lego; 
//      CORBA::String_var internalHello;
	
	};
};
#include<maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(MARS::RCBaseImpl)

#endif /* RCBASEIMPL_H*/
