/* ===========================================================================
    (C) 1999 Vienna University of Technology
   ===========================================================================
    NAME:       fullview.cxx
    TYPE:       c++ header
    PROJECT:    Studierstube/VT
    CONTENT:    simple Inventor based tracker test program
    VERSION:    1.5
   ===========================================================================
    AUTHORS:    ds      Dieter Schmalstieg
                rs      Rainer Splechtna
                jp      Jan Prikryl
   ===========================================================================
    HISTORY:

    22-Jan-01 23:41:59  jp      last modification
    22-Jan-01 23:41:03  jp      Added stdlib.h
=========================================================================== */

#include <assert.h>
#include <stdlib.h>

#include <Inventor/nodes/SoSeparator.h>

#include <stbapi/tracker/TrackerBuilder.h>
#include <stbapi/tracker/SoTrakEngine.h>
#include <stbapi/event3d/Stb3DEventGenerator.h>
#include <stbapi/util/option.h>
#include <stbapi/util/ivutil.h>
#include <stbapi/util/ivio.h>

#include <stbapi/resource/SoUserKit.h>

//---------------------------------------------------------------------------


int
main(int argc, char* argv[])
{
        // init
#ifndef USE_SOQT_LIB
    Widget window;
#else
    QWidget *window;
#endif

	if(!(window = SoXt::init("testviewer"))) exit(-1);
    initStbClasses();

#ifndef USE_SOQT_LIB
	printf("INFO: Fixing stencil plane problems \n"); 
    fixInventorVisual(window, FALSE, 1);
#endif

        // set and parse options
    TrackerBuilderOptions tbo;
    OptionString optUserKitName("userKit","uk","fullviewKit.iv");
    OptionString optSheetKitName("sheetKit","sk","");
    OptionFlag optTimings("timings","time");
    if(!Option::parse(argc,argv)) { printf("syntax error\n"); exit(-1); }
    TimerUtility::setSilence(!optTimings.isSpecified());

        // setup tracker and event generator
    TrackerBuilderParameter tbp = tbo.getParameter();
    Tracker* tracker = TrackerBuilder::createTracker(tbp);
    tracker->activate();
    Stb3DEventGenerator::init(tracker);
    Stb3DEventGenerator::activate();

        // load user kit
    SoUserKit *uk;
    SoNode *f = readNodeFromFile(optUserKitName.getValue());
    if (f && f->isOfType(SoUserKit::getClassTypeId())) {
        uk = (SoUserKit *)f;
        SoUserKit::checkInUser(uk);
    }
    else 
        exit(-1);

    SoSeparator *root = new SoSeparator;
    root->ref();
    root->addChild(uk);
        // set sheet geometry
    if (optSheetKitName.getValue() != "") {
        uk->getPip()->createResources(0);
        uk->getPip()->setPipGeometry(0,readNodeFromFile(optSheetKitName.getValue()));
        uk->getPip()->useResources(0);
    }
        // activate pen
    uk->getPen()->setEventRoot(root);
        // build viewer
    uk->getDisplay()->createViewer(root,TRUE,window);

    printf("starting fullview...\n");
    SoXt::mainLoop();
    return 0;
}

//---------------------------------------------------------------------------
//eof
