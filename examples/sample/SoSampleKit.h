 /* ======================================================================= */
/** header file for SoSampleKit Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header$
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOSAMPLEKIT_H_
#define _SOSAMPLEKIT_H_

#include <stbapi/workspace/SoContextKit.h>
#include "sample.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** SoSampleKit is a simple ContextKit implementing a sample application
 * for the VRLU. It demonstrates how to use the input of several stations,
 * typically ARToolKit markers to move some objects. 
 *
 * @author Gerhard Reitmayr
 */
class SAMPLE_API SoSampleKit
:
    public SoContextKit
{
    SO_KIT_HEADER(SoSampleKit);

public:
    /// static class initialization
    static void initClass();
    /// constructor
    SoSampleKit();               
    /// destructor
    ~SoSampleKit(); 
     
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

}; //class SoSampleKit

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOSAMPLEKIT_H_

