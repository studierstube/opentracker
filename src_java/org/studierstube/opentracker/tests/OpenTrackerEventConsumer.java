package org.studierstube.opentracker.tests;

import org.omg.CORBA.Any;
import org.omg.CosEventComm.Disconnected;

import org.studierstube.opentracker.OT_CORBA.*;

public class OpenTrackerEventConsumer extends OTPushCons {

	public OpenTrackerEventConsumer(int disconnect) {
		super(disconnect);
	}

	/** CORBA method implementation. */
	public void disconnect_push_consumer()
	{
		System.out.println("Push Consumer: disconnected.");
	}
	
	public void push(Any data) throws Disconnected {
		/** CORBA method implementation. */
			//int l=data.extract_ulong();
			Event event = EventHelper.extract(data);
			System.out.println("Push Consumer: push() called");
			System.out.println("Position = [" + event.pos[0] + ", " + event.pos[1] + ", " + event.pos[2] + "]");
			System.out.println("Orientation = [" + event.ori[0] + ", " + event.ori[1] + 
					", " + event.ori[2] +  event.ori[3] + "]\n");
	}
	
	static public void main(String args[]) {
		System.out.println("OpenTrackerEventConsumer.main");
		consumer = new OpenTrackerEventConsumer(0);
		consumer.mainLoop(args);
	}
}
