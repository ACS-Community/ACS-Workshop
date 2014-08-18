/* CSRG - UTFSM Computer Systems Research Group
*  ACS-UTFSM Team
*
* NAME
*      SCTlib.cpp 
* SYNOPSIS
*      RCX Comunication abstraction layer implematation
*/


#include <SCTlib.h>
#include <sys/poll.h>

static int linkmf=-1;

int parseMessage(char *msg) {
	return((int)msg[5]);
}

void send(int fd, unsigned char *buf, int count) {
	unsigned char buf2[BUF_SIZE];
	int j = 0,i = 0;
	unsigned char c = 0x00;
	int nw;
	int pos = 0;
	int remaining = 2*count+5;
	buf2[j++] = 0x55;
	buf2[j++] = 0xff;
	buf2[j++] = 0x00;
	for(i = 0; i < count && i < BUF_SIZE; ++i) {
		buf2[j++] = buf[i];
		buf2[j++] = ~buf[i];
		c += buf[i];
	}
	buf2[j++] = c;
	buf2[j++] = ~c;
	
	while((nw = write(fd, buf2 + pos, remaining)) > 0) {
		pos += nw;
		remaining -= nw;
	}
}

int receive(int fd, unsigned char *buf, int count) 
{
	unsigned char buf2[BUF_SIZE];
	int i=0, j=0;
        int nr = 0;
	int pos = 0;
	int k = 0;
	unsigned char chksum = 0;
	ioctl(fd, LEGO_TOWER_SET_READ_TIMEOUT, LEGO_TOWER_READ_TIMEOUT_VALUE);
	while((nr = read(fd, buf2, BUF_SIZE)) > 0 && pos < BUF_SIZE) {
		for(i = 0; i < nr; ++i) {
			buf[pos++] = buf2[i];
		}
	}
	if(buf[0] != 0x55) {
		return -1;
	}
	if(buf[1] != 0xff) {
		return -1;
	}
	if(buf[2] != 0x00) {
		return -1;
	}
	for(j = 4; j < pos-2 && k < count; j+=2) {
		if((buf[j] | ~buf[j-1]) & 0xFF != 0xFF) {
			return -1;
		}
		buf[k++] = buf[j-1];
		chksum += buf[j-1];
	}
	if(j < pos-2) {
		return -1;
	}
	if(buf[pos-2] != chksum || (buf[pos-2] | ~buf[pos-1]) & 0xFF != 0xFF) {
		return -1;
	}
	return k;
}

/**
 * Get the value of a variable from a link to RCX.
 */
short int getVariable(int fd, unsigned char variable)
{
	unsigned char buf_in[3], buf_out[3];
	static unsigned char mask = 0;
	int result;
	/*
	 * Ensure that commands 0x12 and 0x1A are sent interchangeably because
	 * of RCX's error correction protocol.
	 */
	mask ^= 0x08;
	buf_out[0] = 0x12 | mask;
	buf_out[1] = 0x00;
	buf_out[2] = variable;
	send(fd, buf_out, 3);
	result = receive(fd, buf_in, 3);
	if(result == 3 && (buf_in[0] | buf_out[0]) & 0xFF == 0xFF) {
		return buf_in[1] | (buf_in[2] << 8);
	}
        return -1;
}

/**
 * Set the value of a RCX variable.
 */
int setVariable(int fd, int variable, short int value)
{
	unsigned char buf_in[5], buf_out[1];
	static unsigned char mask = 0;
        int result;
	mask ^= 0x08;
	buf_in[0] = 0x14 | mask;
	buf_in[1] = variable;
	buf_in[2] = 0x02;
	buf_in[3] = value & 0xFF;
	buf_in[4] = value >> 8;
        send(fd, buf_in, 5);
	result = receive(fd, buf_out, 1);
        if(result == 1 && (buf_out[0] | buf_in[0]) & 0xFF == 0xFF) {
           return 0;
        }
        return 1;
}

int openLego(char *dev) {
   if (linkmf!=-1) return(linkmf);
   if ((linkmf = open(dev, O_RDWR)) == -1) {
      return -1;
   } else {
      return(linkmf);
   }
}
/* ___oOo___ */
