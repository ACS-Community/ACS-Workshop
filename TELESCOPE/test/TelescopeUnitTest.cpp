#include <cppunit/TestSuite.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestCaller.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include <maciSimpleClient.h>
#include <loggingMACROS.h>
#include <iostream>

#include "TelescopeC.h"
#include <TypesC.h>
#include <SORTErrC.h>
#include <SORTErr.h>


maci::SimpleClient client;

class TelescopeTest: public CppUnit::TestFixture {
	private:
		TELESCOPE_MODULE::Telescope *comp;
	public:
	
		void setUp() {
			comp = client.getComponent<TELESCOPE_MODULE::Telescope>("TELESCOPE", 0, true);
		}

		void runTest() {
			TYPES::Position res = comp->getCurrentPosition();
			CPPUNIT_ASSERT(res.az >= 0);
		}


		void runTest_moveTo(){
			TYPES::Position_var pos = new TYPES::Position;
			pos->el = 60.0;
			pos->az = 300.0;
			CPPUNIT_ASSERT_NO_THROW(comp->moveTo(pos));

			pos->el = 500.0;
			pos->az = 14.0;
			CPPUNIT_ASSERT_THROW(comp->moveTo(pos), SORTErr::PositionOutOfLimitsEx);

			pos->el = 50.0;
			pos->az = 1400.0;
			CPPUNIT_ASSERT_THROW(comp->moveTo(pos), SORTErr::PositionOutOfLimitsEx);

			pos->el = 500.0;
			pos->az = 1400.0;
			CPPUNIT_ASSERT_THROW(comp->moveTo(pos), SORTErr::PositionOutOfLimitsEx);
		}






		void runTest_observe(){
			// setting the position and exposure time as parameter coming from scheduler
			TYPES::Position_var pos = new TYPES::Position;
			pos->el = 50.0;
			pos->az = 14.0;

			//CORBA::Long exposureTime = 30;

			comp->moveTo(pos.in());

			TYPES::ImageType_var returnedImage = new TYPES::ImageType;
			//CPPUNIT_ASSERT_NO_THROW(returnedImage = comp->observe(pos.in(), exposureTime));
			//CPPUNIT_ASSERT(returnedImage->length() > 0);

//			exposureTime = -10;
//			CPPUNIT_ASSERT_THROW(returnedImage = comp->observe(pos.in(), exposureTime), SORTErr::PositionOutOfLimitsEx);

//			exposureTime = 10;
//			pos->el = 500.0;
//			pos->az = 14.0;
//			CPPUNIT_ASSERT_THROW(returnedImage = comp->observe(pos.in(), exposureTime), SORTErr::PositionOutOfLimitsEx);

//			exposureTime = 10;
//			pos->el = 50.0;
//			pos->az = 1400.0;
//			CPPUNIT_ASSERT_THROW(returnedImage = comp->observe(pos.in(), exposureTime), SORTErr::PositionOutOfLimitsEx);

//			exposureTime = 10;
//			pos->el = 500.0;
//			pos->az = 1400.0;
//			CPPUNIT_ASSERT_THROW(returnedImage = comp->observe(pos.in(), exposureTime), SORTErr::PositionOutOfLimitsEx);


//			exposureTime = -10;
//			pos->el = 50.0;
//			pos->az = 1400.0;
//			CPPUNIT_ASSERT_THROW(returnedImage = comp->observe(pos.in(), exposureTime), SORTErr::PositionOutOfLimitsEx);
		}

		void tearDown(){
			client.releaseComponent("TELESCOPE");
		}

CPPUNIT_TEST_SUITE(TelescopeTest);
CPPUNIT_TEST(runTest);
CPPUNIT_TEST(runTest_moveTo);
CPPUNIT_TEST(runTest_observe);
CPPUNIT_TEST_SUITE_END();
};


int main(int argc, char *argv[]) {
        int ret = -1;
        ret = client.init(argc, argv);
	ret = client.login();

        CppUnit::TextUi::TestRunner runner;
        runner.addTest(TelescopeTest::suite());
        runner.run();

	ret = client.logout();

	return 0;
}
