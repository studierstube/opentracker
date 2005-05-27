/** SoVCToggleWidgetKit.h contains the definition for class SoVCToggleWidgetKit
 *
 * @author  Alexander Terbu
 *
 * @file
 */
#ifndef _SO_VCTOGGLEWIDGETKIT_
#define _SO_VCTOGGLEWIDGETKIT_

#include <stbapi/vc/SoVCWidgetKit.h>

/** SoVCToggleWidgetKit extends a SoVCWidgetKit with additional
 * functionality to allow a toggle-button behaviour
 *
 * @author Alexander Terbu
 */
class STBAPI_API SoVCToggleWidgetKit : public SoVCWidgetKit
{
  SO_BEHAVIOR_HEADER(SoVCToggleWidgetKit);

 public:

  /** Inventor class initialization  */
  static void initClass();
  
  /** The constructor inits all fields */
  SoVCToggleWidgetKit();

  /** The destructor does nothing */
  ~SoVCToggleWidgetKit();
  
  /** The widget's state-machine  */
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

  /** Returns whether the widget is currently in on-state or off-state
   *
   * @return true, if on
   */
  SbBool getToggleState();
  
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

#endif // _SO_VCTOGGLEWIDGETKIT_

