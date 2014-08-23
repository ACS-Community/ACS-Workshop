#ifndef _TELESCOPE_IMPL_H
#define _TELESCOPE_IMPL_H
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

#include <telescopeImpl.h>

    TelescopeImpl::TelescopeImpl(
               const ACE_CString& name,
               maci::ContainerServices * containerServices) :
    baci::CharacteristicComponentImpl(name, containerServices),
	elDevIO_m(new Mount2RefAltDevIO()),
	azDevIO_m(new Mount2RefAzDevIO()),
	azProp_m(new baci::RWdouble(name + ":Az", getComponent(), azDevIO_m), this),
	elProp_m(new baci::RWdouble(name + ":El", getComponent(), elDevIO_m), this)
    {
	ACS_TRACE("::TelescopeImpl::TelescopeImpl");
    }

    /**
     * Destructor
     */
    TelescopeImpl::~TelescopeImpl()
    {
	ACS_TRACE("::TelescopeImpl::~TelescopeImpl");

	delete azDevIO_m;
	delete elDevIO_m;
    }


    TYPES::ImageType *
    TelescopeImpl::observe (const TYPES::Position & position, CORBA::Long exposureTime)
    {
	ACS_TRACE("::TelescopeImpl::observe");
	
	try
	{
		ACS_SHORT_LOG((LM_INFO,"Telescope moved to az=%f el=%f", position.az, position.el));
		moveTo(position);
	}
	catch (SORTErr::PositionOutOfLimitsEx &ex)
	{
		throw ex;
	}

	return(instrument_m->takeImage(exposureTime));
    }

    void
    TelescopeImpl::moveTo (const TYPES::Position & position)
    {
	ACS_TRACE("::TelescopeImpl::moveTo");

	if ((position.az > 360.0) || (position.az < 0.0))
	{
		SORTErr::PositionOutOfLimitsExImpl ex(__FILE__, __LINE__, __PRETTY_FUNCTION__);
		ex.addData("Cause", "Position should be between 0.0 and 360.0");
                throw ex.getPositionOutOfLimitsEx();
	}

	azProp_m->set_sync(position.az);	
	elProp_m->set_sync(position.el);	

    }

    TYPES::Position
    TelescopeImpl::getCurrentPosition ()
    {
	ACS_TRACE("::TelescopeImpl::getCurrentPosition");
	TYPES::Position position;
	ACSErr::Completion_var completion;

	position.az = azProp_m->get_sync(completion.out());
	position.el = elProp_m->get_sync(completion.out());

	return(position);
    }

    void TelescopeImpl::initialize()
    {
	
	//Get reference to the instrument component
        //ACS_SHORT_LOG((LM_INFO, "Getting component: %s", argv[1]));
        instrument_m = getContainerServices()->getComponent<INSTRUMENT_MODULE::Instrument>("INSTRUMENT");
        if (CORBA::is_nil(instrument_m.in()) == true)
        {
           ACS_SHORT_LOG((LM_INFO, "Failed to get a reference to the Instrument component."));
        }
        ACS_SHORT_LOG((LM_INFO, "Instrument reference retrieved."));

    }

    void TelescopeImpl::cleanUp()
    {
	try
	{
	   getContainerServices()->releaseComponent("INSTRUMENT");
	}
	catch(maciErrType::CannotReleaseComponentExImpl & ex)
	{
	    ex.log();
	}
    }

    ACS::RWdouble_ptr TelescopeImpl::Az() throw (CORBA::SystemException)
    {
	ACS::RWdouble_var prop = ACS::RWdouble::_narrow(azProp_m->getCORBAReference());
	return(prop._retn());
    }

    ACS::RWdouble_ptr TelescopeImpl::El() throw (CORBA::SystemException)
    {
	ACS::RWdouble_var prop = ACS::RWdouble::_narrow(elProp_m->getCORBAReference());
	return(prop._retn());
    }


#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(TelescopeImpl)

/*___oOo___*/
#endif /*!_TELESCOPE_INCLUDE_H*/

