 /* ========================================================================
  * (c) 2000,2001 VRVis GmbH, Vienna, Austria
  * (c) 1999,2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: Studierstube VR Slide Show system
  * ======================================================================== */
/** The VRSS application itself.
  * This quite confused piece of code contains the body of the VRSS
  * application - the interface to Studierstube. The functionality itself,
  * that is the callbacks for slide transitions and so on, is in @c
  * callbacks.cxx and @c tools.cxx.
  *
  * @author Jan Prikryl
  * @author Rainer Splechtna
  * @author Anton Furhmann
  * @author Dieter Schmalstieg
  *
  * $Id: vrss.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @ingroup VRSS
  * @file
  */
 /* ======================================================================== */
/*@{*/


// #include <sys/types.h>
// #include <assert.h>
#include <stdlib.h>
// #include <time.h>
//
#ifdef WIN32
#include <windows.h>
// #define strdup _strdup
#endif
//
// #include <Inventor/Xt/SoXt.h>
// #include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#include <Inventor/actions/SoSearchAction.h>
// #include <Inventor/actions/SoWriteAction.h>
#include <Inventor/events/SoKeyboardEvent.h>
// #include <Inventor/sensors/SoAlarmSensor.h>
// #include <Inventor/nodes/SoNode.h>
// #include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCallback.h>
// #include <Inventor/nodes/SoEventCallback.h>
// #include <Inventor/nodes/SoFile.h>
// #include <Inventor/nodes/SoWWWInline.h>
//
// #ifdef USE_TGS
// #include <Inventor/VRMLnodes/SoVRMLSwitch.h>
// #include <Inventor/VRMLnodes/SoVRMLGroup.h>
// #include <Inventor/VRMLnodes/SoVRMLColorInterpolator.h>
// #include <Inventor/VRMLnodes/SoVRMLCoordinateInterpolator.h>
// #include <Inventor/VRMLnodes/SoVRMLNormalInterpolator.h>
// #include <Inventor/VRMLnodes/SoVRMLOrientationInterpolator.h>
// #include <Inventor/VRMLnodes/SoVRMLPositionInterpolator.h>
// #include <Inventor/VRMLnodes/SoVRMLScalarInterpolator.h>
// #include <Inventor/VRMLnodes/SoVRMLTimeSensor.h>
// #include <Inventor/VRMLnodes/SoVRMLPROTODef.h>
// #endif
//
// #include <stbapi/tracker/TrackerBuilder.h>
// #include <stbapi/tracker/SoTrakEngine.h>
// #include <stbapi/event3d/Stb3DEventGenerator.h>
#include <stbapi/misc/SoAppNode.h>
// #include <stbapi/util/option.h>
// #include <stbapi/util/ivutil.h>
#include <stbapi/util/ivio.h>
// #include <stbapi/util/timerutility.h>
// #include <stbapi/widgets/So3DButton.h>
// #include <stbapi/resource/SoUserKit.h>
#include <stbapi/resource/SoPipSheetKit.h>
#include <stbapi/workspace/StbCommunicator.h>
#include <stbapi/workspace/StbWorkspace.h>

#include "vrss.h"
#include "callbacks.h"
#include "tools.h"

/**
 * Global variables.
 */
#ifdef USE_TGS
SoVRMLTimeSensor * TimeSensors[100];    /**< Maximum 100 time sensors. */
SoAlarmSensor    * EndTransitionSensor; /**< Signals the end of transition. */
int                _noTransition = 1;   /**< Suppress slide transitions. */
#endif

#ifdef WIN32
/**
 * WIN32 DLL Entry point
 * This function is needed only when the application runs in WIN32
 * environment.
 */

BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}
#endif

/**
 * Find a named node in the scene graph.
 * This code a slightly changed copy of the `findNode' function from
 * STB ivio.cxx. This version does not call assert when the node with
 * the given name has not been found, but it rather returns NULL.
 *
 * @param root Root of the scene graph that shall be searched for the
 *             named node.
 * @param name Name of the node that will be searched for.
 * @return     The pointer to the node found or NULL if the node of
 *             this name does not exist.
 */

static SoNode *
findNodeNonFatal ( SoNode * root,
           SbString name )
{
    SoSearchAction sa;
    sa.setSearchingAll (TRUE);
    sa.setName (name);
    sa.apply (root);
    SoFullPath * path = (SoFullPath *) sa.getPath ();
    return path != NULL ? path->getTail() : NULL ;
}

/**
 * Initializes a PIP sheet button.
 * Adds a texture and a proper callback to a PIP sheet button of the given
 * name.
 * @param pipSheet    The sheet that is supposed to contain the given button.
 * @param buttonName  The name of the button.
 * @prarm textureFile File name of the image to be used as the button
 *                    texture.
 * @param buttonCB    Callback activated upon button release.
 * @param masterNode  Optional parameter of the callback function.
 */

static void
vrssAddTexturedButton ( SoPipSheetKit * pipSheet,
            const char    * buttonName,
            const char    * textureFile,
            So3DButtonCB  * buttonCB,
            void          * masterNode = NULL
            )
{
    So3DButton * button =
    (So3DButton *) findNodeNonFatal (pipSheet, buttonName);

    if ( button != NULL )
    {
    SoTexture2 * texture  = new SoTexture2 ();
    texture->filename.setValue (textureFile);
    texture->model.setValue (SoTexture2::DECAL);

    button->setupTexture (texture, 0.2, 0.2);
    button->addReleaseCallback (buttonCB, masterNode);
    }
}

/**
 * Creates and sets up a PIP-sheet.
 * The sheet is read from the file @c sheet.iv. This sheet is supposed
 * to contain three named buttons that will offer the user the
 * possibility to switch to the next, previous, and first slide in the
 * presentation. Missing buttons are silentlly ignored.
 * @param userID Current user ID.
 * @param comm   Reference to STB communicator.
 * @param data   Optional data for callbacks initialized in this function.
 */

static void
vrssPipSheet (int userID, StbCommunicator * comm, void * data)
{
    SoPipSheetKit * newPipSheet =
    (SoPipSheetKit *) readNodeFromFile ("sheet.iv",
                        comm->workingDirectory);
    newPipSheet->ref ();

    // Button to the next slide
    vrssAddTexturedButton ( newPipSheet,
                "NEXT_BUTTON", "next.png",
                buttonNextCB, data );

    // Button to the previous slide
    vrssAddTexturedButton ( newPipSheet,
                "PREV_BUTTON", "back.png",
                buttonPrevCB, data );

    // Button to the first slide
    vrssAddTexturedButton ( newPipSheet,
                "FIRST_BUTTON", "first.png",
                buttonFirstCB, data );

    newPipSheet->unrefNoDelete ();
    comm->setPipGeometry (userID, newPipSheet);
}

/**
 * Message handler routine of workspace application.
 */

VRSS_API void Stb_getMessage(void *, int message , int , void *data)
{
    DPRINTF(("DEBUG: Entering Stb_getMessage()\n"));
    DPRINTF(("       Leaving Stb_getMessage()\n"));
}

/**
 * Cleanup routine of workspace application.
 */

VRSS_API SbBool Stb_exit(void *data)
{
    DPRINTF(("DEBUG: Stb_exit() called, returning TRUE\n"));
    return TRUE;
}

/**
 * VRSS application.
 * This is the initialization routine of workspace application. It
 * builds also the structures needed for running the slide show -
 * transition timers, PIP sheet, application window. The routine takes
 * also care of expanding all the SoFile nodes into memory and of
 * initialising the render caches.
 */

VRSS_API void
Stb_init(int userID, StbCommunicator* comm)
{
    time_t timeStart = time(NULL);

#ifdef USE_TGS
    memset(TimeSensors, 0, 100 * sizeof(void*));
#endif

    char pMacro[1000];
    sprintf(pMacro,"cd %s; ./pythonmacro",VRSS_DATA_DIR);
#ifdef WIN32
    /** convert all slashes to backslashes */
    char *badSlash;
    while (badSlash = strchr(pMacro,'/'))
        *badSlash = '\\';
#endif
    /** update presentation */
    printf("INFO:  Calling `pythonmacro' to expand .pym files\n");
    system(pMacro);

    /** create scene root */
    SoSeparator * root = new SoSeparator;
    root->ref();

    /** switch on transparent texturing */
    SoCallback *trTexCB = new SoCallback;
    trTexCB->setCallback(transparentTexturingCB, (void *)NULL);
    root->addChild(trTexCB);

    /** add an event callback node so we can receive key press events */
    SoEventCallback *myEventCB = new SoEventCallback;
    root->addChild(myEventCB);

    /** Read the file supplied by user */

    printf("INFO:  Reading the scene ...\n");
    SoInput::addDirectoryFirst(VRSS_DATA_DIR);

    SbString fileName = "sequence.wrl";
    SoSeparator * stbFile = readFile(fileName.getString());
    assert(stbFile);
    stbFile->ref();
    root->addChild(stbFile);

    printf("       Done.\n");

#ifdef USE_TGS
    if (1)
    {
        // Look for all SoWWWInline nodes
        SoSearchAction sa;
        sa.setType(SoWWWInline::getClassTypeId());
        sa.setInterest(SoSearchAction::ALL);
        sa.setSearchingAll(TRUE);

    DPRINTF(("DEBUG: Searching for SoWWWInline nodes.\n"));

        sa.apply(root);

        SoPathList & p = sa.getPaths();
        if (p == NULL)
        {
            fprintf(stderr,"WARNING: No SoWWWInline found!\n");
        }
        else
        {
            for (int i=0; i< p.getLength(); i++)
            {
                SoWWWInline * node = (SoWWWInline *) p[i]->getTail();
                assert(node);
        node->setReadAsSoFile(TRUE);
                printf("SoWWWInline '%s' processed\n",
               node->name.getValue().getString());
            }
        }

    DPRINTF(("       Finished.\n"));
    }

    // SoVRMLSwitch node
    SoNode * slideNode = stbFile->getByName(SbName("SLIDE_SHOW"));
    if (slideNode != NULL)
    {
    myEventCB->addEventCallback(
                    SoKeyboardEvent::getClassTypeId(),
                    myKeyPressCB, slideNode);

    printf("INFO:  Added KeyPress callback\n");

    // 3D Buttons
    So3DButton * buttonNext =
        (So3DButton *) stbFile->getByName(SbName("BUTTON_NEXT"));
    if (buttonNext == NULL)
    {
        fprintf(stderr,"WARNING: Missing 'BUTTON_NEXT' node!\n");
        fprintf(stderr,"         Disabling button next slide.\n");
    }
    else
    {
        buttonNext->addPressCallback(buttonNextCB, slideNode);
    }

    So3DButton * buttonPrev =
        (So3DButton *) stbFile->getByName(SbName("BUTTON_PREV"));
    if (buttonPrev == NULL)
    {
        fprintf(stderr,"ERROR: Missing 'BUTTON_PREV' node!\n");
         fprintf(stderr,"         Disabling button to previous slide.\n");
    }
    else
    {
        buttonPrev->addPressCallback(buttonPrevCB, slideNode);
    }

    // Look for all SoVRMLTimeSensor nodes
    SoSearchAction sa;
    sa.setType(SoVRMLTimeSensor::getClassTypeId());
    sa.setInterest(SoSearchAction::ALL);
    sa.setSearchingAll(TRUE);
    sa.apply(slideNode);

    SoPathList & p = sa.getPaths();
    if (p == NULL)
    {
        fprintf(stderr,"WARNING: No SoVRMLTimeSensor found!\n");
    }
    else
    {
        DPRINTF(("DEBUG: Loading timers ...\n"));
        for (int i=0; i< p.getLength(); i++)
        {
        TimeSensors[i] = (SoVRMLTimeSensor *) p[i]->getTail();
        DPRINTF(("       Loaded timer %d\n", i));
        }
        DPRINTF(("       Done.\n"));
    }

    // This is called at the end of every transition
    EndTransitionSensor = new SoAlarmSensor(myEndTransitionCB, slideNode);
    }
    else
#endif
    {
    fprintf(stderr,
        "WARNING: Missing 'SlideSwitch' node. \n"
        "         Presentation mode disabled.\n");
    }

    // Nuke SoFile nodes

#ifdef USE_TGS
    if (1)
    {
    SoNode * nuke = (SoNode *) root;

#ifdef DEBUG_PRINT
    writeScene(root, "prenukenode.iv");
#endif
    nukeFileNodes(nuke);
    root = (SoSeparator *) nuke;

#ifdef DEBUG_PRINT
     writeScene(root, "nukenode.iv");
#endif
     }
#endif

     // Save scene callback
     myEventCB->addEventCallback(SoKeyboardEvent::getClassTypeId(),
                 saveSceneCB, NULL);

     // A few viewer tweaks: background colour, different
     // transparency, no autoclipping, no initial viewing mode.
     SoStudierstubeViewer *viewer = comm->getViewer(userID);
     viewer->setBackgroundColor(SbColor(0.3, 0.3, 0.45));
     viewer->setTransparencyType(SoGLRenderAction::BLEND);
     viewer->setViewing(FALSE);
     viewer->setAutoClipping(FALSE);

#ifdef USE_TGS
     // Build the node caches
     cacheSlides((SoVRMLSwitch *)slideNode, viewer);
#endif

     // Now create the application window
     WindowCreate wc;
     wc.position.setValue(0,0,0);
     wc.orientation = SbRotation::identity();
     wc.size.setValue(0.5,0.5,0.5);
     wc.state = BM_NORMAL;
     wc.title = SbString("");
     wc.eventMask = Base3D::BM_ALL_EVENTS;
     wc.clientEventMask = Base3D::BM_ALL_EVENTS;
     wc.state = BM_MAXIMIZED;

     comm->createWindow (NULL, &wc, NULL, NULL);

     // Hang the current root into the STB scene graph.
     StbWorkspace::getInstance()->auxRoot->addChild(root);

#ifdef USE_TGS
     // Create a PIP sheet.
     vrssPipSheet (userID, comm, slideNode);
#endif

     // SoAppNode will now start the application in the moment it will
     // be first rendered.
     SoAppNode::setIgnoreGLRenderAction(FALSE);

#ifdef DEBUG_PRINT
     DPRINTF(("DEBUG: Saving scene to `final.iv'\n"));
     writeScene(root, "final.iv");
#endif

     time_t timeLen = time(NULL) - timeStart;

     printf("INFO:  Stb_init() finished.\n");
     printf("       Spent %d seconds in initialization.\n", timeLen);
     fflush(stdout);
     fflush(stderr);
}

/*@}*/
/* ===========================================================================
   End of vrss.cxx
   ===========================================================================
   Automatic Emacs configuration follows.
   Local Variables:
   mode:c++
   c-basic-offset: 4
   eval: (c-set-offset 'substatement-open 0)
   eval: (c-set-offset 'case-label '+)
   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
   End:
=========================================================================== */
