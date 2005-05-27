 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: Studierstube
  * ======================================================================== */ 
/** SoMessageKit.h contains the definition of class SoMessageKit
  *
  * @author Rainer Splechtna
  *
  * $Id: SoMessageKit.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef _SoMessageKIT_H_
#define _SoMessageKIT_H_

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoSFVec2f.h>
#include <stbapi/interaction/So3DSeparator.h> 
#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/elements/SoProjectionMatrixElement.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

//----------------------------------------------------------------------------

/** SoMessageKit is a nodekit.

@author Rainer Splechtna
 */
class SoMessageKit
:
    public SoBaseKit
{
    SO_KIT_HEADER(SoMessageKit);
    SO_KIT_CATALOG_ENTRY_HEADER(messageRoot);
    SO_KIT_CATALOG_ENTRY_HEADER(camera);
    SO_KIT_CATALOG_ENTRY_HEADER(onOffSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(xForm);
    SO_KIT_CATALOG_ENTRY_HEADER(text);
    SO_KIT_CATALOG_ENTRY_HEADER(material);
    SO_KIT_CATALOG_ENTRY_HEADER(font);

public:

    SoMFString message;
    SoSFColor color;
    SoSFName  fontName;
    SoSFFloat fontSize;
    SoSFVec2f position;

    /** The constructor of the kit inits the fields.
      */
    SoMessageKit(
        );
    /** The destructor of the kit does nothing. */
    ~SoMessageKit(             
        );
    /** Inventor class initialization */
    static void initClass(             
        );

private:
    void connectAndSetFields();
};

//----------------------------------------------------------------------------
#endif//_SOPENKIT_H_

