// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       SoAngularConstraint.h
//  TYPE:       c++ header file
//  PROJECT:    Studierstube
//  CONTENT:    Definition of class SoAngularConstraint
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    rcs	Rainer Splechtna
// ===========================================================================
//  HISTORY:
//
//  6-dec-99 10:10:00  rcs     last modification
// ===========================================================================

#ifndef _SOANGULARCONSTRAINT_H_
#define _SOANGULARCONSTRAINT_H_

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/SbLinear.h>
#include <stbapi/stbapi.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

// This engine multiplies two rotations connected to the input fields: rotA and rotB.
// The product is written to the output-field: product.

class SoAngularConstraint : public SoEngine
{
	SO_ENGINE_HEADER(SoAngularConstraint);

public:
	// input fields: two rotations
	SoMFVec3f	raysP0;
    SoMFVec3f	raysP1;
	SoSFRotation	headRotation;
    SoSFVec3f   headPosition;
    SoSFVec3f   penPosition;
    SoSFVec3f   markerPosition;
    SoSFFloat minimumAngle;
    SoSFFloat validDistance;

	// output is a rotation
	SoEngineOutput valid;	// (SoSFBool) fulfills constraint

	static void initClass();
	
	// Constructor
	SoAngularConstraint();

private:
	// Destructor
	virtual ~SoAngularConstraint();
	
	// evaluation method
	virtual void evaluate();

};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif


#endif

