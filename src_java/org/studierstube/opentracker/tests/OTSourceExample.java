package org.studierstube.opentracker.tests;

import org.omg.CORBA.COMM_FAILURE;
import org.studierstube.opentracker.OT_CORBA.*;

public class OTSourceExample extends CorbaApp {
	public OTSourceExample(String[] args) {
		super(args);
	}
	
	static public Event getEvent() {
		float pos[] = {1.0f, 2.0f, 3.0f};
		float ori[] = {0.707f, 0.0f, 0.0f, 0.707f};
		float timestamp = 0.0f;
		short button = 0;
		float conf = 0.5f;
		org.studierstube.opentracker.OT_CORBA.Event event = new org.studierstube.opentracker.OT_CORBA.Event(pos, ori, timestamp, button, conf);
		return event;
	}
	
	public static void main(String[] args) throws InterruptedException {
		try {
			org.omg.CORBA.Object obj = getObjectReference("CORBA.Source01");
			OTSource source = OTSourceHelper.narrow(obj);
			while(true) {
				try {
					System.out.println("about to set event");
					source.setEvent(getEvent());
				} catch (COMM_FAILURE e) {
					System.err.println("CORBA communication error. Keep trying!");
				}
				Thread.sleep(10);
			}
		} catch (NamingServiceUnavailable e) {
			System.err.println("Unable to get reference to source object");
		}
	}

}
