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
/** SoTabButonKit.h contains the definition of class SoTabButtonKit
  *
  * @author Rainer Splechtna
  *
  * $Id: SoTabButtonKit.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef _SOTABBUTTONKIT_H_
#define _SOTABBUTTONKIT_H_

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTransform.h>
#include <stbapi/widgets/So3DButton.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "StbSkinLoader.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoTabKit;
class SoUserKit;
class SoPipKit;

//----------------------------------------------------------------------------
/** SoTabButtonKit is a nodekit wrapping a normal So3DButton and adding
functionality for use with a SoTabKit.
@warning this kit is not intended for general use, it is only used for the
"internal" handling of buttons of a SoTabKit.

The kit provides methods for texture-handling and adding or removing
pulldown-buttons.
The properities of the tabButton are fetched from a skin, which must have 
these entries:
"BUTTON"
a So3DButton specifying the size and color of the  button
"TRANSLATION"
a SoTranslation specifying the translation between two neighbouring buttons
"BUTTON_ACTIVE_COLOR" 
a SoMaterial specifying a diffuseColor, which is used to mark a button as active

note: the same properties are specified for the parent SoTabKit
@author Rainer Splechtna
 */
class SoTabButtonKit
:
    public SoBaseKit
{
    SO_KIT_HEADER(SoTabButtonKit);
    SO_KIT_CATALOG_ENTRY_HEADER(tabRoot);
    SO_KIT_CATALOG_ENTRY_HEADER(tabTransform);
    SO_KIT_CATALOG_ENTRY_HEADER(tabButton);
    SO_KIT_CATALOG_ENTRY_HEADER(tabPullDownSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(pullDownButtons);

public:
    /** Inventor class initialization */
    static void initClass(        
        );

    /** Returns the ID of the tabButtonKit.
          @return the ID of the tabButtonKit
	 */
    int getButtonID();
    /** Add a pullDownButton to this kit.
          @param name an unique name for the pullDownButton
          @param callback the callback, that should be called, when the button
              of this kit is pressed
          @param data the callback's custom data
	 */
    void addPullDownButton(SbName name,
                           So3DButtonCB* callback,
                           void *data);

    /** Remove a pullDownButton from this kit.
          @param name the name of the pullDownButton
	 */
    void removePullDownButton(SbName name);

    /** Highlight the pullDownButton specified by name.
          @param name the name of the pullDownButton
	 */
    void setActivePullDownButton(SbName name);

    /** Copy the texture from an other kit's button to this kit's button.
          @param from tabButtonKit from which the texture is copied.
	 */
    void copyTexture(SoTabButtonKit *from);
  
   /** Set the texture of this kit's button.
          @param texture Texture to be used by this kit's button.
	 */
    void setTexture(SoTexture2 *texture);

  /** Set a custom text for a pullDownButton of this kit.
          @param name the name of the pullDownButton
          @param text custom text to be set
	 */
    void setPullDownText(SbName name, const char *text);
  /** Set release callback for this kit's button. 
           @param callback the callback, that should be called, when this button
               is pressed
           @param data the callback's custom data
     */
    void setCallback(So3DButtonCB *callback,void *data);
  /** Set parent of this kit 
           @param theParent  tabKit creating this kit
     */
    void setStreamedTab(SoTabKit *theParent, StbSkinLoader* theSkin, char* thePrefix, int theMaxSheets);

private:
    /** The constructor of the kit inits the nodes of this kit.
           It should only be called from SoTabKit, hence private.
           @see SoTabKit::addTab()
           @param parent  tabKit creating this kit
           @param buttonID ID of this kit 
           @param callback the callback, that should be called, when this button
               is pressed
           @param data the callback's custom data
           @param skin the skin containing the properties of  the kit
           @param prefix a prefix identifying the properties of  this kit
           @param buttonTexture an optional texture for the button of this kit
     */
    SoTabButtonKit(SoTabKit *parent,           
                   int buttonID,                // button ID
                   So3DButtonCB* callback,      // release callback 
                   void *data,                  // callcack data
                   StbSkinLoader *skin,
                   char *prefix,
                   SoTexture2* buttonTexture = NULL);   // buttonTexture
    /** ID of the button */
    SoSFInt32 buttonId;
    /** A dummy constructor */
    SoTabButtonKit(                 
        );
    /** Destructor of the kit.
          Releases any grabbers of this kit and deletes sensors.
        */
    virtual ~SoTabButtonKit(   
        );
    /** Get the button of this kit.
          @return the button of this kit.
      */
    So3DButton *getButton();
    /** Get the transform-node of this kit.
          @return the transform-node of this kit.
      */
    SoTransform *getTransform();
    /** Show or hide all pullDownButtons of this kit.
          @param onOff TRUE: show pullDownButtons,
                                    FALSE: hide pullDownButtons
     */
    void showPullDown(SbBool onOff);
    /** Get listPart containing all pullDownButtons of this kit.
           @return listPart containing all pullDownButtons of this kit.
       */
    SoNodeKitListPart *getPullDownButtons();
    /** Callback of the highlightSensor monitoring, if the button of this 
           kit has been highlighted.
          @param data custom callback data
     */
    static void highlightCB(void *data, SoSensor *);

    /** Sensor monitoring, if the button of this 
           kit has been highlighted. */
    SoFieldSensor *highlightSensor;
    /** TabKit which created this kit. */
    SoTabKit *parent;
    /** The skin containing the properties of  this kit. */
    StbSkinLoader *skin;
    /** Prefix a prefix identifying the properties of  this kit. */
    char *prefix;
    /** Pointer to texture of the kit's texture. */
    SoTexture2 *actualTexture;
    /** Release callback of  tabButtonKit-buttons
          @param userData custom callback data
          @param button the actual button, which caused the callback.
    */
    static void releaseCB(
        void* userData, 
        So3DButton* button
        );


    friend class SoTabKit;
    // friend class SoUserKit;
    friend class SoPipKit;
    friend class SoUserManagerKit;
};
  

//----------------------------------------------------------------------------
#endif//_SOTABBUTTONKIT_H_

