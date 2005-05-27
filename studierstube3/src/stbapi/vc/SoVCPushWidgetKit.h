/** SoVCPushWidgetKit.h contains the definition for class SoVCPushWidgetKit
 *
 * @author  Alexander Terbu
 *
 * @file
 */

#ifndef _SO_VCPUSHWIDGETKIT_
#define _SO_VCPUSHWIDGETKIT_

#include <stbapi/vc/SoVCWidgetKit.h>

/** SoVCPushWidgetKit extends a SoVCWidgetKit with additional
 * functionality to allow a push-button behaviour
 *
 * @author Alexander Terbu
 */

class STBAPI_API SoVCPushWidgetKit : public SoVCWidgetKit
{
  SO_BEHAVIOR_HEADER(SoVCPushWidgetKit);

 public:

  /** Inventor class initialization */
  static void initClass();
  
  /** The constructor inits all fields */
  SoVCPushWidgetKit();

  /** The destructor does nothing */
  ~SoVCPushWidgetKit();
  
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

  /** Auto scales the widget's content */  
  virtual void autoScaleContent();

 protected:

  /** Contains the widget's geometry if in on-state */
  static char* widgetOnString;

  /** Contains the widget's geometry if in off-state */
  static char* widgetOffString;

  /** Contains the widget's geometry if in highlighted-on-state */
  static char* widgetHighlightedOnString;

  /** Contains the widget's geometry if in highlighted-off-state */
  static char* widgetHighlightedOffString;

};

#endif // _SO_VCPUSHWIDGETKIT_

