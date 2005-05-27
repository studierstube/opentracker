/** SoVCMainPanelKit.h contains the definition for class SoVCMainPanelKit
 *
 * @author  Alexander Terbu
 *
 * @file
 */

#ifndef _SOVCMAINPANELKIT_H_
#define _SOVCMAINPANELKIT_H_

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoMFNode.h>
#include <Inventor/fields/SoSFVec2f.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTransform.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/workspace/SoContextManagerKit.h>
#include <stbapi/vc/SoVCWidgetKit.h>

//----------------------------------------------------------------------------
class SoVCKit;
class SoVCBarKit;
class SoVCSubPanelKit;

/** SoVCMainPanelKit is a nodekit representing a collection of SubPanels.
 *
 * A SoVCMainPanelKit manages a number of SubPanels. The user can switch between
 * these panels by simply selecting a widget of the desired panel.
 *
 * @author Alexander Terbu
 */
class STBAPI_API SoVCMainPanelKit
:
    public SoBaseKit
{
    SO_KIT_HEADER(SoVCMainPanelKit);
    SO_KIT_CATALOG_ENTRY_HEADER(trayBar);
    SO_KIT_CATALOG_ENTRY_HEADER(quickBar);
    SO_KIT_CATALOG_ENTRY_HEADER(subPanels);
    SO_KIT_CATALOG_ENTRY_HEADER(partSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(barSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(subPanelSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(transform);
    SO_KIT_CATALOG_ENTRY_HEADER(mainPanelRoot);
    SO_KIT_CATALOG_ENTRY_HEADER(subPanelRoot);
    SO_KIT_CATALOG_ENTRY_HEADER(silkPanelRoot);
    SO_KIT_CATALOG_ENTRY_HEADER(silkPanelTransf);
    SO_KIT_CATALOG_ENTRY_HEADER(silkPanelSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(silkPanelGeom);
    SO_KIT_CATALOG_ENTRY_HEADER(silkPanelMat);
    SO_KIT_CATALOG_ENTRY_HEADER(silkPanelTransp);
    SO_KIT_CATALOG_ENTRY_HEADER(statusBar);

public:

    /** This field specifys the size of the MP that should be used.
     *
     * The MP's geometry has to be placed around the origin (in object space).
     */
    SoSFVec3f mpSize;


    /** This field specifys the upper left corner of the effective MP area.
     *
     * It is specified in normalized VC-coordinates, ranging from (0,0) for
     * the upper left corner of the VC to (1,1) for the lower right corner
     * of the VC.	
     */  
    SoSFVec2f mpAreaUpperLeft;

    /** This field specifys the lower right corner of the effective MP area.
     *
     * It is specified in normalized VC-coordinates, ranging from (0,0) for
     * the upper left corner of the VC to (1,1) for the lower right corner
     * of the VC.	
     */  
    SoSFVec2f mpAreaLowerRight;
    
    /** The constructor inits all fields */
    SoVCMainPanelKit();

    /** Inventor class initialization */
    static void initClass();

    /** Adds a new sub panel to manage
     *
     * @param subPanel the new sub panel
     */
    void addSubPanel(SoVCSubPanelKit* subPanel);

    /** Removes a managed sub panel
     *
     * @param subPanel the sub panel to remove
     */
    void removeSubPanel(SoVCSubPanelKit* subPanel);

    /** Shows the main panel geometry
     *
     * @param show TRUE: show geometry
     */
    void showMainPanel(bool show); 

    /** Set the visibility of the MP bars
     *
     * @param tray TRUE: show trayBar, FALSE: hide trayBar
     * @param quick TRUE: show quickBar, FALSE: hide quickBar
     * @param status TRUE: show statusBar, FALSE: hide statusBar
     */
    void showMPBars(bool tray, bool quick, bool status);

    /** Sets up initial transform for MP, depending on the usable area
     *
     * @param vcAreaUL upper left corner of usable area
     * @param vcAreaLR lower right corner of usable area
     */
    void setupTransform(SbVec2f vcAreaUL, SbVec2f vcAreaLR);

    /** Updates info text on status bar.
     *
     * It depends on widgetKit's and spKit's name.
     * 
     * @param widgetKit should be current widgetKit
     * @param spKit should be current subPanelKit
     */
    void updateStatusBar(SoVCWidgetKit *widgetKit, SoVCSubPanelKit *spKit);

    /** Force refresh of MP content.
     *
     * The current subPanel's will be rescaled and the and mpPart-visibility
     * evaluated.
     */    
    void updateMP();

    /** Updates state of a subPanel depending on window action (i.e. min, max)
     *
     * Updates all subPanels states (rescale, show/hide) depending on 
     * the window action, which happenes to the given subPanel
     *
     * @param spk sub Panel, which is involved in window action
     */
    void updateSPState(SoVCSubPanelKit *spk);

    /** Sets the size of the silk panel
     *
     * @param full TRUE: entire VC size, FALSE: entire MP size
     */
    void setSilkPanelSize(bool full);

    /** Returns the VC
     *
     * @return instance of SoVCKit
     */
    SoVCKit* getVC();

    /** Returns the directory of a skin-file, that describes the properties
     * of the user-interface elements on the VC. 	
     *
     * @return skin directory name
     */
    SbString getSkinDirectory();
   
    /** Returns the name of a skin-file, that describes the properties
     * of the user-interface elements on the VC. 
     *
     * @return skin filename
     */
    SbString getSkinFileName();

    /** Rescales the given subPanel to fit into entire MP area
     *
     * @param spk the subPanel to rescale
     */
    void enlargeSubPanel(SoVCSubPanelKit *spk);

    /** Adapts tray bar state, if an tray bar element is selected
     *
     * Removes the selected element from the bar and enlarges
     * the subPanel, which was minimized to tray bar
     */
    void adaptTrayBarState();

    /** Sets the subPanel, which should be removed from tray bar
     *
     * @param spKit subPanel to be removed
     */
    void setTrayBarSPKit(SoVCSubPanelKit *spKit);

    /** Returns whether the MP is fully initialized
     *
     * @return TRUE, if initialized
     */
    bool isActive();

    /** Returns if a maximized subPanel is currently managed
     *
     * @return TRUE, if maximized subPanel is managed
     */
    bool existsMaxedSP();

    /** Get a SoSwitch containing all managed subPanels
     *
     * @return a SoSwitch containing all managed subPanels
     */    
    SoSwitch* getSubPanels();

    /** This field determines the visible parts of the MP */
    SoSFBitMask mpParts;          

    /** Current height and width of MP area */
    float mpaWidth, mpaHeight;

    /** Current centor of MP area */
    SbVec2f mpaCenter;

    /** Indicates if tray bar has been clicked */
    SoSFBool trayBarChanged;

    /** Describes the parts of the MP */
    enum MPPartMask                                        
      { 
	/** Traybar */
	MP_TRAYBAR = 0x0001,        
	/** Quickbar */
	MP_QUICKBAR = 0x0002,
	/** SubPanels */
	MP_SUBPANELS = 0x0004,
	/** Silk panel */
	MP_SILKPANEL = 0x0008,
	/** Statusbar */
        MP_STATUSBAR = 0x0010,
	/** All parts */
	MP_FULL = 0x01F     
      };                     

protected:

    /** The destructor does nothing */
    virtual ~SoVCMainPanelKit();

    /** Indicates if a maximized subPanel is managed */
    bool hasMaxedSP;

    /** ApplicationID of the application this MP is associated with */
    SbAppID appID;

    /** Maximum number of allowed subPanels */
    int maxSubPanels;

    /** Stores information, if MP has been already activated */
    SbBool active;

    /** Activate subPanels of the MP and  build the bars
     *
     * @param parent the SoVCKit that contains this MP
     */
    void activateMainPanel(SoVCKit *parent);
    
    /** Aligns all managed subPanels on the useable given area
     *
     * @param w width of MP
     * @param h height of MP
     * @param c center of MP
     */
    void alignSubPanels(float w, float h, SbVec2f c);

    /** This method merges areas of the MP
     *
     * It merges the mpArea of the given SoVCMainPanelKit and
     * the mpArea definded by resultAreaUL and resultAreaLR.
     * The resulting vcArea will be written back to resultAreaUL 
     * and resultAreaLR.

     * @param mergeArea A SoVCMainPanelKit.
     * @param resultAreaUL upper-left corner of mpArea.
     * @param resultAreaLR lower-right corner of mpArea.
     */
    void mergeMPAreas(SoVCMainPanelKit *mergeArea, SbVec2f &resultAreaUL, SbVec2f &resultAreaLR);

    /** The SoVCKit that contains this SoVCMainPanelKit */
    SoVCKit *parent;

    /** The subPanel, which should be removed from the tray bar */
    SoVCSubPanelKit* trayBarSPKit;

    /** Used for removing subPanels from the tray bar */
    SoFieldSensor trayBarChangedSensor;

    static void trayBarCB(void *userData, SoVCWidgetKit *widgetKit);
    static void sensorTrayBarCB(void * userData, SoSensor * sensor );
    
    friend class SoVCKit;

};


//----------------------------------------------------------------------------
#endif//_SOVCMAINPANELKIT_H_

