/* @(#) $Id: MonitoringTesterImpl.cpp,v 1.1 2007/06/06 23:04:30 rhiriart Exp $
 *
 * Copyright (C) 2001
 * Associated Universities, Inc. Washington DC, USA.
 *
 * Produced for the ALMA project
 *
 * This library is free software; you can redistribute it and/or modify it it 
 * under the terms of the GNU Library General Public License as published by 
 * the Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This library is distributed in the hope that it will be useful but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public 
 * License for more details.
 *
 * You should have received a copy of the GNU Library General Public License 
 * along with this library; if not, write to the Free Software Foundation, 
 * Inc., 675 Massachusetts Ave, Cambridge, MA, 02139, USA.
 *
 * Correspondence concerning ALMA should be addressed as follows:
 * Internet email: alma-sw-admin@nrao.edu
 */

#include <baciDB.h>
#include <baciDevIO.h>
#include <baciDevIOMem.h>
#include <OnePropertyImpl.h>

using namespace baci;

OnePropertyImpl::OnePropertyImpl(const ACE_CString &name,
    maci::ContainerServices* containerServices) :
      CharacteristicComponentImpl(name, containerServices),
      m_doubleProp01_sp(new RWdouble(name + ":doubleProp01", getComponent()), this)
{
    ACS_TRACE("::OnePropertyImpl::OnePropertyImpl");
    ACS_TRACE(name.c_str());
}

OnePropertyImpl::~OnePropertyImpl()
{
    ACS_TRACE("::OnePropertyImpl::~OnePropertyImpl");
}

void
OnePropertyImpl::initialize() throw (acsErrTypeLifeCycle::LifeCycleExImpl)
{
    ACS_TRACE("::OnePropertyImpl::initialize");
    ACS_SHORT_LOG((LM_INFO, "Here I am"));
}

ACS::RWdouble_ptr
OnePropertyImpl::doubleProp01() throw (CORBA::SystemException)
{
    ACS::RWdouble_var prop = ACS::RWdouble::_narrow(
        m_doubleProp01_sp->getCORBAReference());
    return prop._retn();
}

/* --------------- [ MACI DLL support functions ] -----------------*/
#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(OnePropertyImpl)
/* ----------------------------------------------------------------*/
