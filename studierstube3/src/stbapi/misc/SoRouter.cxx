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
/** SoRouter.cxx  - implementation of class SoRouter (version 2).
  * SoRouter is the implementation of a Node that allows to create and manipulate
  * field connections.
  *
  * @author  Thomas Psik
  * @author  Flo Lederman
  *
  * $Id: SoRouter.cxx 3716 2004-09-16 12:49:44Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#include "SoRouter.h"
#include <string>

using namespace std;

SO_NODE_SOURCE(SoRouter);

void
SoRouter::initClass()
{
    SO_NODE_INIT_CLASS(SoRouter, SoRoute, "SoRoute");
}

// -------------------------------
// -------------------------------
#ifdef SUPPORT_OLD_SOROUTER
// -------------------------------
// -------------------------------

SoRouter::SoRouter()
{
    SO_NODE_CONSTRUCTOR(SoRouter);

    SO_NODE_ADD_FIELD(sourceNode, (NULL));
    SO_NODE_ADD_FIELD(sourceEngine, (NULL));
    SO_NODE_ADD_FIELD(sourceField, (""));
    SO_NODE_ADD_FIELD(sourceRef, (""));

    SO_NODE_ADD_FIELD(destNode, (NULL));
    SO_NODE_ADD_FIELD(destEngine, (NULL));
    SO_NODE_ADD_FIELD(destField, (""));
    SO_NODE_ADD_FIELD(destRef, (""));

    SO_NODE_ADD_FIELD(route, (FALSE));
	SO_NODE_ADD_FIELD(setValue, (FALSE));
	SO_NODE_ADD_FIELD(pri, (50));
	SO_NODE_ADD_FIELD(evaluateAtStart, (TRUE));


	SO_NODE_ADD_FIELD(sourceFieldContainer, (NULL));
    SO_NODE_ADD_FIELD(destFieldContainer, (NULL));


	// activated after read-in
	fromSensor.setFunction( SoRouter::routeChangedCB);
	fromSensor.setData( this );

	sourceSensor.setFunction( SoRouter::routeChangedCB);
	sourceSensor.setData( this );

	sourceRefSensor.setFunction( SoRouter::routeChangedCB );
	sourceRefSensor.setData( this );

	toSensor.setFunction( SoRouter::routeChangedCB );
	toSensor.setData( this );

	destSensor.setFunction( SoRouter::routeChangedCB );
	destSensor.setData( this );

	destRefSensor.setFunction( SoRouter::routeChangedCB );
	destRefSensor.setData( this );

	routeSensor.setFunction( SoRouter::routeChangedCB);
	routeSensor.setData( this );
	routeSensor.setPriority(pri.getValue());

	toDeletedSensor.setDeleteCallback(SoRouter::toDeletedCB, this);

}

SoRouter::~SoRouter()
{
	// disconnect at destruction
	if (toField != NULL)
		toField->disconnect();
}

/// Reads value(s) of node
SbBool SoRouter::readInstance(SoInput *in, unsigned short flags )
{
	// --------- copy from SoRouter::readInstance
	SoFieldContainer::readInstance(in, flags);

	// do compatibity for old style
    if (from.getValue().getLength() == 0)
		from.setValue(sourceRef.getValue());
    if (to.getValue().getLength() == 0)
	    to.setValue(destRef.getValue());

	// now check if values are correct !
    SbBool valuesOK = TRUE;

	// do not check routes if flag evaluateAtStart is cleared (set to FALSE)
	routeSensor.attach(&route);
	routeSensor.setPriority(pri.getValue());


	valuesOK &= updateAllFromPtr(in);
	valuesOK &= updateAllToPtr(in);

	if (evaluateAtStart.getValue() == FALSE)
		return TRUE;

	// if either function is FALSE return with failure
	if (!valuesOK) return FALSE;


	// --------- this part differs from SoRouter::readInstance !!
	updateRoute();

	// activate sensors for from && to
    fromSensor.attach(&from);
	sourceSensor.attach(&sourceField);
	sourceRefSensor.attach(&sourceRef);

	toSensor.attach(&to);
	destSensor.attach(&destField);
	destRefSensor.attach(&destRef);


	return TRUE;
}

void SoRouter::toDeletedCB(void* data, SoSensor* which)
{
	((SoRouter*)data)->toField = NULL; // disconnect should happen in destructor of SoField
}

void SoRouter::sourceRefChangedCB(void* data, SoSensor* which)
{
  SoRouter* self = ((SoRouter*)data);
  self->from.setValue(self->sourceRef.getValue());
 // will trigger routeChangedCB
}

void SoRouter::destRefChangedCB(void* data, SoSensor* which)
{
  SoRouter* self = ((SoRouter*)data);
  self->to.setValue(self->destRef.getValue());
 // will trigger routeChangedCB
}


void SoRouter::routeChangedCB(void* data, SoSensor*)
{
	SoRouter* self = ((SoRouter*)data);
	// could be unsecure if node containing the field has been deleted !!
	if (self->toField != NULL)
	{
		self->toField->disconnect();
	}
//	printf(" SoRouter::routeChangedCB\n");
	// update the pointers to get valid field adresses
	self->updateAllFromPtr();
	self->updateAllToPtr();
	// now check route value
	self->updateRoute();
}

void SoRouter::updateRoute()
{
	if (route.getValue() == TRUE)
	{
		if (fromField != NULL)
		{
			toField->connectFrom(fromField);
		}
		else
		{
			if (fromEngineOutput != NULL)
				toField->connectFrom(fromEngineOutput);
			// if there was an error  -> disconnect happend before finding the error (from / to changed)
		}

		if (setValue.getValue() == TRUE)
		{
			SbString value;
			fromField->get(value);
			toField->set(value.getString());
			printf("value set %s\n",value.getString());
		}
	}
	else
	{
		if (toField != NULL)
			toField->disconnect(); // it is allowed to do this, even if the field is not connected
	}
}

SbBool SoRouter::updateAllFromPtr(SoInput *in)
{
	// clear old values.
	fromField = NULL;
	fromEngineOutput = NULL;

	// in SoRouter more than one method exists from specify the from field
	// hierachy is from, then fromFieldContainer, then sourceNode, then sourceEngine

	if (from.getValue().getLength()> 0)
	{
		return updateFromPtr(in);
	}

	SoFieldContainer* sourceNodePtr = NULL;

	if (sourceFieldContainer.getValue() != NULL)
	{
		sourceNodePtr = sourceFieldContainer.getValue();
	}
	else if (sourceNode.getValue() != NULL)
	{
		sourceNodePtr = sourceNode.getValue();
	}

	if (sourceNodePtr != NULL) // source is a Node
	{
		fromField = sourceNodePtr->getField(SbName(sourceField.getValue()));
		if (fromField == NULL)
		{
			printf("SoRouter::updateAllFromPtr [%s] is not a field of node [%s]. Route not established.\n",
                   sourceField.getValue().getString(), sourceNodePtr->getName().getString());
			return FALSE;
		}
		return TRUE;
	}
	else if (sourceEngine.getValue() != NULL)
	{
		SoEngine *sourceEnginePtr = sourceEngine.getValue();
		fromEngineOutput = sourceEnginePtr->getOutput(SbName(sourceField.getValue()));
		if (fromEngineOutput == NULL)
		{
			printf("SoRouter::updateAllFromPtr [%s] is not a field of engine [%s]. Route not established.\n",
                   sourceField.getValue().getString(), sourceEnginePtr->getName().getString());
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		printf("SoRouter::updateAllFromPtr neither from, fromFieldContainer, sourceNode, sourceEngine are valid. No route established.\n");
	}
	return FALSE;
}

SbBool SoRouter::updateAllToPtr(SoInput *in)
{
	// clear old value.
	toField = NULL;

	// in SoRouter more than one method exists to specify the to field
	// hierachy is to, then toFieldContainer, then destNode, then destEngine

	if (to.getValue() != "")
	{
		SbBool ret = updateToPtr(in);
		if (toField != NULL)
			toDeletedSensor.attach(toField);
		return ret;
	}

	SoFieldContainer* dest = NULL;

	if (destFieldContainer.getValue() != NULL)
	{
		dest = destFieldContainer.getValue();
	}
	else if (destNode.getValue() != NULL)
	{
		dest = destNode.getValue();
	}
	else if (destEngine.getValue() != NULL)
	{
		dest = destEngine.getValue();
	}

	if (dest != NULL)
	{
		toField = dest->getField(SbName(destField.getValue()));
		if (toField == NULL)
		{
			printf("SoRouter::updateAllToPtr [%s] is not a field of [%s]. Route not established.\n",
                   destField.getValue().getString(), dest->getName().getString());
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		printf("SoRouter::updateAllToPtr neither to, toFieldContainer, destNode, destEngine are valid. No route established.\n");
	}

	if (toField != NULL)
		toDeletedSensor.attach(toField);

	return FALSE;
}


// -------------------------------
// -------------------------------
#else
// -------------------------------
// NEUE IMPLEMENTIERUNG nur from und to bleiben

SoRouter::SoRouter()
{
    SO_NODE_CONSTRUCTOR(SoRouter);

    SO_NODE_ADD_FIELD(route, (false));


	// activated after read-in
	fromSensor.setFunction( SoRouter::routeChangedCB);
	fromSensor.setData( this );

	toSensor.setFunction( SoRouter::routeChangedCB );
	toSensor.setData( this );

	routeSensor.setFunction( SoRouter::routeChangedCB);
	routeSensor.setData( this );

	toDeletedSensor.setDeleteCallback(SoRouter::toDeletedCB, this);
}

SoRouter::~SoRouter()
{
	// disconnect at destruction
	toField->disconnect();
}

/// Reads value(s) of node
SbBool SoRouter::readInstance(SoInput *in, unsigned short flags )
{
	// --------- copy from SoRouter::readInstance
	SoFieldContainer::readInstance(in, flags);

	// now check if values are correct !
    SbBool valuesOK = TRUE;

	valuesOK &= updateFromPtr(in);
	valuesOK &= updateToPtr(in);
	toDeletedSensor.attach(toField);

	// if either function is FALSE return with failure
	if (!valuesOK) return FALSE;

	// --------- this part differs from SoRouter::readInstance !!
	updateRoute();

	// activate sensors for from && to
    fromSensor.attach(&from);
	toSensor.attach(&to);
	routeSensor.attach(&route);

	return TRUE;
}

void SoRouter::toDeletedCB(void* data, SoSensor* which)
{
	((SoRouter*)data)->toField = NULL; // disconnect should happen in destructor of SoField
}

void SoRouter::routeChangedCB(void* data, SoSensor*)
{
	SoRouter* self = ((SoRouter*)data);
	// could be unsecure if node containing the field has been deleted !!
	if (self->toField != NULL)
	{
		self->toField->disconnect();
	}
	// update the pointers to get valid field adresses
	self->updateFromPtr();
	self->updateToPtr();
	self->toDeletedSensor.attach(self->toField);

	// now check route value
	self->updateRoute();
}

void SoRouter::updateRoute()
{
	if (route.getValue() == TRUE)
	{
		if (fromField != NULL)
		{
			toField->connectFrom(fromField);
		}
		else
		{
			if (fromEngineOutput != NULL)
				toField->connectFrom(fromEngineOutput);
			// if there was an error  -> disconnect happend before finding the error (from / to changed)
		}
	}
	else
	{
		if (toField != NULL)
			toField->disconnect(); // it is allowed to do this, even if the field is not connected
	}
}

#endif
