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
*
* "@(#) $Id: LegoNXTImpl.cpp,v 1.1 2009/11/09 03:01:39 almamgr Exp $"
*
* who       when      what
* --------  --------  ----------------------------------------------
* jslopez  2008-10-18 Refurbish of an old component
*
*/

#include <LegoNXTImpl.h>
#include <bluecomm.h>
#include <opcodes.h>

#include <iostream>

using namespace baci;


LegoNXTImpl::LegoNXTImpl(const ACE_CString& name, 
                           maci::ContainerServices* containerServices)
        : CharacteristicComponentImpl(name, containerServices)
        ,m_macAddress_sp(this)
        ,connectedToLego(false)
{
	const char * _METHOD_ = (const char *)"::LegoNXTImpl::LegoNXTImpl";
	ACS_TRACE(_METHOD_);
	component_name = name.c_str();
}

LegoNXTImpl::~LegoNXTImpl()
{
}

void LegoNXTImpl::initialize(void)
throw (acsErrTypeLifeCycle::acsErrTypeLifeCycleExImpl)
{
	const char * _METHOD_ = (const char *)"::LegoNXTImpl::initialize";
	ACS_TRACE(_METHOD_);

	if(getComponent() != 0) {
		ACSErr::Completion_var completion;

		m_macAddress_sp = new ROstring( (component_name +
					std::string(":macAddress")).c_str(),
				getComponent());

		cout << "MAC: " << m_macAddress_sp->get_sync(completion.out()) << endl;

		try {
			nxt = new BlueComm();
			cout << "CONECTANDO" << endl;
			nxt->connect(m_macAddress_sp->get_sync(completion.out()));
		} catch(NxtEx &ex) {
			cout << "NO PUDO CONECTARSE" << endl;
			cout << ex.toString() << endl;
			return;
		}

		connectedToLego = true;
	}
}

void LegoNXTImpl::cleanUp(void)
{
	ACS_TRACE("::LegoNXTImpl::cleanUp");

	if (getComponent() != 0)
	{
		if( connectedToLego )
			nxt->disconnect();

		ACS_TRACE("::LegoNXTImpl::cleanUp");
		getComponent()->stopAllThreads();
	}
}


/* ------------------ [ Functions ] ----------------- */

void LegoNXTImpl::setMotorState(const TYPES::MotorState& mState) throw(CORBA::SystemException)
{
	const char * _METHOD_ = (const char *)"::LegoNXTImpl::setMotorState";
	ACS_TRACE(_METHOD_);

	if( !connectedToLego ) {
		ACS_DEBUG(_METHOD_,"Not connected to Lego, not setting motor state");
		return;
	}

	try{
		Opcodes op(nxt);
		unsigned char answer;

		op.setOutputState(mState.port,mState.power,0x01,0x00,50,0x20,mState.tachoLimit,true,&answer);

	}catch(NxtEx &ex){
		cout<<ex.toString()<<endl;
		nxt->disconnect();
	}
}

TYPES::MotorState LegoNXTImpl::getMotorState(CORBA::Long port)
	throw(CORBA::SystemException)
{
	const char * _METHOD_ = (const char *)"::LegoNXTImpl::getMotorState";
	ACS_TRACE(_METHOD_);

	if( !connectedToLego ) {
		ACS_DEBUG(_METHOD_,"Not connected to Lego, not getting motor state");
	} else {

		try{
			Opcodes op(nxt);
			MotorState ms;
			TYPES::MotorState myMs;
			ms = op.getOutputState(port);
			myMs.port = ms.runState;
			myMs.power = ms.tachoCount;
			myMs.tachoLimit = ms.tachoCount;
			return myMs;
		}catch(NxtEx &ex){
			cout<<ex.toString()<<endl;
			nxt->disconnect();
		}
	}

}

void LegoNXTImpl::setSensorState(const TYPES::SensorState& sState) throw(CORBA::SystemException)
{
	const char * _METHOD_ = (const char *)"::LegoNXTImpl::setSensorState";
	ACS_TRACE(_METHOD_);

	if( !connectedToLego ) {
		ACS_DEBUG(_METHOD_,"Not connected to Lego, not setting sensor state");
		return;
	}
	
	try{
		Opcodes op(nxt);
		op.setInputMode(sState.port, sState.type, sState.mode);
	}catch(NxtEx &ex){
		cout<<ex.toString()<<endl;
		nxt->disconnect();
	}
}

CORBA::Long LegoNXTImpl::getSensorState(CORBA::Long port) throw(CORBA::SystemException)
{
	const char * _METHOD_ = (const char *)"::LegoNXTImpl::getSensorState";
	ACS_TRACE(_METHOD_);

	if( !connectedToLego ) {
		ACS_DEBUG(_METHOD_,"Not connected to Lego, not getting sensor state");
		return -1;
	}

	try{
		Opcodes op(nxt);
		SensorState ss;

		ss = op.getInputValues(port); 
		return ss.normalizedValue;

	}catch(NxtEx &ex){
		cout<<ex.toString()<<endl;
		nxt->disconnect();
	}

	return -1;
}

ACS::ROstring_ptr LegoNXTImpl::macAddress() throw (CORBA::SystemException){
	if( m_macAddress_sp == 0 ){
		return ACS::ROstring::_nil();
	}
	ACS::ROstring_var prop = ACS::ROstring::_narrow(m_macAddress_sp->getCORBAReference());
	return prop._retn();
}

/* --------------- [ MACI DLL support functions ] -----------------*/
#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(LegoNXTImpl)
/* ----------------------------------------------------------------*/
