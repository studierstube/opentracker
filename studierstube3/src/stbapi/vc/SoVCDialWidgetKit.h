/** SoVCDialWidgetKit.h contains the definition for class SoVCDialWidgetKit
 *
 * @author  Alexander Terbu
 *
 * @file
 */

#ifndef _SO_VCDIALWIDGETKIT_
#define _SO_VCDIALWIDGETKIT_

#include <Inventor/engines/SoInterpolate.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <stbapi/vc/SoVCDraggerKit.h>

/** SoVCDialWidgetKit provides functionality similar to a 2D dial
 *
 * It extends the SoVCDraggerKit by displaying the current, min and max value
 * and provides geometry to simulate dial behaviour
 *
 * @author Alexander Terbu
 */

class SoVCDialWidgetKit : public SoVCDraggerKit
{
  SO_BEHAVIOR_HEADER(SoVCDialWidgetKit);

  SO_BEHAVIOR_CATALOG_ENTRY_HEADER(dialValueContent); 

 public:
  
  /** The dial's current value */
  SoSFFloat value;			
  
  /** The dial's minimum value */
  SoSFFloat minValue;		

  /** The dial's maximum value */
  SoSFFloat maxValue;			

  /** The dial's start angle */
  SoSFFloat startAngle;

  /** The dial's end angle (used to specify the dial's "resolution") */
  SoSFFloat endAngle;
  
  /** The dial's rotation axis */
  SoSFVec3f axis;

 public:

  /** Inventor class initialization */
  static void initClass();
  
  /** The constructor inits all fields */
  SoVCDialWidgetKit();

  /** The destructor does nothing */
  ~SoVCDialWidgetKit();
  
  /** The widget's state-machine */
  void handle3DEvent(SoHandle3DEventAction *h3a);

  /** Returns whether the event occured inside the widgets bounding box 
   *
   * @return true, if inside
   */
  SbBool isInside(const SoHandle3DEventAction *h3a);

  /** Returns whether the widget is interested in the current event
   *
   * @return true, if interested
   */
  SbBool isInterested();

  /** Returns the dial's current value
   *
   * @return current value
   */
  float getValue();

  /** Returns the dial's current angle
   *
   * @return current angle in radiant
   */
  float getAngle();

  /** Auto scales the widget's content */
  virtual void autoScaleContent();

  bool removeValueCallback(SoVCWidgetKitCB* cb);
  void addValueCallback(SoVCWidgetKitCB* cb,void* userData = NULL);

 protected:

  /** Contains the widget's geometry if in on-state */
  static char* widgetOnString;

  /** Contains the widget's geometry if in off-state */
  static char* widgetOffString;

  /** Contains the widget's geometry if in highlighted-on-state */
  static char* widgetHighlightedOnString;

  /** Contains the widget's geometry if in highlighted-off-state */
  static char* widgetHighlightedOffString;
  
  /** Is used to interpolate between the dial's max value and min value
   *
   * The alpha output of the SoConstrainToLine engine is used
   */
  SoInterpolateFloat *interpolator;
  
  /** Is used to fire the callback on value changes */  
  SoFieldSensor *valueChangedSensor;
  
  /** The dial's current angle */
  float currentAngle;
  
  /** Used for content caching on updates, if value changes */
  SoNode* widgetContentSave;

  SoVCWidgetKitCB * valueCallback;
  void *valueCallbackData;
  SbPList* valueCBList;
 
    
 protected:

  /** Returns the SoVCDraggerKit's current transform
   *
   * @return draggers current SoTransform node
   */
  SoTransform* getTransform();

  /** Auto scales the dial's value-content */
  void autoScaleValueContent();

};

#endif // _SO_VCDIALWIDGETKIT_

