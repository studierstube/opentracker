/** SoVCDraggerKit.h contains the definition for class SoVCDraggerKit
 *
 * @author  Alexander Terbu
 *
 * @file
 */

#ifndef _SOVCDRAGGERKIT_H_
#define _SOVCDRAGGERKIT_H_

#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFShort.h>
#include <Inventor/engines/SoCompose.h>

#include <stbapi/misc/SoConstraint.h>
#include <stbapi/vc/SoVCWidgetKit.h>

class SoVCDraggerKit;

typedef void SoVCDraggerKitCB(void* userData, SoVCDraggerKit* dragKit);

/** SoVCDraggerKit is a base class for objects, that can be grabbed, and then shifted and rotated. 
 *
 * It allows to specify a starting and and end gesture, i.e. a
 * certain sequence of events that must happen in order
 * to initiate or to end dragging. The scene object's
 * rotational and translational degrees of freedom
 * can be limited by using constraints.
 *
 * @author Alexander Terbu
 */
class STBAPI_API SoVCDraggerKit : public SoVCWidgetKit
{
    SO_BEHAVIOR_HEADER(SoVCDraggerKit);    

public:

    /** Specifies the point on the drag kit, that touches the pen.
     *
     * Either the pick point or the kit's origin
     */
    SoSFEnum dragPoint;

    enum DragPoint
    {
        PICKPOINT = 0,
        ORIGIN
    };

    /** Inventor class initialization */
    static void initClass();

    /** The constructor inits all fields */
    SoVCDraggerKit();

    /** The destructor does nothing */
    ~SoVCDraggerKit();

    /** Current translation in local coordinates */
    SoSFVec3f translation;

    /** Current rotation in local coordinates */
    SoSFRotation rotation;

    /** Button to drag with */
    SoSFInt32 dragButton;

    /** The widget's state-machine */
    void handle3DEvent(SoHandle3DEventAction *h3a);
        
    void setStartCallback(SoVCDraggerKitCB* cb,void* userData = NULL);
    void setFinishCallback(SoVCDraggerKitCB* cb, void* userData = NULL);
    void setMoveCallback(SoVCDraggerKitCB* cb, void* userData = NULL);

    void removeStartCallback();
    void removeFinishCallback();
    void removeMoveCallback();

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

protected:

    /** Responsible for coordinate mapping 
     *
     * Maps the pen coordinates onto the dragged object's coordinates
     */
    SoConstraint *constraint;
    SoTransform *constrainedTransform;

    void connectConstraint();

    /** Original kit translation, local */
    SbVec3f	kitInitialTransL;

    /** Original kit rotation,    local  */
    SbRotation	kitInitialRotL;			

    /** Original pen translation, global */
    SbVec3f	penInitialTransG;

    /** Original pen rotation,    global */
    SbRotation	penInitialRotG;

    /** Offset to pen-kit origin, local */
    SbVec3f	pickOffsetL;	

    /** Global to local transformation */
    SbMatrix	globalToLocal;

    SoVCDraggerKitCB * startCallback;
    SoVCDraggerKitCB * finishCallback;
    SoVCDraggerKitCB * moveCallback;

    void * startCallbackData;
    void * finishCallbackData;
    void * moveCallbackData;  

}; //class SoVCDraggerKit

#endif//_SOVCDRAGGERKIT_H_


