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
/** implementation of SoRayPickerTracker
*
* SoRayPickerTracker implements a ray picking tool. It outputs the picked points data, it is a cross implementation for SoRayPicker which uses 3D Events.
* etc. 
*
* @author Thomas Psik tomp@ims.tuwien.ac.at
*
* $Id: SoRayPickerTracker.cxx 3525 2004-07-21 10:26:59Z tomp $
* @file                                                                   */
/* ======================================================================= */

#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/engines/SoConcatenate.h>


#include <stbapi/misc/SoRayPickerTracker.h>
#include <stbapi/workspace/StbWorkspace.h>
#include <stbapi/resource/SoUserManagerKit.h>
#include <stbapi/tracker/SoTrakEngine.h>

static const float DEFAULT_RAY_HEIGHT=1.5f;

//--initialization of kit structure in Inventor
SO_KIT_SOURCE(SoRayPickerTracker);

void SoRayPickerTracker::initClass(void)
{
    SO_KIT_INIT_CLASS(SoRayPickerTracker, SoTrackedArtifactKit, "SoTrackedArtifactKit");
}

SoRayPickerTracker::SoRayPickerTracker()
{
    SO_KIT_CONSTRUCTOR(SoRayPickerTracker);
	
    SO_KIT_DEFINE_ENUM_VALUE(Mode, RAY);
    SO_KIT_DEFINE_ENUM_VALUE(Mode, STICK);
    SO_KIT_DEFINE_ENUM_VALUE(Coordinate, NORMALIZED);
    SO_KIT_DEFINE_ENUM_VALUE(Coordinate, ABSOLUT);
    SO_KIT_DEFINE_ENUM_VALUE(Input, POSE);
    SO_KIT_DEFINE_ENUM_VALUE(Input, POSITION);
    SO_KIT_DEFINE_ENUM_VALUE(Input, SCREEN);
	
    SO_KIT_SET_SF_ENUM_TYPE(mode,        Mode);
    SO_KIT_SET_SF_ENUM_TYPE(coordinates, Coordinate);
    SO_KIT_SET_SF_ENUM_TYPE(input,       Input);
	
    SO_KIT_ADD_FIELD(active,             (TRUE));
    SO_KIT_ADD_FIELD(pickGraph,          (NULL));
    SO_KIT_ADD_FIELD(radius,             (1.0f));
    SO_KIT_ADD_FIELD(refPoint,           (0,0,0));
    SO_KIT_ADD_FIELD(nearDistance,       (-1.0f));
    SO_KIT_ADD_FIELD(farDistance,        (-1.0f));
    SO_KIT_ADD_FIELD(mode,               (RAY));
    SO_KIT_ADD_FIELD(coordinates,        (NORMALIZED));
    SO_KIT_ADD_FIELD(input,              (POSE));
    SO_KIT_ADD_FIELD(pickedPath,         (NULL));
    SO_KIT_ADD_FIELD(pickedNodes,        (NULL));
    pickedNodes.setNum(0);						// we cant initialize an empty MFField, so we have to use this hack
    SO_KIT_ADD_FIELD(pickedLastNode,     (NULL));
    SO_KIT_ADD_FIELD(pickOnlySeparators, (FALSE));
	SO_KIT_ADD_FIELD(pickLastNamedNode,  (FALSE));

	SO_KIT_ADD_FIELD(pickedPoint,        (0,0,0));
    SO_KIT_ADD_FIELD(pickedOrientation,  (0,0,1,0));
    SO_KIT_ADD_FIELD(pickedNormal,       (0,0,0));
    SO_KIT_ADD_FIELD(pickedTextureCoord, (0,0,0,0));
    SO_KIT_ADD_FIELD(button,             (0));
    SO_KIT_ADD_FIELD(isPicking,          (FALSE));
    SO_KIT_ADD_FIELD(userId,             (-1));
	
	// connected from SoTrakEngine
	SO_KIT_ADD_FIELD(position,			(0,0,0));
	SO_KIT_ADD_FIELD(orientation,		(0,1,0,0));
	SO_KIT_ADD_FIELD(buttonFlags,		(0));
	buttonFlags.setNum(8);
    
	
	SO_KIT_INIT_INSTANCE();
	
	//    addEventCallback(eventCB);
	
    activeSensor.setFunction( SoRayPickerTracker::fieldChangedCB );
	activeSensor.setData( this );
	
	printf("SoRayPickerTracker\n");

	showGeometry(TRUE);
	setUpConnections(TRUE, TRUE);
	
} // SoRayPickerTracker()

SoRayPickerTracker::~SoRayPickerTracker()
{
}

// ------------------------------------------------------------------------------------

SbBool SoRayPickerTracker::setUpConnections(SbBool onOff, SbBool doItAlways ) 
{
	   if (!doItAlways && connectionsSetUp == onOff)
		   return onOff;
	   if (onOff) {
		   // We connect AFTER base class.
		   SoTrackedArtifactKit::setUpConnections(onOff, doItAlways);
		   // do your own special setup here !
		   
		   activeSensor.attach(&active);
		   position.connectFrom(&(getMover()->translation));
		   orientation.connectFrom(&(getMover()->rotation));

		   SoConcatenate *buttonCollector = new SoConcatenate (SoMFBool::getClassTypeId());
		   buttonCollector->input[0]->connectFrom(&tracker->button0);
		   buttonCollector->input[1]->connectFrom(&tracker->button1);
		   buttonCollector->input[2]->connectFrom(&tracker->button2);
		   buttonCollector->input[3]->connectFrom(&tracker->button3);
		   buttonCollector->input[4]->connectFrom(&tracker->button4);
		   buttonCollector->input[5]->connectFrom(&tracker->button5);
		   buttonCollector->input[6]->connectFrom(&tracker->button6);
		   buttonCollector->input[7]->connectFrom(&tracker->button7);
		   
		   buttonFlags.connectFrom(buttonCollector->output);
		   
			if (offset.getValue() == NULL)
				offset.setValue(new SoTransform());

	   }
	   else {
		   // We disconnect BEFORE base class.
		   // Disconnect the field sensors.
		   activeSensor.detach();
		   position.disconnect();
		   orientation.disconnect();
		   
		   SoTrackedArtifactKit::setUpConnections(onOff, doItAlways);
	   }
	   return !(connectionsSetUp = onOff);
}

// ------------------------------------------------------------------------------------

void SoRayPickerTracker::fieldChangedCB(void* data, SoSensor* which) {
    SoRayPickerTracker* self = (SoRayPickerTracker *) data;
	
	printf("SoRayPickerTracker fieldChangedCB %d\n", self->active.getValue());
	
	/*	
    if (self->active.getValue()) 
	self->showGeometry(TRUE);
    else 
	self->showGeometry(FALSE);
	*/
	//self->showGeometry(self->active.getValue());
	
}

// ------------------------------------------------------------------------------------

void SoRayPickerTracker::movedCB() 
{
	printf("SoRayPickerTracker\n");
	
    if( !active.getValue())
    {
        if( isPicking.getValue() == TRUE )
            isPicking.setValue(FALSE);
        return;
    }
	
    static float curr_ray_len=DEFAULT_RAY_HEIGHT;
    static bool fixedSize=false;
	static float t = 0;
    
    switch (input.getValue())
    {
	case POSE:
		{
			// printf("button state: 1\n");
			
			//So3DEvent::EventType evtype=event->getType();
			// TODO check buttons !!!
			if ( FALSE )
			{
				// get and convert the button state from the 3d event and store it in the "button" field
				int bstate=convertButtonStateToInt();
				
				//printf("button state: %d\n",bstate);
				button.setValue(bstate);
				
				switch (mode.getValue())
				{
				case RAY:
					fixedSize=false;
					curr_ray_len=performRayPickAction();
					scaleRayLength(curr_ray_len);
					break;
					
				case STICK:
					if (bstate&1)
					{
						// button pushed
						scaleRayLength(curr_ray_len);
						fixedSize=true;
					}
					else
					{
						// button released
						curr_ray_len=performRayPickAction();
						scaleRayLength(curr_ray_len);
						fixedSize=false;
					}
					break;
				} // switch
			}
			else // moving or other event occured
			{
				if (!fixedSize) curr_ray_len=performRayPickAction();
				scaleRayLength(curr_ray_len);
			} // else
			break;
		} // case POSE
		
	case POSITION:
		{
            SbVec3f eyepos;
			
			SoUserManagerKit* umk = SoUserManagerKit::getInstance();			
			int userIdLocal = umk->getUserFromStation(station.getValue());
            if( userIdLocal < 0 )
                userIdLocal = userId.getValue();
            if (userIdLocal > -1) {
				SoDisplayKit* dk = umk->getUserKit(userIdLocal)->getDisplay();			
				SoOffAxisCamera* camleft = dk->getViewer()->getStereoCameraKit()->getCameraLeft();
				eyepos = camleft->eyepointPosition.getValue();				// eyepoint position of left camera
            }
            else {
                eyepos = refPoint.getValue();
            }
			
			SbVec3f pointpos = position.getValue();
			SbVec3f o = pointpos - eyepos;
			o.normalize();
			position.setValue(pointpos);
			SbRotation rot = orientation.getValue();
            SbVec3f defaultRot = SbVec3f(0,1,0);
			rot.setValue(defaultRot, o);
			orientation.setValue(rot);
			
			fixedSize=false;
            curr_ray_len=performRayPickAction();
            scaleRayLength(curr_ray_len);
			int bstate=convertButtonStateToInt();
			
			//printf("button state: %d\n",bstate);
			button.setValue(bstate);
			
			break;
		}
        
	case SCREEN:
		{
			SoUserManagerKit* umk = SoUserManagerKit::getInstance();
            int userIdLocal = umk->getUserFromStation(station.getValue());
            if( userIdLocal < 0 )
                userIdLocal = userId.getValue();
            if( userIdLocal < 0 )
            {
                printf("WARNING: SoRayPickerTracker has no userId in SCREEN mode.\n");
                return;
            }
			SoStudierstubeViewer* viewer = umk->getUserKit(userIdLocal)->getDisplay()->getViewer();
			SoOffAxisCamera* camleft = viewer->getStereoCameraKit()->getCameraLeft();
			SbVec3f eyepos = camleft->eyepointPosition.getValue();				// eyepoint position of left camera
            SbVec3f pointpos = camleft->position.getValue();
            SbVec2f planesize = camleft->size.getValue();
            SbRotation planerot = camleft->orientation.getValue();
			SbVec3f rawpos = position.getValue();
			
            if (coordinates.getValue() == ABSOLUT) {
                SbVec2s viewsize = viewer->getViewportRegion().getViewportSizePixels();
                rawpos[0] = rawpos[0] / viewsize[0];
                rawpos[1] = rawpos[1] / viewsize[1];
            }
            rawpos[0] -= 0.5f;
            rawpos[1] -= 0.5f;
			
            SbVec3f offset(0,0,0);
            offset[0] = planesize[0] * rawpos[0];
            offset[1] = planesize[1] * (-rawpos[1]);
			
            planerot.multVec(offset, offset);
            pointpos += offset;
			
            //printf("pointpos: %f %f %f\n",pointpos[0],pointpos[1],pointpos[2]);
			
			SbVec3f o = pointpos - eyepos;
			o.normalize();
			position.setValue(eyepos);
			SbRotation rot = orientation.getValue();
            SbVec3f defaultRot = SbVec3f(0,1,0);
			rot.setValue(defaultRot, o);
			orientation.setValue(rot);
			
			fixedSize=false;
            curr_ray_len=performRayPickAction();
            scaleRayLength(curr_ray_len);
			
			int bstate=convertButtonStateToInt();
			
			//printf("button state: %d\n",bstate);
			button.setValue(bstate);
			
			
			break;
		}
    } // switch
}


SoSeparator* SoRayPickerTracker::getContent()
{
    return (SoSeparator*)geometry.getValue();
	//return SO_GET_PART(this ,"geometry",SoSeparator);
}


SoTransform* SoRayPickerTracker::getTransform()
{
	return (SoTransform*) offset.getValue();
	//return (SoTransform*) mover.getValue();
	//	return SO_GET_PART(this ,"mover",SoTransform);
}


float SoRayPickerTracker::performRayPickAction()
// Returns the measured distance between the station and the picked point.
// If there is no picked point, returns with DEFAULT_RAY_HEIGHT
{
	SoPath *piPath=NULL;
	SbVec3f piPoint;
	SbRotation piOrientation;
	SbVec3f piNormal;
	SbVec4f piTextureCoord;
	SbString piName;
	SoNode *piNode = NULL;
	float result;
	
	// perform a raypick-action
	// ************************
	
	SbViewportRegion vpr;
	SoRayPickAction rpa(vpr);
	
	SbRotation sbRotation=orientation.getValue();
	SbVec3f defaultOrientation(0,1,0); // this is the default orientation
	SbVec3f rayStart=position.getValue();
	SbVec3f rayDir;
	sbRotation.multVec( defaultOrientation, rayDir );
	rayDir.normalize();
	rpa.setRay(rayStart,rayDir,nearDistance.getValue(),farDistance.getValue());
	printf("Raypicking from %f %f %f to %f %f %f\n", rayStart[0], rayStart[1], rayStart[2], rayDir[0], rayDir[1], rayDir[2] );
	
	// rpa.setRadius(self->radius.getValue());
	rpa.setPickAll( FALSE );
	if (pickGraph.getValue()==NULL)
	{
		rpa.apply(StbWorkspace::getInstance()->getGeometryRoot());
	}
	else
	{
		rpa.apply(pickGraph.getValue());
		//		    printNode( self->pickGraph.getValue(), "pickaction on ");
	}
	SoPickedPoint* pp = rpa.getPickedPoint();
	// SoPickedPointList list = rpa.getPickedPointList();
	//printf("list has %d\n", list.getLength());
	if (pp != NULL)
	{
		// get informations about the picked point
		piPath=pp->getPath();
		SoFullPath *fullPath = (SoFullPath *)piPath;
		piNode = fullPath->getTail();

		bool checkSep = (pickOnlySeparators.getValue() == FALSE);
		bool checkName = (pickLastNamedNode.getValue() == TRUE);
		if (pickOnlySeparators.getValue() || checkName )
		{
			int idx = fullPath->getLength();
			while (idx > 0) 
			{
				SoNode *tempNode = fullPath->getNode(idx);
				if (tempNode != NULL) 
				{
					// if pickOnlySeparators is TRUE also check the type of the node
					if (checkSep || tempNode->isOfType(SoSeparator::getClassTypeId())) 
					{
						if (checkName)
						{
							if (tempNode->getName().getLength() > 0) 
							{
								piNode = tempNode;
								idx =0 ;
							}
						}
						else
						{
							piNode = tempNode;
						}
						
					}
				}
				idx--;
			}
		}

		piPoint=pp->getPoint();
		piOrientation=sbRotation;
		piNormal=pp->getNormal();
		piTextureCoord=pp->getTextureCoords();
		SbVec3f world_ray_vector=piPoint-rayStart;
		result=world_ray_vector.length();
		if( isPicking.getValue() != TRUE )
			isPicking.setValue( TRUE );
	}
	else
	{
		//printf("nothing found \n");
		// no objects found, setting default values as output
		SbVec3f tmpVec=rayDir;
		tmpVec.normalize();
		tmpVec*=DEFAULT_RAY_HEIGHT;
		piPath=NULL;
		piPoint=rayStart+tmpVec;
		piOrientation=sbRotation;
		piName = "";
		piNormal.setValue(0,0,0);
		piTextureCoord.setValue(0,0,0,0);
		result=DEFAULT_RAY_HEIGHT;
		if( isPicking.getValue() != FALSE )
			isPicking.setValue( FALSE );
	}
	
	SbVec3f axis;
	float angle;
	sbRotation.getValue( axis, angle );
	
	setPickedPath(piPath);
    if( pickedPoint.getValue() != piPoint )
		pickedPoint.setValue(piPoint);
    if( pickedOrientation.getValue() != piOrientation )
		pickedOrientation.setValue(piOrientation);
    if( pickedNormal.getValue() != piNormal )
		pickedNormal.setValue(piNormal);
    if( pickedTextureCoord.getValue() != piTextureCoord )
		pickedTextureCoord.setValue(piTextureCoord);

	if (pickedLastNode.getValue() != piNode) 
		pickedLastNode.setValue(piNode);
	
	printf ("picked obj name [%s]\n",piName.getString());
	if (piPath!=NULL && piPath->getTail() != NULL)
	{
		printf ("picked obj name [%s]\n",piPath->getTail()->getTypeId().getName().getString());
		// get detail
		SoDetail const *detail = pp->getDetail();
/*		if (detail->isOfType(SoNodeKitDetail::getClassTypeId()) 
		{
			SoNodeKitDetail const *nodeKitDetail = (SoNodeKitDetail *)detail;
			printf ("picked obj name [%s]\n",getPartName().getString());
		}
*/	
	}
	return result;
} // performRayPickAction()

void SoRayPickerTracker::setPickedPath(SoPath * path) 
{
    int i;
	
    if( path != NULL && pickedPath.getValue() != NULL && 
        ((*path) == (*pickedPath.getValue())))
        return;
	
    pickedPath.setValue(path);
	
    if (path != NULL) 
    {
        SoFullPath * fullpath = (SoFullPath *)path;
        pickedNodes.enableNotify( FALSE );
        pickedNodes.setNum(fullpath->getLength());
        for (i=0; i<fullpath->getLength(); i++)
            pickedNodes.set1Value(i,fullpath->getNode(i));
        pickedNodes.enableNotify( TRUE );
        pickedNodes.touch();
    }
    else pickedNodes.setNum(0);
}


void SoRayPickerTracker::scaleRayLength(float neededSize)
// Scales the Y-size of the ray-geometry in "content" by an appropriate scale factor
{
	SoSeparator *cont= getContent();
	SoTransform *transf=getTransform();
	
	// use SoBoundingBox to calculate size of the geometry
	// @TODO: not everytime !! set a node sensor on the geometry part and update it then.
	SbViewportRegion vpr;
	SoGetBoundingBoxAction bbAction(vpr);
	bbAction.setInCameraSpace(false);
	bbAction.apply(cont);
	SbBox3f bbox=bbAction.getBoundingBox();
	
    // short cut to avoid Coin warning
    if( bbox.isEmpty())
        return;
	
	float bbx,bby,bbz;
	bbox.getSize(bbx,bby,bbz);
	
	// now, "bby" is the actual Y-size of "content",
	// "neededSize" is the needed size	
	if (bby!=0.0f)
	{
		float newScaleY=neededSize/bby; // the Y scale factor to apply in "transform"
		
		// set the size of content to the needed size by scaling it by the computed scale factor
		SbVec3f scale_factor=transf->scaleFactor.getValue();
		float xs,ys,zs;
		scale_factor.getValue(xs,ys,zs);
		transf->scaleFactor.setValue(xs,newScaleY,zs);
		
		// positioning the content by setting the translate field in transform
		float posX = 0.0f,posY= 0.0f,posZ= 0.0f;
		SbVec3f transl=transf->translation.getValue();
		transl.getValue(posX,posY,posZ);
//		transf->translation.setValue(posX,posY+bby/2.0f,posZ);
		transf->translation.setValue(0.0f, bby/2.0f, 0.0f);
		
		// setting center for rotation + scaling in transform
		transf->center.setValue(0.0f,-bby/2.0f,0.0f);
		
		printf("geom : scale %f %f %f\n translation %f %f %f\n", xs, newScaleY, zs, posX, posY+bby/2.0f, posZ); 
	} // if
} // scaleRayLength()


int SoRayPickerTracker::convertButtonStateToInt()
{
	int result=0;
	
	for (int i=0; i< buttonFlags.getNum(); i++ )
	{
		result |= buttonFlags[i] << i;	
	}
	return result;
} // convertButtonStateToInt()
