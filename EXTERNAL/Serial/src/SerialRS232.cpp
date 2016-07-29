/******************************************************************************
 * OAN - Observatorio Astron�mico Nacional
 * (c) OAN, 2004
 *
 * This code is under GNU General Public Licence (GPL).
 *
 * serialport_rs232.cpp
 *
 * who       when        what
 * --------  ----------  ----------------------------------------------
 * rbolano   2004/03/14  created
 * rtobar    2007/07/02  adapting to work with Celestron Nexstar 4 SE
 *                       (CSAT Poject)
 */

#include "SerialRS232.h"

#include <cstring>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/times.h>
#include <stdio.h>

SerialRS232::SerialRS232(const char * dev, const baudrates & baudrate,
		const parities & parity, const databits & databitsnum,
		const stopbits & stopbitsnum, const int & termc,
		const unsigned int & buflen, const int & timeout)
		throw (SerialRS232Exception &)
{
	if (index(dev, '\0') == NULL)
		throw SerialRS232Exception("[SerialRS232::SerialRS232] Device name must be null terminated");

	if (strlen(dev) < 0 || strlen(dev) > 64)
		throw SerialRS232Exception("[SerialRS232::SerialRS232] Device name incorrect length");

	if (strchr(dev, ' ') != NULL)
		throw SerialRS232Exception("[SerialRS232::SerialRS232] Device name cannot contain blank spaces");

	if ((m_port = open(dev, O_RDWR | O_NOCTTY ) )<0)
		throw SerialRS232Exception("[SerialRS232::SerialRS232] Error opening the device");
	
	fcntl(m_port, F_SETFL, fcntl(m_port, F_GETFL, 0) & ~O_NDELAY);

	struct termio term;

	if (ioctl(m_port, TCGETA, &oldterm) == -1)
		throw SerialRS232Exception("[SerialRS232::SerialRS232] Error when calling ioctl (GET)");
	term = oldterm;

	term.c_cflag =  CS8 | B9600 ;
	term.c_cflag |= CLOCAL | CREAD;
	term.c_iflag =  IGNBRK;
	term.c_lflag = 0;
	term.c_iflag &= ~(IXON|IXOFF|IXANY);
	term.c_oflag = 0;
	term.c_cflag &= ~(PARENB | PARODD);

	if (ioctl (m_port, TCSETA, &term)==-1)
		throw SerialRS232Exception("[SerialRS232::SerialRS232] Error when calling ioctl (SET)");


	if (buflen <= 0)
		throw SerialRS232Exception("[SerialRS232::SerialRS232] Buffer length must be greater than zero");

	m_dev = new char[strlen(dev) + 1];
	strcpy(m_dev, dev);
	
	m_baudrate = baudrate;
	
	m_parity = parity;
	
	m_databitsnum = databitsnum;
	
	m_stopbitsnum = stopbitsnum;
	
	m_termc = termc;

	m_buflen = buflen;
	m_buf = new char[m_buflen];

	m_timeout = timeout;

}

SerialRS232::~SerialRS232() throw (SerialRS232Exception &)
{
	delete [] m_buf;
	delete [] m_dev;

	/* Restoring the old parameters */
	if (ioctl (m_port, TCSETA, &oldterm)==-1)
		throw SerialRS232Exception("[SerialRS232::SerialRS232] Error when calling ioctl (SET)");
	
	if (close(m_port) != 0)
		throw SerialRS232Exception("[SerialRS232::~SerialRS232] Error closing");

}

char * SerialRS232::read_RS232() throw (SerialRS232Exception &)
{
	struct tms process_time;
	long read_time;
	char read_char = -111; // absurd value, might be STX (decimal 2)
	int read_bytes = 0;
	int i = 0;

	if (m_timeout > 0)
		read_time = times(&process_time) + m_timeout + 1;
	else
		read_time = -1;

	struct termio term;

	if (ioctl(m_port, TCGETA, &term) == -1)
		throw SerialRS232Exception("[SerialRS232::read_RS232] Error when calling ioctl (GET)");

	term.c_cc[VMIN] = 0;
	term.c_cc[VTIME] = (m_timeout + 9)/10;

	if (ioctl (m_port, TCSETA, &term) == -1)
		throw SerialRS232Exception("[SerialRS232::read_RS232] Error when calling ioctl (SET)");

	m_termc = '#';
	while(((unsigned int)i < (m_buflen - 1)) && ((m_termc < 0) || (m_termc != read_char)))
	{
		read_bytes = read(m_port, &read_char, 1);
		if (read_bytes == -1)
			throw SerialRS232Exception("[SerialRS232::read_RS232] Error reading");
		if ((read_time > 0) && (read_time - times(&process_time)) <= 0)
			throw SerialRS232Exception("[SerialRS232::read_RS232] Timeout reading");
		if (read_bytes == 1)
		{
			*(m_buf + i) = read_char;
			i++;
		}
	}

	m_buf[i] = '\0';
	flush_RS232();
	return m_buf;
}

void SerialRS232::write_RS232(const char * s) throw (SerialRS232Exception &)
{
	if (index(s, '\0') == NULL)
		throw SerialRS232Exception("[SerialRS232::write_RS232] String to be written must be null terminated");
	if (strlen(s) > (m_buflen - 1))
		throw SerialRS232Exception("[SerialRS232::write_RS232] Too large string");

	strcpy(m_buf, s);

	if (write(m_port, m_buf, strlen(s)) != (int)(strlen(s)))
		throw SerialRS232Exception("[SerialRS232::write_RS232] Error writing");
	usleep(10000);
}

void SerialRS232::flush_RS232() throw (SerialRS232Exception &)
{
	if (ioctl(m_port, TCFLSH, 0) == -1)
		throw SerialRS232Exception("[SerialRS232::flush_RS232] Error flushing");
}

SerialRS232::SerialRS232Exception::SerialRS232Exception(const char * ex) : exception()
{
	strncpy(exception_msg, ex, 99);
}
