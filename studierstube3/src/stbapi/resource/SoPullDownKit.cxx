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
/** SoPullDownKit.h contains the implementation of class SoPullDownKit
  *
  * @author Rainer Splechtna
  *
  * $Id: SoPullDownKit.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#include <cassert>

#include <stbapi/misc/ivmacros.h>
#include <stbapi/util/ivio.h>
#include <stbapi/event3d/Stb3DEventGenerator.h>

#include "SoTabButtonKit.h"
#include "SoPullDownKit.h"

SO_KIT_SOURCE(SoPullDownKit);

//----------------------------------------------------------------------------
/** Inventor class initialization */

void
SoPullDownKit::initClass()
{
    SO_KIT_INIT_CLASS(SoPullDownKit, SoBaseKit, "BaseKit");
}

//----------------------------------------------------------------------------
/** Dummy constructor. */

SoPullDownKit::SoPullDownKit()
{
}

//----------------------------------------------------------------------------
/** Constructor setting up the whole kit-structure and doing initializations */

SoPullDownKit::SoPullDownKit(SbName name,
                             So3DButtonCB* callback,
                             void *data,
                             StbSkinLoader *skin,
                             char *prefix,
                             SoTexture2* texture,
                             SbBool first)
: name(name), text(NULL), skin(skin), prefix(prefix)
{
    SO_KIT_CONSTRUCTOR(SoPullDownKit);

    // Add the body parts to the catalog...
    // SO_KIT_ADD_CATALOG_ENTRY(
    //   name,className,nullByDefault,parentName,rightSiblingName,isPublic

    SO_KIT_ADD_CATALOG_ENTRY(
        root,    SoGroup ,FALSE, this,     \0,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(
        transform,    SoTransform ,FALSE, root,     \0,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(
        textureRoot,    SoSeparator ,FALSE, root,     \0,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(
        textureTransform,    SoTransform ,FALSE, textureRoot,     \0,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(
        textureButton,    So3DButton ,FALSE, textureRoot,     \0,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(
        textRoot,    SoSeparator ,FALSE, root,     \0,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(
        textTransform,    SoTransform ,FALSE, textRoot,     \0,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(
        textButton,    So3DButton ,FALSE, textRoot,     \0,FALSE);

    SO_KIT_INIT_INSTANCE();

    setSearchingChildren(TRUE);

    if (first) {
        PUAGETNODE(tDef,SoTranslation,prefix,"PULL_DOWN_BUTTON_FIRST_TRANSLATION",skin);
        getTransform()->translation = tDef->translation.getValue();
    }
    else {
        PUAGETNODE(tDef,SoTranslation,prefix,"PULL_DOWN_BUTTON_TRANSLATION",skin);
        getTransform()->translation = tDef->translation.getValue();
    }
    PUAGETNODE(defButton,So3DButton,prefix,"PULL_DOWN_BUTTON",skin);
    getTextureButton()->buttonColor = defButton->buttonColor.getValue();
    getTextureButton()->width = defButton->depth.getValue();
    getTextureButton()->height = defButton->height.getValue();
    getTextureButton()->depth  = defButton->depth.getValue();
    if (texture)
        getTextureButton()->setupTexture(texture,0,0);

    getTextButton()->buttonColor = defButton->buttonColor.getValue();
    getTextButton()->width = defButton->width.getValue();
    getTextButton()->height = defButton->height.getValue();
    getTextButton()->depth  = defButton->depth.getValue();

    getTextTransform()->translation.setValue(
               getTextButton()->width.getValue()/2.0f+
               getTextureButton()->width.getValue()/2.0f
               ,0.0f,0.0f); 

    // set text for textButton
    build3DText(name.getString());

    // connect textButton and textureButton
    getTextButton()->pressed.connectFrom(&getTextureButton()->pressed);
    getTextButton()->highlighted.connectFrom(&getTextureButton()->highlighted);
    getTextureButton()->pressed.connectFrom(&getTextButton()->pressed);
    getTextureButton()->highlighted.connectFrom(&getTextButton()->highlighted);

    getTextButton()->addReleaseCallback(callback,data); 
    getTextureButton()->addReleaseCallback(callback,data); 

}

//----------------------------------------------------------------------------
/** Auxilary method building and attaching text to the textButton. */

void
SoPullDownKit::build3DText(const char* name)
{
	// set font
   PUAGETNODE(font_,SoFont,prefix,"PULL_DOWN_FONT",skin);
   SoFont *myFont = new SoFont;
   myFont->name = font_->name;
   myFont->size = font_->size;
   
   PUAGETNODE(text_,SoText3,prefix,"PULL_DOWN_TEXT",skin);
   text = new SoText3;
   text->string.setValue(name);
   text->justification = text_->justification;
   text->parts = text_->parts;

   PUAGETNODE(material_,SoMaterial,prefix,"PULL_DOWN_TEXT_MATERIAL",skin);
   SoMaterial *textMaterial = new SoMaterial;
	textMaterial->diffuseColor = material_->diffuseColor;

   SoTransform *textTransform = new SoTransform;
   SoTransform *textTransformPressed = new SoTransform;
  
   textTransform->translation.setValue(0.0f,
                      getTextButton()->height.getValue()*0.6f,
                      font_->size.getValue()/4.0f);
   textTransform->rotation.setValue(SbVec3f(1.0,0.0,0.0),-M_PI/2.0);

   textTransformPressed->translation.setValue(0.0f,
                      getTextButton()->height.getValue()*0.1f,
                      font_->size.getValue()/4.0f);
   textTransformPressed->rotation.setValue(SbVec3f(1.0,0.0,0.0),-M_PI/2.0);

   SoSeparator *textRoot = new SoSeparator;
   SoSeparator *textRootPressed = new SoSeparator;

   textRoot->addChild(textTransform);
   textRoot->addChild(myFont);
   textRoot->addChild(textMaterial);
   textRoot->addChild(text);

   textRootPressed->addChild(textTransformPressed);
   textRootPressed->addChild(myFont);
   textRootPressed->addChild(textMaterial);
   textRootPressed->addChild(text);

   getTextButton()->setText(textRoot);
   getTextButton()->setPressedText(textRootPressed);
}

//----------------------------------------------------------------------------
/** Set a custom text to label the textButton. */

void
SoPullDownKit::set3DText(const char *text_)
{
    if (text)
        text->string.setValue(text_);
}

//----------------------------------------------------------------------------
/** Get the textureButton of this kit. */

So3DButton *
SoPullDownKit::getTextureButton()
{
    return SO_GET_ANY_PART(this,"textureButton",So3DButton);
}

//----------------------------------------------------------------------------
/** Get the textButton of this kit. */

So3DButton *
SoPullDownKit::getTextButton()
{
    return SO_GET_ANY_PART(this,"textButton",So3DButton);
}

//----------------------------------------------------------------------------
/** Get the transform-node of this kit. */

SoTransform *
SoPullDownKit::getTransform()
{
    return SO_GET_ANY_PART(this,"transform",SoTransform);
}

//----------------------------------------------------------------------------
/** Get the textTransform-node of this kit. */

SoTransform *
SoPullDownKit::getTextTransform()
{
    return SO_GET_ANY_PART(this,"textTransform",SoTransform);
}

//----------------------------------------------------------------------------
/** Destructor, releases all grabbers of this kit. */

SoPullDownKit::~SoPullDownKit()
{
    Stb3DEventGenerator::releaseAllGrabbers(this);
}


//----------------------------------------------------------------------------
//eof

