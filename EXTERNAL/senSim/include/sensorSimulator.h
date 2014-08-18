#ifndef sensorSimulator_h
#define sensorSimulator_h



#include <baciCharacteristicComponentImpl.h>

#include <sensorSimulatorS.h>

#include <baciROdouble.h>
#include <baciROstring.h>
#include <baciRWpattern.h>

#include <baciSmartPropertyPointer.h>



using namespace baci;

class SensorSimulator: public virtual CharacteristicComponentImpl,
			public virtual POA_SS::SensorSimulator,
			public baci::ActionImplementator

{

public:
	SensorSimulator(const ACE_CString& name, maci::ContainerServices* containerServices);
	virtual ~SensorSimulator();




	virtual baci::ActionRequest
	invokeAction(int function, baci::BACIComponent* cob_p, const int& callbackID, const CBDescIn& descIn, baci::BACIValue* value, Completion& completion, CBDescOut& descOut);
	

	virtual baci::ActionRequest
	onAction(baci::BACIComponent* cob, const int& callbackID, const CBDescIn& descIn,baci::BACIValue* value, Completion& completion, CBDescOut& descOut);

	
	virtual baci::ActionRequest 
	offAction(baci::BACIComponent * cob, const int& callbackID, const CBDescIn& 	descIn, baci::BACIValue* value, Completion& completion, CBDescOut& descOut);
	

	virtual baci::ActionRequest 
	resetAction(baci::BACIComponent * cob, const int& callbackID, const CBDescIn& descIn,baci::BACIValue* value, Completion& completion, CBDescOut& descOut);


	virtual void
	on (ACS::CBvoid_ptr cb, const ACS::CBDescIn & desc)
	throw(CORBA::SystemException);
	
	
	virtual void
	off (ACS::CBvoid_ptr cb, const ACS::CBDescIn & desc)
	throw(CORBA::SystemException);
	

	virtual void
	reset (ACS::CBvoid_ptr cb, const ACS::CBDescIn & desc)
	throw(CORBA::SystemException);
	
	virtual ACS::ROdouble_ptr
	x_position ()
	throw(CORBA::SystemException);


	virtual ACS::ROdouble_ptr
	y_position ()
	throw(CORBA::SystemException);
	

	virtual ACS::RWpattern_ptr
	status ()
	throw(CORBA::SystemException);


	virtual ACS::ROstring_ptr
	sensor_type ()
	throw(CORBA::SystemException);

private:
	/// Properties
	SmartPropertyPointer<ROdouble> m_y_position_sp;
	SmartPropertyPointer<ROdouble> m_x_position_sp;
	SmartPropertyPointer<RWpattern> m_status_sp;
	SmartPropertyPointer<ROstring> m_sensor_type_sp;

};
#endif /* sensorSimulator_h */


