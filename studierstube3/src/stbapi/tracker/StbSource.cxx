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
/** source file for StbSource Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id: StbSource.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/SoLists.h>
#include <stbapi/tracker/StbSource.h>

using namespace std;

void StbSource::deleteCB( void * data, SoSensor * sensor)
{
	((SoFieldSensor *)sensor)->detach();
	StbSource * self = (StbSource *)data;
	self->setup = false;
}

void StbSource::positionCB( void * data, SoSensor * sensor)
{
	StbSource * self = (StbSource *) data;
	self->state.timeStamp();
	SoSFVec3f * vector = (SoSFVec3f *)((SoFieldSensor *)sensor)->getAttachedField();
	vector->getValue().getValue( self->state.position[0], 
								 self->state.position[1], 
								 self->state.position[2]);
	self->changed = true;
}

void StbSource::orientationCB( void * data, SoSensor * sensor)
{

	StbSource * self = (StbSource *) data;
	self->state.timeStamp();
	SoSFRotation * rotation = (SoSFRotation *)((SoFieldSensor *)sensor)->getAttachedField();
	// @todo check if the ordering of the quaternion pars is correct !!!
	rotation->getValue().getValue( self->state.orientation[0], 
								   self->state.orientation[1], 
								   self->state.orientation[2], 
								   self->state.orientation[3]);
	self->changed = true;
}

void StbSource::buttonCB( void * data, SoSensor * sensor)
{
	StbSource * self = (StbSource *) data;
	self->state.timeStamp();	
	SoSFInt32 * int32 = (SoSFInt32 *)((SoFieldSensor *)sensor)->getAttachedField();
	self->state.button = int32->getValue();
	self->changed = true;
}

void StbSource::confidenceCB( void * data, SoSensor * sensor)
{	
	StbSource * self = (StbSource *) data;
	self->state.timeStamp();	
	SoSFFloat * floatValue = (SoSFFloat *)((SoFieldSensor *)sensor)->getAttachedField();
	self->state.confidence = floatValue->getValue();
	self->changed = true;
}

int StbSource::isEventGenerator()
{
    return 1;
}

void StbSource::checkForNodes()
{
	setup = true;
	if( positionSensor.getAttachedField() == NULL && position.compare("") != 0)
	{
		setup = false;
		SoField * fieldInstance = getFieldByName( node + "." + position );
		if( fieldInstance != NULL )
		{
			if( fieldInstance->getTypeId() == SoSFVec3f::getClassTypeId())
			{
				positionSensor.attach( fieldInstance );
				setup = true;
			}
		}		
	}
	if( orientationSensor.getAttachedField() == NULL && orientation.compare("") != 0)
	{
		setup = false;
		SoField * fieldInstance = getFieldByName( node + "." + orientation );
		if( fieldInstance != NULL )
		{
			if( fieldInstance->getTypeId() == SoSFRotation::getClassTypeId())
			{
				orientationSensor.attach( fieldInstance );
				setup = true;
			}
		}		
	}
	if( buttonSensor.getAttachedField() == NULL && button.compare("") != 0)
	{
		setup = false;
		SoField * fieldInstance = getFieldByName( node + "." + button );
		if( fieldInstance != NULL )
		{
			if( fieldInstance->getTypeId() == SoSFInt32::getClassTypeId())
			{
				buttonSensor.attach( fieldInstance );
				setup = true;
			}
		}		
	}
	if( confidenceSensor.getAttachedField() == NULL && confidence.compare("") != 0)
	{
		setup = false;
		SoField * fieldInstance = getFieldByName( node + "." + confidence );
		if( fieldInstance != NULL )
		{
			if( fieldInstance->getTypeId() == SoSFFloat::getClassTypeId())
			{
				confidenceSensor.attach( fieldInstance );
				setup = true;
			}
		}		
	}
}

SoField * StbSource::getFieldByName( const string & longName )
{
	unsigned int pos = longName.find_first_of('.');
	if( pos != string::npos )
	{
		string nodeName = longName.substr(0,pos);
		string fieldName = longName.substr(pos + 1);
		SoNodeList nodeList;
		int len = SoNode::getByName( nodeName.c_str(), nodeList );
		for( int i = 0; i < len; i++ )
		{
			SoNode * nodeInstance = nodeList[i];
			SoField * fieldInstance = nodeInstance->getField( fieldName.c_str());
			if( fieldInstance != NULL )
			{
				return fieldInstance;
			}
		}		
	}
	return NULL;
}
