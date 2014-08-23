#include <cppunit/TestSuite.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestCaller.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include <iostream>

#include <maciSimpleClient.h>
#include <loggingMACROS.h>

#include <DataBaseC.h>
#include <TypesC.h>
#include <SORTErr.h>

maci::SimpleClient client;
class DataBaseTest: public CppUnit::TestFixture {
private:
	DATABASE_MODULE::DataBase *comp;

public:

	void setUp()
	{
		comp = client.getComponent<DATABASE_MODULE::DataBase>("DATABASE", 0, true);
		comp->clean();
	}

	void tearDown()
	{
		comp->clean();
		client.releaseComponent("DATABASE");
		comp = NULL;
	}

	void testStoreProposal()
	{
		TYPES::TargetList targets;
		TYPES::Target t;
		TYPES::Position p;
		targets.length(1);
		p.az = 0.0;
		p.el = 90.0;
		t.coordinates = p;
		t.tid = 0;
		t.expTime = 1;
		targets[0] = t;

		int pid = comp->storeProposal(targets);

		TYPES::ProposalList_var proposals = comp->getProposals();
		CPPUNIT_ASSERT(proposals->length() == 1);
		CPPUNIT_ASSERT(comp->getProposalStatus(pid) == DATABASE_MODULE::DataBase::STATUS_INITIAL_PROPOSAL);

		TYPES::Proposal prop = (*proposals)[0];
		CPPUNIT_ASSERT(prop.targets.length() == 1);
	}

	void testException() {
		TYPES::TargetList targets;
		TYPES::Target t;
		TYPES::Position p;
		targets.length(1);
		p.az = 0.0;
		p.el = 90.0;
		t.coordinates = p;
		t.tid = 0;
		t.expTime = 1;
		targets[0] = t;

		int pid = comp->storeProposal(targets);
		CPPUNIT_ASSERT_THROW(comp->setProposalStatus(pid, 2), SORTErr::InvalidProposalStatusTransitionEx);
	}

	CPPUNIT_TEST_SUITE(DataBaseTest);
	CPPUNIT_TEST(testStoreProposal);
	CPPUNIT_TEST(testException);
	CPPUNIT_TEST_SUITE_END();

};

int main(int argc, char *argv[]) {
	int ret = -1;
	ret = client.init(argc, argv);
	ret = client.login();

	CppUnit::TextUi::TestRunner runner;
	runner.addTest(DataBaseTest::suite());
	runner.run();

	ret = client.logout();

	return 0;
}

