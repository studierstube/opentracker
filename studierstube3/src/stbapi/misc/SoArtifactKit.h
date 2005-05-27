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
//  PROJECT:    Studierstube
//  CONTENT:    Class definition of SoArtifactKit
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    ds      Dieter Schmalstieg
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  27-aug-99 17:03:54  gh      last modification
// ===========================================================================


#ifndef _SOARTIFACTKIT_H_
#define _SOARTIFACTKIT_H_

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/fields/SoSFColor.h>

#include <stbapi/interaction/SoDragSeparator.h>
#include <stbapi/stbapi.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

//-----------------------------------------------------------------------------
// SoArtifactKit
//
// Fields:
//   SoSFBool  selected
//   SoSFBool  dragable
//   SoSFColor color
//   SoSFInt32 dragButton
//
// Public parts:
//   (SoNode) geometry
//   (SoScale) scale
//
// Methods:
//   void setDefaultMatrix(const SbMatrix matrix)
//   void setPressCallback(SoDragSeparatorCB* func, void* data)
//   void setReleaseCallback(SoDragSeparatorCB* func, void* data)
//   void startDragging()
//   void stopDragging()
//   SbString getName()
// 
//-----------------------------------------------------------------------------
/**
 * @ingroup interaction
 */
class STBAPI_API SoArtifactKit 
: 
    public SoBaseKit 
{
    SO_KIT_HEADER(SoArtifactKit);

    SO_KIT_CATALOG_ENTRY_HEADER(dragSep);          //top drag separator
    SO_KIT_CATALOG_ENTRY_HEADER(dragTransform);    //trafo hooked to dragSep
    SO_KIT_CATALOG_ENTRY_HEADER(pipTrafo);         //matrix trafo if pip used
    SO_KIT_CATALOG_ENTRY_HEADER(scale);            //scale of geometry
    SO_KIT_CATALOG_ENTRY_HEADER(material);         //basic color of geometry
    SO_KIT_CATALOG_ENTRY_HEADER(geometry);         //actual geometry. public
    SO_KIT_CATALOG_ENTRY_HEADER(selectSwitch);     //switch for selection
    SO_KIT_CATALOG_ENTRY_HEADER(selectGroup);      //group for selection
    SO_KIT_CATALOG_ENTRY_HEADER(selectMaterial);   //red color for selection
    SO_KIT_CATALOG_ENTRY_HEADER(selectStyle);      //line drawing for selection
    SO_KIT_CATALOG_ENTRY_HEADER(selectTranslation);//translation to obj center
    SO_KIT_CATALOG_ENTRY_HEADER(selectGeometry);   //wireframe selection box

public:
    SoSFBool selected;             //true if artifact is selected
    SoSFBool dragable;             //true if artidfact may be dragged
    SoSFInt32 dragButton;          //number of button used for dragging
    SoSFColor color;               //set the default color of the object

    static void initClass(         //call before fist usage
        );
    SoArtifactKit(                 //constructor
        );
    void setReferenceTransform(    //set basic transform matrix
        const SbMatrix matrix      //matrix to set
        );
    void setPressCallback(         //set callback to execute if object picked
        SoDragSeparatorCB* func,   //callback
        void* data                 //user supplied data
        );
    void setReleaseCallback(       //set callback to execute if object released
        SoDragSeparatorCB* func,   //callback
        void* data                 //user supplied data
        );
    void startDragging(            //immediately go into dragging mode
        );
    void stopDragging(             //immediately exit dragging mode
        );
    SbString getIdString(          //name (DEF) of geometry as object label
        );

private:
    ~SoArtifactKit(                //destructor
        );
    static void updateSelected(    //sensor callback if selection is changed
        void* data,                //user data
        SoSensor* sensor           //pointer to sensor
        );
    static void updateDragable(    //sensor callback if drag state is changed
        void* data,                //user data
        SoSensor* sensor           //pointer to sensor
        );
    static void updateDragButton(  //sensor callback if dragButton state changed
        void* data,                //user data
        SoSensor* sensor           //pointer to sensor
        );
    static void updateColor(       //sensor callback if color is changed
        void* data,                //user data
        SoSensor* sensor           //pointer to sensor
        );

    SoFieldSensor* selectedSensor; //sensor for selected field
    SoFieldSensor* dragableSensor; //sensor for dragable field
    SoFieldSensor* dragButtonSensor; //sensor for dragButton field
    SoFieldSensor* colorSensor;    //sensor for color field

}; //SoArtifactKit

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOARTIFACTKIT_H_
