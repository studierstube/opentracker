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
/**
 *
 * @author Hermann Wurnig, Gerd Hesina, Dieter Schmalstieg, Jan Prikryl
 *
 * $Id: StbWorkspace.h 4041 2004-11-23 16:51:45Z kaufmann $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _STBWORKSPACE_H_
#define _STBWORKSPACE_H_

#include <stbapi/div/StbDiv.h>
#include SOGUI_H
#include <stbapi/workspace/SoContextManagerKit.h>
#include "defines.h"

// time value in seconds used for widgets and draggers
// to snap back to the last valid state
#define SNAPBACK_TIME 2.0

#define GETDIVOBJECTFIELD(field) \
	CDivMain* theDivObject= \
        SoContextManagerKit::getInstance()->getDivObjectFromNode((SoNode*)field.getContainer());

#define GETDIVOBJECTNODE(node) \
	CDivMain* theDivObject= \
        SoContextManagerKit::getInstance()->getDivObjectFromNode(node);

#define DISABLEDIVFIELD(field) \
	{ \
        GETDIVOBJECTFIELD(field); \
        if(theDivObject) theDivObject->disableField(&field); \
    }
#define ENABLEDIVFIELD(field) \
	{ \
	    GETDIVOBJECTFIELD(field); \
        if(theDivObject) theDivObject->enableField(&field); \
    }

#define DISABLEDIVNODE(node) \
	{ \
	    GETDIVOBJECTNODE(node); \
        if(theDivObject) theDivObject->disableNode(node); \
    }

#define ENABLEDIVNODE(node) \
	{ \
	    GETDIVOBJECTNODE(node); \
        if(theDivObject) theDivObject->enableNode(node); \
    }

#define SETCHECKBOXSTATE(checkBox) \
	{ \
	    GETDIVOBJECTNODE(checkBox); \
        if(theDivObject) ((StbDiv*)theDivObject)->setCheckBoxState(checkBox); \
    }

#define SETSLIDERSTATE(slider, val) \
	{ \
	    GETDIVOBJECTNODE(slider); \
        if(theDivObject) ((StbDiv*)theDivObject)->setSliderState(slider, val); \
    }

#define SETBUTTONSTATE(button) \
	{ \
	    GETDIVOBJECTNODE(button); \
        if(theDivObject) ((StbDiv*)theDivObject)->setButtonState(button); \
    }

#define SETDIALSTATE(dial, val) \
	{ \
	    GETDIVOBJECTNODE(dial); \
        if(theDivObject) ((StbDiv*)theDivObject)->setDialState(dial, val); \
    }

#define SETRADIOBUTTONSTATE(button) \
	{ \
	    GETDIVOBJECTNODE(button); \
        if(theDivObject) ((StbDiv*)theDivObject)->setRadioButtonState(button); \
    }

#define SETDRAGKITSTATE(dragkit, trans, rot) \
	{ \
	    GETDIVOBJECTNODE(dragkit); \
        if(theDivObject) ((StbDiv*)theDivObject)->setDragKitState(dragkit, trans, rot); \
    }
	
#define SETDRAGSEPARATORSTATE(dragseparator, trans, rot) \
	{ \
	    GETDIVOBJECTNODE(dragseparator); \
        if(theDivObject) ((StbDiv*)theDivObject)->setDragSeparatorState(dragseparator, trans, rot); \
    }

// forward declarations
class SoUserKit;
class TimerUtility;
class SoTrakEngine;
class SoTimerSensor;

//----------------------------------------------------------------------------
// Workspace main class
//----------------------------------------------------------------------------

/**
 * This class is the main class in the Studierstube workspace. It initializes 
 * all resources such as the basic scene graph, the tracking system, event
 * handling, user initialization etc.
 * 
 * The scene graph built for the workspace, looks like this. For each user there
 * is a userRoot contained in the user. The StbWorkspace class itself builds the
 * common part of all this starting at the geometryRoot node.
 *@verbatim                                                            
 *  userRoot[0]   userRoot[1] .... userRoot[n] 
 *           \          |          /           
 *            \         |         /            
 *             ------   |   -----              
 *                   \  |  /                   
 *                  geometryRoot              
 *                 /     |       \
 *               /       |         \
 *         preRoot SoLocaleManager postRoot
 *                    
@endverbatim
 * preRoot and postRoot are Separators that can be used to add geometry and other
 * nodes to the scene graph for special purpose applications. These should not be used 
 * for regular application development.
 * @ingroup workspace
 * @ingroup core
 */
class STBAPI_API StbWorkspace
{
private:
    StbWorkspace(
        );

public:
    /** A root-node of auxiliary geometry used for special geometry
        (e.g. stationary or tracked props) which have to appear OUTSIDE of
        all windows and relative to the workspace origin (as opposed to
        window origin). DO NOT USE if you are an application programmer
        writing a normal Stb application, this is for special cases only!  
        This root node is to the left of the main scene graph that contains the user kits, 
        applications and windows.
    */
    SoGroup* preRoot;

	/** Separator for node(s) at the end of the scenegraph; useful for synchronization.
     * This post root is to the right of the main scene graph. Therefore its contents are
     * traversed last.
     */
	SoSeparator* postRoot;

    ~StbWorkspace(
        );
    static StbWorkspace* getInstance(
        );
    void parseOptions(
        int argc,
        char* argv[]
        );
    void initTracker(
        );
    void initGeometry(
        );
    void createManagers(
        );
    void initDisplay(
        );
    int loadApplets(
        );
    void createMessageCallback(
        );
    void mainLoop(
        );

    SbBool addUser(SoUserKit *uk, const SbName & locale);

    SbBool removeUser(SoUserKit *uk);

    const SbString & getWorkspaceDirectory(
        );

    const SbString & getSessionManagerAddress() {return sessionManagerAddress;};
    int getSessionManagerPort() {return sessionManagerPort;};

    const SbName & getDefaultLocale() { return defaultLocale; };

	/** returns the root node of the overall Studierstube scene graph. This
	 * is very dangerous information and should not be used by standard 
	 * applications. Do not use this, if you are not completely sure about
	 * what you are doing !!.
	 * @return pointer to the top level separator node.
	 */
	SoSeparator * getGeometryRoot() { return geometryRoot; };	

private:
    static void messageCB(
        void* d, 
        SoSensor* x
        );

//--global members
    static StbWorkspace* instance;
    SoSeparator* geometryRoot;         // root-node of common geometry
    int delay;
    int startApp;
    int sessionManagerPort;

    SbString sessionManagerAddress;
    SbString startAppName;
    SbString userKits;
    SbString workspaceDirectory;
    SbString trackerConfig;

    SbName joinLocale;
    SbName createLocale;	
    SbName defaultLocale;
    int defaultLocaleStation;

    SbString stuberena;                 // no, master, display.xx (xx number of display)

    SoTimerSensor *messageSensor;

    TimerUtility* eventTimer;          // timer for event rate

    //--manager instances
    /// application-manager object
    SoContextManagerKit* appManager;         

	// the widget holding the top-level window to use
	GuiWidget widget;

}; // class StbWorkspace

#endif//_STBWORKSPACE_H_

