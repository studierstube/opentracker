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
/** Implementation of class Stb3DEventGenerator
  *
  * @author Anton Fuhrmann,Dieter Schmalstieg, Andreas Zajic, Gerd Hesina, Zsolt Marx
  *
  * $Id: Stb3DEventGenerator.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <cassert>
#include <Inventor/SoDB.h>
#include <Inventor/actions/SoSearchAction.h>
#include <stbapi/interaction/SoBehaviorKit.h>
#include <stbapi/tracker/TrackerOT.h>

#include "Stb3DEventGenerator.h"

using namespace std;

//-----------------------------------------------------------------------------
TrackerOT *            Stb3DEventGenerator::tracker;
So3DEvent              Stb3DEventGenerator::event;
   
vector<StationEventInfo> Stb3DEventGenerator::backup;    
vector<SoHandle3DEventAction *> Stb3DEventGenerator::h3a;
SoNodeList Stb3DEventGenerator::root;
vector<StationFieldInfo> Stb3DEventGenerator::sfi;
vector<SbName> Stb3DEventGenerator::globalNames;

//-----------------------------------------------------------------------------

void
Stb3DEventGenerator::init(TrackerOT * newTracker)
{
    int i,j;
    tracker = newTracker;

    root.truncate(0);
    backup.resize(getNumStations());
    h3a.resize(getNumStations());
    sfi.resize(getNumStations());
    
    for(i=0; i < getNumStations(); i++)
    {
//--init tracker data cache
        root.append(NULL);
        backup[i].translation = SbVec3f(0,0,0);
        backup[i].rotation = SbRotation(SbVec3f(1,0,0),0);
        for(j=0; j<trackerMaxButtonsPerUnit; j++) 
            backup[i].button[j] = FALSE;
        backup[i].station = i;
        
//--init tracker data fields
        sfi[i].translation = getTranslationField(i);
        assert(sfi[i].translation);
        sfi[i].rotation = getRotationField(i);
        assert(sfi[i].rotation);
        for(j=0; j<trackerMaxButtonsPerUnit; j++) 
        {
           sfi[i].button[j] = getButtonField(i,j);
           assert(sfi[i].button[j]);
        }

//--create global offset fields
        sfi[i].translationOffset = getTranslationOffsetField(i);
        assert(sfi[i].translationOffset); 
        sfi[i].rotationOffset = getRotationOffsetField(i);
        assert(sfi[i].rotationOffset);
//--init handle event action
        h3a[i] = new SoHandle3DEventAction;
    }//for i
}

//---------------------------------------------------------------------------

void
Stb3DEventGenerator::activate()
{
}

//---------------------------------------------------------------------------

void
Stb3DEventGenerator::deactivate()
{
    tracker->deactivate();
    // FIXME: commented out - crashes under windows with Coin when exiting STB
    // 20030224 kaufmann.
#if !(defined(WIN32) && defined(__COIN__))
    delete tracker;
#endif
}

//---------------------------------------------------------------------------

SoSFVec3f*
Stb3DEventGenerator::getTranslationField(int station)
{
    SbString name = "tracker";
    name += SbString(station);
    name += "translation";
    SbName sbName(name);

    SoSFVec3f * field = (SoSFVec3f *)SoDB::getGlobalField(sbName);
    if( field == NULL )
    {
        field = (SoSFVec3f*)SoDB::createGlobalField( sbName, 
            SoSFVec3f::getClassTypeId());
        field->setValue( SbVec3f(0,0,0));
        globalNames.push_back(sbName);
    }
    return field;
}

SoSFVec3f* Stb3DEventGenerator::getTranslationOffsetField(int station)
{
    SbString name = "offset";
    name += SbString(station);
    name += "translation";
    SbName sbName(name);

    SoSFVec3f * field = (SoSFVec3f *)SoDB::getGlobalField(sbName);
    if( field == NULL )
    {
        field = (SoSFVec3f*)SoDB::createGlobalField( sbName, 
            SoSFVec3f::getClassTypeId());
        field->setValue( SbVec3f(0,0,0));
        field->setDefault( TRUE );
        globalNames.push_back(sbName);
    }
    return field;
}
//-----------------------------------------------------------------------------

SoSFRotation*
Stb3DEventGenerator::getRotationField(int station)
{
    SbString name = "tracker";
    name += SbString(station);
    name += "rotation";
    SbName sbName(name);

    SoSFRotation * field = (SoSFRotation *)SoDB::getGlobalField(sbName);
    if( field == NULL )
    {
        field = (SoSFRotation*)SoDB::createGlobalField( sbName, 
            SoSFRotation::getClassTypeId());
        field->setValue( SbRotation::identity());
        globalNames.push_back(sbName);
    }
    return field;
}

SoSFRotation* Stb3DEventGenerator::getRotationOffsetField(int station)
{
    SbString name = "offset";
    name += SbString(station);
    name += "rotation";
    SbName sbName(name);

    SoSFRotation * field = (SoSFRotation *)SoDB::getGlobalField(sbName);
    if( field == NULL )
    {
        field = (SoSFRotation*)SoDB::createGlobalField( sbName, 
            SoSFRotation::getClassTypeId());
        field->setValue( SbRotation::identity());
        field->setDefault( TRUE );
        globalNames.push_back(sbName);
    }
    return field;
}

//-----------------------------------------------------------------------------

SoSFBool*
Stb3DEventGenerator::getButtonField(int station, int buttonNumber)
{
    SbString name = "tracker";
    name += SbString(station);
    name += "button";
    name += SbString(buttonNumber);
    SbName sbName(name);

    SoSFBool * field = (SoSFBool *)SoDB::getGlobalField(sbName);
    if( field == NULL )
    {
        field = (SoSFBool*)SoDB::createGlobalField( sbName, 
            SoSFBool::getClassTypeId());        
        field->setValue( FALSE );
        globalNames.push_back(sbName);
    }
    return field;
}

//-----------------------------------------------------------------------------

void
Stb3DEventGenerator::setRoot(int index, SoNode* newRoot) 
{
    if(0 > index || index >= getNumStations())
    {
        printf("WARNING: Stb3DEventGenerator::setRoot index %d not in valid range [0,%d]\n"
               "         This is probably a result of not defining StbSinks in your\n"
               "         OpenTracker config for stations used by Studierstube.\n",
            index, getNumStations()-1);
        return;
    }
    root.set( index, newRoot );
}

//-----------------------------------------------------------------------------
// process3DEvent. gives the 3d event to the handle event action
// event: 3D event to process
// returns: true if the event has been consumed

void 
Stb3DEventGenerator::process3DEvent(int index, const So3DEvent* event) 
{
    h3a[index]->setEvent(event);

    SoNode* grabber = h3a[index]->getGrabber();

//	printf("Stb3DEventGenerator::process3DEvent idx %d grabber %p\n",index,grabber);

    if(!grabber)
	{
	//	printf("Stb3DEventGenerator::process3DEvent idx %d root %p type %s name [%s]\n",index,root[index],root[index]->getTypeId().getName().getString(),root[index]->getName().getString());
	
		h3a[index]->apply(root[index]);
	}
	else
	{
	    h3a[index]->apply( ((SoFullPath *)h3a[index]->getPathFromRoot())->getTail() );
	}
}

//-----------------------------------------------------------------------------

void 
Stb3DEventGenerator::releaseAllGrabbers(SoNode *root)
{
    int i;
	root->ref();
    for(i=0;i<tracker->getMaxStationNumber();i++)
	    if(h3a[i])
           if (root && h3a[i]->getGrabber()) {
               SoSearchAction sa;
               sa.setNode(h3a[i]->getGrabber());
               sa.setSearchingAll(TRUE);
               sa.apply(root);
               if(sa.getPath() != NULL) 
                   h3a[i]->releaseGrabber();
           }
           else 
               h3a[i]->releaseGrabber();
	root->unrefNoDelete();
}

//-----------------------------------------------------------------------------

// updates global fields (mainly used for positioning pen, PIP & HMDs)

void
Stb3DEventGenerator::updateGlobalTrackerFields( const vector<StationEventInfo *> & changedStations )
{
    vector<StationEventInfo *>::const_iterator it;
    for( it = changedStations.begin(); it != changedStations.end(); it ++ )
    {
        StationEventInfo * info = (*it);
        sfi[info->station].translation->setValue( info->translation );
        sfi[info->station].rotation->setValue(info->rotation);
        for(int j=0; j<trackerMaxButtonsPerUnit; j++) 
            sfi[info->station].button[j]->setValue(info->button[j]);
    }
}

// removes all global fields

void 
Stb3DEventGenerator::deleteGlobalTrackerFields()
{
    vector<SbName>::const_iterator it;
    for(it = globalNames.begin(); it != globalNames.end(); it++ )
    {
        SoDB::renameGlobalField(*it, SbName());
    }
}

SbTime Stb3DEventGenerator::getCurrentTime()
{
	return ((SoSFTime*) SoDB::getGlobalField("realTime"))->getValue();
}

void
Stb3DEventGenerator::sendEvents( const vector<StationEventInfo *> & changedStations )
{
    int j;
	bool eventGenerated = false;

    vector<StationEventInfo *>::const_iterator it;
    for( it = changedStations.begin(); it != changedStations.end(); it ++ )
    {
        StationEventInfo * info = (*it);
        
        if(root[info->station] != NULL)
        {
            // general event preparation
            SbTime current;
            current=((SoSFTime*)SoDB::getGlobalField("realTime"))->getValue();
            event.setStation(info->station);
            event.setTime(current);

            // if offset fields are not default values, transform events first
            if( !sfi[info->station].translationOffset->isDefault() || 
                !sfi[info->station].rotationOffset->isDefault())
            {
                SbVec3f worldPos;
                sfi[info->station].rotationOffset->getValue().multVec( info->translation, worldPos );
                worldPos += sfi[info->station].translationOffset->getValue();
                SbRotation worldRot = sfi[info->station].rotationOffset->getValue() * info->rotation;
                event.setTranslation( worldPos );
                event.setRotation( worldRot );                
            }
            else
            {
                event.setTranslation(info->translation);
                event.setRotation(info->rotation);
            }

            for(j=0; j<trackerMaxButtonsPerUnit; j++)
                event.setButton(j,info->button[j]);

            // generate button events
            for(j=0; j<trackerMaxButtonsPerUnit; j++)
            {
                if(backup[info->station].button[j] != info->button[j])
                {
                    int evType = So3DEvent::ET_BUTTON0_EVENT+j;
                    event.setType(So3DEvent::EventType(evType));
					event.setTime(getCurrentTime());

					eventGenerated = true;

                    process3DEvent(info->station,&event);
                }
            }

            // generate Move-Events
            if ((backup[info->station].translation != info->translation)
            ||  (backup[info->station].rotation    != info->rotation))
            {
				event.setType(So3DEvent::ET_MOVE_EVENT);
				event.setTime(getCurrentTime());

				eventGenerated = true;

				process3DEvent(info->station, &event);
			}

			// generate Exit-Events
			if (eventGenerated && h3a[info->station]->hasConsumerChanged())
			{
				SoNode *prevConsumer = (SoNode *) h3a[info->station]->getPreviousConsumer();

				// check if there's a previous Consumer and if it is a behavior kit

				if ( prevConsumer && prevConsumer->isOfType(SoBehaviorKit::getClassTypeId()))
				{

					SoBehaviorKit *behavior = (SoBehaviorKit *) prevConsumer;

					// send an EXIT event to the old

					// printNode(prevConsumer, "SENDING EXIT");

					event.setType(So3DEvent::ET_EXIT_EVENT);
					event.setTime(getCurrentTime());
					h3a[info->station]->setEvent(&event);

					// give the event to the behavior to handle

					behavior->examineEvent( h3a[info->station] );

					if (behavior->isInterested())
					{
						behavior->handle3DEvent(h3a[info->station]);
						h3a[info->station]->setHandled();
					}
				}

			}

        }
        backup[info->station] = *info;
    }
}

//-----------------------------------------------------------------------------

void 
Stb3DEventGenerator::sample()
{
    const vector<StationEventInfo *> & changedStations = tracker->getStateVector();
    updateGlobalTrackerFields( changedStations );
    sendEvents( changedStations );
}

//-----------------------------------------------------------------------------

// return a pointer to the tracker stored inside
TrackerOT * Stb3DEventGenerator::getTracker()
{
    return tracker;
}

// return number of stations
int Stb3DEventGenerator::getNumStations()
{
    return tracker->getMaxStationNumber();
}


/* ===========================================================================
    End of @FILENAME@
   ===========================================================================
    Automatic Emacs configuration follows.
    Local Variables:
    mode:c++
    c-basic-offset: 4
    eval: (c-set-offset 'substatement-open 0)
    eval: (c-set-offset 'case-label '+)
    eval: (c-set-offset 'statement 'c-lineup-runin-statements)
    eval: (setq indent-tabs-mode nil)
    End:
=========================================================================== */
