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
#include <DevIOTesterImpl.h>

using namespace baci;

DevIOTesterImpl::DevIOTesterImpl(const ACE_CString &name,
    maci::ContainerServices* containerServices) :
      CharacteristicComponentImpl(name, containerServices),
      m_value(3.141516),
      m_memDevIO_p(new DevIOMem<double>(m_value)),
      m_doubleProp01_sp(new RWdouble(name + ":doubleProp01", getComponent(), m_memDevIO_p), this),
      m_randomDevIO_p(new RandomDevIO()),
      m_doubleProp02_sp(new RWdouble(name + ":doubleProp02", getComponent(), m_randomDevIO_p), this),
      m_value3(0.5),
      m_memDevIO3_p(new DevIOMem<double>(m_value3)),
      m_doubleProp03_sp(new ROdouble(name + ":doubleProp03", getComponent(), m_memDevIO3_p), this)
{
    ACS_TRACE("::DevIOTester::DevIOTester");
    ACS_TRACE(name.c_str());
}

DevIOTesterImpl::~DevIOTesterImpl()
{
    ACS_TRACE("::DevIOTester::~DevIOTester");
    delete m_memDevIO_p;
    delete m_randomDevIO_p;
    delete m_memDevIO3_p;
}

void
DevIOTesterImpl::initialize() throw (acsErrTypeLifeCycle::LifeCycleExImpl)
{
    ACS_TRACE("::DevIOTester::startInitialize");
    ACS_SHORT_LOG((LM_INFO, "Here I am"));
    m_value = 3.141516;
}

ACS::RWdouble_ptr
DevIOTesterImpl::doubleProp01() throw (CORBA::SystemException)
{
    ACS::RWdouble_var prop = ACS::RWdouble::_narrow(
        m_doubleProp01_sp->getCORBAReference());
    return prop._retn();
}

ACS::RWdouble_ptr
DevIOTesterImpl::doubleProp02() throw (CORBA::SystemException)
{
    ACS::RWdouble_var prop = ACS::RWdouble::_narrow(
        m_doubleProp02_sp->getCORBAReference());
    return prop._retn();
}

ACS::ROdouble_ptr
DevIOTesterImpl::doubleProp03() throw (CORBA::SystemException)
{
    ACS::ROdouble_var prop = ACS::ROdouble::_narrow(
        m_doubleProp03_sp->getCORBAReference());
    return prop._retn();
}

void DevIOTesterImpl::setValue3(double value)
{
	m_value3 = value;
}

/* --------------- [ MACI DLL support functions ] -----------------*/
#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(DevIOTesterImpl)
/* ----------------------------------------------------------------*/
