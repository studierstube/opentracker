/** SoVCWidgetKit.h contains the definition for class SoVCWidgetKit
 *
 * @author  Alexander Terbu
 *
 * @file
 */
#ifndef _SOVCWIDGETKIT_
#define _SOVCWIDGETKIT_

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/sensors/SoSensor.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodekits/SoNodeKitListPart.h>

#include <stbapi/interaction/SoBehaviorKit.h>

class SoVCWidgetKit;  

typedef void SoVCWidgetKitCB(void* userData, SoVCWidgetKit* widgetKit);


/** SoVCWidgetKit is the base class for all widgets used in the VC.
 *
 * It extends the SoBehaviorKit with functionality to add
 * a content, which is displayed inside the widget and can be 
 * autoscaled. Furthermore it provides a hierarchycal widget 
 * structure to allow for build up of efficient menus.
 *
 * @author Alexander Terbu
 */
class STBAPI_API SoVCWidgetKit : public SoBehaviorKit
{
  SO_BEHAVIOR_HEADER(SoVCWidgetKit);
  
  /** SoSeparator - add normal on geometry here */
  SO_BEHAVIOR_CATALOG_ENTRY_HEADER(onGeometry);

  /** SoSeparator - add normal off geometry here */
  SO_BEHAVIOR_CATALOG_ENTRY_HEADER(offGeometry);

  /** SoSeparator - add highlighted on geometry here */
  SO_BEHAVIOR_CATALOG_ENTRY_HEADER(highlightedOnGeometry);

  /** SoSeparator - add highlighted off geometry here */
  SO_BEHAVIOR_CATALOG_ENTRY_HEADER(highlightedOffGeometry);

  /** SoSwitch - add childWidgets here */
  SO_BEHAVIOR_CATALOG_ENTRY_HEADER(childWidgets);
  
  /** SoSwitch - add widget's content here */
  SO_BEHAVIOR_CATALOG_ENTRY_HEADER(widgetContent); 

  SO_BEHAVIOR_CATALOG_ENTRY_HEADER(widgetTransform);       

  SO_BEHAVIOR_CATALOG_ENTRY_HEADER(invisibleDraw);       
    
 public:

  /** 'highlighted' and 'on' describe the widget's state */
  SoSFBool highlighted;
  SoSFBool on;

  /** Describes whether the widget should be displayed without geometry */
  SoSFBool noGeometry;
  
  /** If the user enters the buttons volume, it becomes highlighted.
   * Depending on its state before highlighting, it is either
   * HIGHLIGHTED_ON or HIGHLIGHTED_OFF
   */
  enum GeometryState
    {
      ON = 0,
      OFF,
      HIGHLIGHTED_ON,
      HIGHLIGHTED_OFF
    };

  /** Inventor class initialization  */
  static void initClass();

  /** The constructor inits all fields */
  SoVCWidgetKit();

  /** The destructor does nothing  */
  ~SoVCWidgetKit();
  
  /** The widget's state-machine */
  void handle3DEvent(SoHandle3DEventAction* h3a);
  
  /** Switches the widget's state to 'off' */
  void turnOff();

  bool removePressCallback(SoVCWidgetKitCB* cb);
  bool removeReleaseCallback(SoVCWidgetKitCB* cb);
  void addPressCallback(SoVCWidgetKitCB* cb,void* userData = NULL);
  void addReleaseCallback(SoVCWidgetKitCB* cb,void* userData = NULL);

  /** Sets the content scenegraph of the widget
   * 
   * @param content the scenegraph to be displayed inside
   */
  void setContent(SoSeparator* content);

  /** Sets an SoText3 as content
   *
   * @param description the text to display
   * @param textColor the color of the text (default=white)
   */
  void setContent(const char* description, SbVec3f textColor = SbVec3f(1.0, 1.0, 1.0));

  /** Sets the widget's ID
   *
   * @param id the widget's ID
   */
  void setWidgetID(int id);

  /** Sets the widget's name
   *
   * It is used for status bar hierarchy-level display
   *
   * @param name the widget's name
   */
  void setWidgetName(const char* name);

  /** Sets the widget's translation
   *
   * @param translation the new translation
   */
  void setTranslation(SbVec3f translation);

  /** Returns the widget's transform
   *
   * @return the widget's transform
   */
  SoTransform* getTransform();

  /** Returns the widget's content scenegraph
   *
   * @return SoSwitch with content scenegraph
   */
  SoSwitch* getContent();

  /** Returns the widget's name
   *
   * @return the widget's name
   */
  SoSFString getWidgetName();

  /** Sets the widget's animation state
   *
   * If set to TRUE, the widget is ignoring 3D-events to
   * avoid animation interruption, due to user interaction 
   *
   * @param on TRUE: ignore 3D-events, FALSE: accept 3D-events
   */
  void setAnimationState(bool on);

  /** Returns the widget's ID
   *
   * @return the widget's ID
   */
  int getWidgetID();

  /**  Adapt the widget's geometry to the widget's state
   * described by the 'highlighted' and 'on' fields
   */
  void adaptGeometryToState();

  /** Returns the widget's children
   *
   * This should only be SoVCWidgetKit's.
   *
   * @return SoSwitch with widget's children
   */
  SoSwitch* getWidgetChildren();

  /** Sets the widget's parent widget
   *
   * @param parent the parent widget
   */
  void setWidgetParent(SoVCWidgetKit* parent);

  /** Returns the widget's parent
   *
   * @return the widget's parent
   */
  SoVCWidgetKit* getWidgetParent();

  /** Sets the visibility of the widget
   *
   * @param status TRUE: show, FALSE: hide
   */
  void setVisibility(bool status);
  
  /** Sets autoscaling for widget's content
   *
   * @param use TRUE: autoscale, FALSE: don't autoscale
   */
  void useAutoScale(bool use);

  /** Autoscales widget's content */
  virtual void autoScaleContent();

  /** Describes, if widget's content should be autoscaled */
  SoSFBool scaleContent;

  /** Describes, if widget should be hidden, if it
   * becomes selected and is the last one in hierarchy
   */
  SoSFBool hideWhenLeaf;

  /** Sets the active child in the childWidgets switch
   *
   * This is used to build the right rendering path,
   * accordingly to the widget hierarchy structure
   *
   * @param switchId the active child's ID
   */
  void setActiveWidgetChildren(int switchId);

  /** Describes, if the active child has changed */
  SoSFBool activeWidgetChildrenChanged;

  /** Changes the active child 
   *
   * This sets the childWidgets switch to the active child 
   */
  void changeActiveWidgetChildren();

  /** Returns whether the widget becomes hidden as a leaf
   *
   * Describes if a widget becoms hidden, if becomes selected
   * and is a leaf in the hierarchy
   *
   * @return TRUE: becomes hidden, FALSE: stays visible
   */
  bool isHideWhenLeaf();

  /** Sets the visibility of the childWidgets
   *
   * @param state TRUE: visible, FALSE: hidden
   */
  void showChildren(bool state);
  
  /** Sets the visibility of the widget's content
   *
   * @param state TRUE: visible, FALSE: hidden
   */
  void showContent(bool state);
  
  protected:

  /** The widget's ID */
  SoSFInt32  widgetID;

  /** The widget's name
   *
   * Is displayed in status bar for hierarchy-level information
   */
  SoSFString widgetName;

  /** Unique internal widget ID */
  int widgetUniqueID;

  /** ID of this widget's active child */
  int activeWidgetChildrenId;
  
  /** The widget's parent widget */
  SoVCWidgetKit* widgetParent;

  SoMFNode actionStartGestures;
  SoMFNode actionEndGestures;
  SoMFNode actionEnterGestures;
  SoMFNode actionExitGestures;
  SoMFNode actionMoveGestures;
  
  SoVCWidgetKitCB * pressCallback;
  SoVCWidgetKitCB * releaseCallback;

  void *pressCallbackData;
  void *releaseCallbackData;

  SoFieldSensor onSensor;
  SoFieldSensor hlSensor;
  SoFieldSensor noGeomSensor;
  SoFieldSensor activeWidgetChildrenSensor;

  SbPList* pressCBList;
  SbPList* releaseCBList;

  /** Describes, if widget's content should be autoscaled */
  bool rescale;

  /** Describes, if widget is currently in animation state */
  bool animating;

  static void stateChangedCB( void * userdata, SoSensor * sensor );

  static void sensorActiveWidgetChildrenCB( void * userdata, SoSensor * sensor);
};

#endif // _SOVCWIDGETKIT_

