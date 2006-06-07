/* ========================================================================
 * Copyright (c) 2006,
 * Institute for Computer Graphics and Vision
 * Graz University of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Graz University of Technology nor the names of
 * its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/** source file for CommonNodeFactory class.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

// this will remove the warning 4786
#include "../tool/disable4786.h"

#include <stdlib.h>
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
#include "EllipsoidTransformNode.h"
#include "GKTransformNode.h"
#include "RangeFilterNode.h"
#include "PositionFilterNode.h"
#include "EventUtilityNode.h"

#include <math.h>
#include <float.h>
#include <stdio.h>
#include <iostream>

#include <algorithm>

//using namespace std;

#include <ace/Log_Msg.h>
#include "../tool/OT_ACE_Log.h"

// Constructor

namespace ot {

  CommonNodeFactory::CommonNodeFactory()
  {
    nodePorts.push_back("MergeAttribute");
    nodePorts.push_back("MergeTime");
    nodePorts.push_back("MergeTrigger");
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

int CommonNodeFactory::parseVector(const std::string & line, float * val )
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

int CommonNodeFactory::parseRotation(const std::string & line,const std::string & type, float * val )
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
			float det = MathUtils::determinant( matrix );
			if( det < 0.9 || det > 1.1 )
			{
				printf("Matrix is not a pure rotation matrix !\n");
				return 1;
			}
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

// constructs a new Node.

Node * CommonNodeFactory::createNode( const std::string& name, StringTable& attributes)
{
    float translation[3] = {0,0,0}, scale[3] = {1,1,1}, rot[4]={0,0,0,1};
    Node * result = NULL;
    if( name.compare("EventPositionTransform") == 0 ||
        name.compare("QueuePositionTransform") == 0 ||
        name.compare("TimePositionTransform") == 0 )
    {
        if( parseVector(attributes.get("translation"), translation ) != 0 )
        {
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error parsing translation !\n")));
        }
        if( parseVector(attributes.get("scale"), scale ) != 0 )
        {
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error parsing scale !\n")));
        }
        if( parseRotation(attributes.get("rotation"),
                           attributes.get("rotationtype"), rot ) != 0 )
        {
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error parsing rotation !\n")));
        }
        result =  new StaticTransformation( translation, scale, rot, true, false );
    }
    else if( name.compare("EventOrientationTransform") == 0 ||
        name.compare("QueueOrientationTransform") == 0 ||
        name.compare("TimeOrientationTransform") == 0 )
    {
        if( parseRotation(attributes.get("rotation"),
                           attributes.get("rotationtype"), rot ) != 0 )
        {
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error parsing rotation !\n")));
        }
        result = new StaticTransformation( translation, scale, rot, false, true );
    }
    else if( name.compare("EventTransform") == 0 ||
        name.compare("QueueTransform") == 0 ||
        name.compare("TimeTransform") == 0 )
    {
        if( parseVector(attributes.get("translation"), translation ) != 0 )
        {
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error parsing translation !\n")));
        }
        if( parseVector(attributes.get("scale"), scale ) != 0 )
        {
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error parsing scale !\n")));
        }
        if( parseRotation(attributes.get("rotation"),
                           attributes.get("rotationtype"), rot ) != 0 )
        {
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error parsing rotation !\n")));
        }
        result = new StaticTransformation( translation, scale, rot, true, true );
    }
    else if( name.compare("EventVirtualTransform") == 0 ||
             name.compare("QueueVirtualTransform") == 0 ||
             name.compare("TimeVirtualTransform") == 0 )
    {
        if( parseVector(attributes.get("translation"), translation ) != 0 )
        {
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error parsing translation !\n")));
        }
        if( parseRotation(attributes.get("rotation"),
                           attributes.get("rotationtype"), rot ) != 0 )
        {
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error parsing rotation !\n")));
        }
        result = new VirtualTransformation( translation, scale, rot, 1, 1 );
    }
    else if( name.compare("EventVirtualPositionTransform") == 0 ||
        name.compare("QueueVirtualPositionTransform") == 0 ||
        name.compare("TimeVirtualPositionTransform") == 0 )
    {
        if( parseVector(attributes.get("translation"), translation ) != 0 )
        {
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error parsing translation !\n")));
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
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error parsing rotation !\n")));
        }
        result = new VirtualTransformation( translation, scale, rot, 0, 1 );
    }
    else if( name.compare("EventQueue") == 0 )
    {
		int length;
		if( attributes.get("length", &length) != 1 )
		{
	        length = 1;
		}
		result = new EventQueueNode( length );
    }
    else if( name.compare("Merge") == 0 )
      {
	float agingFactor;
	MergeNode::CONF_CALCULATION confCalculation;
	attributes.get("agingFactor", &agingFactor);
	if (attributes.get("confidenceCalculation").compare("max") == 0)
	  confCalculation = MergeNode::MAX;
	else if (attributes.get("confidenceCalculation").compare("multiply") == 0)
	  confCalculation = MergeNode::MULTIPLY;
	else
	  confCalculation = MergeNode::MIN;
        result = new MergeNode(agingFactor, confCalculation);
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
        result = new DynamicTransformation( baseEvent, true, true );
    }
    else if( name.compare("EventDynamicPositionTransform") == 0 ||
        name.compare("QueueDynamicPositionTransform") == 0 ||
        name.compare("TimeDynamicPositionTransform") == 0 )
    {
        int baseEvent;
        if( attributes.get("baseevent").compare("true") == 0 )
            baseEvent = 1;
        else
            baseEvent = 0;
        result = new DynamicTransformation( baseEvent, true, false );
    }
    else if( name.compare("EventDynamicOrientationTransform") == 0 ||
        name.compare("QueueDynamicOrientationTransform") == 0 ||
        name.compare("TimeDynamicOrientationTransform") == 0 )
    {
        int baseEvent;
        if( attributes.get("baseevent").compare("true") == 0 )
            baseEvent = 1;
        else
            baseEvent = 0;
        result = new DynamicTransformation( baseEvent, false, true );
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
#ifndef OT_NO_CONFIDENCE_SUPPORT
    else if( name.compare("ConfidenceFilter") == 0 )
    {       
        float treshold= 0.5;
        ConfidenceFilterNode::types type = ConfidenceFilterNode::HIGH;
        attributes.get("treshhold", &treshold, 1 );
        if( attributes.get("type").compare("high") == 0 )
            type = ConfidenceFilterNode::HIGH;
        else if( attributes.get("type").compare("low") == 0 )
            type = ConfidenceFilterNode::LOW;
        result = new ConfidenceFilterNode( treshold, type );
    }
#endif
    else if( name.compare("Filter") == 0 )
    {
        std::vector<float> weights;
        const char * data = attributes.get("weight").c_str();
        char * end = (char *) data;
        weights.push_back((float) strtod( data, &end ));
        while( end != data ){        
            data = end;
            weights.push_back((float) strtod( data, &end ));
        }
		weights.pop_back();
		FilterNode::Type type = FilterNode::ALL;
		if( attributes.get("type").compare("position") == 0)
			type = FilterNode::POSITION;
		else if( attributes.get("type").compare("orientation") == 0 )
			type = FilterNode::ORIENTATION;
		ACE_DEBUG((LM_INFO, ACE_TEXT("ot:FilterNode with %d weights\n"), weights.size()));
        result = new FilterNode( weights, type );
    }
#ifndef OT_NO_CONFIDENCE_SUPPORT
    else if( name.compare("ConfidenceSelect") == 0 )
    {
        double timeout = 100;
        ConfidenceSelectNode::types type = ConfidenceSelectNode::HIGH;
        attributes.get("timeout", &timeout, 1 );        
        if( attributes.get("type").compare("high") == 0 )
            type = ConfidenceSelectNode::HIGH;
        else if( attributes.get("type").compare("low") == 0 )
            type = ConfidenceSelectNode::LOW;
        result = new ConfidenceSelectNode( timeout, type );
    }
#endif
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
#ifndef OT_NO_BUTTON_SUPPORT
	else if( name.compare("ButtonFilter") == 0 )
	{
		result = new ButtonFilterNode( attributes.get("buttonmask").data(), attributes.get("buttonmap").data(), attributes.get("invert").data(), 
			attributes.get("validtrans").data() , attributes.get("radiobuttons").data(), attributes.get("setbuttononvalidtrans").data());
	}
#endif
#ifndef OT_NO_CONFIDENCE_SUPPORT
    else if( name.compare("ButtonOp") == 0 )
    {
        result = new ButtonOpNode( (attributes.get("op").compare("OR") == 0)?(ButtonOpNode::OR):(ButtonOpNode::AND));
    }
#endif
    else if( name.compare("TimeGate") == 0 )
    {
        double timeframe;
        attributes.get("timeframe", &timeframe );
        TimeGateNode::Mode mode = (attributes.get("mode").compare("pass") == 0)?
                                    (TimeGateNode::PASS):(TimeGateNode::BLOCK);
        result = new TimeGateNode( timeframe, mode );
    }
    else if( name.compare("EventEllipsoidTransform") == 0 || 
			 name.compare("QueueEllipsoidTransform") == 0 ||
			 name.compare("TimeEllipsoidTransform") == 0 )
    {
        double a;
		double b;
		EllipsoidTransformNode::Mode mode;
        attributes.get("a", &a );
		if( attributes.containsKey("b"))
		{
			attributes.get("b", &b );
		}
		else
			b = a;
		if( attributes.get("mode").compare("toEllipsoid") == 0 )
			mode = EllipsoidTransformNode::toEllipsoid;
		else
			mode = EllipsoidTransformNode::toCartesian;
        result = new EllipsoidTransformNode( a, b, mode );
    }
#ifndef OT_NO_CONFIDENCE_SUPPORT
	else if( name.compare("EventGKTransform") == 0 || 
			 name.compare("QueueGKTransform") == 0 ||
			 name.compare("TimeGKTransform") == 0 )
    {
        double a = 0, b = 0, meridian = 0, alpha = 0, beta = 0, gamma = 0, delta = 0;
		GKTransformNode::Mode mode;
        attributes.get("a", &a );
		if( attributes.containsKey("b"))
		{
			attributes.get("b", &b );
		}
		else
			b = a;
		attributes.get("meridian", &meridian );
		attributes.get("alpha", &alpha );
		attributes.get("beta", &beta );
		attributes.get("gamma", &gamma );
		attributes.get("delta", &delta );		
		if( attributes.get("mode").compare("to") == 0 )
			mode = GKTransformNode::to;
		else
			mode = GKTransformNode::from;
        result = new GKTransformNode( a, b, meridian, alpha, beta, gamma, delta, mode );
    }
#endif
    else if( name.compare("RangeFilter") == 0 )
    {
        float min, max;
        if( attributes.get("min", &min) != 1 )
            min = 0;
        if( attributes.get("max", &max ) != 1 )
            max = (float)sqrt(FLT_MAX)-0.1f;
        result = new RangeFilterNode( min, max );
    }
    else if( name.compare("PositionFilterNode") == 0 )
    {
        float min[3], max[3];
        if( attributes.get("min", min, 3) != 3)
		{ min[0] = -1; min[1] = -1; min[2] = -1;}
        if( attributes.get("max", max, 3) != 3 )
		{ max[0] = 1; max[1] = 1; max[2] = 1;}
        result = new PositionFilterNode( min, max );
      }
    else if ( name.compare("EventUtilityNode") == 0 )
      {
	std::string rename = attributes.get("rename");
	std::string create = attributes.get("create");
	std::string discard = attributes.get("discard");

	EventUtilityNode *resultNode = new EventUtilityNode();

	if (rename.length() > 0)
	  {
	    std::string::size_type firstBlank = rename.find(' ');
	    if (firstBlank == std::string::npos)
	      ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error parsing names for renaming in EventUtilityNode!\n")));
	    else
	      {
		std::string oldName = rename.substr(0, firstBlank);
		std::string newName = rename.substr(firstBlank + 1, std::string::npos);
		resultNode->setRename(oldName, newName);
	      }
	  }
	if (create.length() > 0)
	  {
	    std::string::size_type firstBlank = create.find(' ');
	    std::string::size_type secondBlank = create.find(' ', firstBlank + 1);
	    if (firstBlank == std::string::npos ||
		secondBlank == std::string::npos)
	      {
		ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error parsing serialized event for creation in EventUtilityNode!\n")));
	      }
	    else
	      {
		std::string type = create.substr(0, firstBlank);
		std::string name = create.substr(firstBlank + 1, secondBlank - (firstBlank + 1));
		std::string value = create.substr(secondBlank + 1, std::string::npos);
		resultNode->setCreate(type, name, value);
	      }
	  }
	if (discard.length() > 0)
	    resultNode->setDiscard(discard);

	result = resultNode;
      }


    // the node ports are just looked up in a simple list
    else if( std::find( nodePorts.begin(), nodePorts.end(), name ) != nodePorts.end())
    {
		LOG_ACE_INFO("ot:Build NodePort %s.\n", name.c_str());
        return new NodePort();
    }
    if( result != NULL )
	{
		LOG_ACE_INFO("ot:Build %s node.\n", name.c_str());
	}
    return result;
}

} // namespace ot
