#ifndef SENSOR_CONTROL
#define SENSOR_CONTROL


//#include <acscomponentImpl.h>
#include <MARSCommonS.h>
//CORBA generated servant stub
#include <SCBaseS.h>
#include<SensorSimulatorS.h>
#include <baciCharacteristicComponentImpl.h>

class RobotSensorThread;

class SCBase: public baci::CharacteristicComponentImpl,
                 public virtual POA_MARS::SCBase {

     public:

     SCBase(const ACE_CString &name,
                maci::ContainerServices *containerServices);
	~SCBase();
	void reset        ();
	MARS::sensor_status_struct status        ();
	MARS::pos_struct position        ();
	CORBA::Long sensorType        ();
void sendAlarm(std::string fFamily, std::string fMember,int code, bool active);

     SS::SensorSimulator_var sensorSimulator;
     CORBA::Long sensorId;
     CORBA::Long sensorTypeVar;
     CORBA::Long sensorPosX;
     CORBA::Long sensorPosY;

     RobotSensorThread * m_RobotSensorThread_p;
 private:
};

#include<maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(SCBase)

#endif
