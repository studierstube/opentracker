 /* ======================================================================= */
/** header file for SoSampleKit Node.
  *
  * @author Stefan Bruckner
  *			Rudolf Seemann
  *
  * $Id: SoVtkKit.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOSAMPLEKIT_H_
#define _SOSAMPLEKIT_H_

#include <stbapi/stbapi.h>
#include <stbapi/workspace/SoContextKit.h>
#include <stbapi/window3d/SoWindowKit.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/tracker/SoStationKit.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCone.h>


#include "vtk.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** SoVtkKit is a simple ContextKit implementing a sample application
 * for the VRLU. It demonstrates how to use VTK Objects in an OpenInventor 
 * scenegraph.
 *
 */
class VTK_API SoVtkKit
:
    public SoContextKit
{
    SO_KIT_HEADER(SoVtkKit);

public:
    /// static class initialization
    static void initClass();
    /// constructor
    SoVtkKit();               
    /// destructor
    ~SoVtkKit(); 

	SoContextKit* factory();                 
     
protected:

    virtual SbBool checkWindowGeometry();

    virtual SbBool checkPipGeometry();
        
    virtual void checkPipConnections( int uid, SoNode * pipSheetContents );

    virtual void checkPipMasterMode( SoNode * pipSheetContents, SbBool masterMode );

    virtual SbBool windowEventCB(      
        void* data,                    
        int messageId,
        SoWindowKit* win,
        int uid,
        So3DEvent* event, 
        SoPath* path
        );
    
    /** override this to react to focus change
     * @param uid user for which focus has changed
     * @param focus TRUE or FALSE to show whether focus was gained or lost
     */
    virtual void focusChange( int uid, SbBool focus  );

}; //class SoVtkKit

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOVTKKIT_H_

