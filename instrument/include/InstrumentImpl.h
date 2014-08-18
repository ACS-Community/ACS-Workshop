#ifndef instrumentImpl_h
#define instrumentImpl_h
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
* "@(#) $Id: InstrumentImpl.h,v 1.3 2010-11-19 13:39:44 instrument Exp $"
*
*/

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

///Contains the defintion of the standard superclass for C++ components
#include <acscomponentImpl.h>
//#include <acsexmplExport.h>

#include <TypesS.h>

///CORBA generated servant stub
#include <InstrumentS.h>
#include <DevCCDImpl.h>

class Instrument: public virtual acscomponent::ACSComponentImpl,
			     public virtual POA_INSTRUMENT_MODULE::Instrument   //CORBA servant stub
{
  public:
     /**
     * Constructor
     * 
     * @param name component's name. This is also the name that will be used to find the
     * configuration data for the component in the Configuration Database.
     * @param containerService The pointer to the container services
     */
    Instrument(
	  ACE_CString name,
	  maci::ContainerServices * containerServices);
    
    /**
	     * Destructor
	     */
	    virtual ~Instrument();

	    
	    virtual void //ACS::ROdouble_ptr duda 
	    cameraOn ();
	    
	    
	    virtual void //ACS::ROdouble_ptr duda 
	    cameraOff ();
	    
	    TYPES::ImageType* takeImage(CORBA::Long exposureTime);

        /* Camera settings */

        /**
         * Set the RGB configuration of the camera
         * @param rgbConfig the TYPES::RGB configuration
         */
    void setRGB(const TYPES::RGB&);

        /**
         * Set the pixel bias configuration of the camera
         * @param bias the pixel bias configuration
         */
    void setPixelBias(CORBA::Long bias);

        /**
         * Set the reset level configuration of the camera
         * @param resetLevel the reset level configuration
         */
    void setResetLevel(CORBA::Long resetLevel);
	      
   
  private:
  	DEVCCD_MODULE::DevCCD_var m_camera_p;
   
};
/*\@}*/

#endif 



