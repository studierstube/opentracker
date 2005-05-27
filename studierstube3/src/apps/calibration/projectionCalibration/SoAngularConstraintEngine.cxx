// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       SoAngularConstraint.cxx
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    Implementation of class SoAngularConstraint
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    rcs	Rainer Splechtna
// ===========================================================================
//  HISTORY:
//
//  6-dec-99 10:10:00  rcs     last modification
// ===========================================================================

#include "SoAngularConstraintEngine.h"

SO_ENGINE_SOURCE(SoAngularConstraint);

void
SoAngularConstraint::initClass() 
{
	SO_ENGINE_INIT_CLASS(SoAngularConstraint, SoEngine, "Engine");
}

SoAngularConstraint::SoAngularConstraint() 
{
	SO_ENGINE_CONSTRUCTOR(SoAngularConstraint);

	// define input fields and their default values
	SoMFVec3f	raysP0;
    SoMFVec3f	raysP1;
	SoSFRotation	headRotation;
    SoSFVec3f   headPosition;
    SoSFVec3f   penPosition;
    SoSFFloat angularConstraint;
    SoSFFloat epsilon;

    SO_ENGINE_ADD_INPUT(raysP0, (SbVec3f(0.0,0.0,0.0)));
    SO_ENGINE_ADD_INPUT(raysP1, (SbVec3f(0.0,0.0,0.0)));
    SO_ENGINE_ADD_INPUT(headPosition, (SbVec3f(0.0,0.0,0.0)));
    SO_ENGINE_ADD_INPUT(headRotation, (SbVec3f(0.0,0.0,1.0),0.0));
    SO_ENGINE_ADD_INPUT(penPosition,  (SbVec3f(0.0,0.0,0.0)));
    SO_ENGINE_ADD_INPUT(markerPosition,  (SbVec3f(0.0,0.0,0.0)));
    SO_ENGINE_ADD_INPUT(minimumAngle, (0.35));
    SO_ENGINE_ADD_INPUT(validDistance, (0.35));
    	
	// define the output
	SO_ENGINE_ADD_OUTPUT(valid, SoSFBool);
}



SoAngularConstraint:: ~SoAngularConstraint()
{
	// NIL
}

/*  An engine computing, whether the current position of the pen
    (input penPosition) intersects (within valid distance) with all rays and 
    the angle between the current tested ray and all rays computed till now is
    greater than the given minimum angle.
*/

void
SoAngularConstraint::evaluate()
{
    SbLine rays[16];
    SbBool result = TRUE;
    if (raysP0.getNum() > 0)
    {
	    SbVec3f p0, p1;
        SbLine test;
	    SbRotation headRot;
	    
        // compute rays
   	    headRotation.getValue().inverse().multVec(
            penPosition.getValue()    - headPosition.getValue(), p0);
   	    headRotation.getValue().inverse().multVec(
            markerPosition.getValue() - headPosition.getValue(), p1);
            
        int i;

        for(i=0; i<raysP0.getNum(); i++)
            rays[i].setValue(raysP0[i], raysP1[i]);
        test.setValue(p0, p1);

        // check intersection of rays
        SbVec3f closest;
        SbVec3f pOnLine2, pOnLine1;
    	for (i=0; i<raysP0.getNum(); i++)
        {
			test.getClosestPoints(rays[i], pOnLine1, pOnLine2);
			closest = pOnLine2 - pOnLine1;
			if (closest.length() > validDistance.getValue()) 
                result = FALSE;
		}
        // check angles of rays
        if (result)
            for (i=0; i< raysP0.getNum(); i++)
            {
                // test direction = t, ray direction = r
                // acos(dot(r,t)/r*t)
                // TODO: r and t have length 1, hence the division is skipped 
                // just calculate acos(dot(r,t)) the result is not that accurate but the 
                // calculation is quicker
                float cosOfAngle = test.getDirection().dot(rays[i].getDirection())
                   / (test.getDirection().length() * rays[i].getDirection().length());
                if (cosOfAngle >= 1.0)
                    cosOfAngle = 1.0;
                float angle = acos(cosOfAngle);
                //printf("angle: %f\n", angle);
                if (angle < minimumAngle.getValue())
                    result = FALSE;
            }
    }
  	SO_ENGINE_OUTPUT(valid, SoSFBool, setValue(result));
}

/* ===========================================================================
    End of SoAngularConstraint.cxx
   ===========================================================================
    Automatic Emacs configuration follows.
    Local Variables:
    mode:c++
    c-basic-offset: 4
    eval: (c-set-offset 'substatement-open 0)
    eval: (c-set-offset 'case-label '+)
    eval: (c-set-offset 'statement 'c-lineup-runin-statements)
    eval: (setq indent-tabs-mode nil)
    End:
=========================================================================== */
