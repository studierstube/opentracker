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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/CommonNodeFactory.cxx,v 1.11 2001/05/22 08:36:13 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "CommonNodeFactory.h"
#include "MergeNode.h"
#include "DynamicTransformation.h"
#include "InvertTransformation.h"

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

CommonNodeFactory::~CommonNodeFactory()
{
    wrapperNodes.clear();
}

int CommonNodeFactory::parseVector(const string & line, float * val )
{
    float help[3];
    int num;
    num = sscanf( line.c_str()," %f %f %f", &help[0], &help[1], &help[2] );
    if( num != 3 )
    { 
        return 1;
    }
    val[0] = help[0];
    val[1] = help[1];
    val[2] = help[2];
    return 0;
}

int CommonNodeFactory::parseRotation(const string & line,const string & type, float * val )
{
    int num;
    float matrix[3][3];

    if( type.compare("quaternion") == 0 )
    {
        float help[4];
        num = sscanf(line.c_str()," %f %f %f %f",&help[0], &help[1], &help[2], &help[3]);
        if( num != 4 )
        {
            return 1;
        }
        val[0] = help[0];
        val[1] = help[1];
        val[2] = help[2];
        val[3] = help[3];
    }
    else if( type.compare("matrix") == 0 )
    {
        num = sscanf(line.c_str()," %f %f %f %f %f %f %f %f %f",
                &matrix[0][0], &matrix[0][1], &matrix[0][2],
                &matrix[1][0], &matrix[1][1], &matrix[1][2],
                &matrix[2][0], &matrix[2][1], &matrix[2][2] );
        if( num != 9 )
        {
            return 1;
        }
        else
        {
            MathUtils::matrixToQuaternion( matrix, val );
        }
    }
    else if( type.compare("euler") == 0 )
    {
        float roll, pitch, yaw;
        num = sscanf(line.c_str()," %f %f %f", &roll, &pitch, &yaw );
        if( num != 3 )
        {
            return 1;
        }
        else
        {
            MathUtils::eulerToQuaternion( roll, pitch, yaw, val );
        }
    }
    else if( type.compare("axisangle") == 0 )
    {
        float axisa[4];
        num = sscanf(line.c_str()," %f %f %f %f", &axisa[0], &axisa[1], &axisa[2], &axisa[3] );
        if( num != 4 )
        {
            return 1;
        }
        else
        {
            MathUtils::axisAngleToQuaternion( axisa, val );
        }
    }
    else 
    {
        return 1;
    }
    return 0;
}

// build a new EventQueue node

EventQueueNode * CommonNodeFactory::buildEventQueue(
                     StringTable& attributes)
{
    int num, length;
    num = sscanf(attributes.get("length").c_str(), " %i", &length );
    if( num != 1 )
    {
        length = 1;
    }
    return new EventQueueNode( length );
}

// constructs a new Node.

Node * CommonNodeFactory::createNode(  string& name,
                                      StringTable& attributes)
{
    float translation[3] = {0,0,0}, scale[3] = {1,1,1}, rot[4]={0,0,0,1};
    if( name.compare("EventPositionTransform") == 0 ||
        name.compare("QueuePositionTransform") == 0 ||
        name.compare("TimePositionTransform") == 0 )
    {        
        if( parseVector(attributes.get("translation"), translation ) != 0 )
        {
            cout << "Error parsing translation !" << endl;
        }
        if( parseVector(attributes.get("scale"), scale ) != 0 )
        {
            cout << "Error parsing scale !" << endl;
        }
        if( parseRotation(attributes.get("rotation"), 
                           attributes.get("rotationtype"), rot ) != 0 )
        {
            cout << "Error parsing rotation !" << endl;
        }
        cout << "Build "<< name << " node." <<endl;
        return new StaticTransformation( translation, scale, rot, 1, 0 );    
    }
    else if( name.compare("EventOrientationTransform") == 0 ||
        name.compare("QueueOrientationTransform") == 0 ||
        name.compare("TimeOrientationTransform") == 0 )
    {        
        if( parseRotation(attributes.get("rotation"), 
                           attributes.get("rotationtype"), rot ) != 0 )
        {
            cout << "Error parsing rotation !" << endl;
        }
        cout << "Build "<< name << " node." <<endl;
        return new StaticTransformation( translation, scale, rot, 0, 1 );    
    }
    else if( name.compare("EventTransform") == 0 ||
        name.compare("QueueTransform") == 0 ||
        name.compare("TimeTransform") == 0 )
    {        
        if( parseVector(attributes.get("translation"), translation ) != 0 )
        {
            cout << "Error parsing translation !" << endl;
        }
        if( parseVector(attributes.get("scale"), scale ) != 0 )
        {
            cout << "Error parsing scale !" << endl;
        }
        if( parseRotation(attributes.get("rotation"), 
                           attributes.get("rotationtype"), rot ) != 0 )
        {
            cout << "Error parsing rotation !" << endl;
        }
        cout << "Build "<< name << " node." <<endl;
        return new StaticTransformation( translation, scale, rot, 1, 1 );    
    } 
    else if( name.compare("EventVirtualTransform") == 0 ||
             name.compare("QueueVirtualTransform") == 0 ||
             name.compare("TimeVirtualTransform") == 0 )
    {
        if( parseVector(attributes.get("translation"), translation ) != 0 )
        {
            cout << "Error parsing translation !" << endl;
        }  
        if( parseRotation(attributes.get("rotation"), 
                           attributes.get("rotationtype"), rot ) != 0 )
        {
            cout << "Error parsing rotation !" << endl;
        }
        cout << "Build "<< name << " node." <<endl;
        return new VirtualTransformation( translation, scale, rot, 1, 1 );
    } 
    else if( name.compare("EventVirtualPositionTransform") == 0 ||
        name.compare("QueueVirtualPositionTransform") == 0 ||
        name.compare("TimeVirtualPositionTransform") == 0 )
    {        
        if( parseVector(attributes.get("translation"), translation ) != 0 )
        {
            cout << "Error parsing translation !" << endl;
        }
        cout << "Build "<< name << " node." <<endl;
        return new VirtualTransformation( translation, scale, rot, 1, 0 );    
    }
    else if( name.compare("EventVirtualOrientationTransform") == 0 ||
        name.compare("QueueVirtualOrientationTransform") == 0 ||
        name.compare("TimeVirtualOrientationTransform") == 0 )
    {        
        if( parseRotation(attributes.get("rotation"), 
                           attributes.get("rotationtype"), rot ) != 0 )
        {
            cout << "Error parsing rotation !" << endl;
        }
        cout << "Build "<< name << " node." <<endl;
        return new VirtualTransformation( translation, scale, rot, 0, 1 );    
    }
    else if( name.compare("EventQueue") == 0 )
    {
        cout << "Build "<< name << " node." <<endl;
        return buildEventQueue( attributes );
    } 
    else if( name.compare("Merge") == 0 )
    {
        cout << "Build "<< name << " node." << endl;
        return new MergeNode();
    } 
    else if( name.compare("EventDynamicTransform") == 0 ||
               name.compare("QueueDynamicTransform") == 0 ||
               name.compare("TimeDynamicTransform") == 0 )
    {
        cout << "Build " << name << " node." << endl;
        return new DynamicTransformation();
    } 
    else if( name.compare("EventInvertTransform") == 0 ||
             name.compare("QueueInvertTransform") == 0 ||
             name.compare("TimeInvertTransform") == 0 )
    {
        cout << "Build " << name << "node." << endl;
        return new InvertTransformation();
    }
    else if( find( wrapperNodes.begin(), wrapperNodes.end(), name ) != wrapperNodes.end())
    {
        cout << "Build WrapperNode " << name << "." << endl;
        return new WrapperNode();
    }              
    return NULL;
}
