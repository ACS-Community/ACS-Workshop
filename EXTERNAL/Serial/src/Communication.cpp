/**
 * \file Communication.cpp
 *
 * Implements the communication with the Celestron Nexstar 4 SE telescope at a
 * logical level; this means that this class is the responsible of formatting
 * the incoming and outcoming strings from/to the device, and convert then into
 * usefull information. The communication with the device itself is done by the
 * SerialRS232 class.
 *
 * \author Rodrigo Tobar <rtobar@csrg.inf.utfsm.cl>
 */

#include <Communication.h>
#include <stdio.h>
#include <string.h>

Communication::Communication(char *deviceName){
	this->sp = new SerialRS232(deviceName);
	this->sp->flush_RS232();
}

Communication::~Communication(){
	delete this->sp;
}

char *Communication::echo(char c){
	char msg[3];
	msg[0] = 'K'; 	msg[1] = c; msg[2] = '\0';
	this->sp->write_RS232(msg);
	return (this->sp->read_RS232());
}

bool Communication::alignmentComplete(){
	char *msg;
	bool retValue = false;
	
	this->sp->write_RS232("J");
	msg = this->sp->read_RS232();
	if( msg[0] == 1 )
			  retValue = true;

	return retValue;
}

bool Communication::Slew(int rate, int direction){
	int movement = 0x0, axis = 0x0;
	if ( rate > 9 || rate < 0 ){
		fprintf(stderr,"ERROR: Rate limits are 1 <= rate <= 9\n");
		return false;
	}

	printf("Moving in ");
	switch ( direction ) {

		case AZM_POS:
			printf("AZM_POS");
			axis     = 0x10;
			movement = 0x24;
			break;
		case AZM_NEG:
			printf("AZM_NEG");
			axis     = 0x10;
			movement = 0x25;
			break;

		case ALT_POS:
			printf("ALT_POS");
			axis     = 0x11;
			movement = 0x24;
			break;
		case ALT_NEG:
			printf("ALT_NEG");
			axis     = 0x11;
			movement = 0x25;
			break;

	}
	printf(" at %d rate\n",rate);
	char command[8];
	sprintf(command,"P%c%c%c%c%c%c%c",2,axis,movement,rate,0,0,0);

	this->sp->flush_RS232();
	this->sp->write_RS232(command);
	this->sp->flush_RS232();
	this->sp->write_RS232(command);
	this->sp->flush_RS232();
	this->sp->read_RS232();

	return true;
}

bool Communication::goToAltAzm(double alt, double azm){
	/*if ( alt > 90 || alt < 0 ){
		printf("Error: Altitud can't be > 90!\n");
		return false;
	}*/
	/* If azm is < 0, convert it into positive coordinates */
	while ( azm < 0 ){
		azm += 360.0;
	}

	azm /= 360;
	alt /= 360;

	char command[18];
	char *msg;
	sprintf(command,"b%08x,%08x",(unsigned int)(azm*MAX_PRECISE_ROTATION), (unsigned int)(alt*MAX_PRECISE_ROTATION));

	this->sp->write_RS232(command);
	this->sp->flush_RS232();
	msg = this->sp->read_RS232();

	return true;
}

double Communication::getAlt(){
	double alt;
	unsigned long read_alt, read_azm;
	char *msg;

	this->sp->write_RS232("z");
	msg = this->sp->read_RS232();
	sscanf(msg,"%08lX,%08lX#",&read_azm,&read_alt);

	printf("Alt: %lu Azm: %lu\n",read_alt,read_azm);
	alt = read_alt / MAX_PRECISE_ROTATION;
	alt *= 360.0;

	return alt;
}

double Communication::getAzm(){
	double azm;
	unsigned long read_alt, read_azm;
	char *msg;

	this->sp->write_RS232("z");
	msg = this->sp->read_RS232();
	sscanf(msg,"%08lX,%08lX#",&read_azm,&read_alt);

	printf("Alt: %lu Azm: %lu\n",read_alt,read_azm);
	azm = (double)read_azm / MAX_PRECISE_ROTATION;
	azm *= 360.0;

	return azm;
}
