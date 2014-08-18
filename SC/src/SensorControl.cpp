#include<SensorControl.h>
#include "ACSAlarmSystemInterfaceFactory.h"
#include "AlarmSystemInterface.h"
#include "FaultState.h"
#include "faultStateConstants.h"
#include "Timestamp.h"


using namespace acsalarm;
//-------------------------------------------

class RobotSensorThread : public ACS::Thread
      {
         public: 
 
         RobotSensorThread(const ACE_CString &name, SCBase * scbase_ptr, 
                 const ACS::TimeInterval& responseTime=ThreadBase::defaultResponseTime, 
                 const ACS::TimeInterval& sleepTime=ThreadBase::defaultSleepTime) :
                 ACS::Thread(name,responseTime,sleepTime) 
         {
                 ACS_TRACE("RobotSensorThread::RobotSensorThread"); 
                 scbase_p = scbase_ptr;
         }
 
         ~RobotSensorThread() 
         { 
                 ACS_TRACE("RobotSensorThread::~RobotSensorThread"); 
         }
     
//         virtual void onStart();
//     virtual void onStop();
 
         virtual void runLoop();

	SCBase * scbase_p;

         private:
};


void RobotSensorThread::runLoop()
{
	MARS::sensor_status_struct thestate = scbase_p->status();
//        ACS_TRACE("RobotSensorThread::runLoop"); 
	if (thestate.isAlarmActive)
	{
        	ACS_TRACE("RobotSensorThread::runLoop-AlarmActive"); 
		//printf("=====> ALARM !!!!!!");
    		try {
			//Reset
		    	scbase_p->sensorSimulator->setStatus(scbase_p->sensorId,0);
	    	} catch (...) {
     			ACS_SHORT_LOG((LM_INFO, "Error resetting sensor."));
	    	}

		//Setting the sensor name
		ACE_CString sensorName;
    		switch (scbase_p->sensorId)
		    {
		    case 0:
			sensorName = "SENSOR1";
			break;
		    case 1:
			sensorName = "SENSOR2";
			break;
		    case 2:
			sensorName = "SENSOR3";
			break;
		    }

		// send Alarm to upper level
		scbase_p->sendAlarm("SC_ALARMS",sensorName.c_str(),scbase_p->sensorTypeVar,true);
				

	}
}

//--------------------------------------------------------

SCBase::SCBase(
    const ACE_CString &name,
    maci::ContainerServices *containerServices):
  baci::CharacteristicComponentImpl::CharacteristicComponentImpl(name, containerServices)
{
    ACS_TRACE("SCBase::SCBase");
    try {
      ACE_CString cdbPath="alma/";
      cdbPath +=name;

      ACE_CString tName="tRobotSensor";
      tName +=name;

      // reading sensor characteristics
      CDB::DAL_var cdb = getContainerServices()->getCDB();    
      CDB::DAO_var dao =  cdb->get_DAO_Servant(cdbPath.c_str());
      sensorId      = dao->get_long("sensorId");
      sensorTypeVar = dao->get_long("sensorType");
      sensorPosX    = dao->get_long("sensorPosX");
      sensorPosY    = dao->get_long("sensorPosY");
 
      // retrieving sensorSimulator proxy
      sensorSimulator = containerServices->getComponent<SS::SensorSimulator>("sensorSimulator");

   SCBase * selfPtr = this;
   m_RobotSensorThread_p = getContainerServices()->getThreadManager()->create<RobotSensorThread, SCBase*>
                  (tName, // Name of the thread
                    selfPtr, 0, 250000); // pass a ptr to this as parameter to thread so it can call back to us
   m_RobotSensorThread_p->resume();


    } catch (...) {
       ACS_SHORT_LOG((LM_INFO, "Error retrieving sensorSimulator component."));
    }
}

SCBase::~SCBase()
{
ACS_TRACE("SCBase::~SCBase");

     getContainerServices()->getThreadManager()->destroy(m_RobotSensorThread_p);
     // cleanUp() done by the container and all threads stopped already ????
}

void SCBase::reset()
{
    ACS_TRACE("SCBase::reset");
    try {
	//Setting the sensor name
	ACE_CString sensorName;
    	switch (sensorId)
	    {
	    case 0:
		sensorName = "SENSOR1";
		break;
	    case 1:
		sensorName = "SENSOR2";
		break;
	    case 2:
		sensorName = "SENSOR3";
		break;
	    }

	// Send Alarm to upper level
	sendAlarm("SC_ALARMS",sensorName.c_str(),sensorTypeVar,false);
		
		
    	sensorSimulator->resetSensor(sensorId);
    } catch (...) {
     	ACS_SHORT_LOG((LM_INFO, "Error resetting all sensors."));
    }
}

MARS::sensor_status_struct SCBase::status()
{
    MARS::sensor_status_struct_var result = new  MARS::sensor_status_struct;
    CORBA::Boolean status = 0;

//    ACS_TRACE("SCBase::status");
    try {
    	status = sensorSimulator->getStatus(sensorId);
    } catch (...) {
     	ACS_SHORT_LOG((LM_INFO, "Error resetting all sensors."));
    }
    

    result->id = sensorId;
    result->type = sensorTypeVar;
    result->position.x = sensorPosX;
    result->position.y = sensorPosY;
    result->isEnabled = 1;
    result->isAlarmActive = status;

    return result._retn();
}

MARS::pos_struct SCBase::position()
{
    MARS::pos_struct_var position = new MARS::pos_struct;

    position->x = sensorPosX;
    position->y = sensorPosY;

    return position._retn();
}

CORBA::Long SCBase::sensorType()
{
    return sensorTypeVar;
}

void SCBase::sendAlarm(std::string family, std::string member, int code, bool active) {
	// constants we will use when creating the fault

		// create the AlarmSystemInterface
		//AlarmSystemInterface* alarmSource = ACSAlarmSystemInterfaceFactory::createSource("ALARM_SYSTEM_SOURCES");
  
                auto_ptr<acsalarm::AlarmSystemInterface> alarmSource = ACSAlarmSystemInterfaceFactory::createSource("ALARM_SYSTEM_SOURCES");

		// create the FaultState
		auto_ptr<acsalarm::FaultState> fltstate = ACSAlarmSystemInterfaceFactory::createFaultState(family, member, code);

		// set the fault state's descriptor
		std::string stateString;
		if (active) 
		{
			stateString = faultState::ACTIVE_STRING;
		} else {
			stateString = faultState::TERMINATE_STRING;
		}
		fltstate->setDescriptor(stateString);
		
		// create a Timestamp and use it to configure the FaultState
		Timestamp * tstampPtr = new Timestamp();
		auto_ptr<Timestamp> tstampAutoPtr(tstampPtr);
		fltstate->setUserTimestamp(tstampAutoPtr);

		// create a Properties object and configure it, then assign to the FaultState
		//Properties * propsPtr = new Properties();
		//propsPtr->setProperty(faultState::ASI_PREFIX_PROPERTY_STRING, "prefix");
		//propsPtr->setProperty(faultState::ASI_SUFFIX_PROPERTY_STRING, "suffix");
		//propsPtr->setProperty("TEST_PROPERTY", "TEST_VALUE");
		//auto_ptr<Properties> propsAutoPtr(propsPtr);
		//fltstate->setUserProperties(propsAutoPtr);

		// push the FaultState using the AlarmSystemInterface previously created
		//acsalarm::FaultState stateToPush(*fltstate);
		alarmSource->push(*fltstate);
}

// ___oOo___
