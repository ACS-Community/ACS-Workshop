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
* "@(#) $Id: RCBaseImpl.cpp,v 1.5 2009/11/13 15:25:36 almamgr Exp $"
*
* who       when      what
* --------  --------  ----------------------------------------------
* almadev  2009-11-11  created 
*/

/************************************************************************
*   NAME
*   
* 
*   SYNOPSIS
*
*   
*   PARENT CLASS
*
* 
*   DESCRIPTION
*
*
*   PUBLIC METHODS
*
*
*   PUBLIC DATA MEMBERS
*
*
*   PROTECTED METHODS
*
*
*   PROTECTED DATA MEMBERS
*
*
*   PRIVATE METHODS
*
*
*   PRIVATE DATA MEMBERS
*
*
*   FILES
*
*   ENVIRONMENT
*
*   COMMANDS
*
*   RETURN VALUES
*
*   CAUTIONS 
*
*   EXAMPLES
*
*   SEE ALSO
*
*   BUGS   
* 
*------------------------------------------------------------------------
*/

#include "vltPort.h"

static char *rcsId="@(#) $Id: RCBaseImpl.cpp,v 1.5 2009/11/13 15:25:36 almamgr Exp $"; 
static void *use_rcsId = ((void)&use_rcsId,(void *) &rcsId);

#include "RCBaseImpl.h"
#include "cdb.h"
#include "cdbErrType.h"

using namespace MARS;

RCBaseImpl::RCBaseImpl(const ACE_CString &name, maci::ContainerServices *containerServices):
  baci::CharacteristicComponentImpl::CharacteristicComponentImpl(name, containerServices)
{
	Lego = 0;
}

void RCBaseImpl::initialize()
{
	myStatus.isParked = true;
	myStatus.isOperational = false;
	myStatus.id = 0;
	// get the robot type
	try
	{
		ACE_CString cdbPath="alma/";
		cdbPath+=name();
		
		CDB::DAL_var cdb = getContainerServices()->getCDB();
		CDB::DAO_var dao = cdb->get_DAO_Servant (cdbPath.c_str());
		myStatus.type = dao->get_long("robot_type");
		parkPosition.x = dao->get_long("park_x");
		parkPosition.y = dao->get_long("park_y");
		// get the reference to the RobotControl PY component
		Lego = getContainerServices()->getComponent<ROBOTCONTROL_MODULE::RobotControl>("ROBOTCONTROL");
	}
//getCDB
	catch(acsErrTypeContainerServices::CanNotGetCDBExImpl & exc)
	{
		exc.log();
		ACS_SHORT_LOG((LM_ERROR,"Cannot get CDB"));
	}
// get_DAO_Servant
	catch(CORBA::SystemException & exc)
	{
		ACS_SHORT_LOG((LM_ERROR,"CORBA System exception"));
	}
	catch(cdbErrType::CDBRecordDoesNotExistExImpl & exc)
	{
		exc.log();
		ACS_SHORT_LOG((LM_ERROR,"Record does not exist in CDB."));
	}
	catch(cdbErrType::CDBXMLErrorExImpl & exc)
	{
		exc.log();
		ACS_SHORT_LOG((LM_ERROR,"Error parsing xml files. Please use cdbChecker !!!"));
	}
// get_long
	catch (cdbErrType::WrongCDBDataTypeExImpl & exc)
	{
		exc.log();
		ACS_SHORT_LOG((LM_ERROR,"Wrong data type on CDB"));
	}
	catch(cdbErrType::CDBFieldDoesNotExistExImpl & exc)
	{
		exc.log();
		ACS_SHORT_LOG((LM_ERROR,"Field does not exist on CDB"));
	}
// getComponent
	catch( maciErrType::CannotGetComponentExImpl & exc )
	{
		exc.log();
		ACS_SHORT_LOG((LM_ERROR,"Cannot get component \"ROBOTCONTROL\""));
	}
	catch(...)
	{
		ACS_SHORT_LOG((LM_ERROR,"Something FAILED. LOL."));
	}

	switch ( myStatus.type )
	{
		case assembly:
			robotName = "Assembly";
			break;
		case welding:
			robotName = "Welding";
			break;
		case debris:
			robotName = "Debris";
			break;
		case transporter:
			robotName = "Transport";
			break;
		default:
			ACS_SHORT_LOG((LM_WARNING,"Robot type not valid: %d", myStatus.type));
	}
	myStatus.position.x = parkPosition.x;
	myStatus.position.y = parkPosition.y;
	myStatus.isOperational = true;
}

RCBaseImpl::~RCBaseImpl()
{

} 

void RCBaseImpl::aboutToAbort()
{
	cleanUp();
}

void RCBaseImpl::cleanUp()
{
	try
	{
		getContainerServices()->releaseComponent("ROBOTCONTROL");
	}
	catch( maciErrType::CannotReleaseComponentExImpl & exc )
	{
		exc.log();
		ACS_SHORT_LOG((LM_ERROR,"Cannot release component \"ROBOTCONTROL\""));
	}
}

void RCBaseImpl::move( const pos_struct &pos )
{
	ACS_SHORT_LOG((LM_TRACE,"RCBaseImpl::move(%d,%d)",pos.x, pos.y));
	Lego->move(robotName.c_str(), pos.x, pos.y);
	ACS_SHORT_LOG((LM_INFO,"Moving to  %d,%d", pos.x, pos.y));
	myStatus.position.x = pos.x;
	myStatus.position.y = pos.y;
	if ((pos.x == parkPosition.x) && (pos.y == parkPosition.y) )
		myStatus.isParked = true;
	else
		myStatus.isParked = false;
} 

void RCBaseImpl::executeCommand( CORBA::Long action )
{
	ACS_SHORT_LOG((LM_TRACE,"RCBaseImpl::executeCommand(%d)",action));
	if (action == firefight)
	{
		ACS_SHORT_LOG((LM_INFO,"Executing firefight action : %d", action));
	}
	else if (action == primary)
	{
		ACS_SHORT_LOG((LM_INFO,"Executing primary action : %d", action));
	}
	else
	{
		ACS_SHORT_LOG((LM_INFO,"Executing NO action : %d", action));
		ACS_SHORT_LOG((LM_ERROR,"Invalid action"));
	}
	sleep(3);
	ACS_SHORT_LOG((LM_INFO,"Action finished : %d", action));
}
 
void RCBaseImpl::park ()
{
	ACS_SHORT_LOG((LM_TRACE,"RCBaseImpl::park()"));
	Lego->move(robotName.c_str(), parkPosition.x, parkPosition.y);
	ACS_SHORT_LOG((LM_INFO,"Moving to park position in %d,%d", parkPosition.x, parkPosition.y));
	myStatus.position.x = parkPosition.x;
	myStatus.position.y = parkPosition.y;
	myStatus.isParked = true;
}
 
robot_status_struct RCBaseImpl::status()
{	
	ACS_SHORT_LOG((LM_TRACE,"RCBaseImpl::status()"));
	return myStatus;
} 
pos_struct RCBaseImpl::position()
{
	ACS_SHORT_LOG((LM_TRACE,"RCBaseImpl::position() -> %d , %d", myStatus.position.x,myStatus.position.y));
	return myStatus.position;
} 
CORBA::Long RCBaseImpl::robotType()
{
	ACS_SHORT_LOG((LM_TRACE,"RCBaseImpl::robotType() -> %d ", myStatus.type));
	return myStatus.type;
} 
/*___oOo___*/
