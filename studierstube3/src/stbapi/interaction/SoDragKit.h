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
/**
 * header of SoDragKit
 *
 * @author Dieter Schmalstieg, Gerhard Reitmayr, Jan Prikryl, Hannes Kaufmann,Reinhard Steiner
 *
 * $Id: SoDragKit.h 4089 2004-12-13 16:42:12Z reitmayr $
 * @file                                                                   */
/* ======================================================================= */

#include <vector>
// Use STL

#ifndef _SODRAGKIT_H_
#define _SODRAGKIT_H_

#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <stbapi/event3d/Base3D.h>
#include <stbapi/workspace/StbWorkspace.h>
#include <Inventor/sensors/SoAlarmSensor.h>
#include <Inventor/sensors/SoFieldSensor.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class STBAPI_API SoDragKit;



typedef void SoDragKitCB(void* userData, SoDragKit* dragKit);

//----------------------------------------------------------------------------

/**
* 3d drag kit allows to drag and drop objects in 3D. Documented at
* http://www.studierstube.org/stbapi/SoDragKit.html
*
* @bug An SoLOD inside an SoDragKit crashes the application when 3D events are fed into the SoDragKit.
*
* @ingroup interaction
*/
class STBAPI_API SoDragKit
:
public SoBaseKit, public Base3D 
{
  SO_KIT_HEADER(SoDragKit);
  BASE3D_HEADER;
  
  SO_KIT_CATALOG_ENTRY_HEADER(base);            //(hidden) root separator
  SO_KIT_CATALOG_ENTRY_HEADER(material);
  SO_KIT_CATALOG_ENTRY_HEADER(transform);       //SoTransform - used for dragging
  SO_KIT_CATALOG_ENTRY_HEADER(highlightSwitch); //SoSwitch - used for switching between normal and highlighted geometry
  
  SO_KIT_CATALOG_ENTRY_HEADER(content);  //SoSeparator - add your content here
  
  SO_KIT_CATALOG_ENTRY_HEADER(highlightedContent);  //SoSeparator - add contend for highlighted display here
  
  
public:
  SoSFVec3f translation;              //current translation, local coords
  SoSFRotation rotation;              //current rotation,    local coords
  
  SoSFBool translationOn;             //turn whole translation on/off
  SoSFBool rotationOn;                //turn whole rotation on/off
  
  SoSFBool xTranslationOn;            //x component can be modified
  SoSFBool yTranslationOn;            //y component can be modified
  SoSFBool zTranslationOn;            //z component can be modified
  SoSFBool xRotationOn;               //x axis rotation possible
  SoSFBool yRotationOn;               //y axis rotation possible
  SoSFBool zRotationOn;               //z axis rotation possible
  SoSFInt32 dragButton;               //button to drag with
  SoSFBool dragging;                  //true if in dragging mode
  SoSFInt32 draggingStation;          // if being dragged, set to the dragging station number, if not, set to -1
  SbBool validFlag;                   //TRUE if currentSet values are valid
  
  static void initClass(              //static class initialization
    );
  SoDragKit(                          //constructor
    );
  ~SoDragKit(                         //destructor
    );
  virtual SbBool isInterested(                //true if node interested in 3d event
    SoHandle3DEventAction* h3a      //action owning 3d event
    );
  virtual void handle3DEvent(                 //deal with 3d event
    SoHandle3DEventAction* h3a      //action owning 3d event
    );

  void setPressCallback(
    SoDragKitCB* cb,
    void* userData = NULL
    );
  void setReleaseCallback(
    SoDragKitCB* cb,
    void* userData = NULL
    );
  void setMoveCallback(
    SoDragKitCB* cb,
    void* userData = NULL
    );

  void removePressCallback(
    SoDragKitCB* cb,
    void* userData = NULL
    );
  void removeReleaseCallback(
    SoDragKitCB* cb,
    void* userData = NULL
    );
  void removeMoveCallback(
    SoDragKitCB* cb,
    void* userData = NULL
    );

  void triggerPressCallback(
    );
  void triggerReleaseCallback(
    );
  void triggerMoveCallback(
    );

  virtual void OnPress() {};    // overload this method to sense press
  virtual void OnRelease() {};  // overload this method to sense release
  virtual void OnMove() {};     // overload this method to sense move

  void updateFromNetwork( //set translation and rotation via network
    SbVec3f trans,
    SbRotation rot
    );
  static void snapBack(void *data,
    SoSensor *sensor
    );
  static void externalUpdate(void *data,
    SoSensor *sensor
    );
  
protected:
  
  /** This will create field connections with internal nodes.
   * It is called at the end of the constructor (to attach).
   * and at the start/end of SoBaseKit::readInstance()
   * and on the new copy at the start/end of SoBaseKit::copy().
   * Returns the state of the node when this was called.
   */
  virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

  SbVec3f trans0L;                    //original kit translation, local
  SbRotation rot0L;                   //original kit rotation,    local
  SbVec3f startTransG;                //original pen translation, global
  SbRotation startRotG;               //original pen rotation,    global
  SbMatrix globalToLocal;             //global to local transformation
  SbVec3f pickOffsetL;                //offset to pen-kit origin, local
  
  SoAlarmSensor *snapBackSensor;
  
private:
  
  void update(
    SbVec3f transIn,
    SbRotation rotIn,
    SbVec3f& transOut,
    SbRotation& rotOut
    );
  void writeBack(                     //write back to fields
    SbVec3f trans,                  //modified translation
    SbRotation rot                  //modified rotation
    );

  SbBool highlighted;                 //true if geometry highlighted
  SbBool allowExternalUpdate;         //true if transform values may be updated
  
  SbVec3f currentSetTranslation;      //current valid translation (from network)
  SbRotation currentSetRotation;      //current valid rotation (from network)
  

  std::vector<SoDragKitCB*> pressCallback;
  std::vector<SoDragKitCB*> releaseCallback;
  std::vector<SoDragKitCB*> moveCallback;
  std::vector<void*> pressCallbackData;
  std::vector<void*> releaseCallbackData;
  std::vector<void*> moveCallbackData;

  /* old version, changed by RS
  SoDragKitCB* pressCallback;
  SoDragKitCB* releaseCallback;
  SoDragKitCB* moveCallback;
  void* pressCallbackData;
  void* releaseCallbackData;
  void* moveCallbackData;
  */
  
  SoFieldSensor *translationSensor;
  SoFieldSensor *rotationSensor;
}; //class SoDragKit

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SODRAGKIT_H_

