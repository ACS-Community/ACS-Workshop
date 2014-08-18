/*******************************************************************************
 *    ALMA - Atacama Large Millimiter Array
 *    (c) Associated Universities Inc., 2002 *
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
 *
 * "@(#) $Id: NXTImpl.cpp,v 1.4 2009/11/13 17:41:14 almamgr Exp $"
 * 
 * who       when      what
 * --------  --------  ----------------------------------------------
 * jslopez  2008-10-26 Initial implementation
 *
 *
 */

#include "NXTImpl.h"
#include <fstream>
#include <time.h>

//#define EDGE 450
//#define SLOW 60
//#define FAST 65
//#define GIRO 300
#define NO_SENSOR 0
#define LIGHT_ACTIVE 5
#define RAWMODE 0

using namespace acscomponent;

int EDGE;
int SLOW;
int FAST;
int GIRO;
int SLOWc;
int FASTc;
long int N;

NXTImpl::NXTImpl(const ACE_CString &name, maci::ContainerServices *containerServices) :
	acscomponent::ACSComponentImpl(name, containerServices)
	,associatedToDevNxt(false)
{
}

NXTImpl::~NXTImpl()
{
}


void NXTImpl::initialize() throw (acsErrTypeLifeCycle::LifeCycleExImpl) 
{
	ifstream file;
	file.open("/tmp/LEGOSETTINGS");
	file >> EDGE;
	file >> SLOW;
	file >> FAST;
	file >> GIRO;
	file >> SLOWc;
	file >> FASTc;
	file >> N;
	printf("EDGE: %d\n", EDGE);
	printf("SLOW: %d\n", SLOW);
	printf("FAST: %d\n", FAST);
	printf("GIRO: %d\n", GIRO);
	printf("SLOW: %d\n", SLOWc);
	printf("FAST: %d\n", FASTc);
	printf("N: %ld\n", N);
	file.close();
}

void NXTImpl::cleanUp()
{
}

void NXTImpl::associate(const char *componentName) throw(CORBA::SystemException)
{
	devnxt_comp = DEVNXT_MODULE::DevNXT::_nil();
	devnxt_comp = getContainerServices()->getComponent<DEVNXT_MODULE::DevNXT>(componentName);
	if( !(CORBA::is_nil(devnxt_comp.in())) )
		associatedToDevNxt = true;
}

void NXTImpl::setObjective(const TYPES::Positions & positions, CORBA::Long legoNumber) throw(CORBA::SystemException)
{
	int i, j;

	// Setting Grid
	for(i = 0; i < 6; i++)
		for(j = 0; j < 6; j++)
			individualGrid.p[i][j] = 1;
	
	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j = j + 2) {
			individualGrid.p[positions.coord[i][j]][positions.coord[i][j+1]] = 0;

			if((legoNumber - 1) > i && j == 0)
				individualGrid.p[positions.coord[i][j]][positions.coord[i][j+1]] = 1;

			if((legoNumber - 1) < i && j == 2)
				individualGrid.p[positions.coord[i][j]][positions.coord[i][j+1]] = 1;

			if((legoNumber - 1) == i && j == 0)
				individualGrid.p[positions.coord[i][j]][positions.coord[i][j + 1]] = 2;
			else if((legoNumber - 1) == i && j == 2)
				individualGrid.p[positions.coord[i][j]][positions.coord[i][j + 1]] = 3;
		}
}

void NXTImpl::runVehicle() throw(CORBA::SystemException)
{
	getRoad(individualGrid);
	if(associatedToDevNxt) {
		int i;
		TYPES::MotorState ms;
	//	TYPES::MotorState msTmp;

		for(i = 0; i < 36; i++)
			cout << "Step: " << road.s[i] << endl;

		// Sensor on
		TYPES::SensorState ss;
		ss.port = 0;
		ss.type = LIGHT_ACTIVE;
		ss.mode = RAWMODE;
		devnxt_comp->setSensorState(ss);
		ss.port = 1;
		devnxt_comp->setSensorState(ss);

		sleep(5);

		for(i = 0; i < 36; i++)
			switch(road.s[i]) {
				case 1:
						forward();
						break;
					case 2:
						turnRight();
						break;
					case 3:
						turnLeft();
						break;
					case 4:
						spin();
						break;
					default:
						i = 36;
				}
			ms.port = 0;
			ms.power = -FAST;
			ms.tachoLimit = 135;
			devnxt_comp->setMotorState(ms);
			ms.port = 2;
			devnxt_comp->setMotorState(ms);

			ms = devnxt_comp->getMotorState(0);
		//	msTmp = devnxt_comp->getMotorState(2);
		//	while(ms.port != 0 || msTmp.port != 0) { // runState <-> port
		//		ms = devnxt_comp->getMotorState(0);
		//		msTmp = devnxt_comp->getMotorState(2);
		//	}
			while(ms.port != 0) // runState <-> port
				ms = devnxt_comp->getMotorState(0);

			// Motor off
			ms.port = 0;
			ms.power = 0;
			ms.tachoLimit = 0;
			devnxt_comp->setMotorState(ms);
			ms.port = 2;
			devnxt_comp->setMotorState(ms);

			// Sensor off
			ss.port = 0;
			ss.type = NO_SENSOR;
			ss.mode = RAWMODE;
			devnxt_comp->setSensorState(ss);
			ss.port = 1;
			devnxt_comp->setSensorState(ss);
		}
	}

	void NXTImpl::getRoad(const TYPES::Grid & gr)
	{
		ASTAR_MODULE::Astar_var astar_comp;
		astar_comp = ASTAR_MODULE::Astar::_nil();
		astar_comp = getContainerServices()->getComponent<ASTAR_MODULE::Astar>("ASTAR_SOLVER");

		if( !(CORBA::is_nil(astar_comp.in())) )
			road = astar_comp->solveAlgorithm(gr);
	}

	void NXTImpl::forward()
	{
		int i = 0;
		int back = 1;

		TYPES::MotorState ms;
		int ssl, ssr;

		// Normalization
		for(i = 0;i < 2; i++){
			do {
				ssl = devnxt_comp->getSensorState(0);
				ssr = devnxt_comp->getSensorState(1);
			} while(ssr < 200 || ssl < 200);
		}

		// Main routine
		//while(!(ssl < EDGE && ssr < EDGE) && (ssl > 200 && ssr > 200))
		while(!(ssl < EDGE && ssr < EDGE))
		{
			back = 1;
			ms.port = 2;
			ms.power = -FAST;
			ms.tachoLimit = 0;
			devnxt_comp->setMotorState(ms);
			ms.port = 0;
			devnxt_comp->setMotorState(ms);

			//do {
			ssl = devnxt_comp->getSensorState(0);
			ssr = devnxt_comp->getSensorState(1);
			//} while(ssr < 200 || ssl < 200);
			//if(ssr > 200 && ssl > 200) { 
			while(ssr < EDGE && ssl > EDGE ) {
				//do {
				//ssl = devnxt_comp->getSensorState(0);
				//ssr = devnxt_comp->getSensorState(1);
				//} while(ssr < 200 || ssl < 200);
				if(back) {
					ms.port = 0;
					ms.power = SLOW;
					ms.tachoLimit = 0;
					devnxt_comp->setMotorState(ms);
					ms.port = 2;
					devnxt_comp->setMotorState(ms);
					usleep(N);
					back=0;
				}
				ms.port = 2;
				ms.power = -FASTc;
				devnxt_comp->setMotorState(ms);
				ms.port = 0;
				ms.power = SLOWc;
				devnxt_comp->setMotorState(ms);
				ssl = devnxt_comp->getSensorState(0);
				ssr = devnxt_comp->getSensorState(1);
			}

			while(ssl < EDGE && ssr > EDGE ) {
				//do {
				//ssl = devnxt_comp->getSensorState(0);
				//ssr = devnxt_comp->getSensorState(1);
				//} while(ssr < 200 || ssl < 200);
				if(back) {
					ms.port = 0;
					ms.power = SLOW;
					ms.tachoLimit = 0;
					devnxt_comp->setMotorState(ms);
					ms.port = 2;
					devnxt_comp->setMotorState(ms);
					usleep(N);
					back=0;
				}
				ms.port = 2;
				ms.power = SLOWc;
				devnxt_comp->setMotorState(ms);
				ms.port = 0;
				ms.power = -FASTc;
				devnxt_comp->setMotorState(ms);
				ssl = devnxt_comp->getSensorState(0);
				ssr = devnxt_comp->getSensorState(1);
			} 	
			//}
		}

	ms.port = 0;
	ms.power = -FAST;
	ms.tachoLimit = 135;
	devnxt_comp->setMotorState(ms);
	ms.port = 2;
	devnxt_comp->setMotorState(ms);

	ms = devnxt_comp->getMotorState(0);
	while(ms.port != 0) // runState <-> port
		ms = devnxt_comp->getMotorState(0);


	// Motor off
	ms.port = 0;
	ms.power = 0;
	ms.tachoLimit = 0;
	devnxt_comp->setMotorState(ms);
	ms.port = 2;
	devnxt_comp->setMotorState(ms);

}

void NXTImpl::turnRight()
{
	TYPES::MotorState ms;

	ms.port = 2;
	ms.power = -FAST;
	ms.tachoLimit = GIRO;
	devnxt_comp->setMotorState(ms);
	ms.port = 0;
	ms.power = FAST;
	devnxt_comp->setMotorState(ms);

	ms = devnxt_comp->getMotorState(0);
	while(ms.port != 0) // runState <-> port
		ms = devnxt_comp->getMotorState(0);
	ms.port = 2;
	ms.power = 0;
	ms.tachoLimit = 0;
	devnxt_comp->setMotorState(ms);
	ms.port = 0;
	devnxt_comp->setMotorState(ms);

}

void NXTImpl::turnLeft()
{
	TYPES::MotorState ms;

	ms.port = 2;
	ms.power = FAST;
	ms.tachoLimit = GIRO;
	devnxt_comp->setMotorState(ms);
	ms.port = 0;
	ms.power = -FAST;
	devnxt_comp->setMotorState(ms);

	ms = devnxt_comp->getMotorState(0);
	while(ms.port != 0) // runState <-> port
		ms = devnxt_comp->getMotorState(0);
	ms.port = 2;
	ms.power = 0;
	ms.tachoLimit = 0;
	devnxt_comp->setMotorState(ms);
	ms.port = 0;
	devnxt_comp->setMotorState(ms);

}

void NXTImpl::spin()
{
	TYPES::MotorState ms;

	ms.port = 2;
	ms.power = -FAST;
	ms.tachoLimit = GIRO*2;
	devnxt_comp->setMotorState(ms);
	ms.port = 0;
	ms.power = FAST;
	devnxt_comp->setMotorState(ms);

	ms = devnxt_comp->getMotorState(0);
	while(ms.port != 0) // runState <-> port
		ms = devnxt_comp->getMotorState(0);

	ms.port = 2;
	ms.power = 0;
	ms.tachoLimit = 0;
	devnxt_comp->setMotorState(ms);
	ms.port = 0;
	devnxt_comp->setMotorState(ms);

}

/* --------------- [ MACI DLL support functions ] -----------------*/
#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(NXTImpl)
/* ----------------------------------------------------------------*/
