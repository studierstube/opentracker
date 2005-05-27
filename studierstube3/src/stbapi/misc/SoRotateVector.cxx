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
/** Source file for SoRotateVector class
  *
  * @author Istvan Barakonyi
  *
  * $Id: SoRotateVector.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file
  * ======================================================================= */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <stbapi/misc/SoRotateVector.h>
#include <stbapi/util/math2.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

SO_ENGINE_SOURCE(SoRotateVector);

void SoRotateVector::initClass()
{
	SO_ENGINE_INIT_CLASS(SoRotateVector, SoEngine, "Engine");
}

SoRotateVector::SoRotateVector()
{
	SO_ENGINE_CONSTRUCTOR(SoRotateVector);

	SO_ENGINE_ADD_INPUT(input, (0.0,0.0,0.0));
    SO_ENGINE_ADD_INPUT(rotation, (SbRotation::identity()));
    SO_ENGINE_ADD_INPUT(invertRotation, (FALSE));
    
	SO_ENGINE_ADD_OUTPUT(output, SoMFVec3f);
}

SoRotateVector::~SoRotateVector()
{
}

void SoRotateVector::evaluate()
{
    int inputNum=input.getNum();
    int rotationNum=rotation.getNum();
    int invertNum=invertRotation.getNum();
    int maxNum=(inputNum && rotationNum && invertNum) ? m2max(m2max(inputNum,rotationNum),invertNum) : 0;

    SO_ENGINE_OUTPUT(output,SoMFVec3f,setNum(maxNum));

    if (output.getNumConnections()){
        for (int i=0;i<maxNum;i++){
            SbVec3f result;
            SbVec3f inp=input[m2min(i,inputNum-1)];
            SbRotation rot=rotation[m2min(i,rotationNum-1)];
            SbBool inv=invertRotation[m2min(i,invertNum-1)];
            if (inv) rot.invert().multVec(inp,result);
            else rot.multVec(inp,result);
            SO_ENGINE_OUTPUT(output,SoMFVec3f,set1Value(i,result));
        }
    }
}


