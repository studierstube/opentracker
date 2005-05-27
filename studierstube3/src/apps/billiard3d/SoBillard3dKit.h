 /* ======================================================================= */
/** header file for SoBillard3dKit Node.
  *
  * @author Team und Struppi
  *
  * $Id: SoBillard3dKit.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOBILLARD3DKIT_H_
#define _SOBILLARD3DKIT_H_

#include <stbapi/widgets/So3DButton.h>
#include <stbapi/workspace/SoContextKit.h>
#include <stbapi/tracker/SoStationKit.h>
#include <Inventor/nodes/SoSeparator.h>
#include "billiard3d.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif


/**
 * Structure holding information about a billard ball.
 */
struct BallInfo {
  SbVec3f dir;                  /** direction and speed of move. */
  SbVec3f initialPos;           /** initial position. */
  int moving;                   /** flag if ball is moving */
  SoSeparator *node;            /** ball node in scene graph */
};

/** SoBillard3dKit is a simple ContextKit implementing a sample application
 * for the VRLU. It demonstrates how to use the input of several stations,
 * typically ARToolKit markers to move some objects. 
 *
 * @author Team und Struppi
 */
class BILLIARD3D_API SoBillard3dKit
:
    public SoContextKit
{
    SO_KIT_HEADER(SoBillard3sKit);

public:
    /// static class initialization
    static void initClass();
    /// constructor
    SoBillard3dKit();
    /** override this to create new context of a specific type */     
    SoContextKit* factory();                 
    /// destructor
    ~SoBillard3dKit(); 

private:

    /** create new pip sheet
     * @param uid user assigned to sheet */ 
    SoNode* createPipSheet( int uid );
    /** create window and content
     * @param index index of window */
    SoWindowKit* createWindowGeometry( int index );     

    
    SbBool SoBillard3dKit::windowEventCB(void* data,
                                         int message,
                                         SoWindowKit* window, 
                                         int userID,
                                         So3DEvent* event,
                                         SoPath* path);

    static void SoBillard3dKit::bNewGameCB(void* data, So3DButton* button);
    static void SoBillard3dKit::bGameModeCB(void* data, So3DButton* button);


}; //class SoBillard3dKit

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOBILLARD3DKIT_H_

