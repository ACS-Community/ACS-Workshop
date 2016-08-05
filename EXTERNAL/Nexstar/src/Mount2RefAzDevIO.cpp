/*
 * "@(#) $Id: Mount2RefAzDevIO.cpp,v 1.1.1.1 2008/11/24 00:37:29 almamgr Exp $"
 *
 * $Log: Mount2RefAzDevIO.cpp,v $
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

#include "Mount2RefAzDevIO.h"
#include "Communication.h"
//#include "SCTdefs.h"
//#include "SCTlibs.h"

//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <unistd.h>

#include <ACSErrTypeCommon.h>

/*
 * Mount2RefAzDevIO base class.
 */


/*
 * Nothing to construct here
 */
Mount2RefAzDevIO::Mount2RefAzDevIO()
{
}


Mount2RefAzDevIO::Mount2RefAzDevIO(void *data)
{
}



/*
 * And nothing to destroy.
 */
Mount2RefAzDevIO::~Mount2RefAzDevIO()
{
   //sct_close_lego();
}



/**
 * Sets the commanded azimuth to actual telescope's azimuth.
 * NOT IMPLEMENTED YET!
 */
bool Mount2RefAzDevIO::initializeValue()
{
	return true;
}



/**
 * Reads the commanded azimuth from Mount2 telescope.
 *
 */
CORBA::Double Mount2RefAzDevIO::read(ACS::Time& timestamp) throw (ACSErr::ACSbaseExImpl)
{
	ACS_SHORT_LOG( (LM_INFO, "Mount2RefAzDevIO: read"));
	CORBA::Double ret_val(0.0);
//	char *msg;
//	unsigned long read_alt, read_azm;
//	CORBA::Double azm;

	Communication comm = Communication("/dev/ttyUSB0");
	ret_val = comm.getAzm();
//	SerialRS232 *sp = new SerialRS232("/dev/ttyUSB0");
//	sp->write_RS232("z");
//	msg = sp->read_RS232();
//	sscanf(msg,"%08lX,%08lX#",&read_azm,&read_alt);
//
//#define MAX_PRECISE_ROTATION 4294967296.0
//
//	azm = (double)read_azm / MAX_PRECISE_ROTATION;
//	azm *= 360.0;
//
//	ret_val = azm;
//
//	timestamp=getTimeStamp();
//	delete sp;
	return ret_val;
}



/**
 * Sets the commanded azimuth to Mount2 telescope.
 * NOT IMPLEMENTED
 */
void Mount2RefAzDevIO::write(const CORBA::Double& value, ACS::Time& timestamp) throw (ACSErr::ACSbaseExImpl)
{
	
	ACS_SHORT_LOG((LM_ERROR, "Mount2RefAzDevIO::write: This method should never be called!"));
	return;
}

