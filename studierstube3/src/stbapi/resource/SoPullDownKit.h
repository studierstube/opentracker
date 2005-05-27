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
/** SoPullDownKit.h contains the definition of class SoPullDownKit
  *
  * @author Rainer Splechtna
  *
  * $Id: SoPullDownKit.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef _SOPULLDOWNKIT_H_
#define _SOPULLDOWNKIT_H_

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/nodes/SoTransform.h>

#include <stbapi/widgets/So3DButton.h>

#include "StbSkinLoader.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

//----------------------------------------------------------------------------

/** SoPullDownKit is a nodekit which represents two linked buttons working
as one. One button is textured, the other is labeled with a text (per default
the name of this kit).
@warning this kit is not intended for general use, it is only used for the
"internal" handling of pullDown-buttons of a SoTabButtonKit.

The properities of the pullDownKit are fetched from a skin, which must
have these entires:
"PULL_DOWN_BUTTON_FIRST_TRANSLATION"
a SoTranslation specifying the translation between the parent
 SoTabButtonKit-button and the first pull-down-button
"PULL_DOWN_BUTTON_TRANSLATION"
a SoTranslation specifying the translation between two neighbouring
pull-down-buttons
"PULL_DOWN_BUTTON"
a So3DButton specifying the size and color of the pull-down-text-button
note: the texture button has the same color and takes the depth as width  
"PULL_DOWN_FONT" 
a SoFont specifying the name and size of the font
"PULL_DOWN_TEXT"
a SoText3 specifying the justification and parts of the label-text
"PULL_DOWN_TEXT_MATERIAL"
a SoMaterial specifying the diffuse color of the label-text

@author Rainer Splechtna
 */
class SoPullDownKit
:
    public SoBaseKit
{
    SO_KIT_HEADER(SoPullDownKit);
    SO_KIT_CATALOG_ENTRY_HEADER(root);
    SO_KIT_CATALOG_ENTRY_HEADER(transform);
    SO_KIT_CATALOG_ENTRY_HEADER(textureRoot);
    SO_KIT_CATALOG_ENTRY_HEADER(textureTransform);
    SO_KIT_CATALOG_ENTRY_HEADER(textureButton);
    SO_KIT_CATALOG_ENTRY_HEADER(textRoot);
    SO_KIT_CATALOG_ENTRY_HEADER(textTransform);
    SO_KIT_CATALOG_ENTRY_HEADER(textButton);

public:
    /** Inventor class initialization */
    static void initClass(        
        );
    /** Set a custom text to label the textButton.
        @param text the text to label the textButton.
     */
    void set3DText(const char *text);
private:
    /** Dummy constructor. */
    SoPullDownKit(  
        );
   /** Constructor setting up the whole kit-structure and doing 
       all the initializations neccessary.
       @param name the name identifying this kit
       @param callback the callback, that should be called, when this button
              is pressed
       @param data the callback's custom data
       @param skin the skin containing the properties of  the kit
       @param prefix a prefix identifying the properties of this kit
       @param texture texture for the textureButton of this kit
       @param first TRUE if this is the first pullDownKit created
              for a SoTabButtonKit
     */
    SoPullDownKit(SbName name,
                  So3DButtonCB* callback,
                  void *data,
                  StbSkinLoader *skin,
                  char *prefix,
                  SoTexture2* texture,
                  SbBool first);
    /** Destructor, releases all grabbers of this kit. */
    virtual ~SoPullDownKit(                
        );
    /** Name identifying this kit. */
    SbName name;

    /** Get the textureButton of this kit.
          @return the textureButton of this kit.
     */
    So3DButton *getTextureButton();
    /** Get the textButton of this kit.
          @return the textButton of this kit.
     */
    So3DButton *getTextButton();
    /** Get the textTransform-node of this kit.
          @return the textTransform-node of this kit.
     */
    SoTransform *getTextTransform();
    /** Get the transform-node of this kit.
          @return the transform-node of this kit.
     */
    SoTransform *getTransform();
    
    /** Auxilary method building and attaching text to the textButton
          @param name text to be built.
     */
    void build3DText(const char *name);

    /** Pointer to the used text-node. */
    SoText3 *text; 
    /** The skin containing the properties of  this kit. */
    StbSkinLoader *skin;
    /** Prefix a prefix identifying the properties of  this kit. */
    char *prefix;

    friend class SoTabButtonKit;
};

//----------------------------------------------------------------------------
#endif//_SOPULLDOWNKIT_H_

