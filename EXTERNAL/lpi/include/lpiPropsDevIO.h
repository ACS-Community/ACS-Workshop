#ifndef _LPI_PROPS_DEVIO_H_
#define _LPI_PROPS_DEVIO_H_

#include <baciDevIO.h>
#include <acstime.h>

/* local storage */
class lpiPropsDevIO: public DevIO<CORBA::Long>
{

        public:

        lpiPropsDevIO(int fd, int id);
        virtual ~lpiPropsDevIO();

        CORBA::Long read(ACS::Time &timestamp)
              throw (ACSErr::ACSbaseExImpl);

        void write(const CORBA::Long &value, ACS::Time &timestap)
                      throw (ACSErr::ACSbaseExImpl);

        private:
        int control_id;
	int fd;
	void set_control(int value);
	int get_control();
};

#endif /* _LPI_PROPS_DEVIO_H_ */
