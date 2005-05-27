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
/** SoTabKit.h contains the definition of class SoTabKit
  *
  * @author Rainer Splechtna
  *
  * $Id: SoTabKit.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef _SOTABKIT_H_
#define _SOTABKIT_H_

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTransform.h>
#include <stbapi/resource/SoPipSheetKit.h>
#include <stbapi/widgets/So3DButton.h>

#include "SoTabButtonKit.h"
#include "StbSkinLoader.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

//----------------------------------------------------------------------------
/** SoTabKit is a nodekit representing a collection of 3DButtons used on a PIP.

@warning this kit is not intended for general use, it is only used for the
"internal" handling of sets of buttons on the PIP (e.g.: the windowTabBar).

A SoTabKit manages a number of 3DButtons. Buttons can be added and removed.
They can be accessed either by an unique ID-number or by a sequential index.
Each button can also trigger an associated pull-down button-list.
@see SoTabButtonKit
The tabBar can be positioned on the PIP with the method setupTransform().
Optionally the active (last pressed button) can be highlighted with a special 
color. 
The tabBar's properities are fetched from a skin, which must have these entries: 
"BUTTON"
a So3DButton specifying the size and color of the tab's buttons
"TRANSLATION"
a SoTranslation specifying the translation between two neighbouring buttons
"BUTTON_ACTIVE_COLOR"
a SoMaterial specifying a diffuseColor, which is used to mark a button as active

@author Rainer Splechtna
 */
class STBAPI_API SoTabKit
:
    public SoBaseKit
{
    SO_KIT_HEADER(SoTabKit);
    SO_KIT_CATALOG_ENTRY_HEADER(tabRoot);
    SO_KIT_CATALOG_ENTRY_HEADER(tabSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(tabTransform);
    SO_KIT_CATALOG_ENTRY_HEADER(tabButtons);

public:
    /** Constructor of the kit, inits the nodes of the kit.
        @param maxTabs the maximum number of buttons this kit should contain
        note: only used for scaling, used by method setupTransform() 
        @param skin the skin containing the properties of  the kit
        @param prefix a prefix identifying the properties of  this kit
               if more than one tabKit is specified within a skin,
               use the prefix to identify the data for this kit
               e.g.: "MY_BUTTON", where "MY_" is the prefix and 
                     "BUTTON" is the name of the property
     */
    SoTabKit(int maxTabs,
             StbSkinLoader *skin,
             char *prefix = "TAB_"
        );

    /** Inventor class initialization */
    static void initClass(     
        );
 
    /** Returns the ID of the given button.
        @param button a pointer to a button, which is member of a tabKit
        @return the ID of the button in the tabKit,
                or -1, if the button was not found.
     */
    int getButtonID(So3DButton *button);
    /** Returns sequential index of given button.
        @param button a pointer to a button, which is member of a tabKit
        @return the sequential index of the button in the tabKit,
                or -1, if the button was not found.
     */
    int getButtonIndex(So3DButton *button);
    /** Returns a tabButtonKit of the tabKit identified by ID or index.
        @param buttonID ID or index of the searched button.
        @param indexed search with index (TRUE) or ID (FALSE)
        @return the tabButtonKit specified by buttonID,
                or NULL, if no tabButtonKit was found.
      */
    SoTabButtonKit *getButtonKit(int buttonID, SbBool indexed = FALSE);

    /** Returns the transform-node, which determines the position on the PIP.
        @return the transform-node, which determines the position on the PIP.
     */
    SoTransform *getTransform();
    /** This method sets the nodekit-internal transform-node, i.e. positions
        the tabKit on the PIP.
        @param start specifys the upper-left position of the first button of
               the tabKit in normalized PIP-coordinates
        @param barSize length of bar in normalized PIP-unit,
               the length of a bar reaches from the first button's
               left(upper) side to the (maxTabs)th button's right(lower) side
        @param pipSize the size of the PIP the tabKit should be fitted on 
     */
    void setupTransform(SbVec3f start, float barSize, SbVec3f pipSize); 

    /** Add a button to the tabKit.

      @param buttonID the ID must be unique in the tabKit, otherwise the button 
             won't be added.
             If only sequential index access is required specify a buttonID
             which is smaller than 0.
      @param callback the callback, that should be called, when this button
             is pressed
      @param data the callback's custom data
      @param buttonTexture an optional texture for the button
      */
    void addTab(int buttonID, 
                So3DButtonCB* callback, 
                void *data, 
                SoTexture2* buttonTexture = NULL);
    /** Remove a button from the tabKit identified by ID.
        @param buttonID the ID of the button in the tabKit.
      */   
    void removeTab(int buttonID);
    /** Remove a button from the tabKit identified by index.
        @param index the index of the button in the tabKit.
      */   
    void removeTabIndexed(int index);

    /** Method to show or hide the tabBar.
        @param onOff TRUE: show bar, FALSE: hide bar
      */
    void showTabs(SbBool onOff);
    /** Returns whether the tabBar is visible or not.
        @return TRUE if tabBar is visible.
      */
    SbBool isVisible();

    /** Hides all pull-down menues in the tabKit but the specified one.

        @param but the tabButtonKit, that should not be hided.
        If but equals NULL all pull-down menues will be hided.
      */    
    void resetAllPullDownMenues(SoTabButtonKit *but = NULL);
    /** Force highlight on the given button and a button in the buttonKit's
        pull-down menue. 
        @param button the tabButtonKit, which should be highlighted
               If button equals NULL no button will be highlighted.
        @param name name of the pull-down menue-entry, which should be
               highlighted.
        If name eqauls "" no pull-down menue-entry will be highlighted.
     */
    void setActiveButton(SoTabButtonKit *button = NULL, SbName name = "");
    /** Determine if active (most recently pressed) buttons should be
        highlighted.
        @param onOff TRUE: highlight active buttons.
     */
    void showActiveButton(SbBool onOff);
  /** Set parent of this kit 
     */
    void setStreamedTab(StbSkinLoader* theSkin, char* thePrefix, int theMaxSheets);

private:
    /** A dummy constructor */
    SoTabKit(                 
        );
    /** The destructor releases all grabbers of this tabKit */
    virtual ~SoTabKit(
        );
    /** Access the list of tabButtonKits */
    SoNodeKitListPart *getButtons();
    /** Pointer to used skin. */
    StbSkinLoader *skin;
    /** Used prefix. */
    char *prefix;
    /** Maximum number of buttons allowed in a tabKit. */
    int maxTabs;
    /** Stores value set by method showActiveButton. */
    SbBool showActiveButton_;
    friend class SoPipSheetKit;
};
  

//----------------------------------------------------------------------------
#endif//_SOTABKIT_H_

