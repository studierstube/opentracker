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
/** SoPipKit.h contains the definition of class SoPipKit
  *
  * @author Rainer Splechtna
  *
  * $Id: SoPipKit.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef _SOPIPKIT_H_
#define _SOPIPKIT_H_

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFVec2f.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <stbapi/workspace/SoContextManagerKit.h>

#include "SoTrackedArtifactKit.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoPipSheetKit;
class SoTabKit;

//----------------------------------------------------------------------------
/** SoPipKit is a nodekit representing a PIP used for interaction in the
studierstube environment.

This nodekit inherits from SoTrackedArtifact and adds functionality for handling
multiple PIP-sheets (add,use,remove).
@author Rainer Splechtna
@ingroup interaction
@ingroup core
 */
class STBAPI_API SoPipKit
:
    public SoTrackedArtifactKit
{
    SO_KIT_HEADER(SoPipKit);
    SO_KIT_CATALOG_ENTRY_HEADER(system);    // system tab bar
    SO_KIT_CATALOG_ENTRY_HEADER(task);      // task tab bar
    SO_KIT_CATALOG_ENTRY_HEADER(window);      // window tab bar
    SO_KIT_CATALOG_ENTRY_HEADER(pipSheets); // SoPipSheetKits

public:
    /** This field specifys the size of the PIP that should be used.

      The PIP's geometry has to be placed around the origin (in object space).
      The PIP's surface should be parallel to the xy-plane (in object space).
    */
    SoSFVec3f pipSize;
    /** This field specifys the upper left corner of the sheet area.

      The sheet area is the area on the PIP which a PIP-sheet should use.
      It is specified in normalized PIP-coordinates, ranging from (0,0) for
      the upper left corner of the PIP to (1,1) for the lower right corner
      of the PIP.	
    */  
    SoSFVec2f sheetAreaUpperLeft;
    /** This field specifys the lower right corner of the sheet area.

      The sheet area is the area on the PIP which a PIP-sheet shoulduse.
      It is specified in normalized PIP-coordinates, ranging from (0,0) for
      the upper left corner of the PIP to (1,1) for the lower right corner
      of the PIP.	
    */  
    SoSFVec2f sheetAreaLowerRight;
    /** Specifys the name of a skin-file, that describes the properties
        of the user-interface elements on the PIP. 
	  */
    SoSFString skinFileName;
    /** Specifys the directory of a skin-file, that describes the properties
        of the user-interface elements on the PIP. 	
    */
    SoSFString skinDirectory;

    /**
        Field to show/hide the system ans task bar.
        */
    SoSFBool systemBar;

    /**
        Field to show/hide the Window bar.
    */
    SoSFBool windowBar;

    /** The constructor inits the fields of the kit an enables all 3D events.
     */
    SoPipKit(   
        );
    /** The destructor releases all grabbers of the PIP
     */
    ~SoPipKit(
        );
    /** Inventor class initialization */
    static void initClass(
        );

    /** This method is used to create an initial PIP-sheet for an application
        (and context).
	    
        Before a PIP-sheet can be set or used for an application (and context),
        this method has to be called. It works like a kind of registration,
        which stores basic information using an empty PIP-sheet.
        For non-workspace applications using only one PIP-sheet, just use an
        arbitrary applicationID and don't specify a context.
        @param appID applicationID
      */
    void createResources(SbAppID appID);
    /** This method is used to free (delete) a PIP-sheet of an application
        (and context).
	    
        The current PIP-sheet kit of this application (context) will be
        removed from the PIP.
        @warning If only an appID is specified, all contexts, which are
                 associated with this application will also be deleted.
        @param appID applicationID
      */
    void freeResources(SbAppID appID);
    /** This method is used to switch to a PIP-sheet of an application
        (and context).
	    
        The PIP-sheet of the specified application (context) will be
        displayed on the PIP.
        @param appID applicationID
      */
    void useResources(SbAppID appID);

    /** Set the PIP-geometry (the PIP-sheet) for the specified application 
        (context).

        If pipGeom is not a SoPipSheetKit, a new SoPipSheetKit is created
        and the given geometry is set as its only sheet,
        otherwise the given SoPipSheetKit is simply added.

        @param appID applicationID
        @param pipGeom the root-node of the PIP-sheet,
               should be of type SoPipSheetKit, but any other node is allowed
               for code-legacy reasons
        @param usePipCube specifys, whether the body of the PIP should be
               visible or not (will only have effect, if pipGeom is not of
               type SoPipSheetKit)			          
      */
    void setPipGeometry(SbAppID appID, 
                        SoNode* pipGeom, 
                        SbBool usePipCube = TRUE);

    /** Get the PIP-geometry (the PIP-sheet) of the specified application 
       (context).

        @param appID applicationID
        @return a SoPipSheetKit associated with the application (context)
    */
    SoPipSheetKit *getPipSheetKit(SbAppID appID);    

    /** Set the visibility of the PIP-parts for the currently used 
        PIP-sheet.

        @param ppm a bit mask specifying the visible pip parts

        \note Use this method only for changing the visibility
         of the PIP, when the sheetKit has already been added to the pipKit,
         otherwise just set the pipParts field of the SoPipSheetKit 
         before calling setPipGeometry().
      */    
    void setPipPartVisibility(unsigned int ppm);

    /** Force refresh of pip content.

        The current PIP-sheet will be rescaled and the pipPart-visibility
        evaluated.
      */    
    void updatePip();

    /** Add systemBar to PIP geometry.

        This method is only used for workspace-PIPs.  
      */        
    void useSystemBar(SoTabKit *systemBar);
    /** Add taskBar to PIP geometry.

        This method is only used for workspace-PIPs.  
      */        
    void useTaskBar(SoTabKit *taskBar);
    /** Add systemBar to PIP geometry.

        This method is only used for workspace-PIPs.  
      */        
    void useWindowBar(SoTabKit *windowBar);

    /**
    Callback method for field sensor.
    */
    static void barCB(void * data, SoSensor* sensor);

private:

    /** Get a SoNodeKitListPart containing all registered PIP-sheets.

        @return a SoNodeKitListPart containing all registered PIP-sheets.
	 */    
    SoNodeKitListPart *getPipSheets();

    /** Returns whether the taskBar is active (set and visible)
     */
    SbBool isActiveTaskBar();
    /** Returns whether the systemBar is active (set and visible)
     */
    SbBool isActiveSystemBar();
    /** Returns whether the windowBar is active (set and visible)
     */
    SbBool isActiveWindowBar();

    /** Set the visibility of the workspace tabs

        @param system TRUE: show systemBar, FALSE: hide systemBar and taskBar
        @param window TRUE: show windowBar, FALSE: hide windowBar
     */
    void showWorkspaceTabs(SbBool system, SbBool window);


    /** This method merges the sheetArea of the given pipKit and
        the sheetArea definded by resultAreaUL and resultAreaLR.
        The resulting sheetArea will be written back to resultAreaUL 
        and resultAreaLR.

        @param mergeArea A SoPipKit.
        @param resultAreaUL upper-left corner of sheet-area.
        @param resultAreaLR lower-right corner of sheet-area.
     */
    void mergeSheetAreas(SoPipKit *mergeArea,
                         SbVec2f &resultAreaUL,
                         SbVec2f &resultAreaLR);

    /** Stores a pointer to the currently visible PIP-sheet.
     */
    SoPipSheetKit *currentSheetKit;  

    /**
    Sensor for detecting changes of the systemBar and windowBar proprties.
    */
    SoFieldSensor systemBarSensor;
	SoFieldSensor windowBarSensor;

    friend class SoPipSheetKit;

};

//----------------------------------------------------------------------------
#endif//_SOPIPKIT_H_

