 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
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
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1090 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for CommonNodeFactory class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/CommonNodeFactory.cxx,v 1.6 2001/03/27 06:08:50 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "CommonNodeFactory.h"
#include "MergeNode.h"
#include "DynamicTransformation.h"

#include<math.h>
#include<stdio.h>
#ifdef WIN32
#include <iostream>    // VisualC++ uses STL based IOStream lib
#else
#include <iostream.h>
#endif

#include<algorithm>

// Constructor

CommonNodeFactory::CommonNodeFactory()
{
    wrapperNodes.push_back("MergePosition");
    wrapperNodes.push_back("MergeOrientation");
    wrapperNodes.push_back("MergeButton");
    wrapperNodes.push_back("MergeConfidence");
    wrapperNodes.push_back("MergeTime");
    wrapperNodes.push_back("MergeDefault");
    wrapperNodes.push_back("TransformBase");
}

// builds and returns a new Transformation node

StaticTransformation * CommonNodeFactory::buildTransformation(
                                          StringMap& attributes)
{
    float translation[3], scale[3], rotation[4];
    float matrix[3][3];
    int num;
    num = sscanf( (*attributes.find("translation")).second.c_str(),
            " %f %f %f", &translation[0], &translation[1], &translation[2] );
    if( num != 3 )
    {
        translation[0] = 0;
        translation[1] = 0;
        translation[2] = 0;
    }
    num = sscanf( (*attributes.find("scale")).second.c_str(),
            " %f %f %f", &scale[0], &scale[1], &scale[2] );
    if( num != 3 )
    {
        scale[0] = 1;
        scale[1] = 1;
        scale[2] = 1;
    }
     std::string &type = (*attributes.find("rotationtype")).second;
    if( type.compare("quaternion") == 0 )
    {
        num = sscanf((*attributes.find("rotation")).second.c_str(),
                " %f %f %f %f", &rotation[0], &rotation[1], &rotation[2], &rotation[3] );
        if( num != 4 )
        {
            rotation[0] = 0;
            rotation[1] = 0;
            rotation[2] = 0;
            rotation[3] = 1;
        }
    }
    else if( type.compare("matrix") == 0 )
    {
        num = sscanf((*attributes.find("rotation")).second.c_str(),
                " %f %f %f %f %f %f %f %f %f",
                &matrix[0][0], &matrix[0][1], &matrix[0][2],
                &matrix[1][0], &matrix[1][1], &matrix[1][2],
                &matrix[2][0], &matrix[2][1], &matrix[2][2] );
        if( num != 9 )
        {
            rotation[0] = 0;
            rotation[1] = 0;
            rotation[2] = 0;
            rotation[3] = 1;
        }
        else
        {
            MathUtils::matrixToQuaternion( matrix, rotation );
        }
    }
    else if( type.compare("euler") == 0 )
    {
        float roll, pitch, yaw;
        num = sscanf((*attributes.find("rotation")).second.c_str(),
                " %f %f %f", &roll, &pitch, &yaw );
        if( num != 3 )
        {
            rotation[0] = 0;
            rotation[1] = 0;
            rotation[2] = 0;
            rotation[3] = 1;
        }
        else
        {
            MathUtils::eulerToQuaternion( roll, pitch, yaw, rotation );
        }
    }
    else if( type.compare("axisangle") == 0 )
    {
        float axisa[4];
        num = sscanf((*attributes.find("rotation")).second.c_str(),
                " %f %f %f %f", &axisa[0], &axisa[1], &axisa[2], &axisa[3] );
        if( num != 4 )
        {
            rotation[0] = 0;
            rotation[1] = 0;
            rotation[2] = 0;
            rotation[3] = 1;
        }
        else
        {
            MathUtils::axisAngleToQuaternion( axisa, rotation );
        }
    }
    return new StaticTransformation( translation, scale, rotation );
}

// builds a new VirtualTransformation node

VirtualTransformation * CommonNodeFactory::buildVirtualTransformation(
                 StringMap& attributes)
{
    // little helper
    StaticTransformation * help = buildTransformation( attributes );
    VirtualTransformation *result = new VirtualTransformation( help->getTranslation(),
            help->getScale(), help->getRotation());
    delete help;
    return result;
}

// build a new EventQueue node

EventQueueNode * CommonNodeFactory::buildEventQueue(
                     StringMap& attributes)
{
    int num, length;
    num = sscanf((*attributes.find("length")).second.c_str(), " %i", &length );
    if( num != 1 )
    {
        length = 1;
    }
    return new EventQueueNode( length );
}

// constructs a new Node.

Node * CommonNodeFactory::createNode(  string& name,
                                      StringMap& attributes)
{
    if( name.compare("EventTransformation") == 0 ||
        name.compare("QueueTransformation") == 0 ||
        name.compare("TimeTransformation") == 0 )
    {
        cout << "Build "<< name << " node." <<endl;
        return buildTransformation( attributes );
    } else if( name.compare("EventVirtualTransformation") == 0 ||
               name.compare("QueueVirtualTransformation") == 0 ||
               name.compare("TimeVirtualTransformation") == 0 )
    {
        cout << "Build "<< name << " node." <<endl;
        return buildVirtualTransformation( attributes );
    } else if( name.compare("EventQueue") == 0 )
    {
        cout << "Build "<< name << " node." <<endl;
        return buildEventQueue( attributes );
    } else if( name.compare("Merge") == 0 )
    {
        cout << "Build "<< name << " node." << endl;
        return new MergeNode();
    } else if( name.compare("EventDynamicTransformation") == 0 ||
               name.compare("QueueDynamicTransformation") == 0 ||
               name.compare("TimeDynamicTransformation") == 0 )
    {
        cout << "Build " << name << " node." << endl;
        return new DynamicTransformation();
    } else if( find( wrapperNodes.begin(), wrapperNodes.end(), name ) != wrapperNodes.end())
    {
        cout << "Build WrapperNode " << name << "." << endl;
        return new WrapperNode();
    }              
    return NULL;
}
