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
/** SoUserManagerKit.h contains the definition of class SoUserManagerKit
  *
  * @author Andreas Zajic
  *
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef _SOUSERMANAGERKIT_H_
#define _SOUSERMANAGERKIT_H_

#include <Inventor/nodekits/SoBaseKit.h>
#include <stbapi/resource/SoUserKit.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * @ingroup workspace
 */
class STBAPI_API SoUserManagerKit
:
public SoBaseKit
{
    SO_KIT_HEADER(SoUserManagerKit);
    SO_KIT_CATALOG_ENTRY_HEADER(userKits);
public:	
	static SoUserManagerKit* getInstance();

    static void initClass();
    
    ~SoUserManagerKit();

    /** @name multiple userkits
     * methods to handle multiple userkits */
    //@{ 

    /** Register a user, checks if the userID of the SoUserKit is unique.

        @param newUserKit the SoUserKit to be registered.
        @return TRUE, if the SoUserKit was successfully registered.
      */
    int checkInUser(SoUserKit *newUserKit, const SbName & locale );
    /** Remove a SoUserKit from the global SoUserKit-register.
        @warning The SoUserKit will be deleted, if it is not referenced.

        @param oldUserKit the SoUserKit to be registered.
        @return TRUE, if the SoUserKit was successfully registered.
      */
    int checkOutUser(SoUserKit *oldUserKit);
    /** Get the number of all registered SoUserKits.
        @return number of all registered SoUserKits.
      */
    int getUserNumber();

    /** Returns a pointer to a SoUserKit from the register, specifying its
        userID.

        @param userID the userID of the desired SoUserKit
        @return a pointer to the SoUserKit with userID, or NULL if
                no SoUserKit was found in the register with userID
      */
    SoUserKit *getUserKit(
          int userID
          );
    /** Returns a pointer to a SoUserKit from the register, specifying its
        internal sequential order index.
        Use this method, if you want to perform an operation on all registered
        SoUserKits. This is an example for the usage of this method:
        (for int i = 0; i<SoUserKit::getUserNumber();i++)
        {
            SoUserKit::getUserKitSequentially(i)-> ... 
            ...
        }

        @param index the sequential index of the SoUserKit
        @return a pointer to the SoUserKit with given index
      */
    SoUserKit *getUserKitSequentially(
          int index
          );
    /** Returns the internal index of a registered SoUserKit.

        @param  userID the userID of a registerd SoUserKit
        @return index the internal sequential index of the SoUserKit or -1 if
                the SoUserKit is not registered
      */
    int getUserKitIndex(
          int userID
          );
    /** Returns a pointer to an array of all registered userIDs.

        @return a pointer to an array of all registered userIDs.
      */
    const int32_t *getUserIDs();

    /** Returns the userID of the user that uses the given trackerstation for
        his pen, pip or display(hmd)

        @return userID of user 
      */
	int	getUserFromStation(int station);
	
    /** Returns the trackerstation of the pen of a user.
	     Returns -1 if no such user exists or the user does not use a tracked
        pen.

        @param userID of user
        @return trackerstation of the pen of a user
      */
	int	getPenFromUser(int userID);

    /** Returns the trackerstation of the pip of a user.
	     Returns -1 if no such user exists or the user does not use a tracked
        pip.

        @param userID of a user
        @return trackerstation of the pip of a user
      */
	int	getPipFromUser(int userID);

    /** Returns the trackerstation of the hmd of a user.
	     Returns -1 if no such user exists or the user does not use a tracked
        hmd.

        @param userID of user
        @return trackerstation of the hmd of a user
      */
	int	getHmdFromUser(int userID);
        
        int getVCFromUser(int userID);               

    /** returns a userkit by searching for a node contained in one.
     * This method is useful to get the userkit from a node in a pipsheet
     * etc. If no userkit containing the node is found, it returns NULL.
     */
    SoUserKit * getUserKitFromNode(SoNode * node);

        /** an array which stores all checked in userIDs
      */
    SoMFInt32 userIDs;

    //@}

    /** @name distribution methods
     * These methods act on userkit messages received from the 
     * distribution. */
    //@{
    void windowBarChanged( int userId, int style, int state );

    void sheetBarChanged(int userId, SbAppID appId, int whichChild );

	void taskBarChanged( int userId, int buttonId );    

  	void systemBarChanged( int userId, int buttonId, int tabInfo );

    void focusChanged( int userId, SbAppID appId );
    //@}

private:
    static SoUserManagerKit* instance;
    SoUserManagerKit();

};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif
