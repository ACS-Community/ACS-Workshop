#ifndef Mount2RefAzDevIO_h
#define Mount2RefAzDevIO_h

/*
 * "@(#) $Id: Mount2RefAzDevIO.h,v 1.1 2010-11-16 13:20:13 its Exp $"
 * 
 * $Log: Mount2RefAzDevIO.h,v $
 * Revision 1.1  2010-11-16 13:20:13  its
 * Moved LEGACY to EXTERNAL
 *
 * Revision 1.1  2010-11-15 21:32:59  its
 * Adding NExstar LEGACY
 *
 * Revision 1.1.1.1  2008/11/24 00:37:29  almamgr
 * Initial import
 *
 * Revision 1.1  2007/07/10 18:50:28  wg2
 *  - First DevIO functional implementation (some TODO)
 *
 * Revision 1.1.1.1  2007/07/09 12:47:39  wg8
 * Repository Setup
 *
 *
 */

#include <baciDevIO.h>
#include <acstime.h>

/*
 * Definition of LegoRefAltDevIO base class.
 */



class Mount2RefAzDevIO: public DevIO<CORBA::Double>
{
	public:
		Mount2RefAzDevIO();
		Mount2RefAzDevIO(void *data);
		virtual ~Mount2RefAzDevIO();

		virtual bool initializeValue();
		CORBA::Double   read(ACS::Time& timestamp) throw (ACSErr::ACSbaseExImpl);
		void         write(const CORBA::Double& value, ACS::Time& timestamp) 
			                                    throw (ACSErr::ACSbaseExImpl);

	private:
	
};
#endif

