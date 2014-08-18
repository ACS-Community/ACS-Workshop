/*******************************************************************************
* CSRG - UTFSM Computer Systems Research Group
* ACS-UTFSM Team
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
*/

//#include <baciDB.h>
#include <AltAzImpl.h>
#include <realAltDevIO.h>
#include <refAltDevIO.h>
#include <realAzDevIO.h>
#include <refAzDevIO.h>

NAMESPACE_USE(baci);

struct __moveto_action {
    double alt;
    double az;
};


AltAz::AltAz(const ACE_CString& name, maci::ContainerServices *containerServices):CharacteristicComponentImpl(name, containerServices),m_refAlt_p(0),	m_refAz_p(0),m_realAlt_p(0),m_realAz_p(0)
{
	ACS_TRACE("::AltAz::AltAz");

	// Create an instance of the custom devIO before it's passed to the BACI property
	m_refAlt_devio_p = new refAltDevIO();
	m_refAz_devio_p = new refAzDevIO();
	m_realAlt_devio_p = new realAltDevIO();
	m_realAz_devio_p = new realAzDevIO();

	// This is a little different from the rest of the BACI properties throughout acsexmpl
	// in the fact a custom devIO is being passed to the constructor.
	m_refAz_p = new RWdouble(name+":refAz", getComponent(), m_refAz_devio_p);
        m_refAlt_p = new RWdouble(name+":refAlt", getComponent(), m_refAlt_devio_p);
	m_realAz_p = new ROdouble(name+":realAz", getComponent(), m_realAz_devio_p);
        m_realAlt_p = new ROdouble(name+":realAlt", getComponent(), m_realAlt_devio_p);
}

AltAz::~AltAz()
{
	ACS_TRACE("::AltAz::~AltAz");
	if(getComponent()){
		ACS_DEBUG_PARAM("::AltAz::~AltAz","Destroying %s...",getComponent()->getName());
		getComponent()->stopAllThreads();
	}

	if(m_refAlt_p){m_refAlt_p->destroy();m_refAlt_p = 0;}
	if(m_refAz_p){m_refAz_p->destroy();m_refAz_p = 0;}
	if(m_realAlt_p){m_realAlt_p->destroy();m_realAlt_p = 0;}
	if(m_realAz_p){m_realAz_p->destroy();m_realAz_p = 0;}

	if (m_refAlt_devio_p != 0)
        	{
        	delete m_refAlt_devio_p;
        	m_refAlt_devio_p=0;
        	}
	if (m_refAz_devio_p != 0)
        	{
        	delete m_refAz_devio_p;
        	m_refAz_devio_p=0;
        	}
	if (m_realAlt_devio_p != 0)
        	{
        	delete m_realAlt_devio_p;
        	m_realAlt_devio_p=0;
        	}
	if (m_realAz_devio_p != 0)
        	{
        	delete m_realAz_devio_p;
        	m_realAz_devio_p=0;
        	}


}
/************************Action implementator interface***************************/

ActionRequest AltAz::invokeAction	(int function, BACIComponent* cob_p, const int& callbackID,
		const CBDescIn& descIn, BACIValue* value, Completion& completion,
		CBDescOut& descOut){
	switch(function){
		case MOVETO_ACTION:
			return movetoAction(cob_p,callbackID,descIn,value,completion,descOut);
		default:
			return reqDestroy;
	}
}






/************************Action implementations***************************/

ActionRequest
AltAz::movetoAction	(BACIComponent* cob_p, const int& callbackID,const CBDescIn& descIn,
                        BACIValue* value, Completion& completion,CBDescOut& descOut){
	ACS_DEBUG_PARAM("::AltAz::movetoAction","%s",getComponent()->getName());

//put actual code here(communicate with device, etc)
	ACSErr::Completion_var comp;
 	__moveto_action *param_p = static_cast<__moveto_action *>(const_cast<void *>(value->pointerValue()));
	m_refAlt_p->set_sync(param_p->alt);
	m_refAz_p->set_sync(param_p->az);
	
	completion=ACSErrTypeOK::ACSErrOKCompletion();
	return reqInvokeDone;
}

void
AltAz::moveto 	(ACS::CBvoid_ptr cb, const ACS::CBDescIn & desc, double alt, double az)
  		throw(CORBA::SystemException)
{
   __moveto_action *param_p = new __moveto_action();
    param_p->alt=alt;
    param_p->az=az;

  getComponent()->registerAction(BACIValue::type_null, cb, desc, this, MOVETO_ACTION,BACIValue(param_p));
}



ACS::RWdouble_ptr
AltAz::refAlt()
    throw(CORBA::SystemException)
{
	if (m_refAlt_p==0)
    		return ACS::RWdouble::_nil();

    ACS::RWdouble_var prop = ACS::RWdouble::_narrow(m_refAlt_p->getCORBAReference());
    return prop._retn();
}


ACS::RWdouble_ptr
AltAz::refAz()
    throw(CORBA::SystemException)
{
        if (m_refAz_p==0)
                return ACS::RWdouble::_nil();

    ACS::RWdouble_var prop = ACS::RWdouble::_narrow(m_refAz_p->getCORBAReference());
    return prop._retn();
}


ACS::ROdouble_ptr
AltAz::realAlt()
    throw(CORBA::SystemException)
{
        if (m_realAlt_p==0)
                return ACS::ROdouble::_nil();

    ACS::ROdouble_var prop = ACS::ROdouble::_narrow(m_realAlt_p->getCORBAReference());
    return prop._retn();
}


ACS::ROdouble_ptr
AltAz::realAz()
    throw(CORBA::SystemException)
{
        if (m_realAz_p==0)
                return ACS::ROdouble::_nil();

    ACS::ROdouble_var prop = ACS::ROdouble::_narrow(m_realAz_p->getCORBAReference());
    return prop._retn();
}

/* --------------- [ MACI DLL support functions ] -----------------*/
#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS (AltAz)

//TODO:
//use smart pointers

/*___oOo___*/
