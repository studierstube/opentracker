/** SoVCSubPanelKit.h contains the definition for class SoVCSubPanelKit
 *
 * @author  Alexander Terbu
 *
 * @file
 */
#ifndef _SOVCSUBPANELKIT_H_
#define _SOVCSUBPANELKIT_H_

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoMFNode.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTransform.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/workspace/SoContextManagerKit.h>
#include <stbapi/vc/SoVCMainPanelKit.h>
#include <stbapi/vc/SoVCPushWidgetKit.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoTimerSensor.h>

#include <stbapi/interaction/SoBehaviorKit.h>

//----------------------------------------------------------------------------
class SoVCMainPanelKit;
class SoVCWidgetKit;


/** SoVCSubPanelKit is a container class for managing groups of widgets
 *
 * It provides functionality to add, remove and autolign widgets as well
 * as the display management of the widgets hierarchy structure.
 *
 * @author Alexander Terbu
 */
class STBAPI_API SoVCSubPanelKit 
: public SoBehaviorKit

{
    SO_BEHAVIOR_HEADER(SoVCSubPanelKit);
    SO_BEHAVIOR_CATALOG_ENTRY_HEADER(transform);
    SO_BEHAVIOR_CATALOG_ENTRY_HEADER(subPanelRoot);
    SO_BEHAVIOR_CATALOG_ENTRY_HEADER(partSwitch);
    SO_BEHAVIOR_CATALOG_ENTRY_HEADER(barSwitch);
    SO_BEHAVIOR_CATALOG_ENTRY_HEADER(widgetSwitch);
    SO_BEHAVIOR_CATALOG_ENTRY_HEADER(widgets);
    SO_BEHAVIOR_CATALOG_ENTRY_HEADER(windowBar);
    SO_BEHAVIOR_CATALOG_ENTRY_HEADER(widgetTransform);
    SO_BEHAVIOR_CATALOG_ENTRY_HEADER(widgetRoot);

public:

    /** The constructor inits all fields */
    SoVCSubPanelKit();

    /** Inventor class initialization */
    static void initClass();

    /** Sets visibility of current widgetKit's child
     *
     * @param state TRUE: visible, FALSE: hide
     */
    void displayCurrentWKitChilds(bool state);

    /** Activates subPanel and sets up the window bar */
    void activateSubPanel(SoVCMainPanelKit* parent);

    /** Set the visibility of the subPanel parts
     *
     * @param sppm a bit mask specifying the visible subPanel parts
     */    
    void setSPPartVisibility(unsigned int sppm);

    /** Force refresh of SP content.
     *
     * The current subPanel' spPart-visibility will be evaluated.
     */    
    void updateSP();

    /** Set the visibility of the SP bars
     *
     * @param window TRUE: show windowBar, FALSE: hide windowBar
     */
    void showSPBars(bool window);

    /** Sets up initial transform for the SP.
     *
     * It depends on the mainPanel area an the size
     * specified in the skin file.
     */
    void setupTransform();

    /** Aligns the managed widgets.
     *
     * It depends on the settings in the skin file.
     */
    void alignWidgets();
    
    /** Adapts displayed widget-hierarchy-level
     *
     * Displayed widgets are adapted to the current hierarchy
     * level and the animation sensors for hierarchy switching
     * are set up
     */
    void adaptHierarchyLevel();

    /** Initiates subPanel state update, depending on window action */
    void adaptSubPanelToState();

    /** Sets the name of the currently active widgetKit
     *
     * @param widgetName name of currently active widgetKit
     */
    void setCurrentWidgetKitName(SoSFString widgetName);

    /** Sets the currently active widgetKit
     *
     * \note This widgetKit's children will be displayed in the subPanel
     *
     * @param widgetKit currently active widgetKit
     */
    void setCurrentWidgetKit(SoVCWidgetKit* widgetKit);

    /** Sets the window widgets, which was pressed 
     *
     * @param state the pressed widget, defined in globdef.h
     */
    void setWindowWidgetPressed(int state);
    
    /** Returns the current window state of the subPanel (see SPsizedStates)
     *
     * @return the window state
     */
    int getSizedState();

    /** Returns the subPanel's title
     *
     * @return the title
     */
    SbString getTitle();

    /** Returns the name of the current active widgetKit
     *
     * @return name of widgetKit
     */
    SoSFString getCurrentWidgetKitName();

    /** Returns the current active widgetKit
     *
     * @return the widgetKit
     */
    SoVCWidgetKit* getCurrentWidgetKit();

    /** Returns the subPanel's transform node
     *
     * @return the subPanel's transform node
     */
    SoTransform* getTransform();

    /** Returns the subPanel's managed widgets
     *
     * Without their transform node
     *
     * @return the widget switch
     */
    SoSwitch* getWidgets();

    /** Returns the subPanel's parent
     *
     * @return the SoVCMainPanelKit, which manages this subPanel
     */
    SoVCMainPanelKit* getParent();

    /** Returns the subPanel's managed widgets
     *
     * With their transform node
     *
     * @return the widget root
     */
    SoSeparator* getWidgetRoot();

    /** Indicates if subPanel should be maximized */
    SoSFBool maximized;

    /** Indicates is subPanel should be highlighted */
    SoSFBool highlighted;

    /** Indicates if subPanel should be on */
    SoSFBool on;

    /** indicates if a subPanel's widget has been pressed */
    SoSFBool widgetPressed;

    /** indicates if a subPanel's window widget has been pressed */
    SoSFBool windowBarChanged;

    /** This field determines the visible parts of the SP */
    SoSFBitMask spParts;          

    /** Describes the parts of the SP */
    enum SPPartMask                                        
      { 
	/** Windowbar */
	SP_WINDOWBAR = 0x0001,        
	/** Body (= All - Windowbar) */
	SP_BODY = 0x0002,
	/** Widgets */
	SP_MENUWIDGETS = 0x0003,
	/** All parts */
	SP_FULL = 0x01F      
      };                     

    /** Describes the subPanel's window states */
    enum SPSizedStates
      {
	/** Normal (overview) */
	NORMAL = 1,
	/** Minimized (traybar) */
	MINIMIZED = 2,
	/** Maximized (enlarged to useable MP area) */
	MAXIMIZED = 3
      };

    /** The state-machine */
    void handle3DEvent(SoHandle3DEventAction* h3a);

    /** Adapts subPanel's geometry, depending on event
     *
     * Depending on bounding box check and pen button state.
     */
    void adaptGeometryToState();
    
    /** Sets whether to handle a hierachy-up or -down event
     *
     * @param state TRUE: hierarchy up, FALSE: hierarchy down
     */
    void setHandleHierarchyUp(SbBool state);

    /** Returns the so far displayed animation frames
     *
     * @return frames displayed so far
     */
    int getAnimStepsCount();

    /** Increments the so far displayed animation frames by one */
    void inc1AnimStepsCount();

    /** Gets the current widgetKit's bounding box at animation start 
     *
     * @return bounding box at animation start
     **/
    SbBox3f& getAnimSaveCurrWidgetPreBBox();

    /** Gets the current widgetKit's bounding box at animation end
     *
     * @return bounding box at animation end
     **/
    SbBox3f& getAnimSaveCurrWidgetPostBBox();


private:
    
    /** The destructor does nothing */
    virtual ~SoVCSubPanelKit();

    /** The current application ID */
    SbAppID appID;

    /** Stores information, if subPanelKit has been already activated */
    SbBool active;

    /** Indicates if a hierarchy up event should be processed */
    SbBool handleHierarchyUp;

    /** The title of the subPanel, used in the status bar */
    SoSFString title;

    /** The SoVCMainPanelKit, which manages this subPanel */
    SoVCMainPanelKit *parent;

    /** Sensor for widget events */
    SoFieldSensor widgetPressedSensor;

    /** Sensor for window widget events */
    SoFieldSensor windowBarChangedSensor;

    /** Name of currently active widgetKit */
    SoSFString currentWidgetKitName;

    /** Currently active widgetKit */
    SoVCWidgetKit* currentWidgetKit;

    /** Last active widgetKit */
    SoVCWidgetKit* oldWidgetKit;

    /** Indicates if a window widget has been pressed */
    int windowWidgetPressed;

    /** The current window state of the supPanel (see SPSizedStates) */
    int sizedState;

    // for animation

    /** The current widgeKit's bounding box at animation start */
    SbBox3f animSaveCurrWidgetPreBBox;

    /** The currentWidgetKit's bounding box at animation end */
    SbBox3f animSaveCurrWidgetPostBBox;

    /** Frames animated so far */
    int animStepsCount;

    SoMFNode actionStartGestures;
    SoMFNode actionEndGestures;
    SoMFNode actionEnterGestures;
    SoMFNode actionExitGestures;
    SoMFNode actionMoveGestures;

    SoFieldSensor onSensor;
    SoFieldSensor hlSensor;

    static void windowBarCB(void *userData, SoVCWidgetKit *widgetKit);
    static void sensorWindowBarCB(void *userData, SoSensor *sensor);
    static void widgetPressedCB(void* userData, SoVCWidgetKit* widgetKit);
    static void sensorWidgetPressedCB(void * userData, SoSensor * sensor );

    static void stateChangedCB( void * userdata, SoSensor * sensor );

    static void animationHDownSensorCB(void *userdata, SoSensor * sensor);
    static void animationHUpSensorCB(void *userdata, SoSensor * sensor);

    /** Activates the given widget and sets it's parent
     *
     * @param widgetKit widgetKit to activate
     * @param parent widgetKit which is the parent
     */
    void activateWidget(SoVCWidgetKit* widgetKit, SoVCWidgetKit* parent);

    /** deprecated */
    void scaleSPGeometry();

    friend class SoVCMainPanelKit;
};
  

//----------------------------------------------------------------------------
#endif//_SOVCSUBPANELKIT_H_

