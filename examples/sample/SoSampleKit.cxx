 /* ======================================================================= */
/** source file for SoSampleKit Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header$
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/nodes/SoSphere.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/util/ivio.h>
#include <stbapi/event3d/Stb3DEventGenerator.h>
#include <stbapi/resource/SoPipSheetKit.h>

#include "SoSampleKit.h"

//--initialization of kit structure in Inventor
SO_KIT_SOURCE(SoSampleKit);

//--initialization of context in Studierstube
CONTEXT_APPLICATION(SoSampleKit);

//----------------------------------------------------------------------------
// static class initialization
//----------------------------------------------------------------------------

void SoSampleKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoSampleKit, SoContextKit, "SoContextKit");
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoSampleKit::SoSampleKit()
{
    SO_KIT_CONSTRUCTOR(SoSampleKit);
    SO_KIT_INIT_INSTANCE();
}

//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------

SoSampleKit::~SoSampleKit()
{
}

//----------------------------------------------------------------------------

SbBool SoSampleKit::checkWindowGeometry()
{
    // implement this method, if you want to do something on your application
    // geometry stored in the windowGroup part of the kit. It will be called
    // once after loading the application.
    return SoContextKit::checkWindowGeometry();
}

SbBool SoSampleKit::checkPipGeometry()
{
    // implement this method, if you want to do something with your template
    // pip sheet stored in templatePipSheet. It will be called once after
    // loading the application.
    return SoContextKit::checkPipGeometry();
}

void SoSampleKit::checkPipConnections( int uid, SoNode * pipSheetContents )
{
    // this method is called for each user that is registered in the system 
    // and whenever a user is added to the system. It allows you to setup
    // connections, sensors, callbacks and such on the user's pip sheet.
}

void SoSampleKit::checkPipMasterMode( SoNode * pipSheetContents, SbBool masterMode )
{
    // this method is called for each pip sheet, if the master mode for
    // that sheet changes. This is only necessary for distributed applications
}

SbBool SoSampleKit::windowEventCB( void* data, int messageId, SoWindowKit* win, int uid,
                     So3DEvent* event, SoPath* path )
{
    // this callback is executed for all 3D events passed to any child window
    // of your application. Implement it to react to window and 3D events.
    return SoContextKit::windowEventCB( data, messageId, win, uid, event, path );
}

void SoSampleKit::focusChange( int uid, SbBool focus  )
{
    // this method is called, if your application gets or looses the focus
    // of a certain user. Override it to add your own implementation.
}