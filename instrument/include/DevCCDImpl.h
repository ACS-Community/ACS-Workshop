#ifndef DevCCDImpl_h
#define DevCCDImpl_h

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif


///Contains the defintion of the standard superclass for C++ components
#include <baciCharacteristicComponentImpl.h>
#include <baciROlong.h>
#include <baciRWlong.h>
#include <baciRWstring.h>
#include <baciROlongSeq.h>
#include <baciSmartPropertyPointer.h>
#include <baciROdouble.h>
#include <DevCCDS.h>

#define SN9C102_V4L2_CID_GREEN_BALANCE (V4L2_CID_PRIVATE_BASE + 1)
#define SN9C102_V4L2_CID_RESET_LEVEL (V4L2_CID_PRIVATE_BASE + 2)
#define SN9C102_V4L2_CID_PIXEL_BIAS_VOLTAGE (V4L2_CID_PRIVATE_BASE + 3)
#define SN9C102_V4L2_CID_LOW_RESET_LEVEL_COUNT (V4L2_CID_PRIVATE_BASE + 7)
#define SN9C102_V4L2_CID_HIGH_RESET_LEVEL_COUNT (V4L2_CID_PRIVATE_BASE + 8)

class DevCCDImpl: public baci::CharacteristicComponentImpl,     //Standard component superclass
                             public virtual POA_DEVCCD_MODULE::DevCCD   //CORBA servant stub
{
  public:
     /**
     * Constructor
     *
     * @param name component's name. This is also the name that will be used to find the
     * configuration data for the component in the Configuration Database.
     * @param containerService The pointer to the container services
     */
    DevCCDImpl(
          ACE_CString name,
          maci::ContainerServices * containerServices);

    /**
     * Destructor
     */
    virtual ~DevCCDImpl(){
    };

    void on();
    void off();

		

    /**
     * ALMA C++ coding standards state copy operators should be disabled.
     */
    void operator=(const DevCCDImpl&){};
    
	ACS::ROlongSeq_ptr frame();    
	ACS::RWstring_ptr device();
	ACS::RWlong_ptr red();
	ACS::RWlong_ptr blue();
	ACS::RWlong_ptr green();
	ACS::RWlong_ptr pixelBias();
	ACS::RWlong_ptr resetLevel();
	ACS::RWlong_ptr exposure();
	
	CORBA::Boolean locking(){ return 1;};
	CORBA::Boolean powered(){return 1;};
	TYPES::ImageType* image(CORBA::Double expo);
	void lock(){};
	void unlock(){};
	
//	private:
	baci::SmartPropertyPointer<baci::ROlongSeq> m_frame_sp;
	baci::SmartPropertyPointer<baci::RWstring> m_device_sp;
	baci::SmartPropertyPointer<baci::RWlong> m_red_sp;
	baci::SmartPropertyPointer<baci::RWlong> m_blue_sp;
	baci::SmartPropertyPointer<baci::RWlong> m_green_sp;
	baci::SmartPropertyPointer<baci::RWlong> m_pixelBias_sp;
	baci::SmartPropertyPointer<baci::RWlong> m_resetLevel_sp;
	baci::SmartPropertyPointer<baci::RWlong> m_exposure_sp;
	
	int video_fd;
};
/*\@}*/
/*\@}*/
#endif
