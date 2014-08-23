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
#include <baciROdouble.h>
#include <baciDevIO.h>
#include <MonitoringTesterImpl.h>

using namespace baci;

MonitoringTesterImpl::MonitoringTesterImpl(const ACE_CString &name,
    maci::ContainerServices* containerServices) :
      CharacteristicComponentImpl(name, containerServices),
      m_doubleProp01_sp(new RWdouble(name + ":doubleProp01", getComponent()), this),
      m_longProp01_sp(new RWlong(name + ":longProp01", getComponent()), this),
      m_stringProp01_sp(new RWstring(name + ":stringProp01", getComponent()), this),
      m_floatProp01_sp(new RWfloat(name + ":floatProp01", getComponent()), this),
      m_booleanProp01_sp(new RWboolean(name + ":booleanProp01", getComponent()), this),
      m_patternProp01_sp(new RWpattern(name + ":patternProp01", getComponent()), this),
      m_doubleSeqProp01_sp(new RWdoubleSeq(name + ":doubleSeqProp01", getComponent()), this),
      m_longLongProp01_sp(new RWlongLong(name + ":longLongProp01", getComponent()), this),
      m_uLongProp01_sp(new RWuLong(name + ":uLongProp01", getComponent()), this),
      m_uLongLongProp01_sp(new RWuLongLong(name + ":uLongLongProp01", getComponent()), this)
{
    ACS_TRACE("::MonitoringTester::MonitoringTester");
    ACS_TRACE(name.c_str());
}

MonitoringTesterImpl::~MonitoringTesterImpl()
{
    ACS_TRACE("::MonitoringTester::~MonitoringTester");
}

void
MonitoringTesterImpl::initialize() throw (acsErrTypeLifeCycle::LifeCycleExImpl)
{
    ACS_TRACE("::MonitoringTester::startInitialize");
    ACS_SHORT_LOG((LM_INFO, "Geronimo!!!!!!"));
}

ACS::RWdouble_ptr
MonitoringTesterImpl::doubleProp01() throw (CORBA::SystemException)
{
    ACS::RWdouble_var prop = ACS::RWdouble::_narrow(
        m_doubleProp01_sp->getCORBAReference());
    return prop._retn();
}

ACS::RWlong_ptr
MonitoringTesterImpl::longProp01() throw (CORBA::SystemException)
{
    ACS::RWlong_var prop = ACS::RWlong::_narrow(
        m_longProp01_sp->getCORBAReference());
    return prop._retn();
}

ACS::RWstring_ptr
MonitoringTesterImpl::stringProp01() throw (CORBA::SystemException)
{
    ACS::RWstring_var prop = ACS::RWstring::_narrow(
        m_stringProp01_sp->getCORBAReference());
    return prop._retn();
}

ACS::RWfloat_ptr
MonitoringTesterImpl::floatProp01() throw (CORBA::SystemException)
{
    ACS::RWfloat_var prop = ACS::RWfloat::_narrow(
        m_floatProp01_sp->getCORBAReference());
    return prop._retn();
}

ACS::RWboolean_ptr
MonitoringTesterImpl::booleanProp01() throw (CORBA::SystemException)
{
    ACS::RWboolean_var prop = ACS::RWboolean::_narrow(
        m_booleanProp01_sp->getCORBAReference());
    return prop._retn();
}

ACS::RWpattern_ptr
MonitoringTesterImpl::patternProp01() throw (CORBA::SystemException)
{
    ACS::RWpattern_var prop = ACS::RWpattern::_narrow(
        m_patternProp01_sp->getCORBAReference());
    return prop._retn();
}

ACS::RWdoubleSeq_ptr
MonitoringTesterImpl::doubleSeqProp01() throw (CORBA::SystemException)
{
    ACS::RWdoubleSeq_var prop = ACS::RWdoubleSeq::_narrow(
        m_doubleSeqProp01_sp->getCORBAReference());
    return prop._retn();
}

ACS::RWlongLong_ptr
MonitoringTesterImpl::longLongProp01() throw (CORBA::SystemException)
{
    ACS::RWlongLong_var prop = ACS::RWlongLong::_narrow(
        m_longLongProp01_sp->getCORBAReference());
    return prop._retn();
}

ACS::RWuLong_ptr
MonitoringTesterImpl::uLongProp01() throw (CORBA::SystemException)
{
    ACS::RWuLong_var prop = ACS::RWuLong::_narrow(
        m_uLongProp01_sp->getCORBAReference());
    return prop._retn();
}

ACS::RWuLongLong_ptr
MonitoringTesterImpl::uLongLongProp01() throw (CORBA::SystemException)
{
    ACS::RWuLongLong_var prop = ACS::RWuLongLong::_narrow(
        m_uLongLongProp01_sp->getCORBAReference());
    return prop._retn();
}

/* --------------- [ MACI DLL support functions ] -----------------*/
#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(MonitoringTesterImpl)
/* ----------------------------------------------------------------*/
