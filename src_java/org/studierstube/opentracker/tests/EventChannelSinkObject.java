package org.studierstube.opentracker.tests;

import javax.vecmath.Vector3d;
import javax.vecmath.Vector3f;

import org.omg.CORBA.Any;
import org.omg.CosEventComm.Disconnected;

import org.studierstube.opentracker.OT_CORBA.*;

import org.studierstube.opentracker.tests.*;

public class EventChannelSinkObject extends OTPushCons {
	private float x,y,z;
	
	public EventChannelSinkObject(String[] args) {
		super(args);
		x=0.0f; y=0.0f; z=0.0f;
	}
	
	public void disconnect_push_consumer() {
		// TODO Auto-generated method stub

	}

	synchronized public void push(Any data) throws Disconnected {
		/** CORBA method implementation. */
		//int l=data.extract_ulong();
		Event event = EventHelper.extract(data);
		x = event.pos[0];
		y = event.pos[1];
		z = event.pos[2];
	}

	synchronized public float X() {
		return x;
	}
	
	synchronized public float Y() {
		return y;
	}
	
	synchronized public float Z() {
		return z;
	}
	
	synchronized public Vector3f Pos() {
		return new Vector3f(x, y, z);
	}
	
	synchronized public static void main(String[] args) {
		EventChannelSinkObject ec = new EventChannelSinkObject(args);
		ec.connect();
		while (true) {
			System.out.println("pos = (" + ec.X() + ", " + ec.Y() + ", " + ec.Z() + ")");
		}
	}
}
