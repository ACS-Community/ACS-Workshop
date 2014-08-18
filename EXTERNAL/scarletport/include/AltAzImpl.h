#ifndef AltAzImpl_H
#define AltAzImpl_H
/*******************************************************************************
* CSRG - UTFSM Computer Systems Research Group
*  ACS-UTFSM Team
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
* "@(#) $Id: AltAzImpl.h,v 1.4 2010-11-17 20:39:01 its Exp $"
*
* who       when      what
* --------  --------  ----------------------------------------------
* nbarriga  2005-08-04  created
 */

/************************************************************************
 *
 *----------------------------------------------------------------------
 */

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

#include <baciCharacteristicComponentImpl.h>

#include <AltAzS.h>

#include <baciROdouble.h>
#include <baciRWdouble.h>

NAMESPACE_USE(baci);

#define MOVETO_ACTION 0

class AltAz:	public virtual CharacteristicComponentImpl,//because idl does
   public virtual POA_AltAzModule::AltAz,//POA_module::interface in idl
   public ActionImplementator//to b able to invoke actions
{
   public:
      AltAz(const ACE_CString& name, maci::ContainerServices* containerServices);
      //		AltAz(PortableServer::POA_ptr poa,const ACE_CString& name);
      virtual ~AltAz();


      /*************************ACTIONIMPLEMENTATOR INTERFACE**********************************/
      /**
       *called from method inherited from actionImplementator when async action is waiting in queue
       */
      virtual ActionRequest 
         invokeAction	(int function, BACIComponent* cob_p, const int& callbackID,
               const CBDescIn& descIn, BACIValue* value, Completion& completion,
               CBDescOut& descOut);
      
      /**
       *implementation of async.moveto() - called by invokeAction
       */
      virtual ActionRequest
         movetoAction	(BACIComponent* cob_p, const int& callbackID,const CBDescIn& descIn,
               BACIValue* value, Completion& completion,CBDescOut& descOut);


      /*************************CORBA INTERFACE**********************************/
      /**
       *sends the telescope to a new position - register the call to the async queue
       */
      virtual void
         moveto	(ACS::CBvoid_ptr cb, const ACS::CBDescIn& desc, double alt,  double az)
         throw(CORBA::SystemException);

      /**
       *refXx -> where you want the telescope to be
       */
      virtual ACS::RWdouble_ptr
         refAlt()
         throw(CORBA::SystemException);		
      virtual ACS::RWdouble_ptr
         refAz()
         throw(CORBA::SystemException);		



      /**
       *realXx -> where the telescope actually is
       */
      virtual ACS::ROdouble_ptr
         realAlt()
         throw(CORBA::SystemException);		
      virtual ACS::ROdouble_ptr
         realAz()
         throw(CORBA::SystemException);		


   private:

      //Properties
      RWdouble* m_refAlt_p;
      RWdouble* m_refAz_p;
      ROdouble* m_realAlt_p;
      ROdouble* m_realAz_p;

      DevIO<CORBA::Double> *m_refAlt_devio_p;
      DevIO<CORBA::Double> *m_refAz_devio_p;
      DevIO<CORBA::Double> *m_realAlt_devio_p;
      DevIO<CORBA::Double> *m_realAz_devio_p;
};

#endif /*AltAzImpl_H*/
