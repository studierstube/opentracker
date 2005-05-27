// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       piputil
//  TYPE:       c++ header
//  PROJECT:    Studierstube
//  CONTENT:    Function prototypes for Auxilliary function for landscaping de
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    ds      Dieter Schmalstieg
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  27-aug-99 17:17:57  gh      last modification
//  17-sep-98 21:47:52  ds      created
// ===========================================================================


#ifndef _PIPUTIL_H_
#define _PIPUTIL_H_

#include <Inventor/SbLinear.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <stbapi/misc/SoArtifactKit.h>

//-----------------------------------------------------------------------------

extern SoSeparator* artifacts; //all artifacts sit under this separator

// pipSide: if the pip is turned around, another mode springs into place
extern int pipSide;            //0=frontfacing, 1=backfacing

//-----------------------------------------------------------------------------
//get position of an artifact

SbVec3f getNodePosition( //returns: position of artifact in world space
    SoNode* node         //artifact to search for
    );

//-----------------------------------------------------------------------------
//compute projection of an artifact's center to the plane defined by the 
//poly points. returns true if projection inisde poly and projection point
//as the passed reference parameter

SbBool projectArtifactToPip(   //returns true if projection inside
    SoArtifactKit* artifact,   //pointer to the artifact
    SbVec3f eye,               //position of camera in pip space
    SbMatrix pipMatrix,        //matrix world->pip space
    SoCoordinate3* poly,       //polygon in pip space
    SbVec3f& intersectionPoint //return value: projected point
    );

//-----------------------------------------------------------------------------

SbBool getMatrix(     //get matrix for given node, returns: true if successful             
    SoNode* node,     //node to search for
    SbMatrix& matrix  //returns computed matrix
    );

//-----------------------------------------------------------------------------

SbBool getPipMatrix(  //get matrix for given node, returns: true if successful             
    SoNode* node,     //node to search for
    SbMatrix& matrix  //returns computed matrix
    );

//-----------------------------------------------------------------------------

SbVec3f getPenPosition( //returns: position of the pen in world space
    );

//-----------------------------------------------------------------------------
#endif//_PIPUTIL_H_
