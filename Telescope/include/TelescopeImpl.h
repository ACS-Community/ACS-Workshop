#ifndef TelescopeImpl_h
#define TelescopeImpl_h
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
* "@(#) $Id: acsexmplDoorImpl.h,v 1.108 2008/10/09 08:41:11 cparedes Exp $"
*
* who       when      what
* --------  --------  ----------------------------------------------
* acaproni 2004-04-06 Use the smart pointer for the properties
* david 2002-07-02 added GNU license info
* blopez  2002-04-05  m_poa declaration removed
* blopez  2002-04-04  Modified for ACSDO usage
* blopez  2002-03-27  Comments changed to doxygen format
* blopez  2002-03-11  Created
*
*/

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

///Contains the defintion of the standard superclass for C++ components
#include <acscomponentImpl.h>

#include <TypesS.h>

#include <TelescopeS.h>

#include <maciACSComponentDefines.h>


///CORBA generated servant stub
//#include <acsexmplBuildingS.h>


///Include the smart prointer for the properties
//#include <baciSmartPropertyPointer.h>

///Include the acs thread header 
//#include <acsThread.h>

/** @file acsexmplDoorImpl.h
 */

//class TelescopeImpl; // declaration

class TelescopeImpl: public acscomponent::ACSComponentImpl,     //Standard component superclass
  public virtual POA_TELESCOPE_MODULE::Telescope //CORBA servant stub
{
 public:
  
  TelescopeImpl(const ACE_CString& name,
			     maci::ContainerServices *& 
		containerServices);

  virtual ~TelescopeImpl();

  virtual ::TYPES::ImageType *
    observe (const ::TYPES::Position & coordinates,
	     ::CORBA::Long exposureTime);

  virtual void
    moveTo (
      const ::TYPES::Position & coordinates);

  virtual ::TYPES::Position
    getCurrentPosition (void);

 private:
  TYPES::Position Pos;

}; 


#endif   /* TelescopeImpl_h */



