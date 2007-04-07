package org.studierstube.opentracker;

import org.studierstube.opentracker.OT_CORBA.*;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Set;
import javax.vecmath.*;

public class OTEvent extends HashMap<String, org.omg.CORBA.Any> {
	/**
	 * 
	 */
	private static final long serialVersionUID = -6849795742997397981L;
	private org.omg.CORBA.ORB orb;
	
	public OTEvent(org.omg.CORBA.ORB _orb) {
		orb = _orb;
	}
	
	public OTEvent(org.omg.CORBA.ORB _orb, EventAttribute[] atts) {
		this(_orb);
		for (int i = 0; i < atts.length; i++) {
			System.out.println("name of event attribute is " + atts[i].name);
			put(atts[i].name, atts[i].value);
		}
	}
	
	public Vector3f getPosition() {
		return new Vector3f(FloatVectorHelper.extract(get("position")));
	}
	
	public void setPosition(Vector3f pos) {
		org.omg.CORBA.Any value = orb.create_any();
		float fv[] = {pos.x, pos.y, pos.y};
		FloatVectorHelper.insert(value, fv);
		put("position", value);
	}

	public Vector4f getOrientation() {
		return new Vector4f(FloatVectorHelper.extract(get("orientation")));
	}
	
	public void setOrientation(Vector4f ori) {
		org.omg.CORBA.Any value = orb.create_any();
		float fv[] = {ori.x, ori.y, ori.z, ori.w};
		FloatVectorHelper.insert(value, fv);
		put("orientation", value);
	}
	
	public float getTimestamp() {
		return get("timestamp").extract_float();
	}
	
	public void setTimestamp(float timestamp) {
		org.omg.CORBA.Any value = orb.create_any();
		value.insert_float(timestamp);
		put("timestamp", value);
	}
	
	public void setButton(short button) {
		org.omg.CORBA.Any value = orb.create_any();
		value.insert_short(button);
		put("button", value);
	}
	
	public short getButton() {
		return get("button").extract_short();
	}
	
	public EventAttribute[] getEventAttributes() {
		Set<EventAttribute> atts = new HashSet<EventAttribute>();
		Set<String> keys = keySet();
		for (String key : keys) {
			EventAttribute att = new EventAttribute();
			att.name  = key;
			att.value = get(key);
			atts.add(att);
		}
		return (EventAttribute[]) atts.toArray(new EventAttribute[atts.size()]);
	}
	
	public org.omg.CORBA.Any asAny() {
		org.omg.CORBA.Any ret = orb.create_any();
		EventHelper.insert(ret, getEventAttributes());
		return ret;
	}
}
 