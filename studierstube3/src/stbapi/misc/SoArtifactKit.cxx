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
/** Implementation of SoArtifactKit class
  *
  * @author Gerd Hesina, Dieter Schmalstieg
  *
  * $Id: SoArtifactKit.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <cassert>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoInfo.h>
#include <Inventor/nodes/SoMatrixTransform.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>

#include <stbapi/misc/SoArtifactKit.h>


SO_KIT_SOURCE(SoArtifactKit);

//-----------------------------------------------------------------------------
// static class initialization. call before first use

void
SoArtifactKit::initClass(void) 
{
    SO_KIT_INIT_CLASS(SoArtifactKit, SoBaseKit, "BaseKit");
}

//-----------------------------------------------------------------------------
// constructor. initializes al parts and fields.

SoArtifactKit::SoArtifactKit() 
{
    SO_KIT_CONSTRUCTOR(SoArtifactKit);

    SO_KIT_ADD_FIELD(selected, (FALSE));
    SO_KIT_ADD_FIELD(dragable, (TRUE));
    SO_KIT_ADD_FIELD(dragButton, (So3DEvent::BUTTON0));
    SO_KIT_ADD_FIELD(color, (1,1,1));

//--kit parts

    SO_KIT_ADD_CATALOG_ENTRY(
       dragSep, SoDragSeparator, FALSE,this, \0x0 , FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(
       dragTransform, SoTransform, FALSE,dragSep,\0x0 , FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(
        pipTrafo, SoMatrixTransform, FALSE,dragSep,\0x0 , FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(
        scale, SoScale, TRUE,dragSep,\0x0 , TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(
        material, SoMaterial, FALSE,dragSep,\0x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(
        geometry, SoSeparator, FALSE,dragSep,\0x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(
        selectSwitch, SoSwitch, FALSE,dragSep,\0x0 , FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(
        selectGroup, SoGroup, FALSE,selectSwitch,\0x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(
        selectMaterial, SoMaterial, FALSE,selectGroup,\0x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(
        selectStyle, SoDrawStyle, FALSE,selectGroup,\0x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(
        selectTranslation, SoTranslation, FALSE,selectGroup,\0x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(
        selectGeometry, SoCube, FALSE,selectGroup,\0x0, FALSE);

    SO_KIT_INIT_INSTANCE();

    //init part fields
    ((SoMaterial*)material.getValue())->diffuseColor.setValue(color.getValue());
    ((SoMaterial*)selectMaterial.getValue())->diffuseColor = SbVec3f(1,1,1);
    ((SoMaterial*)selectMaterial.getValue())->transparency = 0.95f;
//    ((SoDrawStyle*)selectStyle.getValue())->style = SoDrawStyle::LINES;
//    ((SoDrawStyle*)selectStyle.getValue())->lineWidth = 2;

    // attach sensors to fields
    selectedSensor = new SoFieldSensor(updateSelected, (void*)this);
    selectedSensor->attach(&selected);
    dragableSensor = new SoFieldSensor(updateDragable, (void*)this);
    dragableSensor->attach(&dragable);
    colorSensor = new SoFieldSensor(updateColor, (void*)this);
    colorSensor->attach(&color);
    dragButtonSensor = new SoFieldSensor(updateDragButton, (void*)this);
    dragButtonSensor->attach(&dragButton);

    // make sure selection box is correct
    updateDragable((void*)this,NULL);

    // node should be searching its children, in partiular geometry
    setSearchingChildren(TRUE);
}

//-----------------------------------------------------------------------------
// destructor. does nothing

SoArtifactKit::~SoArtifactKit() {}

//-----------------------------------------------------------------------------

void
SoArtifactKit::updateSelected(void* data, SoSensor*)
{
    SoArtifactKit* self = (SoArtifactKit*)data;
    // if selected is true, set switch to child 0
    SbBool value = self->selected.getValue() ? 0 : -1;
    ((SoSwitch*)self->selectSwitch.getValue())->whichChild = value;
    if(self->selected.getValue()) //update bbox
    {
        SoCube* cube = ((SoCube*)self->selectGeometry.getValue());
        static SbViewportRegion vp;
        static SoGetBoundingBoxAction bba(vp);
        bba.apply(self->geometry.getValue());
        SbBox3f bbox = bba.getBoundingBox(); //find geometry bbox
        float x,y,z;
        bbox.getSize(x,y,z);
        cube->width  = x; //set cube to reflect extent of geometry bbox
        cube->height = y;
        cube->depth  = z;
        ((SoTranslation*)self->selectTranslation.getValue())->translation =
            bbox.getCenter();
    }
}

//-----------------------------------------------------------------------------
// callback to (dis)connect dragSep from tracker if dragable field changes
// data: pointer to SoArtifactKit instance (as method must be static)

void
SoArtifactKit::updateDragable(void* data, SoSensor*)
{
    SoArtifactKit* self = (SoArtifactKit*)data;
    SoTransform* t = (SoTransform*)self->dragTransform.getValue();
    SoDragSeparator* ds = (SoDragSeparator*)self->dragSep.getValue();
    if(self->dragable.getValue()) //connect to tracker
    {
        t->translation.connectFrom(&ds->translation);
        t->rotation.connectFrom(&ds->rotation);
    }
    else //disconnect from tracker
    {
        t->translation.disconnect();
        t->rotation.disconnect();
    }
}

//-----------------------------------------------------------------------------
// sets the dragButton field of the artifact kit to the changed field value

void
SoArtifactKit::updateDragButton(void* data, SoSensor*)
{
    SoArtifactKit* self = (SoArtifactKit*)data;
    SoDragSeparator* ds = (SoDragSeparator*)self->dragSep.getValue();
	ds->dragButton.setValue(self->dragButton.getValue());
}

//-----------------------------------------------------------------------------
// callback to update material part if color changes
// data: pointer to SoArtifactKit instance (as method must be static)

void
SoArtifactKit::updateColor(void* data, SoSensor*)
{
    SoArtifactKit* self = (SoArtifactKit*)data;

    ((SoMaterial*)self->material.getValue())->diffuseColor =
        self->color.getValue();
}

//-----------------------------------------------------------------------------
// set the basic transform applied to geometry
// matrix: transform to set

void
SoArtifactKit::setReferenceTransform(const SbMatrix matrix)
{
    ((SoMatrixTransform*)pipTrafo.getValue())->matrix = matrix;
}

//-----------------------------------------------------------------------------
// set the callback to be executed when geometry is pressed
// func: callback function
// data: user supplied data

void
SoArtifactKit::setPressCallback(SoDragSeparatorCB* func, void* data)
{
    ((SoDragSeparator*)dragSep.getValue())->setPressCallback(func,data);
}

//-----------------------------------------------------------------------------
// set the callback to be executed when geometry is released
// func: callback function
// data: user supplied data

void
SoArtifactKit::setReleaseCallback(SoDragSeparatorCB* func, void* data)
{
    ((SoDragSeparator*)dragSep.getValue())->setReleaseCallback(func,data);
}

//-----------------------------------------------------------------------------
// wrapper function for SoDragSeparator::startDragging

void
SoArtifactKit::startDragging()
{
    ((SoDragSeparator*)dragSep.getValue())->startDragging();
}

//-----------------------------------------------------------------------------
// wrapper function for SoDragSeparator::stopDragging

void
SoArtifactKit::stopDragging()
{
    ((SoDragSeparator*)dragSep.getValue())->stopDragging();
}

//-----------------------------------------------------------------------------
// returns: name (DEF) of geometry

SbString
SoArtifactKit::getIdString()
{
    return geometry.getValue()->getName().getString();
}

//-----------------------------------------------------------------------------
//eof
