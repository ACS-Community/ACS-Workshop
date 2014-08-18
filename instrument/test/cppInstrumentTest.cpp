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
                        comp = client.getComponent<INSTRUMENT_MODULE::Instrument>("INSTRUMENT", 0, true); //From the CDB config.
                }

                void runTest() {
                        comp->cameraOn();                       
			TYPES::ImageType *resImage = comp->takeImage(10);
	                CPPUNIT_ASSERT(resImage != NULL);
			comp->cameraOff();
                }

                void tearDown(){
                        client.releaseComponent("INSTRUMENT");
                }

CPPUNIT_TEST_SUITE(InstrumentTest);
CPPUNIT_TEST(runTest);
CPPUNIT_TEST_SUITE_END();
};


int main(int argc, char *argv[]) {
        client.init(argc, argv);
        client.login();

	CppUnit::TextUi::TestRunner runner;
	runner.addTest(InstrumentTest::suite());
	runner.run();

        client.logout();
	return 0;
}
