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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/CommonNodeFactory.cxx,v 1.27 2003/03/24 12:28:46 flo Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "CommonNodeFactory.h"
#include "MergeNode.h"
#include "VirtualTransformation.h"
#include "DynamicTransformation.h"
#include "InvertTransformation.h"
#include "MatrixTransformation.h"
#include "SelectionNode.h"
#include "ConfidenceFilterNode.h"
#include "ConfidenceSelectNode.h"
#include "FilterNode.h"
#include "ThresholdFilterNode.h"
#include "ButtonFilterNode.h"
#include "ButtonOpNode.h"
#include "TimeGateNode.h"
#include "ElasticFilterNode.h"

#include <cmath>
#include <cfloat>
#include <cstdio>
#include <iostream>

#include<algorithm>

using namespace std;

// Constructor

CommonNodeFactory::CommonNodeFactory()
{
    nodePorts.push_back("MergePosition");
    nodePorts.push_back("MergeOrientation");
    nodePorts.push_back("MergeButton");
    nodePorts.push_back("MergeConfidence");
    nodePorts.push_back("MergeTime");
    nodePorts.push_back("MergeDefault");
    nodePorts.push_back("TransformBase");
    nodePorts.push_back("Select");
    nodePorts.push_back("Arg1");
    nodePorts.push_back("Arg2");
    nodePorts.push_back("Gate");
}

CommonNodeFactory::~CommonNodeFactory()
{
    nodePorts.clear();
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
        MathUtils::normalizeQuaternion( val );
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

Node * CommonNodeFactory::createNode( const string& name, StringTable& attributes)
{
    float translation[3] = {0,0,0}, scale[3] = {1,1,1}, rot[4]={0,0,0,1};
    Node * result = NULL;
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
        result =  new StaticTransformation( translation, scale, rot, 1, 0 );
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
        result = new StaticTransformation( translation, scale, rot, 0, 1 );
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
        result = new StaticTransformation( translation, scale, rot, 1, 1 );
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
        result = new VirtualTransformation( translation, scale, rot, 1, 1 );
    }
    else if( name.compare("EventVirtualPositionTransform") == 0 ||
        name.compare("QueueVirtualPositionTransform") == 0 ||
        name.compare("TimeVirtualPositionTransform") == 0 )
    {
        if( parseVector(attributes.get("translation"), translation ) != 0 )
        {
            cout << "Error parsing translation !" << endl;
        }
        result = new VirtualTransformation( translation, scale, rot, 1, 0 );
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
        result = new VirtualTransformation( translation, scale, rot, 0, 1 );
    }
    else if( name.compare("EventQueue") == 0 )
    {
        result = buildEventQueue( attributes );
    }
    else if( name.compare("Merge") == 0 )
    {
        result = new MergeNode();
    }
    else if( name.compare("Selection") == 0 )
    {
        double timeOut = 100;
        attributes.get("timeout", &timeOut, 1 );        
        result = new SelectionNode(timeOut);
    }
    else if( name.compare("EventDynamicTransform") == 0 ||
               name.compare("QueueDynamicTransform") == 0 ||
               name.compare("TimeDynamicTransform") == 0 )
    {
        int baseEvent;
        if( attributes.get("baseevent").compare("true") == 0 )
            baseEvent = 1;
        else
            baseEvent = 0;
        result = new DynamicTransformation( baseEvent );
    }
    else if( name.compare("EventInvertTransform") == 0 ||
             name.compare("QueueInvertTransform") == 0 ||
             name.compare("TimeInvertTransform") == 0 )
    {
        result = new InvertTransformation();
    }
    else if( name.compare("EventMatrixTransform") == 0 ||
             name.compare("QueueMatrixTransform") == 0 ||
             name.compare("TimeMatrixTransform") == 0 )
    {
        float data[12];
        attributes.get( "matrix", data, 12 );
        result = new MatrixTransformation( data );
    }
    else if( name.compare("ConfidenceFilter") == 0 )
    {       
        float treshold= 0.5;
        ConfidenceFilterNode::types type;
        attributes.get("treshhold", &treshold, 1 );
        if( attributes.get("type").compare("high") == 0 )
            type = ConfidenceFilterNode::HIGH;
        else if( attributes.get("type").compare("low") == 0 )
            type = ConfidenceFilterNode::LOW;
        result = new ConfidenceFilterNode( treshold, type );
    }
    else if( name.compare("Filter") == 0 )
    {
        vector<float> weights;
        const char * data = attributes.get("weight").c_str();
        char * end = (char *) data;
        weights.push_back((float) strtod( data, &end ));    
        while( *end != 0 ){        
            data = end;
            weights.push_back((float) strtod( data, &end ));
        }
		FilterNode::Type type = FilterNode::ALL;
		if( attributes.get("type").compare("position") == 0)
			type = FilterNode::POSITION;
		else if( attributes.get("type").compare("orientation") == 0 )
			type = FilterNode::ORIENTATION;
        cout << "FilterNode with " << weights.size() << " weights\n";
        result = new FilterNode( weights, type );
    }
    else if( name.compare("ConfidenceSelect") == 0 )
    {
        double timeout = 100;
        ConfidenceSelectNode::types type;
        attributes.get("timeout", &timeout, 1 );        
        if( attributes.get("type").compare("high") == 0 )
            type = ConfidenceSelectNode::HIGH;
        else if( attributes.get("type").compare("low") == 0 )
            type = ConfidenceSelectNode::LOW;
        result = new ConfidenceSelectNode( timeout, type );
    }
    else if( name.compare("ThresholdFilter") == 0 )
    {
        float posmin = 0, posmax = FLT_MAX, rotmin = 0, rotmax = 3.141592654f;
        if( attributes.get("positionmax").compare("inf") == 0 )
            posmax = FLT_MAX;
        else
            attributes.get("positionmax", &posmax, 1 );
        attributes.get("positionmin", &posmin, 1 );
        attributes.get("rotationmin", &rotmin, 1 );
        attributes.get("rotationmax", &rotmax, 1 );
        if( posmin < 0 )
            posmin = 0;
        if( posmax < posmin )
            posmax = posmin;
        if( rotmin < 0 )
            rotmin = 0;
        if( rotmax < rotmin )
            rotmax = rotmin;
        if( rotmax > 3.141592654f )
            rotmax = 3.141592654f;
        result = new ThresholdFilterNode( posmin, posmax, rotmin, rotmax );
    }
	else if( name.compare("ButtonFilter") == 0 )
	{
		result = new ButtonFilterNode( attributes.get("buttonmask").data(), attributes.get("buttonmap").data() );
	}
    else if( name.compare("ButtonOp") == 0 )
    {
        result = new ButtonOpNode( (attributes.get("op").compare("OR") == 0)?(ButtonOpNode::OR):(ButtonOpNode::AND));
    }
    else if( name.compare("TimeGate") == 0 )
    {
        double timeframe;
        attributes.get("timeframe", &timeframe );
        TimeGateNode::Mode mode = (attributes.get("mode").compare("pass") == 0)?
                                    (TimeGateNode::PASS):(TimeGateNode::BLOCK);
        result = new TimeGateNode( timeframe, mode );
    }
    // the node ports are just looked up in a simple list
    else if( find( nodePorts.begin(), nodePorts.end(), name ) != nodePorts.end())
    {
        cout << "Build NodePort " << name << "." << endl;
        return new NodePort();
    }
    if( result != NULL )
        cout << "Build "<< name << " node." <<endl;
    return result;
}
