/** SoVCStatusBarKit.h contains the definition for class SoVCStatusBarKit
 *
 * @author  Alexander Terbu
 *
 * @file
 */
#ifndef _SOVCSTATUSBARKIT_H_
#define _SOVCSTATUSBARKIT_H_

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/SbString.h>
#include <stbapi/resource/StbSkinLoader.h>
#include <stbapi/vc/SoVCMainPanelKit.h>
#include <Inventor/SbLinear.h>

class SoVCMainPanelKit;

//----------------------------------------------------------------------------
/** SoVCStatusBarKit provides functionality for flexible display of status information
 *
 * The bar is displayed at the bottom of the silkPanel. Several single status areas 
 * can be added and managed seperately. The content can be any scenegraph and is
 * scaled automatically.
 *
 * @author Alexander Terbu
 */
class STBAPI_API SoVCStatusBarKit
:
    public SoBaseKit
{
    SO_KIT_HEADER(SoVCStatusBarKit);
    
    SO_KIT_CATALOG_ENTRY_HEADER(barRoot);
    SO_KIT_CATALOG_ENTRY_HEADER(barTransf);
    SO_KIT_CATALOG_ENTRY_HEADER(barTransp);
    SO_KIT_CATALOG_ENTRY_HEADER(barGeomRoot);
    SO_KIT_CATALOG_ENTRY_HEADER(barGeomTransf);
    SO_KIT_CATALOG_ENTRY_HEADER(barGeomMat);
    SO_KIT_CATALOG_ENTRY_HEADER(barGeomShape);
    SO_KIT_CATALOG_ENTRY_HEADER(barElements);

public:

    /** The constructor inits all fields
     *
     * @param skin instance to the skin loader
     * @param prefix prefix to be used, when loading information from skin file
     */
    SoVCStatusBarKit(StbSkinLoader *skin = NULL, char *prefix = "STATUSBAR_");

    /** Inventor class initialization */
    static void initClass();

    /** Adds a new element to the bar
     *
     * @param content the information to be displayed in the status bar area
     * @param usedSize the amount of status bar space to use (0.0 - 1.0)
     * @return the ID of the new added area, or -1 if not enough space was left
     */
    int addBarElement(SoSeparator* content, double usedSize);

    /** Specifys if all status bar areas should be shown
     *
     * @param show TRUE: show all, FALSE: show none
     */
    void showElements(bool show);

    /** Specifys if a specific status bar area should be shown
     * 
     * @param elementID ID of the affected area
     * @param show TRUE: show area, FALSE: hide area
     */
    void showElement(int elementID, bool show);

    /** Sets up the initial bar transformation matrix
     *
     * @param start initial translation of bar (should be previously read from skin file)
     * @param barSize initial size of bar (should be previously read from skin file)
     * @param vcSize size of parent
     */
    void setupTransform(SbVec3f start, SbVec3f barSize, SbVec3f vcSize);   

    /** Sets new content of a status bar area
     *
     * @param elementID ID of the affected area
     * @param newContent new content of area
     */
    void updateElementsContent(int elementID, SoSeparator* newContent);

    /** Used to initiate an update of an area's content */
    SoSFBool updateElemContent;   

protected:

    /** The destructor does nothing */
    virtual ~SoVCStatusBarKit();

    /** Holds the current size of and the size up-to an single area */
    std::vector< std::pair<double, double> > elementSizes;

    /** Total used status bar size so far */
    double usedTotalSize;

    /** Current number of single areas */
    int idCount;

    /** The skinloader to be used for reading the skin file */
    StbSkinLoader *skin;

    /** New content of the element to be updated */
    SoSeparator *updatedElementsContent;

    /** ID of the element to be updated */
    int updatedElementsID;

    /** Prefix to be used for reading skin file data */
    char *prefix;

    /** Used to update single areas content */
    SoFieldSensor updateElemsContentSensor;

    /** Initiates update of status bar content, if necessary */
    void adaptBarToState();

    /** Updates content of area specified by updatedElementsID */
    void updateContent();

    static void stateChangedCB( void * userdata, SoSensor * sensor );
    static void updateElementsContentCB(void *userData, SoVCWidgetKit *widgetKit);

    friend class SoVCMainPanelKit;
};
  

//----------------------------------------------------------------------------
#endif//_SOVCSTATUSBARKIT_H_

