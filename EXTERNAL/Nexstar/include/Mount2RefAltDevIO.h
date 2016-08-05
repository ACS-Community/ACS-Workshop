#ifndef Mount2RefAltDevIO_h
#define Mount2RefAltDevIO_h

/*
 * "@(#) $Id: Mount2RefAltDevIO.h,v 1.1.1.1 2008/11/24 00:37:29 almamgr Exp $"
 * 
 * $Log: Mount2RefAltDevIO.h,v $
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



class Mount2RefAltDevIO: public DevIO<CORBA::Double>
{
	public:
		Mount2RefAltDevIO();
		Mount2RefAltDevIO(void *data);
		virtual ~Mount2RefAltDevIO();

		virtual bool initializeValue();
		CORBA::Double   read(ACS::Time& timestamp) throw (ACSErr::ACSbaseExImpl);
		void         write(const CORBA::Double& value, ACS::Time& timestamp) 
			                                    throw (ACSErr::ACSbaseExImpl);

	private:
	
};
#endif

