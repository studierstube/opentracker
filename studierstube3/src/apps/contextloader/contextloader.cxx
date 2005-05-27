/* ===========================================================================
    (C) 2000 Vienna University of Technology
   ===========================================================================
    NAME:       contextloader.cxx
    TYPE:       c++ code
    PROJECT:    Studierstube
    CONTENT:    application loader utility
    VERSION:    2.2
   ===========================================================================
    AUTHORS:    ds      Dieter Schmalstieg
                jp      Jan Prikryl
   ===========================================================================
    HISTORY:

=========================================================================== */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <Inventor/nodes/SoInfo.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <stbapi/workspace/StbCommunicator.h>
#include <stbapi/workspace/defines.h>
#include <stbapi/stbapi.h>
#include <stbapi/util/ivio.h>
#include <stbapi/misc/ivmacros.h>
#include <stbapi/interaction/So3DSensor.h>
#include <stbapi/workspace/SoContextManagerKit.h>
#include "contextloaderdll.h"

//-----------------------------------------------------------------------------

int userID;
StbCommunicator* cMan;

//-----------------------------------------------------------------------------

void
loadFunction(void*, So3DSensor* sensor)
{
    char realName[1024];
    AGETCHILD(infoApp,SoInfo,sensor,0);
    AGETCHILD(infoDir,SoInfo,sensor,1);
    AGETCHILD(infoAppType,SoInfo,sensor,2);
    NEWNODE(icon,SoSeparator);
    for(int i=0; i<sensor->getNumChildren(); i++)
        icon->addChild(sensor->getChild(i));
    strcpy(realName,infoApp->string.getValue().getString());
#ifdef WIN32
    if(strstr(realName, STB_FILE_EXTENSION_UNIX))
        sprintf(realName,"%s%s",
                infoApp->string.getValue().getSubString
                (0, infoApp->string.getValue().getLength() -
                 strlen(STB_FILE_EXTENSION_UNIX)-1).getString(),
                STB_FILE_EXTENSION_WIN32);
#else
    if(strstr(realName, STB_FILE_EXTENSION_WIN32))
        sprintf(realName,"%s%s", 
                infoApp->string.getValue().getSubString
                (0, infoApp->string.getValue().getLength() -
                 strlen(STB_FILE_EXTENSION_WIN32)-1).getString(),
                STB_FILE_EXTENSION_UNIX);
#endif

    if(infoAppType->string.getValue() == "CONTEXT")
    {
        SoContextManagerKit::getInstance()->newContext(realName,
            infoDir->string.getValue(),userID,TRUE,"");
    }
    else // old-style application
    {
        SbString f = infoDir->string.getValue();
        f += SbString(realName);
        int res = cMan->loadApplication(
            (char*)f.getString(),userID,cMan->appID,icon);
    }
}

//-----------------------------------------------------------------------------
// Initialization routine of workspace application
//-----------------------------------------------------------------------------

CONTEXTLOADER_API void 
Stb_init(int u, StbCommunicator* c)
{
    userID = u;
    cMan = c;
    WindowCreate wc;
    SoWindowKit::defaultWindow(wc);
    wc.style = 	wc.style | WS_NOBORDER;
    SoWindowKit* windowKit = cMan->createWindow(NULL, &wc, NULL, NULL);
    SoSeparator * clientVolume = windowKit->getClientVolumeSeparator();
    SoSeparator* root = readFile("application.iv",cMan->workingDirectory);
    clientVolume->addChild(root);

    SbViewportRegion vpr;
    SoGetBoundingBoxAction bba(vpr);
    bba.apply(root);
    SbBox3f box = bba.getBoundingBox();
    windowKit->size = (box.getMax()-box.getMin())*1.1;
    SoTransform* winXf;
    winXf = (SoTransform*)readNodeFromFile("winxf.iv",cMan->workingDirectory);
    SbVec3f p = winXf->translation.getValue();
    windowKit->setPosition(p);
    SbRotation r = winXf->rotation.getValue();
    windowKit->setRotation(r);

    for(int i=0; i<root->getNumChildren(); i++)
    {
        SoNode* n = root->getChild(i);
        if(n->isOfType(So3DSensor::getClassTypeId()))
            ((So3DSensor*)n)->setPressCallback(loadFunction,NULL);
    }
}


//-----------------------------------------------------------------------------
// Cleanup routine of workspace application
//-----------------------------------------------------------------------------

CONTEXTLOADER_API SbBool
Stb_exit(void*)
{
    return TRUE;
}

/* ===========================================================================
    End of contextloader.cxx
   ===========================================================================
    Automatic Emacs configuration follows.
    Local Variables:
    mode:c++
    c-basic-offset: 4
    eval: (c-set-offset 'substatement-open 0)
    eval: (c-set-offset 'case-label '+)
    eval: (c-set-offset 'statement 'c-lineup-runin-statements)
    eval: (setq indent-tabs-mode nil)
    End:
=========================================================================== */
