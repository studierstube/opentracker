 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: Studierstube
  * ======================================================================== */ 
/** SoMessageKit.h contains the implementation of class SoMessageKit
  *
  * @author Rainer Splechtna
  *
  * $Id: SoMessageKit.cxx 4187 2005-04-28 06:28:41Z bornik $
  * @file                                                                   
  */
 /* ======================================================================== */

#include <cassert>

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/engines/SoCompose.h>

#include <stbapi/util/ivio.h>

#include "SoMessageKit.h"

SO_KIT_SOURCE(SoMessageKit);

//----------------------------------------------------------------------------
/** Inventor class initialization */

void
SoMessageKit::initClass()
{
  SO_KIT_INIT_CLASS(SoMessageKit, SoBaseKit, "SoBaseKit");
}

//----------------------------------------------------------------------------
/** The constructor of the kit inits the fields and connects the tracker. */

SoMessageKit::SoMessageKit()
{
    SO_KIT_CONSTRUCTOR(SoMessageKit);

    // Add the body parts to the catalog...
    // SO_KIT_ADD_CATALOG_ENTRY(
    //   name,className,nullByDefault,parentName,rightSiblingName,isPublic

    SO_KIT_ADD_CATALOG_ENTRY(
        messageRoot,    SoSeparator,FALSE,this,     \0,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(
        camera,      SoOrthographicCamera,         FALSE, messageRoot,    \0,FALSE );
    SO_KIT_ADD_CATALOG_ENTRY(
        onOffSwitch,    SoSeparator,  FALSE, messageRoot,    \0,FALSE );
    SO_KIT_ADD_CATALOG_ENTRY(
        material, SoMaterial,         FALSE,onOffSwitch,    \0,FALSE );
    SO_KIT_ADD_CATALOG_ENTRY(
        xForm,    SoTransform,         FALSE,onOffSwitch,    \0,FALSE );
    SO_KIT_ADD_CATALOG_ENTRY(
        font,     SoFont,         FALSE,onOffSwitch,    \0,FALSE );
    SO_KIT_ADD_CATALOG_ENTRY(
        text,     SoText2,         FALSE,onOffSwitch,    \0,FALSE );

    // add field
    SO_KIT_ADD_FIELD(message, ("")); 
    SO_KIT_ADD_FIELD(color, (1,0,0)); 
    SO_KIT_ADD_FIELD(fontName, ("Utopia")); 
    SO_KIT_ADD_FIELD(fontSize, (10)); 
    SO_KIT_ADD_FIELD(position, (0,0)); 

    SO_KIT_INIT_INSTANCE();
    
    connectAndSetFields();
}


//----------------------------------------------------------------------------
/** The destructor of the kit does nothing. */

SoMessageKit::~SoMessageKit()
{
    // nil
}

//----------------------------------------------------------------------------
/** Sets up a SoTrakEngine and connects it to the SoTransform-node. */

void 
SoMessageKit::connectAndSetFields()
{
    SO_GET_ANY_PART(this,"camera",SoOrthographicCamera)->viewportMapping = 
        SoCamera::LEAVE_ALONE;
    SO_GET_ANY_PART(this,"camera",SoOrthographicCamera)->height = 1.0;

    SO_GET_ANY_PART(this,"font",SoFont)->name.connectFrom(&fontName);
    SO_GET_ANY_PART(this,"font",SoFont)->size.connectFrom(&fontSize);;

    SO_GET_ANY_PART(this,"material",SoMaterial)->emissiveColor.connectFrom(&color);
    SO_GET_ANY_PART(this,"material",SoMaterial)->ambientColor.setValue(0,0,0);
    SO_GET_ANY_PART(this,"material",SoMaterial)->diffuseColor.connectFrom(&color);

    SO_GET_ANY_PART(this,"text",SoText2)->string.connectFrom(&message);
    SO_GET_ANY_PART(this,"text",SoText2)->justification = SoText2::CENTER;

    SoDecomposeVec2f *d = new SoDecomposeVec2f;
    d->vector.connectFrom(&position);
    SoComposeVec3f *c = new SoComposeVec3f;
    c->x.connectFrom(&d->x);
    c->y.connectFrom(&d->y);
    SO_GET_ANY_PART(this,"xForm",SoTransform)->translation.connectFrom(&c->vector);
}

//----------------------------------------------------------------------------
//eof

