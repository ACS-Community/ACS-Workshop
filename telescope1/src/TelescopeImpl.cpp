/*******************************************************************************
*    ALMA - Atacama Large Millimiter Array
*    (c) Associated Universities Inc., 2002 *
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
*
* "@(#) $Id: acsexmplHelloWorldImpl.cpp,v 1.95 2008/10/01 04:30:47 cparedes Exp $"
*
* who       when      what
* --------  --------  ----------------------------------------------
* david 2002-09-26 added many more comments
* david  25/09/02  created 
*/
 
#include <TelescopeImpl.h>
#include <ACSErrTypeCommon.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>


CORBA::Long position(0);

/* ----------------------------------------------------------------*/
TelescopeImpl::TelescopeImpl( 
		       const ACE_CString &name,
		       maci::ContainerServices * containerServices) :
    ACSComponentImpl(name, containerServices) 
{
    // ACS_TRACE is used for debugging purposes
    ACS_TRACE("::TelescopeImpl::TelescopeImpl");
	std::cout << "TelescopeImpl Constructor" << std::endl;
	srand(time(NULL));
}
/* ----------------------------------------------------------------*/
TelescopeImpl::~TelescopeImpl()
{
    // ACS_TRACE is used for debugging purposes
    ACS_TRACE("::TelescopeImpl::~TelescopeImpl");
    ACS_DEBUG_PARAM("::TelescopeImpl::~TelescopeImpl", "Destroying %s...", name());
}
/* --------------------- [ CORBA interface ] ----------------------*/
CORBA::Long 
TelescopeImpl::observe ()
{
    std::cout << "TelescopeImpl" << std::endl;
	ACS_TRACE("::TelescopeImpl::TelescopeImpl Observe");
	long response = 1234;

	this->instrument = getContainerServices()->getComponent<INSTRUMENT_MODULE::Instrument>("INSTRUMENT");
	response = this->instrument->takeImage();
	if (response == 1)
	{
		std::cout << "TelescopeImpl: Imagen obtenida correctamente" << std::endl;
	        ACS_TRACE("::TelescopeImpl: Imagen obtenida correctamente");
	}
	else
	{
		std::cout << "TelescopeImpl: Error al obtener imagen" << std::endl;
	        ACS_TRACE("::TelescopeImpl: Error al obtener la imagen");
	}
	return response;
}
/* ---------------------------------------------------------------*/
void 
TelescopeImpl::moveTo() 
{
	std::cout << "TelescopeImpl::MoveTo" << std::endl;
	ACS_TRACE("::TelescopeImpl::TelescopeImpl MoveTo");
	for (int i=0; i < 5; i++)
	{
		std::cout << "\aMoving\n" << std::flush;
		usleep(1000000);
	}
	position = rand();
}
/* ----------------------------------------------------------------*/
CORBA::Long 
TelescopeImpl::getCurrentPosition() 
{
	std::cout << "TelescopeImpl::getCurrentPosition" << std::endl;
	ACS_TRACE("::TelescopeImpl::TelescopeImpl getCurrentPostion");
	//long response = 12345;
	CORBA::Long response = position;
	return response;    
}
/* --------------- [ MACI DLL support functions ] -----------------*/
#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(TelescopeImpl)
/* ----------------------------------------------------------------*/


/*___oOo___*/



