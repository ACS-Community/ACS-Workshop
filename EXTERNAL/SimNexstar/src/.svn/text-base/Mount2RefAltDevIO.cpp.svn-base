/*
 * "@(#) $Id: Mount2RefAltDevIO.cpp,v 1.2 2010-11-18 20:57:54 its Exp $"
 *
 * $Log: Mount2RefAltDevIO.cpp,v $
 * Revision 1.2  2010-11-18 20:57:54  its
 * Changing ttyUSB0
 *
 * Revision 1.1  2010-11-16 13:20:13  its
 * Moved LEGACY to EXTERNAL
 *
 * Revision 1.1  2010-11-15 21:33:28  its
 * Adding Legacy Nexstar
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

#include "Mount2RefAltDevIO.h"
#include "SerialRS232.h"
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

double Mount2RefAltDevIO::written_value = 0.0;

/*
 * Nothing to construct here
 */
Mount2RefAltDevIO::Mount2RefAltDevIO()
{
}


Mount2RefAltDevIO::Mount2RefAltDevIO(void *data)
{
}



/*
 * And nothing to destroy.
 */
Mount2RefAltDevIO::~Mount2RefAltDevIO()
{
   //sct_close_lego();
}



/**
 * Sets the commanded azimuth to actual telescope's azimuth.
 */
bool Mount2RefAltDevIO::initializeValue()
{
	Mount2RefAltDevIO::written_value = 15.0;
	return true;
}



/**
 * Reads the commanded azimuth from Mount2 telescope.
 */
CORBA::Double Mount2RefAltDevIO::read(ACS::Time& timestamp) throw (ACSErr::ACSbaseExImpl)
{
    AUTO_TRACE(__PRETTY_FUNCTION__);
	ACS_SHORT_LOG( (LM_INFO, "Mount2RefAltDevIO: read"));
    return Mount2RefAltDevIO::written_value;
}


/**
 * Sets the commanded azimuth to Mount2 telescope.
 */
void Mount2RefAltDevIO::write(const CORBA::Double& value, ACS::Time& timestamp) throw (ACSErr::ACSbaseExImpl)
{
	
	ACS_SHORT_LOG((LM_INFO, "Mount2RefAltDevIO::write"));
    if (Mount2RefAltDevIO::written_value != value) {
		sleep(5);
    	Mount2RefAltDevIO::written_value = value;
	}
	return;
}

