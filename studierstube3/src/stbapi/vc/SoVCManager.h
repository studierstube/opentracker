/** SoVCManager.h contains the definition for class SoVCManager
 *
 * @author  Alexander Terbu
 *
 * @file
 */

#ifndef _SOVCMANAGER_H_
#define _SOVCMANAGER_H_

#include <stbapi/stbapi.h>
#include <stbapi/util/ivio.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/SbString.h>

class SoVCKit;
class SoUserKit;
class SoVCWidgetKit;

/** SoVCManager is used as a interface between Studierstube and
 * the VC, to encapsulate basic initialization and setup steps
 *
 * @author Alexander Terbu
 */

class STBAPI_API SoVCManager : public SoNode
{
SO_NODE_HEADER(SoVCManager);
 
 public:
 
 /** The constructor inits all members */
 SoVCManager();
 
 /** The destructor does nothing */
 ~SoVCManager();
 
public:

 /** Inventor class initialization */
 static void initClass();
 
 /** Setup of all necessary bars of the SoVCKit (i.e. systembar) */
 void setupBars();
 
 /** Sets the SoVCKit to be used
  *
  * @param vc the SoVCKit to be used
  */
 void setVC(SoVCKit* vc) { virtualCockpit = vc; }

 /** Sets the SoUserKit to be used
  *
  * @param uk the SoUserKit to be used
  */
 void setUserKit(SoUserKit* uk) { userKit = uk; }

 /** Calls the createResources method of the current SoVCKit
  *
  * @param appID the application ID
  */
 void createResources(SbAppID appID);
 
 /** Calls the useResources method of the current SoVCKit
  *
  * @param appID the application ID
  */
 void useResources(SbAppID appID);
 
 /** Calls the freeResources method of the current SoVCKit
  *
  * @param appID the application ID
  */
 void freeResources(SbAppID appID);
 
 /** Calls the setMainPanelGeometry method of the current SoVCKit
  *
  * @param appID the application ID
  * @param mpGeom the MP Geometry associated with the given appID
  */
 void setMainPanelGeometry(SbAppID appID, SoNode* mpGeom);

private:

 /** The current SoVCKit */
 SoVCKit* virtualCockpit;

 /** The current SoUserKit */
 SoUserKit* userKit;

 /** The original station, which was bound to the SoVCKit */
 int origStation;

 /** Indicates, if the SoVCKit's transformation matrix should be frozen */
 bool pinVC;

 static void systemBarCB(void *userData, SoVCWidgetKit *widgetKit); 
 static void windowBarCB(void *userData, SoVCWidgetKit *widgetKit);
 static void taskBarCB(void *userData, SoVCWidgetKit *widgetKit);
};

//----------------------------------------------------------------------------
#endif//_SOVCMANAGER_H_

