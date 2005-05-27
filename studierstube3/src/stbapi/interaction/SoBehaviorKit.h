/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** class definition for SoBehaviorKit
  *
  * base class for scene objects the user can interact with
  *
  * @author  Zsolt Marx
  *
  * $Id: SoBehaviorKit.h 4137 2005-03-22 09:57:22Z kaufmann $
  * @file                                                                   */
 /* ======================================================================= */



// We cannot simply use a virtual method for handle3DEvent,
// in order to make the Action choose the correct handle3DEvent,
// when traversing the graph, because a function registered at
// an SoAction must be static and virtual static is an
// oxymoron

#ifndef _SO_BEHAVIOR_KIT_
#define _SO_BEHAVIOR_KIT_

#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoMFNode.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFEnum.h>

#include <Inventor/engines/SoCompose.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <stbapi/event3d/SoHandle3DEventAction.h>
#include <stbapi/event3d/So3DEvent.h>
#include <stbapi/event3d/StbEventInspector.h>
#include <stbapi/interaction/SoLayoutKit.h>
#include <stbapi/interaction/SoSubBehavior.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class StbEventInspector;
/**
 * To make leaf nodes interactive, i.e.
 * sensitive to a SoHandle3DEventAction, they must be
 * subclassed from SoBehaviorKit. A SoBehaviorKit's
 * interface contains the method handle3DEventAction(), which
 * represents the widget's behavior.

CATALOG PARTS

  All Parts
  <table>
  <tr> <td>Part Name</td><td>Part Type</td><td>Default Type</td><td>NULL Default</td></tr>
  <tr> <td>label </td><td>SoLabelKit</td><td>SoLabelKit</td><td>YES</td></tr>
  <tr> <td>tooltip </td><td>SoTooltipKit</td><td>SoTooltipKit</td><td>YES</td></tr>
 </table>

   File Format / defaults:

@verbatim

SoBehaviorKit {

 
	SoSFFloat	hysteresisBoxScale		 1.0    # in percent of widget dimensions (1.0 means 100%)
	SoSFFloat   hysteresisBoxScaleHeight 1.0    # in percent of widget height (1.0 means 100%)
	SoSFBool	enable					TRUE	# if true the widget will be enabled else it is disabled (should not react on interaction)
	SoSFBool	applyWidgetStyle		TRUE    # if true a widget style will replace the geometry and parameters of this widget
												# else the widget will not be changed
  	# from SoLayoutKit

	SoSFFloat	width					 1.0
	SoSFFloat	height					 1.0
	SoSFFloat	depth					 1.0

	  }
  
@endverbatim

Examples

  DO NOT INSTATIATE THIS CLASS !!

 * @ingroup widgets
 */
class STBAPI_API SoBehaviorKit : public SoLayoutKit {

	SO_KIT_HEADER(SoBehaviorKit);


	/**
	 * SoTranslation - used to move the widget (to make space for the label)
	 */
    SO_BEHAVIOR_CATALOG_ENTRY_HEADER(translationOffset);

	/**
	 * SoAbortGroup - used to keep the BoundingBoxAction to take label and tooltip into account
	 */
    SO_BEHAVIOR_CATALOG_ENTRY_HEADER(abortGroup);
	
	SO_BEHAVIOR_CATALOG_ENTRY_HEADER(label);
	SO_BEHAVIOR_CATALOG_ENTRY_HEADER(tooltip);
	SO_BEHAVIOR_CATALOG_ENTRY_HEADER(tooltipDisabled);

	/**
	 * SoSwitch - used for switching between normal and highlighted geometry
	 */
    SO_BEHAVIOR_CATALOG_ENTRY_HEADER(geometrySwitch);

    /**
     * the switch used to turn the hysteresis geometry on and off
     */
    SO_BEHAVIOR_CATALOG_ENTRY_HEADER(hysteresisSwitch);

    /**
     * the hysteresis cube is the invisible geometry that is added to the kit's
     * geometry to enlarge it's bounding box beyond its visible geometry borders
     */
    SO_BEHAVIOR_CATALOG_ENTRY_HEADER(hysteresisCube);

    /**
     * the part used to turn the cube invisible
     */
    SO_BEHAVIOR_CATALOG_ENTRY_HEADER(invisibility);

	/**
     * the part used to encapsulate hysteresis parts
     */
    SO_BEHAVIOR_CATALOG_ENTRY_HEADER(hysteresisSep);

	/**
     * the part used to set material for hysteresisCube
     */
    SO_BEHAVIOR_CATALOG_ENTRY_HEADER(hysteresisMaterial);

	
public:

    /**
	 * scale factor for the hysteresis Box (uniform)
	 */
	SoSFFloat hysteresisBoxScale;

    /**
	 * scale factor for the hysteresis Box (just height), this will be multiplied with the uniform hysteresisBoxScale
	 */
	SoSFFloat hysteresisBoxScaleHeight;

	/**
	 * enable / disable widget flag
	 */
	SoSFBool enable;

	/**
	 * specify wheter the widget styles should be applied
	 */
	SoSFBool applyWidgetStyle;

	/**
	 * specify which style should be applied. If the styleName is not "", a style for the widgetType is searched.
	 * No style is applied if there is no style with that name.
	 *
	 */
	SoSFString styleName;

	/*
	 * constructor
	 */
	SoBehaviorKit();

	/**
	 * destructor
	 */
	virtual ~SoBehaviorKit();

	/**
	 * handles the low level event. is only reached,
	 * if it has ocurred in its bounding box or it
	 * is directly called by the StbEventGenerator
	 *
	 * @param h3a is the SoHandle3DEventAction the application
	 *        of which caused the scene graph traversal and
	 *        the invocation of handle3DEvent
	 */
	virtual void handle3DEvent(SoHandle3DEventAction *h3a);

	/**
	 * returns whether the event ocurred in the kit's bounding
	 * box
	 */
	SbBool isInside(const SoHandle3DEventAction *h3a);

	void inspect(const SoHandle3DEventAction *h3a);

	/**
	 * examineEvent simply passes the event handed in
	 * to the kit's inspector.
	 */
	void examineEvent(const SoHandle3DEventAction *h3a);

  /**
   * helper method to detect a action
   * @return true, if one of the gestures detect returns true
   */
  bool detectAction(SoMFNode &gestures);

	SbBool isInterested();

	SoSwitch* getGeometrySwitch();
	SoSwitch* getHysteresisSwitch();
	SoCube* getHysteresisCube();

    void setHysteresis(bool on);

    /**
     * traverses the list of eventReceivers
     */
    void traverseEventReceivers(SoHandle3DEventAction *h3a);

	virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways);

    /**
     * reads a scene graph from a string
     */
    static SoNode *getGeometryFromString(char *string);

	enum LabelPlacing
	{
		NONE = 0,		/// ignore lable
		TOP = 1,		/// label will be placed above the widget 
		LEFT = 2,		/// label will be placed left of the widget 
		RIGHT = 3,      /// label will be placed right of the widget 
		BOTTOM = 4,     /// label will be placed below of the widget 
		ONWIDGET = 5,   /// label will be placed on top of the widget
	} ;

	/// specifiy where the label should be placed (ONWIDGET means that it will be above the widget)
    SoSFEnum labelPlacing;

    //
	//  SoSFInt32 labelPlacingInt;

	virtual void adaptToTooltips();


	SoSFBool inside;
	SoMFString statusBoxText;

SoINTERNAL public:

	static void initClass();

protected:
	SbBool selfActivation;
	SbVec3f widgetDim;    ///< for internal use
	SbVec3f widgetMove;   ///< for internal use

	SoFieldSensor enableSensor;
	static void enableChangedCB(void *, SoSensor*);
	virtual void adaptToEnable();
	
	virtual void resizeCB();						///< each sub class should behave correctly on dimension changes
	virtual void resizeWidgetCB();						///< each sub class should behave correctly on dimension changes

	SoFieldSensor labelPlacingSensor;
	static void labelPlacingChangedCB(void *, SoSensor*);
	virtual void adaptToLabelPlacing();

	SoFieldSensor labelSensor;
	static void labelChangedCB(void *, SoSensor*);

	SoFieldSensor tooltipSensor;
	static void tooltipChangedCB(void *, SoSensor*);

	SoFieldSensor tooltipDisabledSensor;
	static void tooltipDisabledChangedCB(void *, SoSensor*);


	SoNode *oldLabelPtr;
	/**
	 * the gesture inspector, a helper component,
	 * that provides means to comfortably handle low
	 * level events
	 */
	StbEventInspector *inspector;

    /**
     * a list of SoSeparators or SoBehaviorKits to be traversed, before
     * this SoBehaviorKit is traversed. 'this' is only traversed, if no
     * node in the receiver list has handled the event. Take a look into
     * the handle3DEventS() method for a deeper understanding
     */
    SoChildList *eventReceiverList;

	/**
	sets the translation for the label (is overridden by the SoButtonKit to adapt
	to the on/off geometry
	*/
	void setLabel2Translation(float &moveLabel2_1);

private:

	/**
     * This static methods is registered with the database - it
     * simply calls the appropriate virtual function
	 */
	static void handle3DEventS(SoAction* action, SoNode* node);
	

};

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#endif //_SO_BEHAVIOR_KIT_

