 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for CommonNodeFactory class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/CommonNodeFactory.cxx,v 1.2 2001/01/31 14:49:57 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "CommonNodeFactory.h"

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
}

// builds and returns a new Transformation node

Transformation * CommonNodeFactory::buildTransformation(
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
    return new Transformation( translation, scale, rotation );
}

// builds a new VirtualTransformation node

VirtualTransformation * CommonNodeFactory::buildVirtualTransformation(
                 StringMap& attributes)
{
    // little helper
    Transformation * help = buildTransformation( attributes );
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
    } else if( find( wrapperNodes.begin(), wrapperNodes.end(), name ) != wrapperNodes.end())
    {
        cout << "Build WrapperNode " << name << "." << endl;
        return new WrapperNode( name );
    }              
    return NULL;
}
