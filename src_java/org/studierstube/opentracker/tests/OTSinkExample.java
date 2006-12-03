package org.studierstube.opentracker.tests;

import org.studierstube.opentracker.OT_CORBA.Event;

public class OTSinkExample extends CorbaApp {
	class Sink_i extends org.studierstube.opentracker.OT_CORBA.OTSinkPOA {

		public void setEvent(Event ev) {
			System.out.println("position = (" + ev.pos[0] + ", " + ev.pos[1] + ", " + ev.pos[2] + ")");
			System.out.println("orientation = [(" + ev.ori[0] + ", " + ev.ori[1] + ", " + ev.ori[2] + "), " + ev.ori[3] + "]");
		}
	}
	
	protected Sink_i sink;
	
	public OTSinkExample(String[] args) {
		super(args);
		sink = new Sink_i();
	}

	public void activateSink(String name) {
		try {
			activateObject(sink);
			bindObectToName(sink._this(), name);
		} catch (ObjectActivationFailure e1) {
			System.err.println("Unable to activate sink. Exiting...");
			System.exit(-1);
		} catch (NamingServiceUnavailable e) {
			System.err.println("Unable to contact naming service. Exiting...");
			System.exit(-1);
		}
	}
	
	public static void main(String[] args) {
		System.out.println("Starting sink.");
		OTSinkExample _sink = new OTSinkExample(args);
		_sink.activateSink("CORBA.Sink01");
		getORB().run();
	}

}
