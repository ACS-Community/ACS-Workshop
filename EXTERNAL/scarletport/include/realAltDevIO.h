#ifndef realAltDevIO_h
#define realAltDevIO_h
/*******************************************************************************
*
* CSRG - UTFSM Computer Systems Research Group
* ACS-UTFSM Team
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
* "@(#) $Id: realAltDevIO.h,v 1.2 2010-11-19 13:30:20 its Exp $"
*
* who       when      what
* --------  --------  ----------------------------------------------
* nbarriga 2005-08-05 created
*/

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

///Need the defition of the class to inherit from
#include <baciDevIO.h>
#include <baciTime.h>
#include <iostream>
#include <fstream>
#include <SCTlib.h>

using namespace std;
/**
 * This class provides an example of the implementation of a devIO subclass.  Developers 
 * should use devIOs when you have a BACI property where you need to change the underlying value,
 * but do not want the property to be read-write where anyone can change it. 
 */
class realAltDevIO : public DevIO<CORBA::Double>
{
  public:
    
    /**
     * Default constuctor - nothing to do.
     */
    realAltDevIO();

    /**
     * Destructor - nothing to destroy.
     */
    virtual ~realAltDevIO();

    /**
     * ???
     * @return True if the value has been initialized.
     * @htmlonly
       <br><hr>
       @endhtmlonly
     */
    virtual bool 
    initializeValue();
  
    /**
     * Reads the double value from "somewhere".  We could get this value from
     * physical hardware, files on disk, etc.  
     *
     * Last but definitely not least, it is important to note that this method can be invoked
     * from both ROdouble and RWdouble properties.
     * @param errcode The error code (as defined in the ACS Error System) that occured while
     *        reading this value or 0 if no error occured.  It's up to the developer to 
     *        determine whether an error occured or not.
     * @param timestamp The time when the double value was read (presumably from hardware).
     * @return The value we are looking for.
     * @htmlonly
       <br><hr>
       @endhtmlonly
     */
    virtual CORBA::Double
    read(unsigned long long& timestamp);
    /**
     * Writes the double value to "someplace".  This could be to physical hardware, files on 
     * disk, etc.  Every time the value of a BACI RWdouble property using this class is set,
     * a message will be printed to standard out.
     *
     * Last but definitely not least, it is important to note this method can be only be invoked
     * from RWdouble properties.
     * @param value The double value some ACS client (human or software) is trying to set for a
     *              BACI RWlong property that uses this devIO subclass.
     * @param errcode The error code (as defined in the ACS Error System) that occured while
     *        writing this value or 0 if no error occured.  It's up to the developer to 
     *        determine whether an error occured or not.
     * @param timestamp The time when the double value was written (presumably to hardware).
     * @htmlonly
       <br><hr>
       @endhtmlonly
     */
    virtual void 
    write(const CORBA::Double &value, unsigned long long& timestamp);
private:
};

#endif



