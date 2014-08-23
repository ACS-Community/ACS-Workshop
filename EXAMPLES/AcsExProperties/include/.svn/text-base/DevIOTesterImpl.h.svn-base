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
 
#ifndef DevIOTesterImpl_h
#define DevIOTesterImpl_h

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

#include <baciCharacteristicComponentImpl.h>
#include <DevIOTesterS.h>
#include <baciROdouble.h>
#include <baciRWdouble.h>
#include <RandomDevIO.h>
#include <baciSmartPropertyPointer.h>

using namespace baci;

class DevIOTesterImpl : public CharacteristicComponentImpl,
			            public virtual POA_test::DevIOTester
{
    public:

    DevIOTesterImpl(const ACE_CString& name,
		            maci::ContainerServices* containerServices);

    virtual ~DevIOTesterImpl();

    void initialize() throw(acsErrTypeLifeCycle::LifeCycleExImpl);

    virtual ACS::RWdouble_ptr doubleProp01() throw (CORBA::SystemException);

    virtual ACS::RWdouble_ptr doubleProp02() throw (CORBA::SystemException);

    virtual ACS::ROdouble_ptr doubleProp03() throw (CORBA::SystemException);

    void setValue3(double value);

    private:

    double m_value;

    DevIOMem<double>* m_memDevIO_p;

    SmartPropertyPointer<RWdouble> m_doubleProp01_sp;

    RandomDevIO *m_randomDevIO_p;

    SmartPropertyPointer<RWdouble> m_doubleProp02_sp;

    double m_value3;

    DevIOMem<double>* m_memDevIO3_p;

    SmartPropertyPointer<ROdouble> m_doubleProp03_sp;

    void operator=(const DevIOTesterImpl&);

};

#endif /* DevIOTesterImpl_h */
