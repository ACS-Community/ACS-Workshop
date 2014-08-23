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


		void pietro() {
			//char * res = comp->sayHello();
           comp->cameraOn();

           TYPES::ImageType_var image = comp->takeImage(1L);
           CPPUNIT_ASSERT(image->length() != 0);

           comp->cameraOff();
		};
		void pietro2() {
			//char * res = comp->sayHello();
           comp->cameraOn();

           TYPES::ImageType_var image = comp->takeImage(1L);
           CPPUNIT_ASSERT(image->length() != 0);

           comp->cameraOff();
		};

		void tearDown(){
			client.releaseComponent("INSTRUMENT");
		};

	CPPUNIT_TEST_SUITE(InstrumentTest);
	CPPUNIT_TEST(pietro);
	CPPUNIT_TEST(pietro2);
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
