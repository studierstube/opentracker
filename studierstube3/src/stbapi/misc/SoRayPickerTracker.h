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
/** class definition SoRayPickerTracker
  *
  * SoRayPickerTracker implements a ray picking tool. It outputs the picked points data, it is a cross implementation for SoRayPicker which uses 3D Events
  * etc. 
  *
  * @author Thomas Psik tomp@ims.tuwien.ac.at
  *
  * $Id: SoRayPickerTracker.h 3525 2004-07-21 10:26:59Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SoRayPickerTracker_H_
#define _SoRayPickerTracker_H_

#include <stbapi/resource/SoTrackedArtifactKit.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFPath.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFVec4f.h>
#include <Inventor/fields/SoMFNode.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoMFBool.h>
#include <Inventor/fields/SoSFString.h>

#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/SoPath.h>
#include <Inventor/sensors/SoFieldSensor.h>

/** 
This class implements a unified RayPicker. It computes the first intersection of
a ray with the scene. For generating the ray, three different modes are available:

@li POSE mode takes the position and orientation information from the connected station,
and generates a ray along the z-Axis of the station's local coordinate system.

@li In POSITION mode, only the position data from the station is taken, and the ray is
calculated as having its origin in the eye-point of the users camera, going through the
stations position.

@li SCREEN mode interprets the x and y components of the stations position as coordinates in the
users viewport, and calculates the ray from the eye-point through the tracked point an the image
plane. This is useful for pointing to objects with the mouse or similar 2d input devices.
 
 * The POSITION mode needs an eyepoint as start of the ray. This is either the users camera
 * position, if it is associated with a camera or a fixed point defined in the field refPoint.
 * The userid is takenfrom the input station, or if this does not exist, the value of the field userId. 
 *
 * The SCREEN mode works only if the ray picker is associated with a 
 * user that specifies a camera. This is either by taking the user id from the 
 * input station, or if this does not exist, the value of the field userId. 
 * The input from the station is assumed to 
 * be mouse coordinates as provided by the StbMouseSource. The coordinates field 
 * selects between normalized and absolute coordinates as in the StbMouseSource. 

 * The basic behaviour is to use the tracking input from station station and generate 
 * a raypick using the given position and orientation. This raypick is visualized by 
 * a line that extends from the current position to the picked point or to infinity, 
 * if there is no hit. The visualization of the line can be specified in the geometry 
 * part. The RayPicker will transform the geometry by scaling it along the X axis and 
 * positioning it so that it stretches between the input point and the picked point. 
 * It also outputs the found node via the pickedPath field and the found position via 
 * pickedPoint. The orientation of the ray given by either the interaction device or 
 * the up direction of the camera is output in pickOrientation. The button state of the 
 * input station is reflected by pickButton. There will be no output rayStation station. 
 * This is done by the @ref StbSource OpenTracker node the takes output from Studierstube nodes 
 * as input. In addition to that, the fields pickedNormal and pickedTextureCoord just 
 * output information retrieved from the SoRayPickAction (see there).
 *
 * If a button is pressed it operates in one of two modes selected by the mode field: 
 * RAY or STICK. In RAY mode it behaves in the same manner, if a button is pressed or not, 
 * that is it always casts rays for updates to the input station. In STICK mode it 
 * behaves like a fixed stick once any button is pressed, that is the current 
 * length is fixed and a new position and orientation is computed from the fixed
 * length of the stick and posted via the rayStation.
 *
 * @note STICK is not implemented for POSITION and SCREEN mode.
 *
 * The ray pick action also needs a scenegraph to operate on. This can either be
 * set explicitly by using the field pickGraph or implicitly by using the head
 * of the path passed via the event traversal. We decided to use the geometry 
 * root in a fixed manner. To access the geometry root node use the method 
 * ref StbWorkspace::getGeometryRoot().
 *
 * The fields radius, nearDistance and farDistance control the respective 
 * parameters of a SoRayPickAction. radius defines the size the ray in pixels. 
 * nearDistance and farDistance provide a clipping along the ray.
 *
File format syntax / defaults:

@verbatim
SoRayPickerTracker {
    # general parameters
    SoSFBool  active      FALSE
    SoSFInt32 stations    -1
    SoSFNode  pickGraph   NULL
 
    # pick parameters
    SoSFFloat radius     1.0
    SoSFFloat nearDistance -1.0
    SoSFFloat farDistance  -1.0
    SoSFVec3f refPoint     0 0 0
    SoSFInt32 userId     -1
 
    #modes
    SoSFEnum  mode       RAY # | STICK
    SoSFEnum  coordinates NORMALIZED # | ABSOLUTE
    SoSFEnum  input      POSE # | POSITION | SCREEN

    content Group {
        DrawStyle {}
        Material {}
        Cylinder ()
    }
 
    # outputs
    SoSFPath  pickedPath           NULL
    SoSFVec3f pickedPoint          0 0 0
    SoSFRotation pickedOrientation 0 0 1 0
    SoSFVec3f pickedNormal     0 0 0
    SoSFVec4f pickedTextureCoord   0 0 0 0
    SoSFInt32 button            0
    SoSFBool  isPicking   FALSE
}
@endverbatim
 * @author Peter Alexay, ledermann@ims.tuwien.ac.at
 *
 * @ingroup interaction
 */
class STBAPI_API SoRayPickerTracker : public SoTrackedArtifactKit  
{
	SO_KIT_HEADER(SoRayPickerTracker);
	
public:
	/// static class initialization
	static void initClass();
	
	SoRayPickerTracker();
	
	virtual ~SoRayPickerTracker();
	
	enum Mode
	{
		RAY,
		STICK
	};
	
	enum Coordinate
    {
		NORMALIZED,
		ABSOLUT
	};
	
	enum Input
	{
		POSE,
		POSITION,
		SCREEN
	};
    
	// general parameters
    /// switches the RayPicker on/off.
	SoSFBool active;
    /// the geometry the raypicker operates on. If NULL, the whole scenegraph is searched (can be sloooow).
	SoSFNode pickGraph;
	// note: 'stations' and 'content' fields are inherited from the parent class
	
	// pick parameters
    ///sets the reference Point for POSITION and SCREEN modes.
    SoSFVec3f refPoint;
    /// controls radius of SoRayPickAction
	SoSFFloat radius;
    /// controls nearDistance of SoRayPickAction
	SoSFFloat nearDistance;
    /// controls farDistance of SoRayPickAction
	SoSFFloat farDistance;
    /// sets a default user id to define the camera, if it is not specified by the station
    SoSFInt32 userId;
	
	// modes
    /// RAY or STICK mode (see description)
	SoSFEnum mode;
    /// mode for interpreting coordinates: NORMALIZED or ABSOLUTE, works like StbMouseSource in OpenTracker.
	SoSFEnum coordinates;
    /// input mode: POSE, POSITION or SCREEN (see introduction)
	SoSFEnum input;
	
    /// outputs the path to the currently selected node.
	SoSFPath     pickedPath;
    /** All the nodes on the picked path. This is a workaround, because of a bug in inventor that prohibits
        fieldconnections on SoSFPath fields, we have to convert the path internally and export it through
        a SoMFNode field.*/
    SoMFNode    pickedNodes;

	/// node that has been picked can be filtered with pickParameters
	SoSFNode    pickedLastNode;

	/// returns the last node that is a separator
    SoSFBool pickOnlySeparators;

	/// returns the last node that has a def name
	SoSFBool pickLastNamedNode;

		

    /// outputs the coordinates of the intersection of the ray with the object
	SoSFVec3f    pickedPoint;
    /// outputs the orientation of the ray itself
	SoSFRotation pickedOrientation; 
    /// outputs the normal of the picked object at the pickedPoint
	SoSFVec3f    pickedNormal;
    /// outputs the texture coordinate of the picked object at the pickedPoint
	SoSFVec4f    pickedTextureCoord;
    
	/// state of the stations buttons
	SoSFInt32    button;

	SoMFBool     buttonFlags;
    /** whether there was a pick with the last event or not, will also be set to 
     * false if active is false.
     */
    SoSFBool     isPicking;

	/** the current position of the SoRayPickerTracker.
	*/
    SoSFVec3f position;
    
	/** the current orientation of the SoRayPickerTracker. 
	*/
    SoSFRotation orientation;
	
protected:

      /** This will detach/attach the fieldSensor.
     * It is called at the end of the constructor (to attach).
     * and at the start/end of SoBaseKit::readInstance()
     * and on the new copy at the start/end of SoBaseKit::copy().
     * Returns the state of the node when this was called.
     */
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

	SoFieldSensor activeSensor;
    
	SoSeparator *getContent();
	SoTransform *getTransform();
	float performRayPickAction();
	void scaleRayLength(float neededSize);

	/** applies the rayPickAction with the new data
	*/
	virtual void movedCB();

	/** reads the buttonstates from the TrakEngine and combines them in an int.
	*/
	int convertButtonStateToInt();

	//	static void eventCB( void * data, int station, SoStationKit * kit, SoHandle3DEventAction * h3a );
    static void fieldChangedCB( void * data, SoSensor * which);

    void setPickedPath(SoPath * path);
};

#endif // _SoRayPickerTracker_H_
