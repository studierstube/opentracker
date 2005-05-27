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
/** SoPipSheetKit.h contains the definition of class SoPipSheetKit
  *
  * @author Rainer Splechtna
  *
  * $Id: SoPipSheetKit.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef _SOPIPSHEETKIT_H_
#define _SOPIPSHEETKIT_H_

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoMFNode.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTransform.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/workspace/SoContextManagerKit.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

//----------------------------------------------------------------------------
class SoPipSheetKit;
class SoPipKit;
class SoTabKit;

/*
struct SheetCallbackData 
{
    int index;               //index of pip sheet
    SoPipSheetKit *parent; 
    So3DButtonCB* callback;  //associated callback 
    void* realUserData;      //user supplied data
};
*/

//----------------------------------------------------------------------------

/** SoPipSheetKit is a nodekit representing a collection of PIP-sheets.

A SoPipSheetKit contains a number of PIP-sheets. The user can switch between
these sheets by pressing a button of the sheetTabBar, which contains a button
for every sheet in the kit. The pipPartVisibility is defined on a per
SoPipSheetKit basis.
@author Rainer Splechtna
@ingroup interaction
 */
class STBAPI_API SoPipSheetKit
:
    public SoBaseKit
{
    SO_KIT_HEADER(SoPipSheetKit);
    SO_KIT_CATALOG_ENTRY_HEADER(sheetsRoot);
    SO_KIT_CATALOG_ENTRY_HEADER(sheetsTrans);
    SO_KIT_CATALOG_ENTRY_HEADER(tabs);
    SO_KIT_CATALOG_ENTRY_HEADER(sheets);

public:
    /** Describes parts of the PIP.
        */
    enum PipPartMask                                        
    { 
        /** The body (the geometry representing the PIP's physical shape) */
        PIP_BODY = 0x0001,        
        /** The sheet itself*/
        SHEET = 0x0002, 
        /** The buttons which switch between PIP-sheets of one SoPipSheetKit */
        SHEET_TABS = 0x0004,      
        /** Part of the workspace tabs: containing currentTask and exit button */
        SYSTEM_TABS = 0x0008,  
        /** Part of the workspace tabs: containing window manipulation buttons */
        WINDOW_TABS = 0x0010,  
        /** All parts of the PIP */
        FULL = 0x001F      
    };                     

    /** If autoScaling is TRUE any sheet of the kit will be scaled to fit into
        the PIP's sheetArea */
    SoSFBool autoScaling;     
    /** This field determines the visible parts of the PIP, when using this
        sheet kit. */
    SoSFBitMask pipParts;          
    /** The constructor inits the fields of the kit. */    
    SoPipSheetKit( 
        );
    /** Inventor class initialization */
    static void initClass(        
        );
    /** Add a sheet to the sheetKit.

        @param geometry the root of the new sheet
        @param a texture for the associated sheetTab-button
     */
    void addSheet(
        SoNode* geometry,
        SoTexture2* buttonTexture = NULL
        );
    /** Remove a sheet from the sheetKit.

        @param geometry the root of the sheet to be removed
      */
    void removeSheet(
        SoNode* geometry
        );
    /** Switch to a pipSheet directly, without using the sheetTab-buttons

        @param index index of pipSheet
     */
    void switchPipSheet(int index);
    /** Get the switch containing all sheets of this kit
        @return switch containing all sheets of this kit
     */
    SoSwitch *getSheets();

private:
    /** The destructor does nothing. */
    virtual ~SoPipSheetKit(                
        );
    /** ApplicationID of the application this pipSheetKit is associated with */
    SbAppID appID;
    /** Context-name of the context this pipSheetKit is associated with */
    // SbName contextName;

    /** Maximum count of sheets allowed for this kit 
    Per default 16 sheets are allowed */
    int maxSheets;

    /** Stores information,if pipSheetKit has been already activated */
    SbBool active;

    /** Activate sheets of the kit, i.e. build sheetTab-buttons and
        switch to first sheet 
        @param parent the pipKit that contains this pipSheetKit 
     */
    void activateSheets(SoPipKit *parent);

    /** Get the sheetTabButton-kit.
        @return the sheetTabButton-kit.
     */
    SoTabKit *getTabs();
    /** Switch sheets on or off.
        @param onOff TRUE: show current sheet FALSE: hide sheets
      */
    void showSheets(SbBool onOff);   
    /** Stores index of sheet shown before showSheets(FALSE) was called */
    int lastSheetShown;
    /** Get the transform node of the current sheet. 
        @return transform node of the current sheet 
     */
    SoTransform *getSheetsTrans();
    /** Scales current sheet to fit into given sheetArea.
       @param sheetAreaUL upper-left corner of sheetArea
       @param sheetAreaLR lower-right corner of sheetArea
     */
    void scaleCurrentSheet(SbVec2f sheetAreaUL,
                           SbVec2f sheetAreaLR);

    /** The pipKit that contains this pipSheetKit */
    SoPipKit *parent;

    /** Stores textures for sheetTab-buttons before the pipSheetKit
        is activated.
      */
    SoMFNode tabTextures;

    /** Handles callbacks from sheetTab-buttons, i.e. switches
        to corresponding sheet.
        @param userData a pointer to the instance that contains the button
        @param button button that triggered the callback
     */
    static void switchPipSheetCB(
        void* userData, 
        So3DButton* button
        );

    friend class SoPipKit;

};
  

//----------------------------------------------------------------------------
#endif//_SOPIPSHEETKIT_H_

