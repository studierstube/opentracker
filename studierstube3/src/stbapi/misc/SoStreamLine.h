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
//  CONTENT:    Class definition of SoStreamLine class
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    peci    Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  12-Apr-00 15:02:21  peci      created
// ===========================================================================


#ifndef _SOSTREAMLINE_H_
#define _SOSTREAMLINE_H_


#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFInt32.h> 
#include <Inventor/fields/SoMFVec3f.h> 
#include <Inventor/fields/SoSFFloat.h> 
#include <Inventor/fields/SoMFVec2f.h> 
#include <Inventor/fields/SoMFColor.h> 
#include <Inventor/sensors/SoFieldSensor.h> 

#include <Inventor/nodes/SoIndexedTriangleStripSet.h>
#include <Inventor/nodes/SoLineSet.h>
#include <stbapi/stbapi.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

//-----------------------------------------------------------------------------

/**
 * Documented at
 * http://www.cg.tuwien.ac.at/groups/vr/studierstube/stbapi/SoStreamLine.html
 * This node generates tube trianglestripsets to emulate a poly line. This step
 * is neccesary to avoid problems using the Studierstube stereo viewer with 2D 
 * Primitives such as lines. 
 *
 * The array vertex holds the coordinates of the poly-line. They have to be more
 * than two, otherwhise they will be ignored and nothing will be drawn. The 
 * Field numOfSlides specifies the number of slides within the ring. The Field
 * withTexCoord tells wether to generate Texture coordinates or not. If this 
 * field is set to FALSE the texCoords array will be ignored. If the 
 * withTexCoord field is set to TRUE texture coordinates will be generated from
 * 0 to 1 if no coordinates are within the texCoords array otherwhise they are
 * taken from the texCoords array. The field generateVTexCoord tells to generate
 * the V texture coordinate dependend on the slide of the ring (TRUE) or to take
 * the V texture coordinate from the texCoords array for every slide in the ring
 * (FALSE). The field withColor spezifies the usage of the RGBA array. If the 
 * field is set to the value of TRUE a color property is generated from the RGBA
 * color array for every spezified vertex, which means for the whole ring. If it
 * is set to FALSE the RGBA array will be ignored. 
 *
 * Every Field has a sensor which calls the internal method to correct the 
 * update of the internal trianglestripset. To avoid the unnecessary generation
 * of the trianglestripset after the specification of every polyline coordinate
 * the fiels generateStripSet tells to generate the trianglestripset (TRUE) or 
 * not (FALSE). So before specifieing all coordinates, set this field to the 
 * value of FALSE then specify the coordinates, colors whatever you want, and 
 * after the specification you set the value to TRUE.
 */

//-----------------------------------------------------------------------------
// SoStreamLine
//
// Fields:
//   SoSFInt32 numOfSides
//   SoSFBool  withTexCoord
//   SoSFBool  withColor   
//   SoSFFloat vertex
//   SoMFFloat radius
//   SoMFColor RGBA
//   SoSFBool  generateVTexCoord;
//   SoMFVec2f texCoords
//   SoSFBool  generateStripSet;
//              
// children-parts:
//   SoTriangleStripSet stripSet
//   SoVertexProperty vp
//
//-----------------------------------------------------------------------------


class STBAPI_API SoStreamLine:public SoBaseKit 
{
    SO_KIT_HEADER(SoStreamLine);

    SO_KIT_CATALOG_ENTRY_HEADER(stripSet);   //the stripSet definition
    SO_KIT_CATALOG_ENTRY_HEADER(vp);         //the vertex properties

public:
    
    //The fields arew described in the description above.
    SoSFInt32 numOfSides;
    SoSFBool  withTexCoord;
    SoSFBool  withColor;
    SoMFVec3f vertex;
    SoSFFloat radius;
    SoMFColor RGBA;
    SoSFBool  generateVTexCoord;
    SoMFVec2f texCoords;
    SoSFBool  generateStripSet;

    static void initClass();       //call before fist usage
    SoStreamLine();                //constructor

private:
    ~SoStreamLine();               //destructor
    
	//sensor callback, generates the stripSet with its vertexproperties
    static void regenerate(void* data, SoSensor* sensor);

    //field sensor used to call the regenerate methode 
    //when a field value has been changed

    SoFieldSensor* numOfSlidesSensor;
    SoFieldSensor* withTexCoordSensor; 
    SoFieldSensor* withColorSensor;
    SoFieldSensor* vertexSensor;
    SoFieldSensor* radiusSensor;
    SoFieldSensor* RGBASensor;
    SoFieldSensor* generateVTexCoordSensor;
    SoFieldSensor* texCoordsSensor;
    SoFieldSensor* generateStripSetSensor;
    
	//Generates a ring of vertexproperties with the specified values for
    //one ring segment.
    static void addRing(int num, int steps, float radius,
        SbVec3f P, SbVec3f start,SbVec3f normal, SbColor *col,SbVec2f *texcoord, 
        SbBool generateVTexCoord, SoVertexProperty *VP);

    //flag to prevent multiple initClass()
	static SbBool baseInitFlag;        

}; //SoStreamLine

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOSTREAMLINEKIT_H_
