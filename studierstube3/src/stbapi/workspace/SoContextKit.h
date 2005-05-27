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
/** SoContextKit.h contains the definition of the SoContextKit node kit class.
  *
  * @author Dieter Schmalstieg, Andreas Zajic
  *
  * $Id: SoContextKit.h 4113 2004-12-21 14:27:35Z breiting $
  * @file SoContextKit.h
  */
 /* ======================================================================= */
#ifndef _SOCONTEXTKIT_H_
#define _SOCONTEXTKIT_H_

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <stbapi/div/SoDivGroup.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class So3DEvent;
class SoHandle3DEventAction;
class SoPipSheetKit;
class SoWindowKit;
class SoApplicationKit;
class SoSensor;
class SoVCMainPanelKit;

typedef void AppMessageCB(void* userData,int message,int sender,void* data);

//----------------------------------------------------------------------------

/** SoContextKit is the foundation class for Studierstube apps.
 * The SoContextKit node kit class represents the actual application node containing
 * the applications functionality. SoContextKit acts as a base class to be used by the 
 * programmer to implement a custom application by deriving a new subclass from SoContextKit.
 *
 * Additionally the SoContextKit is supplied with window geometry associated with the 
 * application, and pip sheet geometry. 
 * Both window and pip sheet geometry are usually passed to the application using the loader
 * file, but it is also possible to hardcode the construction of the geometry into the 
 * application node, though it is not recommended.
 *
 * StbAPI V2.0 Docs can be found at http://www.studierstube.org/stbapi/SoContextKit.html
 * @ingroup workspace
 * @ingroup core
 */
class STBAPI_API SoContextKit
:
public SoBaseKit
{
    SO_KIT_HEADER(SoContextKit);

    SO_KIT_CATALOG_ENTRY_HEADER(context);
    SO_KIT_CATALOG_ENTRY_HEADER(windowGroup);
    
    SoSFString templateMainPanelFilename;

    /** pip sheet geometry associated with the application.
    */
	SoSFNode templatePipSheet;

    /** pip sheet geometry flag.
     * TRUE: the pip sheet template is copied for every user.
     * FALSE: the pip sheet geometry is shared among all users.
     */
    SoSFBool clonePipSheet;
    
    SoSFBool cloneMainPanel;

    /** master mode flag.
     * TRUE: context in master mode.
     * FALSE: context in slave mode.
     */
    SbBool isMaster;

	/** trigger to exit application
	*/
	SoSFTrigger exitTrigger;

public:

    /** application identifier (long integer value).
     */
    SoSFInt32 appID;

    /** user identifier of the application owner.
     */
    SoSFInt32 userID;

    /** standard Open Inventor node kit class initialization.
     */
    static void initClass(
        );

    /** constructor.
     */
    SoContextKit(
        );

    /** destructor.
     */
    virtual ~SoContextKit(
        );

    /** initialization of the application.
     * @param masterMode application mode (TRUE = master mode)
     */
	SbBool initContext(
        SbBool masterMode
        );

    /** register a pip sheet for a given user.
     * @param uid user identifier.
     * @return TRUE if registration was successful.
     */
    SbBool registerPipSheet(int uid);
    
    SbBool registerMainPanel(int uid);
    
    void exitContext();

    /** 
     * this method is called to notify the ContextKit of master/slave mode
     * changes. It is also called after initialization to set the
     * initial master/slave mode. Make sure to call the superclass method
     * as it handles some additional master/slave mode calls !
     * @param masterMode the new mode
     */
    virtual void setMasterMode(        //override this to implement your
        SbBool masterMode              //own master/slave behavior
        );
        
    static SbBool staticWindowEventCB( //callback function that recognizes
        void* data,                    //that window got focus and switches
        int messageId,                 //pip sheet accordingly. Then forwards
        SoWindowKit* win,              //message to user's winCBFunc
        int uid,
        So3DEvent* event, 
        SoPath* path
        );

    /** get the name of the application.
     * @return name of the application.
     */
    SbString getAppName(               

        );

    /** get the identifier of the application owner.
     * @return user ID of application owner.
     */
    int getUserId(                     

        );

    /** get current application mode.
     * @return TRUE if master mode, FALSE if application is in slave mode.
     */
    SbBool getMasterMode(              //return master mode
        );

    /** get a window associated with the application.
     * @param i index of window.
     * @return pointer to an SoWindowKit.
     */
    SoWindowKit* getWindow(            
        int i                          
        );

    /** get number of windows owned by the application.
     * @return current number of windows.
     */
    virtual int getNumWindows(         
        );                             

    virtual void closeWindow(
        int i
        );
    
    /** get the pip sheet of a user
     * @param uid user ID.
     * @return pip sheet of user with ID <uid>.
     */
    SoNode* getPipSheet(
        int uid
        );
        
    SoNode* getMainPanel(int uid);

    /** get the user ID of the user assiociated with a pip sheet from a node.
     * @param nodeInSheet a node from the pip sheet of the user.
     * @return user identifier.
     */
    int getUserFromSheet(              
        SoNode* nodeInSheet            
        );

    /** get the pip sheet of which a given node is a member.
     * @param nodeInSheet a node contained by a pip sheet.
     * @return pip sheet of which nodeInSheet is a member.
     */
    SoPipSheetKit *getSheetFromNode(   
        SoNode* nodeInSheet            
        );

    SoVCMainPanelKit* getMainPanelFromNode(SoNode* nodeInMainPanel);

    void forceFocusChange(             //called by context manager
        );                             //to change focus
    void forceFocusChange(int userID   //called by user kit
        );                             //to change focus

    /** check if node is contained by application.
     * @param nodeInContext node to be searched for.
     * @return TRUE if node is contained in application.
     */
    SbBool isNodeInContext(            
		SoNode* nodeInContext          
		);

    /** check if node is contained by a pip sheet associated with the application.
     * @param nodeInContext node to be searched for.
     * @return TRUE if node is contained in the applicatiions pip sheets.
     */
    SbBool isNodeInSheet(              
		SoNode* nodeInSheet            
		);

    /** check if node is associated with the application.
     * @param nodeInContext node to be searched for.
     * @return TRUE if node is associated with the application.
     */
    SbBool isNodeAssociated(           
		SoNode* associatedNode         
		);

    /// Div accessor just another way to get at the AppGroup.
	SoDivGroup * getDivObject()
    {
        return agroup;
    }


    /// AppGroup accessor.
    SoDivGroup * getAppGroup() 
    {
        return agroup;
    }

    /** get the SoApplicationKit that contains the current SoContextKit.
     * @return pointer to the SoContextKit's SoApplicationKit wrapper.
     */
    SoApplicationKit * getAppKit() {return (SoApplicationKit*)(agroup->getChild(0));}

protected:

    void setAppGroup(SoDivGroup* appGroup);

    /** check the pip geometry.
     * this method is used to check the pip geometry that is usually passed to the
     * application from a loader .iv-file. It is called upon initialization of the
     * SoContextKit instance after loading and should be overridden by the programmer
     * to implement custom pip sheet geometry checking.
     */
    virtual SbBool checkPipGeometry();

    virtual SbBool checkMainPanelGeometry();
    
    /** check the window geometry.
     * This method is used to check the window geometry passed to the application from
     * the associated loader .iv-file. It is called upon initialization of the
     * SoContextKit instance after loading and should be overridden by the programmer
     * to implement custom window geometry checking.
     *
     * hint: by overriding this method the programmer can easyly implement automatic creation
     * of an SoWindowKit if none is passed from the loader file.
     */
    virtual SbBool checkWindowGeometry();

    /** check pip sheet connections for a given user.
     * This method should be overridden by the programmer to set up connections of pip
     * geometry elements for the specified user, which can not be specified by using a
     * template geometry in the loader file.
     * It is called after a pip sheet is set up for a user.
     */
    virtual void checkPipConnections( int uid, SoNode * pipSheetContents );

    /** behaviour when application mode changes.
     * The checkPipMasterMode(...) method is called whenever the mode of the application
     * changes. It is used by the programmer to implement the custom behaviour of the pip
     * sheet when the applications mode changes.
     */
    virtual void checkPipMasterMode( SoNode * pipSheetContents, SbBool masterMode );
    
    virtual void checkMainPanelConnections(int uid, SoNode* mainPanelContents);

    virtual void checkMainPanelMasterMode(SoNode* mainPanelContents, SbBool masterMode);

    virtual SbBool windowEventCB(      //override this to get your own
        void* data,                    //window message behavior
        int messageId,
        SoWindowKit* win,
        int uid,
        So3DEvent* event, 
        SoPath* path
        );

    /** set mode for a given pip sheet.
     * can be overridden by the programmer to implement custom behaviour.
     * @param pipSheet pip sheet.
     * @param masterMode TRUE/FALSE.
     */
    void setSheetMasterMode(   
        SoNode* pipSheet,              
        SbBool masterMode              
        );               
    
    void setMainPanelMasterMode(SoNode* mpk, SbBool masterMode);
    
    /** call this to delete a context from within a callback. It will
     * schedule a one shot sensor that calls deleteContextCB which will
     * do the actual work.
     */
    void scheduleDelete();                             

    /** override this to react to focus change.
     * @param uid user for which focus has changed.
     * @param focus TRUE or FALSE to show whether focus was gained or lost.
     */
    virtual void focusChange( int uid, SbBool focus  );
    
    /** create and set a unique node name.
     * @param node node to manipulate.
     * @param uid user identifier.
     */
	void setUniqueNodeName(            
		SoNode* node,                  
		int uid                        
		);

    /** create a unique node name.
     * @param templateString if empty use "EMPTY" as template.
     * @param uid user identifier.
     * @return unique node name constructed using template string and user ID.
     */
    SbString& makeUniqueNodeName(     
		SbString templateString,      
		int uid                       
		);

    /** callback function called when application mode changes
     */
	static void masterChangeCB( void * data, SoSensor * sensor );

// not static

    /// action to switch master/slave mode of pip sheet widgets
    SoHandle3DEventAction* modeAction; 
    
    /// the basic container for an application and distribution root
    SoDivGroup * agroup;  

	SoFieldSensor exitTriggerSensor;
	static void exitTriggerCB( void * userdata, SoSensor * sensor );

private:

    ///flag to prevent multiple initClass()
    static SbBool baseInitFlag;        

    friend class SoContextManagerKit;
    friend class SoUserKit;

}; //class SoContextKit

//----------------------------------------------------------------------------

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOCONTEXTKIT_H_

