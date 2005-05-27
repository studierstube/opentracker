/** SoVCBarKit.h contains the definition for class SoVCBarKit
 *
 * @author  Alexander Terbu
 *
 * @file
 */

#ifndef _SOVCBARKIT_H_
#define _SOVCBARKIT_H_

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/SbString.h>
#include <stbapi/resource/StbSkinLoader.h>
#include <stbapi/vc/SoVCWidgetKit.h>
#include <stbapi/vc/SoVCMainPanelKit.h>
#include <Inventor/SbLinear.h>


class SoVCMainPanelKit;

//---------------------------------------------------------------------------

/** SoVCBarKit is a nodekit representing a bar, which can hold and manage several widgets.
 * 
 * @author Alexander Terbu
 */


class STBAPI_API SoVCBarKit
:
    public SoBaseKit
{
    SO_KIT_HEADER(SoVCBarKit);
    
    SO_KIT_CATALOG_ENTRY_HEADER(barRoot);
    SO_KIT_CATALOG_ENTRY_HEADER(barSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(barTransform);
    SO_KIT_CATALOG_ENTRY_HEADER(barWidgets);
    SO_KIT_CATALOG_ENTRY_HEADER(barTransp);

public:
    
    /** The constructor inits all fields
     *
     * @param maxElements specifies maximum expected number of elements in bar
     * @param skin specifies instance to loaded skin file
     * @param prefix specifies the prefix used for this class in the skin file
     */
    SoVCBarKit(int maxElements = 0, StbSkinLoader *skin = NULL, char *prefix = "BAR_");

    /** Inventor class initialization */
    static void initClass();

    /** Adds an SoVCWidgetKit element to the bar
     *
     * @param widgetID is used to refer to the new element
     * @param widgetName is used for refering and status bar display
     * @param content content, which should be displayed inside the new SoVCWidgetKit
     * @param cbFunc callback function, which is called when widget becomes pressed
     * @param cbData callback data, which is forwarded to the callback function, if widget gets used
     * @param numSpacers specifies number of free space (in size dimension of a widget) used, before adding the new element (default=(0,0,0))
     * @param scaleContent specifies whether the content should be autoscaled (default=TRUE)
     */
    void addBarElement(int widgetID, const char* widgetName, SoSeparator* content, SoVCWidgetKitCB* cbFunc, void* cbData, SbVec3f numSpacers = SbVec3f(0,0,0), bool scaleContent = true);

    /** Removes the given SoVCWidgetKit from the bar 
     *
     * @param widgetKit the SoVCWidgetKit to be removed
     */
    void removeBarElement(SoVCWidgetKit* widgetKit);

    /** Turns visibility of elements on/off
     *
     * @param show sets state (on/off)
     */
    void showElements(bool show);

    /** Sets up the initial bar transformation matrix
     *
     * @param start initial translation of bar (should be previously read from skin file)
     * @param barSize initial size of bar (should be previously read from skin file)
     * @param vcSize size of parent
     */
    void setupTransform(SbVec3f start, SbVec3f barSize, SbVec3f vcSize);
    
    /** Returns number of bar elements
     *
     * @return number of elements
     */
    int getNumElements();

    /** Returns bar element refered by given ID
     *
     * @param widgetID the refering widgetID
     * @return the refered SoVCWidgetKit
     */
    SoVCWidgetKit* getElement(int widgetID);

    /** Returns bar element refered by given name
     *
     * @param widgetName the refering widgetName
     * @return the refered SoVCWidgetKit
     */
    SoVCWidgetKit* getElement(const char* widgetName);

    /** Sets the element, which should be removed
     *
     * @param widgetKit the element to be removed
     */
    void setElementToRemove(SoVCWidgetKit* widgetKit);
    
    /** Indicates whether an element should be removed.
     *
     * Attached to a sensor, which initiates removeal.
     */
    SoSFBool removeElement;   

protected:

    /** Destructor does nothing */
    virtual ~SoVCBarKit();

    /** Returns the root node of managed widgets
     *
     * @return the widgets node kit list
     */
    SoNodeKitListPart* getWidgets();

    /** The skin loader */
    StbSkinLoader *skin;

    /** The next element to be removed */
    SoVCWidgetKit *elementToRemove;

    /** The prefix used for loads from skin files */
    char *prefix;

    /** The maximum number of bar elements */
    int maxElements;

    /** The sensor, which is attached to the removeElement field */
    SoFieldSensor removeElemSensor;

    /** Updates the bar depending on the current state */
    void adaptBarToState();

    /** Callback on state changes */
    static void stateChangedCB( void * userdata, SoSensor * sensor );

    /** Callback to remove an element from the bar */
    static void removeBarElementCB(void *userData, SoVCWidgetKit *widgetKit);

    friend class SoVCMainPanelKit;
};
  

//----------------------------------------------------------------------------
#endif//_SOVCBARKIT_H_

