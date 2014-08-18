import javax.swing.BoxLayout;
import javax.swing.JApplet;
import javax.swing.JFrame;
import abeans.pluggable.acs.ACSAbeansEngine;
import abeans.pluggable.acs.maci.settings.ACSPlugSettingsPlugIn;
import javax.swing.JInternalFrame;

import com.cosylab.gui.plugins.*;	
import com.cosylab.gui.framework.Desktop;
import com.cosylab.gui.framework.Launcher;
import com.cosylab.gui.framework.LauncherEnvironment;
import alma.AltAzModule.abeans.AltAz;
import alma.AltAzModule.*;
import java.awt.*;
import com.cosylab.abeans.*;
import com.cosylab.abeans.plugins.*;
import com.cosylab.abeans.displayers.*;


public class AltAzPanel extends AbeansLaunchable{
	
	private ACSAbeansEngine engine;
	private Slider jSlider1 = null;
	private Gauger jGauger = null;
	private Slider jSlider2 = null;
	private Gauger jGauger2 = null;
	private DialKnob jDialKnob = null;
//	private com.cosylab.abeans.SingleAbeanSelector singleAbeanSelector1;

	private AltAz altAz = null;  //  @jve:decl-index=0:visual-constraint="31,83"

	public AltAzPanel() {
		super();
		//initialize();
	}

	public AltAzPanel(Launcher arg0, LauncherEnvironment arg1, Desktop arg2,
			JInternalFrame arg3) {
		super(arg0, arg1, arg2, arg3);
		//initialize();
	}

	public AltAzPanel(Launcher arg0, LauncherEnvironment arg1, JApplet arg2) {
		super(arg0, arg1, arg2);
		//initialize();
	}

	public AltAzPanel(Launcher arg0, LauncherEnvironment arg1, JFrame arg2) {
		super(arg0, arg1, arg2);
		//initialize();
	}


	public AbeansEngine getAbeansEngine() {
        if (engine == null)
            engine = new ACSAbeansEngine(getClass().getName());
        return engine;
    }
	
	/**
	 * This method initializes this
	 * 
	 */
	private void initialize() {
	altAz = getAltAz();
	
	//singleAbeanSelector1 = new com.cosylab.abeans.SingleAbeanSelector();
	
	/* Widgets */
	jSlider1 = getJSlider1();
	jGauger = getJGauger();
	jSlider2 = new Slider();
	jGauger2 = new Gauger();
	jDialKnob = getJDialKnob();

	/* Definition of the layout to use */
	

        //GridLayout gridLayout = new GridLayout();
        //gridLayout.setRows(1);
        //gridLayout.setColumns(1);
        this.setLayout(new FlowLayout());


        this.setSize(new java.awt.Dimension(317,267));
	
	//add(singleAbeanSelector1);

	jDialKnob.setDoubleProperty(altAz.getRefAz());
	add(jDialKnob);

	jSlider1.setDoubleProperty(altAz.getRefAz());
        add(jSlider1);

	jGauger.setDoubleProperty(altAz.getRealAz());
        add(jGauger);

	jSlider2.setDoubleProperty(altAz.getRefAlt());
        add(jSlider2);

	jGauger2.setDoubleProperty(altAz.getRealAlt());
        add(jGauger2);

	}

	/**
	 * This method initializes jSlider1	
	 * 	
	 * @return com.cosylab.abeans.displayers.Slider	
	 */
	private Slider getJSlider1() {
		if (jSlider1 == null) {
			jSlider1 = new Slider();
		}
		return jSlider1;
	}

	/**
	 * This method initializes jGauger	
	 * 	
	 * @return com.cosylab.abeans.displayers.Gauger	
	 */
	private Gauger getJGauger() {
		if (jGauger == null) {
			jGauger = new Gauger();
		}
		return jGauger;
	}

	/**
	 * This method initializes altAz	
	 * 	
	 * @return alma.AltAzModule.abeans.AltAz	
	 */
	private AltAz getAltAz(){
		if (altAz == null) {
			altAz = new AltAz();
		}
		try{
			altAz.setRemoteName("TEST_ALTAZ_1");
		} catch(Exception e)
		{
			e.printStackTrace();
		}
		return altAz;
	}

	private DialKnob getJDialKnob() {
		if (jDialKnob == null) {
			jDialKnob = new DialKnob();
			jDialKnob.setSize(new java.awt.Dimension(67,76));
		}
		return jDialKnob;
	}


	public static void main(String[] args) {
		AbeansLaunchable.launch(AltAzPanel.class, args);
	}

    public void userInitializeGUI() {
        initialize();
        setSize(800,600);
    }

	
	public void userInitializePlugIns() {
        try
        {
                installPlugIn(AbeansSystemMenuPlugIn.class);
                installPlugIn(CosyStandardActionsPlugIn.class);
                installPlugIn(AboutPlugIn.class);
                installPlugIn(AbeansStandardActionsPlugIn.class);
                installPlugIn(VitragePlugIn.class);
                installPlugIn(ACSPlugSettingsPlugIn.class);

                // uncomment following line(s) to get plugin(s) installed at startup
                installPlugIn(AbeansExceptionPanelPlugIn.class);
                installPlugIn(TreeBrowserPlugIn.class);
                installPlugIn(LoggingPlugIn.class);
                installPlugIn(ReportAreaPlugIn.class);
        } catch (Exception e)
        {
                e.printStackTrace();
        }
}	
	
}  //  @jve:decl-index=0:visual-constraint="92,10"
