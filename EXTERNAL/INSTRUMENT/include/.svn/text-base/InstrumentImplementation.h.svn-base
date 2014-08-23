#ifndef acsexmplLampImpl_h
#define acsexmplLampImpl_h
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
* "@(#) $Id: acsexmplLampImpl.h,v 1.99 2008/10/01 04:30:47 cparedes Exp $"
*
* who       when      what
* --------  --------  ----------------------------------------------
* acaproni 2004-04-06 Use smart pointers for properties
* david 2002-07-02 added GNU license info
* david 2002-06-16 Finished adding doxygen comments.
* david 2002-06-13 Deleted typedefinition of Lamp::*ActionFunction as it is not used in this example.
* david 2002-06-10 Added some doxygen comments to clean it up for documentation
* gchiozzi 2002-01-28 cleaned up remaining "mount" strings and replaced with "acsexmpl"
* gchiozzi 2001-02-15 Added declaration of get_interface() method for Object Explorer
* gchiozzi 2001-02-15 created created standard header
* msekoran 2001-03-10 integrated with new BACI; ALMA coding convention used; doc.
*/

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

#include <acscomponentImpl.h>
#include <InstrumentS.h>
#include <TypesC.h>



/** @file acsexmplLampImpl.h
 */

/** @addtogroup ACSEXMPLTOC
*/
/*@{
*/

/** @addtogroup ACSEXMPLTOCCOMPONENTS
*/
/*@{
*/

/** @defgroup ACSEXMPLLAMPDOC Lamp
 *  @{
 * @htmlonly
<hr size="2" width="100%">
<div align="left">
<h2>Description</h2>
The class Lamp is a very simple example of a component that simulates
the behavior of a lamp. &nbsp;<br>
It has two asynchronous methods: on(...) and off(...). &nbsp;It also
has one read-write double property: brightness.
<br>
<br>
<h2>What can I gain from this example?</h2>
<ul>
  <li>an example derived from the ACS::CharacteristicComponent IDL interface.</li>
  <li>an understanding of simple asynchronous method implementation accomplished by inheriting methods from the ActionImplementator class.</li>
</ul>
<br>
<br>
<h2>Links</h2>
<ul>
  <li><a href="classLamp.html">Lamp Class Reference</a></li>
  <li><a href="interfaceacsexmplLamp_1_1Lamp.html">Lamp IDL Documentation</a></li>
  <li>Lamp CDB XML Schema</li>
</ul>
</div>
   @endhtmlonly
 * @}
 */

/**
 * Simple component with asynchronous methods.
 * This class Lamp simulates the behavior of a lamp.
 * It provides two asynchronous methods: on and off.
 * It also provides the property brightness.
 * Lastly, it implements timeout negotiation between client and servant
 * for the two methods.
 * Asynchronous calls are implemented using the ...... pattern and the ..... support classes.
 * For each xxx action defined in the IDL interface two methods are provided:
 *  - xxx() just registers the action and installs the callback
 *  - xxxAction() performs (asyncronously) the action and invokes the callback when finished.
 * The Lamp::invokeAction method is called by the asynchronous dispatcher whenever there is an
 * xxx pending action and it calls the xxxAction corresponding method.
 * 
 * @author <a href=mailto:matej.sekoranja@ijs.si>Matej Sekoranja</a>,
 * Jozef Stefan Institute, Slovenia<br>
 * @version "@(#) $Id: acsexmplLampImpl.h,v 1.99 2008/10/01 04:30:47 cparedes Exp $"
 */
class InstrumentInt: public acscomponent::ACSComponentImpl,
	    public virtual POA_INSTRUMENT_MODULE::Instrument
{
  public:
    /**
     * Constructor
     * 
     * @param name component's name. This is also the name that will be used to find the
     * configuration data for the component in the Configuration Database.
     * @param containerServices The pointer to the ContainerServices
     */
    InstrumentInt(
	 const ACE_CString& name,
	 maci::ContainerServices * containerServices);
    
    /**
     * Destructor
     */
    virtual ~InstrumentInt();
   
    
    /* --------------------- [ CORBA interface ] ----------------------*/
    /**
     * Switches on the lamp.
     * Implementation of IDL on() interface.
     * This method just registers the request in the asyncronous queue, together with the associated callback 
     * and returns control immediatly.
     * The actual action will be invoked asyncronously by the asynchonous call manager by calling Lamp::onAction
     * The given callback is used to inform the caller when the action is
     * performed.
     * @param callBack Callback when action has finished.
     * @param desc Callback used for holding information on timeout periods
     * @htmlonly
       <br><hr>
       @endhtmlonly
     */     
    virtual void 
    cameraOn ();
    
    /**
 	 .
     */ 
    virtual void 
    cameraOff ();
    
    /**
     * Returns a reference to the Brightness property
     * Implementation of IDL interface for the property.
     * @htmlonly
       <br><hr>
       @endhtmlonly
     */ 
    virtual
    TYPES::ImageType* takeImage(CORBA::Long exposureTime);

    virtual void
    setRGB(const TYPES::RGB& rgbConfig);

    virtual void
    setPixelBias(CORBA::Long bias);

    virtual void
    setResetLevel(CORBA::Long resetLevel);


    
  private:
   bool CameraStatus;
};
/*\@}*/
/*\@}*/

#endif   /* acsexmplLampImpl_h */



