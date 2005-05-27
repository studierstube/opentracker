/** SoVCKit.h contains the definition for class SoVCKit
 *
 * @author  Alexander Terbu
 *
 * @file
 */

#ifndef _SOVCKIT_H_
#define _SOVCKIT_H_

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFVec2f.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <stbapi/workspace/SoContextManagerKit.h>
#include <stbapi/resource/SoTrackedArtifactKit.h>

class SoVCMainPanelKit;
class SoVCBarKit;

/** SoVCKit is a nodekit representing a cockpit used for interaction in the
 * studierstube environment.
 *
 * This nodekit inherits from SoTrackedArtifact and adds functionality for handling
 * multiple main panels and bars.
 *
 * @author Alexander Terbu
 */

class STBAPI_API SoVCKit
:
    public SoTrackedArtifactKit
{
    SO_KIT_HEADER(SoVCKit);   
    SO_KIT_CATALOG_ENTRY_HEADER(transform);    
    SO_KIT_CATALOG_ENTRY_HEADER(systemBar);  
    SO_KIT_CATALOG_ENTRY_HEADER(taskBar);    
    SO_KIT_CATALOG_ENTRY_HEADER(windowBar);  
    SO_KIT_CATALOG_ENTRY_HEADER(mainPanels); 

public:
    
    /** deprecated */
    SoSFFloat distFromEye;

    /** This field specifys the size of the VC that should be used.
     *
     * The VC's geometry has to be placed around the origin (in object space).
     */
    SoSFVec3f vcSize;

    /** This field specifys the upper left corner of the effective VC area.
     *
     * It is specified in normalized VC-coordinates, ranging from (0,0) for
     * the upper left corner of the VC to (1,1) for the lower right corner
     * of the VC.	
     */  
    SoSFVec2f vcAreaUpperLeft;


    /** This field specifys the lower right corner of the effective VC area.
     *
     * It is specified in normalized VC-coordinates, ranging from (0,0) for
     * the upper left corner of the VC to (1,1) for the lower right corner
     * of the VC.	
     */  
    SoSFVec2f vcAreaLowerRight;

    /** Specifys the name of a skin-file, that describes the properties
     * of the user-interface elements on the VC. 
     */
    SoSFString skinFileName;

    /** Specifys the directory of a skin-file, that describes the properties
     * of the user-interface elements on the VC. 	
     */
    SoSFString skinDirectory;
    
    /** The constructor inits all fields and enables all 3D events. */
    SoVCKit();
    
    /** The destructor releases all grabbers of the VC */
    ~SoVCKit();
    
    /** Inventor class intialization */
    static void initClass();

    /** This method is used to create an initial VC-main-panel for an application
     * (and context).
     *
     * Before a VC-main-panel can be set or used for an application (and context),
     * this method has to be called. It works like a kind of registration,
     * which stores basic information using an empty VC-main-panel.
     * For non-workspace applications using only one VC-main-panel, just use an
     * arbitrary applicationID and don't specify a context.
     *
     * @param appID the application ID
     */
    void createResources(SbAppID appID);
    
    /** This method is used to free (delete) a VC-main-panel of an application
     * (and context).
     *    
     * The current VC-main-panel of this application (context) will be
     * removed from the VC.
     *
     * @warning If only an appID is specified, all contexts, which are
     *          associated with this application will also be deleted.
     *
     * @param appID the application ID
     */
    void freeResources(SbAppID appID);
    
    /** This method is used to switch to a VC-main-panel of an application
     * (and context).
     *    
     * The VC-main-panel of the specified application (context) will be
     * displayed on the VC.
     * @param appID the application ID
     */
    void useResources(SbAppID appID);

    /** Set the VC-geometry (the VC-main-panel) for the specified application 
     * (context).
     *
     * If vcGeom is not a SoVCMainPanelKit, initialization is aborted.
     *
     * @param appID the application ID
     * @param mpGeom the root-node of the VC-main-panel,
     *        should be of type SoVCMainPanelKit
     */
    void setMainPanelGeometry(SbAppID appID, SoNode* mpGeom);

    /** Get the VC-geometry (the VC-main-panel) of the specified application 
     * (context).
     *
     *  @param appID the application ID
     *  @return a SoVCMainPanelKit associated with the application (context)
     */
    SoVCMainPanelKit *getMainPanelKit(SbAppID appID);    

    /** Set the visibility of the VC-parts for the currently used 
     * VC-main-panel.
     *
     * @param vcpm a bit mask specifying the visible VC parts
     */    
    void setVCPartVisibility(unsigned int vcpm);

    /** Set the visibility of the VC-main-panel parts for the currently used 
     * VC-main-panel.
     *
     * @param mppm a bit mask specifying the visible VC-main-panel parts
     *
     * \note Use this method only for changing the visibility
     *  of the VC-main-panel, when the SoVCMainPanelKit has already been added to the VC,
     *  otherwise just set the mpParts field.
     */    
    void setMPPartVisibility(unsigned int mppm);

    /** Force refresh of VC content.
     *
     * The current VC-main-panel will be rescaled and the vcPart (and mpPart)-visibility
     * evaluated.
     */    
    void updateVC();

    /** Sets the VC system bar */
    void setSystemBar(SoVCBarKit *systemBar);

    /** Sets the VC task bar */
    void setTaskBar(SoVCBarKit *taskBar);

    /** Sets the VC window bar */
    void setWindowBar(SoVCBarKit *windowBar);

    /** Returns the VC window bar
     *
     * @return barKit used for window bar
     */
    SoVCBarKit* getWindowBar();

    /** Returns the VC system bar
     *
     * @return barKit used for sytem bar
     */ 
    SoVCBarKit* getSystemBar();
 
    /** Returns the VC task bar
     *
     * @return barKit used for task bar
     */
    SoVCBarKit* getTaskBar();
    
    /** Used to freeze the VC current transformation matrix
     * 
     * @param state true, if it should freeze
     */
    void pinMe(bool state);

    /** Returns whether the VC's transformation is currently frozen
     *
     * @return true, if frozen
     */
    bool isPinned();

    /** Sets up the connections to the tracker engine after
     * freeze of transformation matrix
     */
    void setUpVCConnections(bool onoff, bool always);

    /** Sets the VC current transformation matrix
     *
     * @param newMover new transformation matrix
     */
    void setMover(SoTransform* newMover);

    /** This field determines the visible parts of the VC */
    SoSFBitMask vcParts;          

    /** Set the visibility of the VC bars
     *
     * @param system TRUE: show systemBar, FALSE: hide systemBar
     * @param window TRUE: show windowBar, FALSE: hide windowBar
     * @param task TRUE: show taskBar, FALSE: hide taskBar
     */
    void showVCBars(bool system, bool window, bool task);

    /** Used to minimize/maximize the VC
     *
     * @param state TRUE: minimize, FALSE: maximize
     */
    void setActionMinMax(bool state);

    /** Describes the parts of the VC */
    enum VCPartMask                                        
      { 
	/** SystemBar */
	VC_SYSTEMBAR = 0x0001,        
	/** WindowBar */
	VC_WINDOWBAR = 0x0002,
	/** MainPanel */
	VC_MAINPANEL = 0x0008,
	/** TaskBar */
	VC_TASKBAR = 0x00010,      
	/** All parts */
	VC_FULL = 0x01F      
      };              

    /** Indicates whether the VC's transformation matrix is frozen */
    SoSFBool pinned;       


protected:

    /** Indicates whether the VC is currently minimized */
    bool actionMinMax;

    /** Get a SoNodeKitListPart containing all registered VC-main-panels
     *
     * @return a SoNodeKitListPart containing all registered PIP-sheets.
     */    
    SoNodeKitListPart *getMainPanels();

    /** Stores a pointer to the currently visible VC-main-panel */
    SoVCMainPanelKit *currentMainPanelKit;  

    /** This method merges areas of the VC.
     *
     * It merges the vcArea of the given SoVCKit and
     * the vcArea definded by resultAreaUL and resultAreaLR.
     * The resulting vcArea will be written back to resultAreaUL 
     * and resultAreaLR.

     * @param mergeArea A SoVCKit.
     * @param resultAreaUL upper-left corner of vcArea.
     * @param resultAreaLR lower-right corner of vcArea.
     */
    void mergeVCAreas(SoVCKit *mergeArea, SbVec2f &resultAreaUL, SbVec2f &resultAreaLR);

    friend class SoVCMainPanelKit;
};

//----------------------------------------------------------------------------
#endif//_SOVCKIT_H_

