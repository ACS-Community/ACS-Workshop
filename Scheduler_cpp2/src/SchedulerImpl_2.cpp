#include "SchedulerImpl_2.h"
#include "SYSTEMErr.h"

/**
 * Start the scheduler.
 * The scheduler will loop through all available proposals,
 * either until all proposals are done or until the stop method is called.
 * Raises an exception if called twice.
 * @return None
 */
void SchedulerImpl_2::start(void) throw (SYSTEMErr::SchedulerAlreadyRunningEx) {
	if (isStarted_)
	{
		SYSTEMErr::SchedulerAlreadyRunningExImpl ex(__FILE__,  __LINE__, "SchedulerImpl_2::start");
		throw ex.getSchedulerAlreadyRunningEx();
	} else {
		isStarted_ = true;
		LOG_TO_DEVELOPER(LM_INFO, "Starting scheduler");
		SchedulerImpl_2* selfPtr = this;
		try {
			controlLoop_ = getContainerServices()->getThreadManager()->create<
					LoopThroughProposalsThread, SchedulerImpl_2*>(
					"schedulerControlLoop", selfPtr);
		} catch (ACSErr::ACSbaseExImpl &ex) {
			ex.log();
		}
		askedForStop_ = false;
		controlLoop_->resume();
	}

}

/**
 * Stops the scheduler.
 * This will stop the scheduler from scheduling more proposals. 
 * 
 * It will not(!) break the ongoing observation, and will return only
 * when the running observation has finished.
 *
 * Raises an exception if called twice.
 * @return None
 */
void SchedulerImpl_2::stop(void) throw (SYSTEMErr::SchedulerAlreadyStoppedEx){

	if (isStarted_ && ! askedForStop_)
	{
		STATIC_LOG_TO_DEVELOPER(LM_INFO, "Stopping scheduler...");
		askedForStop_ = true;
		if (controlLoop_ != 0)
			controlLoop_->suspend();
	}
	else
	{
		SYSTEMErr::SchedulerAlreadyStoppedExImpl ex(__FILE__, __LINE__, "SchedulerImpl_2::stop");
		throw ex.getSchedulerAlreadyStoppedEx();
	}
}

/**
 * Returns the pid of the proposal currently under execution
 *
 * Raises exception if no proposal is executing.
 * @retun Proposal ID
 */
::CORBA::Long SchedulerImpl_2::proposalUnderExecution(void) throw(SYSTEMErr::NoProposalExecutingEx){

	if (-1 == propId_)
	{
		SYSTEMErr::NoProposalExecutingExImpl ex(__FILE__, __LINE__, "SchedulerImpl_2::proposalUnderExecution");
		throw ex.getNoProposalExecutingEx();
	}
	return propId_;
}

SchedulerImpl_2::SchedulerImpl_2(const ACE_CString &name,
		maci::ContainerServices *cs) :
		acscomponent::ACSComponentImpl(name, cs), propId_(-1), isStarted_(false), dataBaseComponent_(
				DATABASE_MODULE::DataBase::_nil()), instrumentComponent_(
				INSTRUMENT_MODULE::Instrument::_nil()), telescopeComponent_(
				TELESCOPE_MODULE::Telescope::_nil()), askedForStop_(false) {

}

SchedulerImpl_2::~SchedulerImpl_2()
{
	// Here we have to stop all threads
	getContainerServices()->getThreadManager()->stopAll();
}

void SchedulerImpl_2::execute() {

	ACS_SHORT_LOG((LM_INFO,"SchedulerImpl_2::execute"));
	try {
		LOG_TO_DEVELOPER(LM_INFO, "Getting DataBase component");
		dataBaseComponent_ = getContainerServices()->getComponent<
				DATABASE_MODULE::DataBase>("DATABASE");

		LOG_TO_DEVELOPER(LM_INFO, "Getting Instrument component");
		instrumentComponent_ = getContainerServices()->getComponent<
				INSTRUMENT_MODULE::Instrument>("INSTRUMENT");

		LOG_TO_DEVELOPER(LM_INFO, "Getting Telescope component");
		telescopeComponent_ = getContainerServices()->getComponent<
				TELESCOPE_MODULE::Telescope>("TELESCOPE");
	} catch (maciErrType::CannotGetComponentExImpl &ex) {
		ex.log();
	} catch (ACSErr::ACSbaseExImpl &ex) {
		ex.log();
	} catch (CORBA::Exception &ex) {
		LOG_TO_DEVELOPER(LM_ERROR, "CORBA exception while getting component");
	} catch (...) {
		LOG_TO_DEVELOPER(LM_ERROR, "Unknown problem while getting component");
	}
}

void SchedulerImpl_2::cleanUp() {

	ACS_SHORT_LOG((LM_INFO,"SchedulerImpl_2::cleanUp"));

	LOG_TO_DEVELOPER(LM_INFO, "Releasing DataBase component");
	if (dataBaseComponent_ != DATABASE_MODULE::DataBase::_nil()) {
		getContainerServices()->releaseComponent("DATABASE");
		dataBaseComponent_ = DATABASE_MODULE::DataBase::_nil();
	}

	LOG_TO_DEVELOPER(LM_INFO, "Releasing Instrument component");
	if (instrumentComponent_ != INSTRUMENT_MODULE::Instrument::_nil()) {
		getContainerServices()->releaseComponent("INSTRUMENT");
		instrumentComponent_ = INSTRUMENT_MODULE::Instrument::_nil();
	}

	LOG_TO_DEVELOPER(LM_INFO, "Releasing Telescope component");
	if (telescopeComponent_ != TELESCOPE_MODULE::Telescope::_nil()) {
		getContainerServices()->releaseComponent("TELESCOPE");
		telescopeComponent_ = TELESCOPE_MODULE::Telescope::_nil();
	}

}

void SchedulerImpl_2::LoopThroughProposalsThread::runLoop() {
	if (! scheduler_->isStarted_) return;

	// Get proposals
	scheduler_->propList_ = scheduler_->dataBaseComponent_->getProposals();

	long propCount = scheduler_->propList_->length();
	std::ostringstream msg;
	msg << "Found " << propCount << " proposal(s)";
	LOG_TO_DEVELOPER(LM_INFO, msg.str());

	// Loop over proposals
	for (long propNum = 0; propNum < propCount; ++propNum) {
		// Get proposal ID
		TYPES::Proposal prop = (*scheduler_->propList_)[propNum];
		scheduler_->propId_ = prop.pid;


		try {
			// Check proposal status
			if (2 == scheduler_->dataBaseComponent_->getProposalStatus(scheduler_->propId_))
			{
				continue;
			}

			msg.str("");
			msg << "Processing proposal no. " << propNum << "(ID " << scheduler_->propId_ << ')';
			LOG_TO_DEVELOPER(LM_INFO, msg.str());

			// Set proposal status
			scheduler_->dataBaseComponent_->setProposalStatus(scheduler_->propId_, 1);

			// Switch camera ON
			scheduler_->instrumentComponent_->cameraOn();

			// Get number of targets
			long targetCount = prop.targets.length();
			msg.str("");
			msg << "Found " << targetCount << " target(s) in proposal no. " << propNum;
			LOG_TO_DEVELOPER(LM_INFO, msg.str());

			// Loop over targets
			for (long targetNum = 0; targetNum < targetCount; ++targetNum) {

				const ::CORBA::Long targetId = prop.targets[targetNum].tid;
				msg.str("");
				msg << "Observing target no. " << targetNum << " of proposal no. " << propNum;
				LOG_TO_DEVELOPER(LM_INFO, msg.str());

				// Observe target
				TYPES::ImageType_var imageCurrent =
						scheduler_->telescopeComponent_->observe(
								prop.targets[targetNum].coordinates,
								prop.targets[targetNum].expTime);

				// Store image
				scheduler_->dataBaseComponent_->storeImage(scheduler_->propId_, targetId, imageCurrent);
			}

			// Switch camera OFF
			scheduler_->instrumentComponent_->cameraOff();

			// Set proposal status to ready
			scheduler_->dataBaseComponent_->setProposalStatus(scheduler_->propId_, 2);
			if (scheduler_->askedForStop_)
				break;
		} catch (SYSTEMErr::InvalidProposalStatusTransitionEx &ex) {
			LOG_TO_DEVELOPER(LM_INFO, "Invalid proposal status... skipping current proposal");
		} catch (ACSErr::ACSbaseExImpl &ex) {
			ex.log();
		} catch (CORBA::Exception &ex) {
			LOG_TO_DEVELOPER(LM_ERROR, std::string("CORBA exception while processing proposals: ") + ex._name());

		} catch (...) {
			LOG_TO_DEVELOPER(LM_INFO, "Unexpected error... skipping current proposal");
		}
		if (scheduler_->askedForStop_)
			break;
	}
	LOG_TO_DEVELOPER(LM_INFO, "Stopped processing proposals");
	scheduler_->propId_ = -1;
	scheduler_->isStarted_ = false;
}

#include <maciACSComponentDefines.h>
MACI_DLL_SUPPORT_FUNCTIONS(SchedulerImpl_2)
