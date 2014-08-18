package alma.COORDINATOR_MODULE.CoordinatorImpl;

import java.util.logging.Logger;
import alma.ACS.*;
import alma.TYPES.*;
import alma.JavaContainerError.wrappers.AcsJContainerServicesEx;
import alma.ACS.ComponentStates;
import alma.acs.component.ComponentLifecycle;
import alma.acs.container.ContainerServices;
import java.util.*;
import alma.COORDINATOR_MODULE.CoordinatorOperations;
import java.io.*;

public class CoordinatorImpl implements CoordinatorOperations, ComponentLifecycle
{
	private ContainerServices m_containerServices;
	private Logger m_logger;
	private alma.NXT_MODULE.NXT nxtAlpha;
	private alma.NXT_MODULE.NXT nxtBeta;
	private alma.NXT_MODULE.NXT nxtGamma;
	private alma.NXT_MODULE.NXT nxtDelta;
	private alma.NXT_MODULE.NXT nxtEpsilon;

	public void initialize(ContainerServices containerServices) {
		m_containerServices = containerServices;
		m_logger = m_containerServices.getLogger();
		m_logger.info("initialize() called");
	}

	public void execute() {
		m_logger.info("execute() called");
	}
	public void cleanUp() {
		m_logger.info("cleanUp() called");
	}
	public void aboutToAbort() {
		cleanUp();
		m_logger.info("managed to abort...");
	}

	/////////////////////////////////////////////////////////////
	// Implementation of ACSComponent
	/////////////////////////////////////////////////////////////

	public ComponentStates componentState() {
		return m_containerServices.getComponentStateManager().getCurrentState();
	}
	public String name() {
		return m_containerServices.getName();
	}

	/////////////////////////////////////////////////////////////
	// Implementation of CoordinatorOperations
	/////////////////////////////////////////////////////////////

	public void instantiate(String componentName){
		org.omg.CORBA.Object obj = null;

		try {
			obj = m_containerServices.getComponent(componentName);
			if(componentName.equals("ALPHA")){
				nxtAlpha = alma.NXT_MODULE.NXTHelper.narrow(obj);
				m_logger.info("Connected to " + componentName);
				nxtAlpha.associate("LEGONXT_" + componentName);
				m_logger.info("Associated to LEGONXT_" + componentName);
			} else if(componentName.equals("BETA")){
				nxtBeta = alma.NXT_MODULE.NXTHelper.narrow(obj);
				m_logger.info("Connected to " + componentName);
				nxtBeta.associate("LEGONXT_" + componentName);
				m_logger.info("Associated to LEGONXT_" + componentName);
			} else if(componentName.equals("GAMMA")){
				nxtGamma = alma.NXT_MODULE.NXTHelper.narrow(obj);
				m_logger.info("Connected to " + componentName);
				nxtGamma.associate("LEGONXT_" + componentName);
				m_logger.info("Associated to LEGONXT_" + componentName);
			} else if(componentName.equals("DELTA")){
				nxtDelta = alma.NXT_MODULE.NXTHelper.narrow(obj);
				m_logger.info("Connected to " + componentName);
				nxtDelta.associate("LEGONXT_" + componentName);
				m_logger.info("Associated to LEGONXT_" + componentName);
			} else if(componentName.equals("EPSILON")){
				nxtEpsilon = alma.NXT_MODULE.NXTHelper.narrow(obj);
				m_logger.info("Connected to " + componentName);
				nxtEpsilon.associate("LEGONXT_" + componentName);
				m_logger.info("Associated to LEGONXT_" + componentName);
			}

		} catch (alma.JavaContainerError.wrappers.AcsJContainerServicesEx e) {
			m_logger.fine("Failed to get " + componentName + " component reference");
		}
	}

	public void setObjectivesCaller(alma.TYPES.Positions pos, String componentName){
		if(componentName.equals("ALPHA")){
			nxtAlpha.setObjective(pos, 1);
			m_logger.info(componentName + "::setObjective() called");
		} else if(componentName.equals("BETA")){
			nxtBeta.setObjective(pos, 2);
			m_logger.info(componentName + "::setObjective() called");
		} else if(componentName.equals("GAMMA")){
			nxtGamma.setObjective(pos, 3);
			m_logger.info(componentName + "::setObjective() called");
		} else if(componentName.equals("DELTA")){
			nxtDelta.setObjective(pos, 4);
			m_logger.info(componentName + "::setObjective() called");
		} else if(componentName.equals("EPSILON")){
			nxtEpsilon.setObjective(pos, 5);
			m_logger.info(componentName + "::setObjective() called");
		}
	}

	public void runVehiclesCaller(String componentName){
		if(componentName.equals("ALPHA")){
			nxtAlpha.runVehicle();
			m_logger.info(componentName + "::runVehicle() called");
		} else if(componentName.equals("BETA")){
			nxtBeta.runVehicle();
			m_logger.info(componentName + "::runVehicle() called");
		} else if(componentName.equals("GAMMA")){
			nxtGamma.runVehicle();
			m_logger.info(componentName + "::runVehicle() called");
		} else if(componentName.equals("DELTA")){
			nxtDelta.runVehicle();
			m_logger.info(componentName + "::runVehicle() called");
		} else if(componentName.equals("EPSILON")){
			nxtEpsilon.runVehicle();
			m_logger.info(componentName + "::runVehicle() called");
		}
	}
}
