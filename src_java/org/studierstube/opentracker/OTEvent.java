package org.studierstube.opentracker;

import org.studierstube.opentracker.OT_CORBA.*;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Set;

import org.omg.CORBA.Any;
import javax.vecmath.*;

public class OTEvent extends HashMap<String, Any> {
	/**
	 * 
	 */
	private static final long serialVersionUID = -6849795742997397981L;
	
	public OTEvent(EventAttribute[] atts) {
		for (int i = 0; i < atts.length; i++) {
			//System.out.println("name of event attribute is " + atts[i].name);
			put(atts[i].name, atts[i].value);
		}
	}
	
	public Vector3f getPosition() {
		return new Vector3f(FloatVectorHelper.extract(get("position")));
	}
	
	public void setPosition(Vector3f pos) {
		Any value = null;
		float fv[] = {pos.x, pos.y, pos.y};
		FloatVectorHelper.insert(value, fv);
		put("position", value);
	}

	public Vector4f getOrientation() {
		return new Vector4f(FloatVectorHelper.extract(get("orientation")));
	}
	
	public void setOrientation(Vector4f ori) {
		Any value = null;
		float fv[] = {ori.x, ori.y, ori.z, ori.w};
		FloatVectorHelper.insert(value, fv);
		put("orientation", value);
	}
	
	public float getTimestamp() {
		return get("timestamp").extract_float();
	}
	
	public void setTimestamp(float timestamp) {
		Any value = null;
		value.insert_float(timestamp);
		put("timestamp", value);
	}
	
	public void setButton(short button) {
		Any value = null;
		value.insert_short(button);
		put("button", value);
	}
	
	public short getButton() {
		return get("button").extract_short();
	}
	
	public EventAttribute[] getEventAttributes() {
		Set<EventAttribute> atts = null;
		Set<String> keys = keySet();
		for (String key : keys) {
			EventAttribute att = null;
			att.name  = key;
			att.value = get(key);
			atts.add(att);
		}
		EventAttribute[] attributes = null;
		return atts.toArray(attributes);
	}
	
	public Any asAny() {
		Any ret = null;
		EventHelper.insert(ret, getEventAttributes());
		return ret;
	}
}
 