#ifndef _H
#define _H
/*******************************************************************************
* ALMA - Atacama Large Millimiter Array
* Copyright (c) Associated Universities Inc., 2013 
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
*
* who       when      what
* --------  --------  ----------------------------------------------
* almamgr  2013-09-10  created
*/

/************************************************************************
 *
 *----------------------------------------------------------------------
 */

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

///This example is unique because it is derived from CharacteristicComponent's
///superclass, ACSComponent.
#include <baciCharacteristicComponentImpl.h>

///This is the CORBA stub client header for ACSErrTypeCommon.idl where the
///definition of the CORBA exception is found.
#include <ACSErrTypeCommon.h>

#include <TelescopeS.h>
#include <TypesC.h>
#include <InstrumentC.h>
#include <baciSmartPropertyPointer.h>
#include <baciRWdouble.h>
#include <Mount2RefAltDevIO.h>
#include <Mount2RefAzDevIO.h>
#include <SORTErr.h>

class TelescopeImpl: public baci::CharacteristicComponentImpl,     //Component superclass
                  public virtual POA_TELESCOPE_MODULE::Telescope    //CORBA servant stub
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
    TelescopeImpl(
               const ACE_CString& name,
               maci::ContainerServices * containerServices);

    /**
     * Destructor
     */
    virtual ~TelescopeImpl();

    /* --------------------- [ CORBA interface ] ----------------------*/
    /**
     */
    virtual TYPES::ImageType *
    observe (const TYPES::Position & position, CORBA::Long exposureTime);

    virtual void 
    moveTo (const TYPES::Position & position);
   
    virtual TYPES::Position
    getCurrentPosition ();
    
    virtual void initialize();
    virtual void cleanUp();
    virtual ACS::RWdouble_ptr Az() throw (CORBA::SystemException);
    virtual ACS::RWdouble_ptr El() throw (CORBA::SystemException);


private:
		INSTRUMENT_MODULE::Instrument_var instrument_m;
		Mount2RefAltDevIO * elDevIO_m; 
		Mount2RefAzDevIO * azDevIO_m; 
	    baci::SmartPropertyPointer<baci::RWdouble> azProp_m;
	    baci::SmartPropertyPointer<baci::RWdouble> elProp_m;

};


#endif /*!_H*/
