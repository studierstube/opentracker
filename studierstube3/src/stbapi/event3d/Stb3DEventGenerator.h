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
//  PROJECT:    Studierstube
//  CONTENT:    Class definition of Stb3DEventGenerator
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    alf     Anton Fuhrmann
//              ds      Dieter Schmalstieg
//              az      Andreas Zajic
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
// ===========================================================================

#ifndef _STB3DEVENTGENERATOR_H_
#define _STB3DEVENTGENERATOR_H_

#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/SoLists.h>

#include <stbapi/event3d/So3DEvent.h>
#include <stbapi/event3d/SoHandle3DEventAction.h>

#ifdef WIN32
#pragma warning(disable:4786)   // avoid warnings about too long stl names
#include <SoWinLeaveScope.h>
#endif

#include <vector>

const int trackerMaxButtonsPerUnit = 8;

class TrackerOT;

//-----------------------------------------------------------------------------

/** This struct stores the values of a given station. It is used to store
 * the last values of all stations and to pass the event data between the
 * OpenTracker library and the Studierstube. 
 */
struct StationEventInfo
{
    int station;
    SbVec3f translation;
    SbRotation rotation;
    SbBool button[trackerMaxButtonsPerUnit];
};

//-----------------------------------------------------------------------------

/** This struct stores the pointers to the global field associated with one
 * station. It allows faster access to the fields than the usual query methods.
 */
struct StationFieldInfo
{
    SoSFVec3f* translation;
    SoSFVec3f* translationOffset;
    SoSFRotation* rotation;
    SoSFRotation* rotationOffset;
    SoSFBool* button[trackerMaxButtonsPerUnit];
};

//-----------------------------------------------------------------------------

/**
 * @ingroup event
 */
class STBAPI_API Stb3DEventGenerator 
{
public:
    /** initialize instance, sets internal variables etc.
     * @param newTracker pointer to tracker
     */
    static void init(  TrackerOT * newTracker );
     /** set root of scene graph of a given station for 3D events
      * @param index number of station
      * @param newRoot pointer to the root node
      */
    static void setRoot( int index, SoNode* newRoot );
    /** sample & hold tracker and update global fields
     */
    static void sample();    
    /// send events for all stations
    static void sendEvents( const std::vector<StationEventInfo *> & changedStations);
    /** get pointer to translational field
     * @param station number of the station to consider */
    static SoSFVec3f* getTranslationField( int station );
    /** get pointer to rotation field
     * @param station station to consider */
    static SoSFRotation* getRotationField( int station );
    /** get pointer to translational offset field
     * @param station number of the station to consider */
    static SoSFVec3f* getTranslationOffsetField( int station );
    /** get pointer to rotation offset field
     * @param station station to consider */
    static SoSFRotation* getRotationOffsetField( int station );
    /** get pointer to button field   
     * @param station station to consider
     * @param buttonNumber number of button */
    static SoSFBool* getButtonField( int station,  int buttonNumber );
    /// turn on event dispatching
    static void activate();
    /// turn off event dispatching
    static void deactivate();
    /// update global fields
    static void updateGlobalTrackerFields(const std::vector<StationEventInfo *> & changedStations);
    /// removes all global fields
    static void deleteGlobalTrackerFields();
    /** reset event grabbers. Depending on the value of root all grabbers
     * are reset, if root is NULL, or only those that have root as root nodes.
     * @param root which grabbers to reset */
    static void releaseAllGrabbers( SoNode *root = NULL );                              

    /** returns a pointer to the tracker instance stored within
     * @return pointer to current tracker */
    static TrackerOT * getTracker();
    
    /** returns the number of stations currently configured
     * @return number of stations */
    static int getNumStations();

private:
    /** send 3D event to scene graph
     * @param index station number
     * @param event the new event */
    static void process3DEvent( int index, const So3DEvent* event );

	/**
	 * returns the current time
	 */
	static SbTime getCurrentTime();

    /// tracker instance
    static TrackerOT * tracker;                           
    /// event data
    static So3DEvent event;                               
    /// processed data buffer
    static std::vector<StationEventInfo> sei;       
    /// backup data buffer
    static std::vector<StationEventInfo> backup;    
    /// handle event actions
    static std::vector<SoHandle3DEventAction *> h3a;             
    /// root nodes
    static SoNodeList root;     
    /// global fields
    static std::vector<StationFieldInfo> sfi;    
    /// names of global fields for later deallocation
    static std::vector<SbName> globalNames;
    
}; // class Stb3DEventGenerator

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_STB3DEVENTGENERATOR_H_

