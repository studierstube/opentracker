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
/** implementation of SoRayPicker
  *
  * SoRayPicker implements a ray picking tool. It outputs the picked points data
  * etc. 
  *
  * @author  Peter Alexay, Flo Ledermann ledermann@ims.tuwien.ac.at
  *
  * $Id: SoRayPicker.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/SoPickedPoint.h>

#include <stbapi/misc/SoRayPicker.h>
#include <stbapi/workspace/StbWorkspace.h>
#include <stbapi/resource/SoUserManagerKit.h>

static const float DEFAULT_RAY_HEIGHT=1.5f;

//--initialization of kit structure in Inventor
SO_KIT_SOURCE(SoRayPicker);

void SoRayPicker::initClass(void)
{
    SO_KIT_INIT_CLASS(SoRayPicker, SoStationKit, "SoStationKit");
}

SoRayPicker::SoRayPicker()
{
    SO_KIT_CONSTRUCTOR(SoRayPicker);
	
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
    SO_KIT_ADD_FIELD(pickedPoint,        (0,0,0));
    SO_KIT_ADD_FIELD(pickedOrientation,  (0,0,1,0));
    SO_KIT_ADD_FIELD(pickedNormal,       (0,0,0));
    SO_KIT_ADD_FIELD(pickedTextureCoord, (0,0,0,0));
    SO_KIT_ADD_FIELD(button,             (0));
    SO_KIT_ADD_FIELD(isPicking,          (FALSE));
    SO_KIT_ADD_FIELD(userId,             (-1));
	
    SO_KIT_INIT_INSTANCE();
	
    addEventCallback(eventCB);

    activeSensor.setFunction( SoRayPicker::fieldChangedCB );
	activeSensor.setData( this );
    activeSensor.attach(&active);
} // SoRayPicker()

SoRayPicker::~SoRayPicker()
{
}

void SoRayPicker::fieldChangedCB(void* data, SoSensor* which) {
    SoRayPicker* self = (SoRayPicker *) data;

    if (self->active.getValue()) self->visible.setValue(TRUE);
    else self->visible.setValue(FALSE);
}

void SoRayPicker::eventCB(void *data, int station, SoStationKit *kit, SoHandle3DEventAction *h3a)
{     
    SoRayPicker *self = (SoRayPicker *)kit;

    if( !self->active.getValue())
    {
        if( self->isPicking.getValue() == TRUE )
            self->isPicking.setValue(FALSE);
        return;
    }

    const So3DEvent *event = h3a->getEvent();
    static float curr_ray_len=DEFAULT_RAY_HEIGHT;
    static bool fixedSize=false;
	static float t = 0;
    
    switch (self->input.getValue())
    {
	case POSE:
		{
			So3DEvent::EventType evtype=event->getType();
			if (evtype==So3DEvent::ET_BUTTON0_EVENT ||
				evtype==So3DEvent::ET_BUTTON1_EVENT ||
				evtype==So3DEvent::ET_BUTTON2_EVENT ||
				evtype==So3DEvent::ET_BUTTON3_EVENT ||
				evtype==So3DEvent::ET_BUTTON4_EVENT ||
				evtype==So3DEvent::ET_BUTTON5_EVENT ||
				evtype==So3DEvent::ET_BUTTON6_EVENT ||
				evtype==So3DEvent::ET_BUTTON7_EVENT)
			{
				// get and convert the button state from the 3d event and store it in the "button" field
				int bstate=convertButtonStateToInt(event);
				//printf("button state: %d\n",bstate);
				self->button.setValue(bstate);
				
				switch (self->mode.getValue())
				{
				case RAY:
					fixedSize=false;
					curr_ray_len=performRayPickAction(self);
					scaleRayLength(self,curr_ray_len);
					break;
					
				case STICK:
					if (bstate&1)
					{
						// button pushed
						scaleRayLength(self,curr_ray_len);
						fixedSize=true;
					}
					else
					{
						// button released
						curr_ray_len=performRayPickAction(self);
						scaleRayLength(self,curr_ray_len);
						fixedSize=false;
					}
					break;
				} // switch
			}
			else // moving or other event occured
			{
				if (!fixedSize) curr_ray_len=performRayPickAction(self);
				scaleRayLength(self,curr_ray_len);
			} // else
			break;
		} // case POSE
		
	case POSITION:
		{
            SbVec3f eyepos;

			SoUserManagerKit* umk = SoUserManagerKit::getInstance();			
			int userId = umk->getUserFromStation(station);
            if( userId < 0 )
                userId = self->userId.getValue();
            if (userId > -1) {
			    SoDisplayKit* dk = umk->getUserKit(userId)->getDisplay();			
			    SoOffAxisCamera* camleft = dk->getViewer()->getStereoCameraKit()->getCameraLeft();
			    eyepos = camleft->eyepointPosition.getValue();				// eyepoint position of left camera
            }
            else {
                eyepos = self->refPoint.getValue();
            }

			SbVec3f pointpos = self->position.getValue();
			SbVec3f o = pointpos - eyepos;
			o.normalize();
			self->position.setValue(pointpos);
			SbRotation rot = self->orientation.getValue();
            SbVec3f defaultRot = SbVec3f(0,1,0);
			rot.setValue(defaultRot, o);
			self->orientation.setValue(rot);

			fixedSize=false;
            curr_ray_len=performRayPickAction(self);
            scaleRayLength(self,curr_ray_len);

            So3DEvent::EventType evtype=event->getType();
            if (evtype==So3DEvent::ET_BUTTON0_EVENT ||
                evtype==So3DEvent::ET_BUTTON1_EVENT ||
                evtype==So3DEvent::ET_BUTTON2_EVENT ||
                evtype==So3DEvent::ET_BUTTON3_EVENT ||
                evtype==So3DEvent::ET_BUTTON4_EVENT ||
                evtype==So3DEvent::ET_BUTTON5_EVENT ||
                evtype==So3DEvent::ET_BUTTON6_EVENT ||
                evtype==So3DEvent::ET_BUTTON7_EVENT)
			{
                // get and convert the button state from the 3d event and store it in the "button" field
                int bstate=convertButtonStateToInt(event);
                self->button.setValue(bstate);
            }
			break;
		}
        
	case SCREEN:
		{
			SoUserManagerKit* umk = SoUserManagerKit::getInstance();
            int userId = umk->getUserFromStation(station);
            if( userId < 0 )
                userId = self->userId.getValue();
            if( userId < 0 )
            {
                printf("WARNING: SoRayPicker has no userId in SCREEN mode.\n");
                return;
            }
			SoStudierstubeViewer* viewer = umk->getUserKit(userId)->getDisplay()->getViewer();
			SoOffAxisCamera* camleft = viewer->getStereoCameraKit()->getCameraLeft();
			SbVec3f eyepos = camleft->eyepointPosition.getValue();				// eyepoint position of left camera
            SbVec3f pointpos = camleft->position.getValue();
            SbVec2f planesize = camleft->size.getValue();
            SbRotation planerot = camleft->orientation.getValue();
			SbVec3f rawpos = self->position.getValue();

            if (self->coordinates.getValue() == ABSOLUT) {
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
			self->position.setValue(eyepos);
			SbRotation rot = self->orientation.getValue();
            SbVec3f defaultRot = SbVec3f(0,1,0);
			rot.setValue(defaultRot, o);
			self->orientation.setValue(rot);

			fixedSize=false;
            curr_ray_len=performRayPickAction(self);
            scaleRayLength(self,curr_ray_len);

            So3DEvent::EventType evtype=event->getType();
            if (evtype==So3DEvent::ET_BUTTON0_EVENT ||
                evtype==So3DEvent::ET_BUTTON1_EVENT ||
                evtype==So3DEvent::ET_BUTTON2_EVENT ||
                evtype==So3DEvent::ET_BUTTON3_EVENT ||
                evtype==So3DEvent::ET_BUTTON4_EVENT ||
                evtype==So3DEvent::ET_BUTTON5_EVENT ||
                evtype==So3DEvent::ET_BUTTON6_EVENT ||
                evtype==So3DEvent::ET_BUTTON7_EVENT)
            {
                // get and convert the button state from the 3d event and store it in the "button" field
                int bstate=convertButtonStateToInt(event);
                self->button.setValue(bstate);
            }

			break;
		}
    } // switch
} // eventCB()


SoNode* SoRayPicker::getContent(SoRayPicker *self)
{
    return SO_GET_PART(self,"content",SoNode);
}


SoTransform* SoRayPicker::getTransform(SoRayPicker *self)
{
    return SO_GET_PART(self,"transform",SoTransform);
}


float SoRayPicker::performRayPickAction(SoRayPicker *self)
// Returns the measured distance between the station and the picked point.
// If there is no picked point, returns with DEFAULT_RAY_HEIGHT
{
	SoPath *piPath=NULL;
	SbVec3f piPoint;
	SbRotation piOrientation;
	SbVec3f piNormal;
	SbVec4f piTextureCoord;
	float result;
	
	// perform a raypick-action
	// ************************
	
	SbViewportRegion vpr;
	SoRayPickAction rpa(vpr);

        SbRotation sbRotation=self->orientation.getValue();
	    SbVec3f defaultOrientation(0,1,0); // this is the default orientation
	    SbVec3f rayStart=self->position.getValue();
	    SbVec3f rayDir;
	    sbRotation.multVec( defaultOrientation, rayDir );
	    rayDir.normalize();
	    rpa.setRay(rayStart,rayDir,self->nearDistance.getValue(),self->farDistance.getValue());
        // printf("Raypicking from %f %f %f to %f %f %f\n", rayStart[0], rayStart[1], rayStart[2], rayDir[0], rayDir[1], rayDir[2] );

	    // rpa.setRadius(self->radius.getValue());
	    rpa.setPickAll( FALSE );
	    if (self->pickGraph.getValue()==NULL)
	    {
		    rpa.apply(StbWorkspace::getInstance()->getGeometryRoot());
	    }
	    else
	    {
		    rpa.apply(self->pickGraph.getValue());
//		    printNode( self->pickGraph.getValue(), "pickaction on ");
	    }
	    SoPickedPoint* pp = rpa.getPickedPoint();
	    // SoPickedPointList list = rpa.getPickedPointList();
	    //printf("list has %d\n", list.getLength());
	    if (pp != NULL)
	    {
		    // get informations about the picked point
		    piPath=pp->getPath();
		    piPoint=pp->getPoint();
		    piOrientation=sbRotation;
		    piNormal=pp->getNormal();
		    piTextureCoord=pp->getTextureCoords();
		    SbVec3f world_ray_vector=piPoint-rayStart;
		    result=world_ray_vector.length();
            if( self->isPicking.getValue() != TRUE )
                self->isPicking.setValue( TRUE );
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
		    piNormal.setValue(0,0,0);
		    piTextureCoord.setValue(0,0,0,0);
		    result=DEFAULT_RAY_HEIGHT;
            if( self->isPicking.getValue() != FALSE )
                self->isPicking.setValue( FALSE );
	    }
	    
	    SbVec3f axis;
	    float angle;
	    sbRotation.getValue( axis, angle );

	self->setPickedPath(piPath);
    if( self->pickedPoint.getValue() != piPoint )
	    self->pickedPoint.setValue(piPoint);
    if( self->pickedOrientation.getValue() != piOrientation )
	    self->pickedOrientation.setValue(piOrientation);
    if( self->pickedNormal.getValue() != piNormal )
	    self->pickedNormal.setValue(piNormal);
    if( self->pickedTextureCoord.getValue() != piTextureCoord )
	    self->pickedTextureCoord.setValue(piTextureCoord);
	
	return result;
} // performRayPickAction()

void SoRayPicker::setPickedPath(SoPath * path) 
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


void SoRayPicker::scaleRayLength(SoRayPicker *self,float neededSize)
// Scales the Y-size of the ray-geometry in "content" by an appropriate scale factor
{
	SoGroup *cont=(SoGroup *)getContent(self);
	SoTransform *transf=getTransform(self);
	
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
		float posX,posY,posZ;
		SbVec3f transl=transf->translation.getValue();
		transl.getValue(posX,posY,posZ);
		transf->translation.setValue(posX,posY+bby/2.0f,posZ);
		
		// setting center for rotation + scaling in transform
		transf->center.setValue(0.0f,-bby/2.0f,0.0f);
		
		//printf("geom : scale %f %f %f\n translation %f %f %f\n",
		//  xs, newScaleY, zs, posX, posY+bby/2.0f, posZ); 
	} // if
} // scaleRayLength()


int SoRayPicker::convertButtonStateToInt(const So3DEvent *event)
{
	int currBit=1;
	int result=0;
	
	for (int i=0; i<8; i++, currBit<<=1)
	{
		if (event->getButton(i)) result|=currBit;
	}
	
	return result;
} // convertButtonStateToInt()
