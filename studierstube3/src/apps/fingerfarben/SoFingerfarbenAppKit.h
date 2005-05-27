/* ========================================================================
  * Copyright (C) 2000  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Dieter Schmalstieg under
  * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ======================================================================== 
  * PROJECT: Studierstube
  * ======================================================================== */ 

 /* ======================================================================= */
 /** 
  * Defines SoFingerfarbenAppKit, a Studierstube ContextKit that represents an 
  * application for rapid prototyping of shapes.
  *
  * \author  Flo Ledermann <flo@subnet.at>
  *
  * $Id: SoFingerfarbenAppKit.h 3801 2004-09-30 14:02:02Z reitmayr $
  * \file                                                                   */
 /* ======================================================================= */

/**
 * @defgroup fingerfarben Fingerfarben
 * @ingroup apps
 * Group for all classes that were implemented during the Praktikum
 * of Flo Lermann <ledermann@ims.tuwien.ac.at>.
 */

#ifndef _SOFINGERFARBENAPPKIT_H_
#define _SOFINGERFARBENAPPKIT_H_

#include <stbapi/workspace/SoContextKit.h>
#include <common/StaticTransformation.h>

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>

#include "fingerfarben.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** 
Studierstube Application for rapid prototyping of architectural and design
models. This application is intended to be used with two pointing devices 
(e.g. pinch-gloves) to define the points in space that constitute the vertices
of an IndexedFaceSet.

Button 1 is used to define the point pairs that extend the current surface, 
button 2 starts a new surface. The material used for the current surface is looked
up in a node with the name "pen_material" in the primary pen of the user.

@todo Implement ray-snapping machanism for easier snapping to existing points in the model.
@todo Implement creation methods for geometric primitives, NURBS etc.

@ingroup fingerfarben
*/
class FINGERFARBEN_API SoFingerfarbenAppKit : public SoContextKit
{
    SO_KIT_HEADER(SoFingerfarbenAppKit);

public:

    /// Static class initialization.
    static void initClass();

    /// Constructor.
    SoFingerfarbenAppKit();

    // Override this to create new context of a specific type.  
    SoContextKit* factory();  
    
    /// Destructor.
    ~SoFingerfarbenAppKit(); 

    // Currently unused.
	void checkPipMasterMode(SoNode* pipSheet, SbBool masterMode);

	virtual void checkPipConnections( int uid, SoNode * pipSheetContents );

    /// Sets the current material used for creating surfaces.
	void setMaterial(SoMaterial* material);

    /**
    Watches changes in the applications geometry (e.g if the model is deleted by
    some other application.
    */
    static void geometrySensorCB(void* data, SoSensor* sensor);

    /** 
    Create window and content. Basically reads the contents of geometry/app_geom.iv.
    @param index index of window 
    */
    SbBool checkWindowGeometry();

    /// The geometry node of our model.
	SoGroup* modelGeometry;

protected:
    /// Callback for events, for example button clicks or tracker movements.
	SbBool windowEventCB(void* data, int message, SoWindowKit* window, int uid, So3DEvent* event, SoPath* rootPath);

private:
    /// The translation of the primary hand (= "official" stb pen).
	SoTransform* pPenTrans;

    /// The translation of the secondary hand (second pen).
	SoTransform* pSecondTrans;

    /// Temporary line drawn when beginning a new surface.
	SoSeparator* tempLine;
	
    /// The applications window.
	SoWindowKit* myWindow;

    /// Indicates whether we are currently building a surface.
	bool inBuild;

    /// Indicates whether we just started a new surface.
	bool newSurface;

    /// The current surface we are working on.
	SoSeparator* currentSurface;

    /// Create new pip sheet. Since we dont use the pip, we just need this to get the user id.
    SbBool checkPipGeometry();
	
    /// Helper function to create a line with given start- and endpoints.
	SoSeparator* makeLine(SbVec3f & start, SbVec3f & end);
	
    /// Helper function to create a SoIndexedFaceSet with given start- and endpoints.
    SoSeparator* makeSurface(SbVec3f & start, SbVec3f & end);

    /// Helper function to extend the current indexedFaceSet by two points.
	void extendSurface(SoSeparator* surface, SbVec3f &start, SbVec3f &end);

    /**
    Helper function to find a named node within another node.
    @return The found node or NULL if the node was not found.
    */
    static SoNode* myFindNode(SoNode* root, SbString name);

    /**
    Snaps the point to the nearest already set point within
    a given radius (4cm).

    @todo Make this configurable.
    */
    void snap(SbVec3f &point);
    /**
    Snaps the point to the nearest already set point within
    a given radius of a ray that originates in the users eye and
    passes through the point.

    @todo Implement this ;).
    */
    void raySnap(SbVec3f &point);

    /// Geometry of the left snap-point.
    SoSeparator* leftCursor;
    /// Geometry of the right snap-point.
    SoSeparator* rightCursor;

}; //class SoFingerfarbenAppKit

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOFINGERFARBENAPPKIT_H_

