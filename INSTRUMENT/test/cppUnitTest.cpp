#include <cppunit/TestSuite.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestCaller.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include <maciSimpleClient.h>

#include "InstrumentC.h"

maci::SimpleClient client;

class InstrumentTest: public CppUnit::TestFixture {
	private:

	INSTRUMENT_MODULE::Instrument *comp;
	public:
	
		void setUp() {
			comp = client.getComponent<INSTRUMENT_MODULE::Instrument>("INSTRUMENT", 0, true);
		};

		void TestCameraOn() {
					comp->cameraOn();

				};

		void TestCameraOff() {
					comp->cameraOff();

				};

		void TestTakeImage() {
			//char * res = comp->sayHello();

		   comp->cameraOn();

           TYPES::ImageType_var image = new TYPES::ImageType;
           CPPUNIT_ASSERT_NO_THROW(image = comp->takeImage(CORBA::Long(1L)));
		   CPPUNIT_ASSERT(image->length() != 0);

		   comp->cameraOff();

		   CPPUNIT_ASSERT_THROW(image = comp->takeImage(CORBA::Long(1L)), SORTErr::CameraIsOffEx);
		};

		void TestsetRGB() {
			//char * res = comp->sayHello();


			comp->cameraOn();

           TYPES::RGB rgbTestConfig;
           rgbTestConfig.blue = 0;
           rgbTestConfig.green= 0;
           rgbTestConfig.red = 0;

           comp->setRGB(rgbTestConfig);
           comp->cameraOff();

		};

		void TestsetPixelBias() {

		   comp->cameraOn();

           comp->setPixelBias(0L);
           comp->cameraOff();

		};

		void TestsetResetLevel() {
				//char * res = comp->sayHello();
			   comp->cameraOn();
	           comp->setResetLevel(0L);
	           comp->cameraOff();

			};

/*		void runTest() {
				//char * res = comp->sayHello();

				//comp->cameraOn();


	           try {
	        	   TYPES::ImageType_var image = comp->takeImage(CORBA::Long(1L));
		           CPPUNIT_ASSERT(image->length() != 0);
	           } catch (...) {
	        	   printf("Super boring!!!\n");
	        	   return;
	           }


	           TYPES::RGB rgbTestConfig;
	           rgbTestConfig.blue = 0;
	           rgbTestConfig.green= 0;
	           rgbTestConfig.red = 0;

	           comp->setRGB(rgbTestConfig);
	           comp->setPixelBias(0L);
	           comp->setResetLevel(0L);
	           comp->cameraOff();

			};
*/
		void tearDown(){
			client.releaseComponent("INSTRUMENT");
		};

	CPPUNIT_TEST_SUITE(InstrumentTest);
	CPPUNIT_TEST(TestCameraOn);
	CPPUNIT_TEST(TestCameraOff);
	CPPUNIT_TEST(TestTakeImage);
	CPPUNIT_TEST(TestsetPixelBias);
	CPPUNIT_TEST(TestsetRGB);
	CPPUNIT_TEST(TestsetResetLevel);
	//CPPUNIT_TEST(runTest);
	CPPUNIT_TEST_SUITE_END();
};


int main(int argc, char *argv[]) {
	client.init(argc, argv);
	client.login();

	CppUnit::TextUi::TestRunner runner;
    runner.addTest(InstrumentTest::suite());
    runner.run();

//    CppUnit::TestCaller<InstrumentTest> test ("testComponent",
//    		&InstrumentTest::runTest);

	client.logout();
}
