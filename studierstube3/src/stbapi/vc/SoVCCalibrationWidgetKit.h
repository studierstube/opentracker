/** SoVCCalibrationWidgetKit.h contains the definition for class SoVCCalibrationWidgetKit
 *
 * @author  Alexander Terbu
 *
 * @file
 */

#ifndef _SO_VCCALIBRATIONWIDGETKIT_
#define _SO_VCCALIBRATIONWIDGETKIT_

#include <stbapi/vc/SoVCPushWidgetKit.h>
#include <stbapi/event3d/SoHandle3DEventAction.h>
#include <Inventor/SoPath.h>

/** SoVCCalibrationWidgetKit is used for calibration of the VC interface
 *
 * It is an extended SoVCPushWidgetKit, to allow proper handling of the
 * pen translation used for the calibration process
 *
 * @author Alexander Terbu
 */

class STBAPI_API SoVCCalibrationWidgetKit : public SoVCPushWidgetKit
{
  SO_BEHAVIOR_HEADER(SoVCCalibrationWidgetKit);

 public:

  /** Inventor Class initialization */
  static void initClass();
  
  /** The constructor inits all fields */
  SoVCCalibrationWidgetKit();

  /** The destructor does nothing */
  ~SoVCCalibrationWidgetKit();
  
  /** The widget's state-machine */
  void handle3DEvent(SoHandle3DEventAction *h3a);

 protected:

  /** Handling of the actual calibration process */
  void performCalibration(SoHandle3DEventAction *h3a);

};

#endif // _SO_VCCALIBRATIONWIDGETKIT_

