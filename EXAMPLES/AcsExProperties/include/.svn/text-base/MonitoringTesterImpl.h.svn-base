/* @(#) $Id: MonitoringTesterImpl.h,v 1.1 2007/06/06 23:04:30 rhiriart Exp $
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
 
#ifndef MonitoringTesterImpl_h
#define MonitoringTesterImpl_h

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

#include <baciCharacteristicComponentImpl.h>
#include <MonitoringTesterS.h>

#include <baciRWdouble.h>
#include <baciRWlong.h>
#include <baciRWstring.h>
#include <baciRWfloat.h>
#include <baciRWboolean.h>
#include <baciRWpattern.h>
#include <baciRWdoubleSeq.h>
#include <baciRWlongLong.h>
#include <baciRWuLong.h>
#include <baciRWuLongLong.h>
#include <baciSmartPropertyPointer.h>

using namespace baci;

class MonitoringTesterImpl : public CharacteristicComponentImpl,
			                 public virtual POA_test::MonitoringTester
{
    public:

    MonitoringTesterImpl(const ACE_CString& name,
		                 maci::ContainerServices* containerServices);

    virtual ~MonitoringTesterImpl();

    void initialize() throw(acsErrTypeLifeCycle::LifeCycleExImpl);

    virtual ACS::RWdouble_ptr doubleProp01() throw (CORBA::SystemException);

    virtual ACS::RWlong_ptr longProp01() throw (CORBA::SystemException);
    
    virtual ACS::RWstring_ptr stringProp01() throw (CORBA::SystemException);

    virtual ACS::RWfloat_ptr floatProp01() throw (CORBA::SystemException);
    
    virtual ACS::RWboolean_ptr booleanProp01() throw (CORBA::SystemException);

    virtual ACS::RWpattern_ptr patternProp01() throw (CORBA::SystemException);

    virtual ACS::RWdoubleSeq_ptr doubleSeqProp01() throw (CORBA::SystemException);

    virtual ACS::RWlongLong_ptr longLongProp01() throw (CORBA::SystemException);

    virtual ACS::RWuLong_ptr uLongProp01() throw (CORBA::SystemException);

    virtual ACS::RWuLongLong_ptr uLongLongProp01() throw (CORBA::SystemException);

    private:

    SmartPropertyPointer<RWdouble> m_doubleProp01_sp;

    SmartPropertyPointer<RWlong> m_longProp01_sp;
    
    SmartPropertyPointer<RWstring> m_stringProp01_sp;
    
    SmartPropertyPointer<RWfloat> m_floatProp01_sp;

    SmartPropertyPointer<RWboolean> m_booleanProp01_sp;

    SmartPropertyPointer<RWpattern> m_patternProp01_sp;

    SmartPropertyPointer<RWdoubleSeq> m_doubleSeqProp01_sp;
    
    SmartPropertyPointer<RWlongLong> m_longLongProp01_sp;

    SmartPropertyPointer<RWuLong> m_uLongProp01_sp;

    SmartPropertyPointer<RWuLongLong> m_uLongLongProp01_sp;

    void operator=(const MonitoringTesterImpl&);

};

#endif /* MonitoringTesterImpl_h */
