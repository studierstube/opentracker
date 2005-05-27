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
/** SoContextManagerKit node kit class definition
  *
  * @author Dieter Schmalstieg, Andreas Zajic
  *
  * $Id: SoContextManagerKit.h 4113 2004-12-21 14:27:35Z breiting $
  * @file SoContextManagerKit.h                                             */
 /* ======================================================================= */


#ifndef _SOCONTEXTMANAGERKIT_H_
#define _SOCONTEXTMANAGERKIT_H_

#include <Inventor/nodekits/SoBaseKit.h>

#include <stbapi/stbapi.h>
#include <stbapi/div/divmain.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

typedef int32_t SbAppID;

class SoApplicationKit;
class SoDivGroup;
class SoContextKit;
class SoPipSheetKit;
class SoHandle3DEventAction;
class SoVCMainPanelKit;

/** SoContextManagerKit application management class.
 * The node kit class of SoContextManagerKit functions as a managing class for application 
 * nodes. It manages the part of the scene graph where the application nodes reside. 
 * Furthermore it supplies methods for loading, saving, starting and stopping application 
 * to the programmer.
 * @ingroup workspace
 */
class STBAPI_API SoContextManagerKit : public SoBaseKit
{


    SO_KIT_HEADER(SoContextManagerKit);

    SO_KIT_CATALOG_ENTRY_HEADER(masterContexts);
    SO_KIT_CATALOG_ENTRY_HEADER(slaveContexts);


public:

    /** singleton accessor
     */
    static SoContextManagerKit* getInstance(
        );

    ~SoContextManagerKit(                   //destructor
        );

    /** get an application by its ID
     * @param appid application ID.
     * @return pointer to an SoApplication Kit.
     */
    SoApplicationKit* getApp(SbAppID appid);

    /** get the DIV-Group associated with an application.
     * @param appid application ID.
     * @return pointer to DIV-Group.
     */
    SoDivGroup* getAppGroup(SbAppID appid);

    /// get number of master/slave applications
    int getAppNum(SbBool isMaster);

    /// get total number of managed applications
    int getAppNum();

    /// returns pointer to the first application kit with the given name
    /// note that this searches the masters first, probably.
    SoApplicationKit * getAppByName( const SbName & name );

    void setMasterMode(                //set master/slave state
        SoDivGroup* appGroup,             //context to change
        SbBool isMaster                //new master state
        );                             //will insert ctx if not there already

 	CDivMain* getDivObjectFromNode(    //get a master div object for a given node
		SoNode* nodeInContext          //node to search for
		);

	// for the following SoContextKit* get*FromNode functions
	// see the corresponding SbBool isNodeIn*
    SoContextKit* getMasterContextFromNode(
		SoNode* nodeInMasterContext
		);

    SoContextKit* getSlaveContextFromNode(
		SoNode* nodeInSlaveContext
		);

    SoContextKit* getAnyContextFromNode(
		SoNode* nodeInAnyContext
		);

	SbBool isNodeInMasterContext(      //returns TRUE if
		SoNode* nodeInMasterContext    //nodeInMasterContext is associated with a master context
		);

	SbBool isNodeInSlaveContext(      //returns TRUE if
		SoNode* nodeInSlaveContext    //nodeInSlaveContext is associated with a slave context
		);

	SbBool isNodeInAnyContext(      //returns TRUE if
		SoNode* nodeInAnyContext    //nodeInMasterContext is associated with a master or slave context
		);
    
   static void initClass();
    
    SoPipSheetKit* getSheetFromNode(SoNode* nodeInSheet, SbAppID appID);

   /**
      Virtual Cockpit declarations
    */
    SoVCMainPanelKit* getMainPanelFromNode(SoNode* nodeInMainPanel, SbAppID appID);
    int getUserFromMainPanel(SoNode* nodeInMainPanel, SbAppID appID);
    SoNode* getMainPanel(int uid, SbAppID appID);

    int getUserFromSheet(SoNode* nodeInSheet, SbAppID appID);

    SoNode* getPipSheet(int uid, SbAppID appID);

    /** load an application from a file
     * @param fileName name of the loader file.
     * @param appList list of applications found in the loader file.
     * @param userID application owner.
     * @param locale name of the locale the application is associated with.
     * @return number of applications loaded from the file.
     */
	int loadApplication(
		const SbString fileName,
		SoNodeList& appList,
        int userID, const SbName & locale);

    /** load an application from a buffer
     * @param buffer pointer to the memory buffer.
     * @param buffersize size of the buffer.
     * @param appList list of applications found in the loader file.
     * @param userID application owner.
     * @param locale name of the locale the application is associated with.
     * @return number of applications loaded from the buffer.
     */
	int loadApplication(
		void* buffer,
        int buffersize,
		SoNodeList& appList,
        int userID, const SbName & locale);

    /** register an application node to the management system.
     * @param appGroup DIV-Group containing the application.
     * @param isMaster master mode flag (TRUE/FALSE).
     * @param locale name of locale associated with the application.
     * @return TRUE if registration was successful.
     */
    int registerApp(SoDivGroup *appGroup, SbBool isMaster, const SbName & locale);

    /** register PipSheets of ALL Applications for a given user
     * @param uid user identifier.
     */
    void registerPipSheets(int uid);

    /** register MainPanels for Virtual Cockpit
     * @param uid user identifier.
     */
    void registerMainPanels(int uid);

    /** save the application state to a file.
     * the state will be written to the loader .iv-file if the @see readOnly flag is not set.
     * Otherwise saveApplicationAs() is called.
     */
    void saveApplication(SbAppID appID);

    /** save the application state to a new file.
     * the name of the new file is created by attaching "_xxx" to the original loader
     * file's name. (xxx resembles a counter that is increased at each saving)
     */
    void saveApplicationAs(SbAppID appID);

    /** save the state of all applications */
    void saveAll();

    /** stop an application */
    SbBool stopApplication(SbAppID appID);

    /** shut down the Studierstube system */
    void shutDown();

    static void registerSlaveAppCB(
        void *data,
        SoSensor* s);
 
    long prepareApplication(const SbName & locale);


    void setDivParams( SbAppID appId, const SbString & address, int port,
                       SbBool active, SbBool master, SbBool initFromMaster  );
protected:

    static void deleteContextCB( void * data, SoSensor * sensor );

private:

    int loadApps(SoSeparator* loadSep, const SbString fileName, SoNodeList& appList,int userID,const SbName & locale);

    /** private constructor
     * only one instance of the SoContextManagerKit class can exist. It can only be accessed
     * using the singleton accessor method @see getInstance.
     */
    SoContextManagerKit();                  


    ///pointer to singleton instance 
    static SoContextManagerKit* instance;   

    ///action to set master/slave mode for all 3D aware nodes of a context
    SoHandle3DEventAction* modeAction; 
    friend class SoContextKit;
}; //class SoContextManagerKit

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOCONTEXTMANAGERKIT_H_

