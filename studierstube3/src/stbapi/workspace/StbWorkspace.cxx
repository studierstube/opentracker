/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** Startup Application of Workspace
  *
  * @author  Hermann Wurnig,  Gerd Hesina, Dieter Schmalstieg, Jan Prikryl
  *
  * $Id: StbWorkspace.cxx 4041 2004-11-23 16:51:45Z kaufmann $
  * @file                                                                   */
 /* ======================================================================= */

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <vector>
#include <string>
#include <ace/Reactor.h>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <cctype>

#include "StbWorkspace.h"

#include SOGUI_EXAMINERVIEWER_H

#include <Inventor/nodes/SoTransformSeparator.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoPointLight.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoFile.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/nodes/SoEnvironment.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/SoPath.h>
#include <Inventor/SoInput.h>

#include <stbapi/util/ivio.h>
#include <stbapi/util/os.h>
#include <stbapi/util/ivutil.h>
#include <stbapi/util/timerutility.h>
#include <stbapi/interaction/So3DSeparator.h>
#include <stbapi/workspace/SoContextManagerKit.h>
#include <stbapi/resource/SoUserKit.h>
#include <stbapi/resource/SoUserManagerKit.h>
#include <stbapi/workspace/distrManager/StbDistrManager.h>
#include <stbapi/util/option.h>
#include <stbapi/event3d/Stb3DEventGenerator.h>
#include <stbapi/tracker/TrackerOT.h>
#include <stbapi/tracker/SoTrakEngine.h>

#include <stbapi/workspace/SoLocaleManagerKit.h>
#include <stbapi/workspace/SoLocaleKit.h>

using namespace std;

//----------------------------------------------------------------------------
// static members

StbWorkspace* StbWorkspace::instance = NULL;

//----------------------------------------------------------------------------

StbWorkspace::StbWorkspace()
{
    // some environment variables fixing for differen stuff
#ifdef USE_SOWIN
    ACE_OS::putenv("SOWIN_MSGS_TO_CONSOLE=1");
#endif
    // init toolkit and Open Inventor
    if ((this->widget = SoGui::init("Studierstube")) == NULL) 
        exit(1);
    // init Studierstube Open Inventor classes
    initStbClasses();
    // add system directory to directory stack
    char * line = ACE_OS::getenv("STBROOT");
    if( line != NULL )
    {
        SbString stbroot(line);
        stbroot += "/bin/workspace";
        printf("INFO: system dir set to %s\n", stbroot.getString());
        SoInput::addDirectoryLast (stbroot.getString());
    }
}

//----------------------------------------------------------------------------
StbWorkspace*
StbWorkspace::getInstance()
{
	if(instance == NULL) instance = new StbWorkspace;
    return instance;
}
//----------------------------------------------------------------------------

const SbString &
StbWorkspace::getWorkspaceDirectory()
{
    return workspaceDirectory;
}

//----------------------------------------------------------------------------
// here we initialize the geometry-nodes for the various users and
// their tracker-stations

void
StbWorkspace::initGeometry()
{
    int i;
    SoInput::addDirectoryFirst(".");  

    geometryRoot = new SoSeparator;
    geometryRoot->ref();
    
    preRoot = new SoGroup;
    geometryRoot->addChild(preRoot);

    // add the singleton locale manager to the geometry root
    // is created here probably.
    geometryRoot->addChild(SoLocaleManagerKit::getInstance());
    // create default locale for this host
    SoLocaleKit * defLoc = SoLocaleManagerKit::getInstance()->createLocale(defaultLocale);
    if (defaultLocaleStation > -1) defLoc->station.setValue(defaultLocaleStation);

    string create(createLocale.getString());

    if (create[0] != '\0'){
        string locale;
        int start = 0;
        unsigned int end = create.find(';');
        while( end != string::npos )
        {
            locale = create.substr( start, end - start );
            if( locale.length() > 0 )
            {
                unsigned int token = locale.find(':');
                int station = -1;
                if( token != string::npos )
                {
                    station = atoi( locale.substr( token+1, locale.length() - token - 1).c_str());
                    locale = locale.substr(0, token );
                }
                SoLocaleKit * localeKit = SoLocaleManagerKit::getInstance()
                    ->createLocale( locale.c_str());
                if( station != -1 )
                    localeKit->station.setValue( station );
                printf("SETUP: Created locale %s - %d\n", locale.c_str(), station);
            }
            start = end+1;
            end = create.find(';', start );
        }
        locale = create.substr( start, create.length() - start);
        unsigned int token = locale.find(':');
        int station = -1;
        if( token != string::npos )
        {
            station = atoi( locale.substr( token+1, locale.length() - token - 1).c_str());
            locale = locale.substr(0, token );
        }
        SoLocaleKit * localeKit = SoLocaleManagerKit::getInstance()
            ->createLocale( locale.c_str());
        if( station != -1 )
            localeKit->station.setValue( station );
        printf("SETUP: Created locale %s - %d\n", locale.c_str(), station);
    }
    
    //zaa
    SoContextManagerKit::getInstance()->ref();
    
    // check stuberena option for tiled display
    // option --stuberena or -sr
    // argument: filename - blending mask of tile

    postRoot = new SoSeparator;
    if (0 != stuberena.getString()[0])
    {
        printf("SETUP: starting as tiled display, loading file %s\n",stuberena.getString());

        SoSeparator *renaMask = readFile(stuberena.getString(),workspaceDirectory.getString());
        if(NULL == renaMask)
        {
            printf("WARNING: failed to load %s.\n",stuberena.getString());
        }
        else
        {
            postRoot->addChild(renaMask);
        }
    }
    geometryRoot->addChild(postRoot);


    // load user kits from file
    printf("SETUP: loading SoUserKit(s) from file %s\n", userKits.getString());
    SoSeparator *fileRoot = readFile(userKits.getString(),workspaceDirectory.getString());
    if( fileRoot != NULL )
    {
        //assert(fileRoot);
        fileRoot->ref();
        
        // search for SoFile nodes
        SoSearchAction sAction;
        sAction.setType(SoFile::getClassTypeId());
        sAction.setSearchingAll(TRUE);
        sAction.setInterest(SoSearchAction::ALL);
        sAction.apply(fileRoot);
        SoPathList paths = sAction.getPaths();
        
        // extract content of file nodes
        for (i=0;i<paths.getLength();i++)
        {
            if (paths[i]->getTail()->isOfType(SoFile::getClassTypeId()))
            {
                SoGroup *fileContent =
                    ((SoFile *)paths[i]->getTail())->copyChildren();
                fileRoot->replaceChild(paths[i]->getTail(),fileContent);
            }
        }
        
        // set root nodes for all tracker that generate events
        TrackerOT * tracker = Stb3DEventGenerator::getTracker();
        for( i = 0; i < tracker->getMaxStationNumber(); i++ )
        {
            if( tracker->isEventEnabled( i ) == 1 )
            {
                Stb3DEventGenerator::setRoot( i, geometryRoot );
            }
        }
        
        // search for SoUserKits
        fileRoot->ref();
        sAction.reset();
        sAction.setType(SoUserKit::getClassTypeId());
        sAction.setSearchingAll(TRUE);
        sAction.setInterest(SoSearchAction::ALL);
        sAction.apply(fileRoot);
        paths = sAction.getPaths();
        fileRoot->unref();

        // add found SoUserKits to workspace
        for (i=0;i<paths.getLength();i++)
        {
            if (!addUser((SoUserKit *)paths[i]->getTail(), StbWorkspace::getInstance()->getDefaultLocale()))
            {
                printf("WARNING: Could not check in %i. user !\n", i);
                assert(0);
            }
            else
                StbDistrManager::getInstance()->addUser((SoUserKit *)paths[i]->getTail());
        }
    }   
}

//----------------------------------------------------------------------------

SbBool
StbWorkspace::addUser(SoUserKit *uk, const SbName & locale)
{
    if (SoUserManagerKit::getInstance()->checkInUser(uk, locale ))
    {
        // create user-root and init workspace tabs and activate pen
        uk->setupUserForWorkspace(geometryRoot);
        printf("INFO: user with userID %d entered workspace\n",
            uk->getUserID());
        
        // generate pipsheets for all applications
        SoContextManagerKit::getInstance()->registerPipSheets(uk->getUserID());
        
        return TRUE;
    }
    return FALSE;
}

//----------------------------------------------------------------------------

SbBool
StbWorkspace::removeUser(SoUserKit *uk)
{
    uk->ref();
    if (SoUserManagerKit::getInstance()->checkOutUser(uk))
    {
        if( uk->getDisplay() != NULL && uk->getDisplay()->getViewer() != NULL)
        {
            delete uk->getDisplay()->getViewer();
            printf("INFO: viewer destructed for user %d\n", uk->userID.getValue());
        }
        uk->unref();
        return TRUE;
    }
    uk->unref();
    return FALSE;
}

//----------------------------------------------------------------------------

void
StbWorkspace::messageCB(void* d, SoSensor* x)
{    
    Stb3DEventGenerator::sample();
    StbWorkspace::getInstance()->eventTimer->timeThis();
    // handle system events etc, for up to 5 ms
    static ACE_Time_Value tv(0, 50);
    if( ACE_Reactor::instance()->work_pending())
        ACE_Reactor::instance()->handle_events(tv);        
}

//----------------------------------------------------------------------------

void
StbWorkspace::parseOptions(int argc, char* argv[])
{
    OptionString optApp("application", "a", "");
    OptionString optWorkspaceDirectory("workspacedir","wd","./");

    OptionString optSMAddress("smaaddress", "smo","127.0.0.1");
    OptionLong optSMPort("smport","smp",1234);

    OptionString optUserKits("userkits", "uk","defaultUserKit.iv");

    OptionFlag optTimings("timings","time");

	OptionString optOpenTrackerConfig("tracker", "tr", "defaultTracker.xml");

	OptionString optJoinLocale("joinlocale", "jl", "");
	OptionString optCreateLocale("createlocale", "cl", "");
    OptionString optDefaultLocale("defaultlocale", "dl", "default");
    OptionString optStuberena("stuberena","sr","");

    if(!Option::parse(argc,argv)) { Option::printUsage(); exit(-1); }

    TimerUtility::setSilence(!optTimings.isSpecified());

    userKits = optUserKits.getValue();

	trackerConfig = optOpenTrackerConfig.getValue();

    stuberena = optStuberena.getValue();

    if(optApp.isSpecified())
    {
        startApp=1;
        startAppName = optApp.getValue();
    }

    workspaceDirectory = optWorkspaceDirectory.getValue();

    joinLocale = optJoinLocale.getValue();
    createLocale = optCreateLocale.getValue();

    string defaultname(optDefaultLocale.getValue());

    unsigned int token = defaultname.find(':');
    if( token != string::npos )
    {
        defaultLocaleStation = atoi( defaultname.substr( token+1, defaultname.length() - token - 1).c_str());
        defaultname = defaultname.substr(0, token );
    }
    else {
        defaultLocaleStation = -1;
    }

    defaultLocale = SbName(defaultname.c_str());

    sessionManagerAddress = optSMAddress.getValue();
    sessionManagerPort = optSMPort.getValue();
}

//----------------------------------------------------------------------------

void
StbWorkspace::initTracker()
{
    SbBool test;
    SoInput::addDirectoryLast(workspaceDirectory.getString());
    if( checkFile( trackerConfig, test ))
    {
        printf("INFO: reading tracker configuration %s\n", trackerConfig.getString());
    }
    else
    {
        printf("WARNING: could not read tracker config file %s\n", trackerConfig.getString());
    }
    TrackerOT * tracker = new TrackerOT( trackerConfig.getString() );
    tracker->activate();
    Stb3DEventGenerator::init( tracker );
    eventTimer = new TimerUtility("EVENTGENERATOR",5000);
    SoInput::removeDirectory(workspaceDirectory.getString());
}

//----------------------------------------------------------------------------
// create manager-objects

void
StbWorkspace::createManagers()
{
    appManager = SoContextManagerKit::getInstance();
}

//----------------------------------------------------------------------------

void
StbWorkspace::initDisplay()
{
    for(int i=0; i < SoUserManagerKit::getInstance()->getUserNumber(); i++)
    {
        SoUserKit *uk = SoUserManagerKit::getInstance()->getUserKitSequentially(i);
        if(uk->getDisplay() && uk->getDisplay()->isRendering.getValue())
		{
            printf("INFO: creating viewer for user with ID %d:\n",
                          uk->getUserID());
            // first call uses the new widget gathered by SoGui::init() for all
            // others let the viewer be created with a new toplevel widget by calling
            // it with NULL.
            uk->getDisplay()->createViewer(uk->root, (i == 0) ? this->widget : NULL);
        }
		else
            printf("INFO: viewer for user with ID %d was not created\n",
			              uk->getUserID());
    }
}

//----------------------------------------------------------------------------

int 
StbWorkspace::loadApplets()
{
    StbDistrManager::getInstance();

    if (startApp == 1) {
        SbString startapp(startAppName);
        SoNodeList alist;
		SoUserKit * uKit = SoUserManagerKit::getInstance()->getUserKitSequentially(0);
		int uID = (uKit != NULL)?(uKit->getUserID()):(-1);
        if (appManager->loadApplication(startapp,alist, uID, getDefaultLocale()) < 0)
			return -1;
    
	}

    string join(joinLocale.getString());
    if( join[0] != '\0' )
    {
        string locale;
        int start = 0;
        unsigned int end = join.find(';');
        while( end != string::npos )
        {
            locale = join.substr( start, end - start );
            if( locale.length() > 0 )
            {
                StbDistrManager::getInstance()->joinLocale(locale.c_str());
                printf("SETUP: joined locale %s\n", locale.c_str());
            }
            start = end+1;
            end = join.find(';', start );
        }
        locale = join.substr( start, join.length() - start);
        if( locale.length() > 0 )
        {
            StbDistrManager::getInstance()->joinLocale( locale.c_str());
            printf("SETUP: joined locale %s\n", locale.c_str());
        }
    }
	return 0;
}

//----------------------------------------------------------------------------

StbWorkspace::~StbWorkspace()
{
    StbDistrManager::getInstance()->shutDown();    
	SoContextManagerKit::getInstance()->shutDown();
    if(messageSensor) delete messageSensor;
    printf("INFO: Exiting Studierstube\n");
    delete eventTimer;
    Stb3DEventGenerator::deactivate();
    Stb3DEventGenerator::deleteGlobalTrackerFields();
}

//----------------------------------------------------------------------------

void
StbWorkspace::createMessageCallback()
{
    messageSensor = new SoTimerSensor(messageCB,NULL);
    messageSensor->setBaseTime(SbTime::getTimeOfDay());
    messageSensor->setInterval(0.005);
    messageSensor->schedule();
}

//----------------------------------------------------------------------------

void
StbWorkspace::mainLoop()
{
    SoGui::mainLoop();
}
//----------------------------------------------------------------------------

/* ===========================================================================
    End of StbWorkspace.cxx
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
