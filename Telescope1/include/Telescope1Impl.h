#ifndef _TELESCOPE1_H
#define _TELESCOPE1_H
/*******************************************************************************
*    ALMA - Atacama Large Millimiter Array
*    (c) Associated Universities Inc., 2002 
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
* "@(#) $Id: acsexmplHelloWorldImpl.h,v 1.99 2010/10/22 14:10:30 rbourtem Exp $"
*
* who       when      what
* --------  --------  ----------------------------------------------
* david 2002-09-26 added more comments
* david  25/09/02  created
*/

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

///This example is unique because it is derived from CharacteristicComponent's
///superclass, ACSComponent.
#include <acscomponentImpl.h>

///This is the CORBA stub client header for ACSErrTypeCommon.idl where the 
///definition of the CORBA exception is found.
#include <ACSErrTypeCommon.h>

/**
 *  The empty CORBA servant interface, POA_acsexmplHelloWorld::HelloWorld,
 *  is obtained from this header file and is automatically generated from 
 *  HelloWorld's Interface Definition File (i.e., acsexmplHelloWorld.idl) 
 *  by CORBA.
 */
#include <TelescopeS.h>
#include <TelescopeControlC.h>
#include <InstrumentC.h>
#include <unistd.h>
#include <SYSTEMErr.h>
#include <math.h>


#define SLEEP_TIME 100000  // in usec
#define TIME_OUT 120 // in sec


/**
 * This class shows how to make an ACS Hello World component.
 * It provides one synchronous method: displayMessage() which just prints
 * "Hello World" to the command-line where the container that activated
 * an instance of this component was started from. badMethod() is a trivial
 * method showing developers how to raise ACS (i.e., CORBA) exceptions.
 *
 */
class Telescope1: public virtual acscomponent::ACSComponentImpl,     //Component superclass
			  public POA_TELESCOPE_MODULE::Telescope    //CORBA servant stub
{    
  public:
    /**
     * Constructor
     * @param poa Poa which will activate this and also all other components. Developers need
     * not be concerned with what a PortableServer does...just pass it to the superclass's
     * constructor.
     * @param name component's name. All components have a name associated with them so other 
     * components and clients can access them.
     */
    Telescope1(const ACE_CString& name,
	       maci::ContainerServices * containerServices);
    
    /**
     * Destructor
     */
    virtual ~Telescope1();
    
    virtual ::TYPES::ImageType * observe(const ::TYPES::Position & coordinates,::CORBA::Long exposureTime) throw (SYSTEMErr::PositionOutOfLimitsEx, CORBA::SystemException) ;  

   
    virtual void moveTo(const ::TYPES::Position & coordinates) throw (SYSTEMErr::PositionOutOfLimitsEx, CORBA::SystemException) ;

    
    virtual ::TYPES::Position getCurrentPosition(void);
 
  private:
     void waitOnSource(const ::TYPES::Position & coord);
     TELESCOPE_MODULE::TelescopeControl_var cmount_p;
};  

#endif /*!_TELESCOPE1_H*/
