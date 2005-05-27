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
/** SoUserKit.h contains the definition of class SoUserKit
  *
  * @author Rainer Splechtna
  *
  * $Id: SoUserKit.h 4141 2005-03-29 10:46:07Z kaufmann $
  * @file                                                                   
  */
 /* ======================================================================== */


#ifndef _SOUSERKIT_H_
#define _SOUSERKIT_H_

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoMFNode.h>

#include <stbapi/resource/viewer/SoDisplayKit.h>
#include <stbapi/workspace/appdef.h>

#include "SoPenKit.h"
#include "SoPipKit.h"

#include <stbapi/vc/SoVCKit.h>
#include <stbapi/vc/SoVCManager.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

// forward declarations
class StbSkinLoader;
class fileBrowser;
class SoWindowKit;
class So3DButton;

//----------------------------------------------------------------------------

/**
@example sampleUserKit.iv

This is a commented example for a SoUserKit.
*/
/** 
SoUserKit is a nodekit containing the resources (pen,pip,hmd) of a user
and provides handling for multiple instances of SoUserKit via static methods.
SoUserKit can be used in workspace or non-workspace applications. If used  for
non-workspace applications, only the static methods and the access-methods 
getPen(), getPip(), getDisplay() and getAuxGeometry() are useful.
@warning Don't use any other methods in a non-workspace application.

For use with the workspace environment SoUserKit provides a user-interface
created by the setupTabs() method. 
The user-interface consits of a system-bar for turning on/off the task-bar and
exiting the current application.
The task-bar shows all running applications. To switch to an application
(i.e. to switch to its pip-sheet and focus its window) just press the 
corresponding button. If the application has multiple contexts, a 
pulldown-menue will be shown, when the corresponding task-bar button is 
highlighted. To switch to one of the shown contexts, press the corresponding 
pulldown-button, or press the main task-bar button to switch to the first 
context in the pulldown-list. The currently active task (and/or context) is
marked by a special color of its task-bar button (default green). 
The window-bar provides manipulation of the currently focussed window.
There are buttons for minimizing, maximizing and turning the window-border 
on/off.

A commented example UserKit is given below:

@include sampleUserKit.iv

@author Rainer Splechtna
@ingroup workspace
@ingroup core

*/
class STBAPI_API SoUserKit
:
    public SoBaseKit
{
    SO_KIT_HEADER(SoUserKit);
    SO_KIT_CATALOG_ENTRY_HEADER(context);
    SO_KIT_CATALOG_ENTRY_HEADER(pen);
    SO_KIT_CATALOG_ENTRY_HEADER(pip);
    SO_KIT_CATALOG_ENTRY_HEADER(vc);
    SO_KIT_CATALOG_ENTRY_HEADER(display);
	SO_KIT_CATALOG_ENTRY_HEADER(auxGeometry);

public:
    /** Root of the user's scene graph. 
        (only used for storing)
      */
    SoSeparator *root;         
    /** Root of the user's exclusive geometry (geometry only displayed
	    for the user).
        note: only useful when used in the workspace environment
      */
	SoSeparator *exclusiveGeometryRoot;         

    /** indicator that a user is dragging
        used by StbWinManager for storing this inforamtion per user 
      */
    SbBool userDoesDrag;

    /** event to stop dragging again    
        used by StbWinManager for storing this inforamtion per user 
      */
    int dragReleaseEvent;

    /** data passed to application when drop occurs
        used by StbWinManager for storing this inforamtion per user 
      */
    winDropDataMessage dragData;

    /** Constructor of SoUserKit,
        inits fields and nodes of the nodekit.
      */
    SoUserKit(                 
        );
    /** Destructor of SoUserKit, 
        does some internal deletion and checks out the user.
        @see checkOutUser
      */
    ~SoUserKit(     
        );
    /** Inventor class initialization */
    static void initClass(   
        );

    /** Sets the userID of a SoUserKit.
        A userID can only be set before a SoUserKit is checked in.
       
        @param userID The user ID that should be set
        @return TRUE, if userID was set
      */
    int  setUserID(int userID);
    /** Returns the userID of a SoUserKit.

        @return userID of a SoUserKit
      */
    int  getUserID();
    /** This method performs all tasks neccessary for setting up a user for 
	    the workspace environment.

        This function must be called, before any other workspace-function of
        this class is invoked.
		It sets up the workspace user-interface tabs on the pip and
		activates the pen. The (user) root is also build by this method.

		@param geometryRoot the root of the geometry, which shall be
		                    displayed for every user
      */
	void setupUserForWorkspace(SoSeparator *geometryRoot);
    /** Workspace-function: Adds a new button to the task-bar.
        The appID has to be unique. The texture may be NULL, but this will
        affect the task-bar's self-describing nature.         
       
        @param appID The applicationID of the task, the button is associated
               with
        @param texture An icon-texture representing the application
      */
    void addTaskBarButton(SbAppID appID, SoTexture2 *texture);
    /** Workspace-function: Removes a button from the task-bar.
       
        @param appID The applicationID of the task, the button is associated
               with
      */
    void removeTaskBarButton(SbAppID appID);
       
    void addTaskBarElement(SbAppID appID);
    
    void removeTaskBarElement(SbAppID appID);

    /** A simple access-method returning the SoPipKit of a SoUserKit

        @return The SoPipKit of a SoUserKit (or NULL if it has none).
      */
    SoPipKit *getPip();
    /** A simple access-method returning the SoPenKit of a SoUserKit.

        @return The SoPenKit of a SoUserKit (or NULL if it has none).
      */
    SoPenKit *getPen();
    
    SoVCKit *getVC();  
    
    /** A simple access-method returning the SoDisplayKit of a SoUserKit.

        @return The SoDisplayKit of a SoUserKit (or NULL if it has none).
      */
    SoDisplayKit *getDisplay();    
    /** A simple access-method returning the root of the auxiliary 
	    geometry of the SoUserKit.

        @return Root of the auxiliary geometry of the SoUserKit.
      */
    SoSeparator *getAuxGeometry();
   
    /** A simple method to remove the content of the auxiliary 
	    geometry of the SoUserKit.
      */
	void clearAuxGeometry();

	/** Workspace-function: A simple access-method returning currently active
        application of a user.

        @return the applicationID of the currently active application
      */
    int getCurrentApp();

    /** Workspace-function: Stores the SoWindowKit, which is currently
        focussed by a user.

        @param window The window that has the focus for a user.
      */
    void setFocus(                   
        SoWindowKit *window
        ); 
    /** Workspace-function: A simple access-method returning the currently
        focussed window of a user.

        @return The window, that has the focus for a user
      */
    SoWindowKit *getFocus(           
        );       

    void setMainPanelGeometry(SbAppID appID, SoNode* mpGeom);    
            
    /** Workspace-function: Set the pip-geometry, i.e. the pip-sheet for an
        application (context or classic).

        @param appID The applicationID of the application                  
        @param pipGeom The pip-sheet to be set @see SoPipKit::setPipGeometry
        @param usePipCube A legacy parameter, decides whether the body of the
               pip should be rendered, when using this pip-sheet
      */
    void setPipGeometry(SbAppID appID, 
                        SoNode* pipGeom, 
                        SbBool usePipCube = TRUE);


    /** Workspace-function: Creates an initial pip-sheet for an application
        (context or classic) for a user, to register it.

        @param appID The applicationID of the application                  
        @param appName The name of the Application
      */
    void            createResources(SbAppID appID, SbName appName);

    /** Workspace-function: Switch to an application (context or classic),
        i.e. show its pip-sheet and focus its window for a user.

        @param appID The applicationID of the application                  
      */
    void             useResources(SbAppID appID);

    /** Workspace-function: Remove the pip-sheet of an application (context or
        classic) for a user.

        @param appID The applicationID of the application                  
      */
    void            freeResources(SbAppID appID);

	/** Inits workspace user-interface tabs.
      */
    void setupTabs();

    SoVCManager* getVCManager();

    /** user identification number of the "owner" of the SoUserKit
        each user must have an unique userID to be able to register it
      */
    SoSFInt32 userID; 

    /** user "identification" color during scene graph traversal
      * the currently active window for this user will be drawn with
      * this color (added 2002/07/02 ge) */
    SoSFColor color;

    /**
     * indicates that the user is supported by the local Studierstube process, as
     * opposed to being rendered by a remote host. This distinguishes which userkits
     * are removed / added when locales are joined / left.
     */
    SoSFBool local;

    fileBrowser* stbLoader;


private:	
    /** This field contains geometry exclusively displayed for this user.
	    It is only used to specify geometry within a file, and will only 
		be used within the workspace environment.
	*/
	SoMFNode exclusiveGeometry;      

    SoVCManager* vcManager;

    /** Enum to identify the buttons of the system-bar */
    enum systemButtonIDs {
        ACTIVE_TASK = 0,
        SYSTEM_MENU,
        SAVE_APP,
        SAVE_APP_AS,
        SAVE_ALL,
        CLOSE_APP
    };
    /** Enum to identify the buttons of the window-bar */
    enum winButtonIDs {
        WIN_BORDER = 0,
        WIN_MIN,
        WIN_MAX,
        WIN_RESTORE
    };

	SbBool systemBarLocked;
	SbBool systemBarMessageSent;
    /** pointer to the window that has the focus of user */
    SoWindowKit* focus;           
    /** applicationID of a user's current application */
    SbAppID currentApp;               

    /** pointer to the system-bar of a user */
    SoTabKit *systemBar;
    /** pointer to the task-bar of a user */
    SoTabKit *taskBar;
    /** pointer to the window-bar of a user */
    SoTabKit *windowBar;

    /** pointer to the skin used by a user. */
    StbSkinLoader *skin;

    
    /** Internal method used for texturing the window-bar buttons accordingly
        to the state and style of the focussed window for a user.
      */
    void setWindowButtons();

    /** Internal method used to call setWindowButtons for all registered
        users, which have the same focus. 

        @param focus, the window that changed its style or state
      */
    static void updateWindowButtons(SoWindowKit *focus);

    /** Callback function for all system-bar buttons.

        @param userData Data sent with the callback.
        @param button The button, which triggered the callback.
      */
    static void systemBarCB(
        void* userData, 
        So3DButton* button
        );

    /** Callback function for all task-bar buttons.

        @param userData Data sent with the callback
        @param button The button, which triggered trhe callback
      */
    static void taskBarCB(
        void* userData, 
        So3DButton* button
        );

    /** Callback function for all window-bar buttons.

        @param userData Data sent with the callback
        @param button The button, which triggered trhe callback
      */
    static void windowBarCB(
        void* userData, 
        So3DButton* button
        );

    /** Callback function for all pull-down-list buttons.

        @param userData Data sent with the callback
        @param button The button, which triggered trhe callback
      */
    static void pullDownCB(
        void* userData, 
        So3DButton* button
        );

    static void browseCB(
        void* data, 
        const char *filePath
        );

    static void systemMenuCB(
        void* userData,
        So3DButton* button);

    friend class SoUserManagerKit;
};

//----------------------------------------------------------------------------
#endif//_SOUSERKIT_H_

