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
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for InterSenseModule module.
  *
  * @author Ivan Viola, Matej Mlejnek, Gerhard Reitmayr, Jan Prikryl
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/InterSenseModule.cxx,v 1.3 2001/05/02 15:15:39 reitmayr Exp $
  * @todo optimize compare and copy with mem* calls
  * @todo implement IRIX / Linux version with Stb code
  * @file                                                                   */
 /* ======================================================================= */

#include "InterSenseSource.h"
#include "InterSenseModule.h"

#include <stdio.h>
#ifdef WIN32
#include <iostream>    /// VisualC++ uses STL based IOStream lib
#else
#include <iostream.h>
#endif
#include <isense.h>

// an essential constant

const double GradToRad = 3.141592654 / 180.0;

// change the names from the Unix InterSense code to fit the Windows names
#ifndef WIN32
    #define BOOL                    IS_BOOL
    #define ISD_TRACKER_INFO_TYPE   ISD_TRACKER_TYPE
    #define ISD_STATION_INFO_TYPE   ISD_STATION_CONFIG_TYPE
    #define ISLIB_OpenTracker       ISD_OpenTracker
    #define ISLIB_CloseTracker      ISD_CloseTracker
    #define ISLIB_GetTrackerConfig  ISD_GetTrackerState
    #define ISLIB_SetStationConfig  ISD_SetStationState
    #define ISLIB_GetStationConfig  ISD_GetStationState
    #define ISLIB_GetTrackerData    ISD_GetTrackerData
    #define ISD_MAX_BUTTONS         MAX_NUM_BUTTONS
#endif

struct ISTracker {
    ISD_TRACKER_HANDLE handle;
    ISD_TRACKER_INFO_TYPE info;
    ISD_DATA_TYPE data;
    string id;
    int comport;
    NodeVector sources;
};

// Destructor method
InterSenseModule::~InterSenseModule()
{
}

// initializes trackers
void InterSenseModule::init(StringTable& attributes, ConfigNode * localTree)
{
    for( unsigned i = 0; i < localTree->countChildren(); i++ )
    {
        ConfigNode * trConfig = (ConfigNode *)localTree->getChild(i);
        StringTable & trackerAttrib = trConfig->getAttributes();
        const string & id = trackerAttrib.get("id");
        int comport = 0;
        int num = sscanf(trackerAttrib.get("comport").c_str(), " %i", &comport );
        if( num == 0 ){
            cout << "Error in converting serial port number !" << endl;
            comport = 0;
        }
        for(ISTrackerVector::iterator it = trackers.begin(); it != trackers.end(); it ++ )
        {
            if( (*it)->id.compare( id ) == 0 && (*it)->comport == comport )
                break;
        }
        if(it == trackers.end())  // we got a truly new tracker
        {
#ifdef WIN32        
            ISD_TRACKER_HANDLE handle = ISLIB_OpenTracker( NULL, comport, FALSE, FALSE );
#else
            ISD_TRACKER_HANDLE handle = ISLIB_OpenTracker( comport, FALSE, FALSE );
#endif            
            if( handle <= 0 )
            {
                cout << "Failed to open tracker " << id << endl;                
            } 
            else {
                ISTracker * tracker = new ISTracker;
                tracker->id = id;
                tracker->comport = comport;
                tracker->handle = handle;
                BOOL res;
                res = ISLIB_GetTrackerConfig( tracker->handle, &tracker->info , FALSE);
                if( res == FALSE )
                {
                    cout << "Failed to get tracker config for " << id << endl;
                }
                if (tracker->info.TrackerType != ISD_INTERTRAX_SERIES)
                {
                    /* Set up each station for Precision class of trackers.
                       Ensure the orientation measure of the tracker stations is
                       quaternions.  Ensure that button values are returned. */
                    ISD_STATION_INFO_TYPE station;
                    for( int j = 1; j <= ISD_MAX_STATIONS; j++ )
                    {   
                        int error = 0;
                        if( !ISLIB_GetStationConfig( tracker->handle, &station, j, FALSE ) )
                        {
                            error = 1;                
                        }
                        else {
                            if( station.State == TRUE )
                            {
                                station.AngleFormat = ISD_QUATERNION;
                                station.GetButtons = TRUE;
                                if( !ISLIB_SetStationConfig( tracker->handle, 
                                    &station, j, FALSE ) )
                                    error = 2;
                            }
                        }               
                        if( error )
                        {
                            cout << "WARNING: InterSenseModule cannot "
                                 << ((error == 1 ) ? "get " : "set ")
                                 << "state for tracker " << id << " station "
                                 << j 
                                 << "\n - orientation measure may not be quaternion."
                                 << endl;
                        }                       
                    }
                }   // setup not intertrax
                trackers.push_back( tracker );
                cout << "Configured tracker " << id << endl;
            }       // open tracker ok
        }           // got a new tracker
        else {      // some conflict with another tracker
            cout << "Tracker " << id << " at port " << comport << " conflicts with " << (*it)->id << endl;
        }
    }               // all ConfigNodes
    Module::init( attributes, localTree );
}

// This method is called to construct a new Node.
Node * InterSenseModule::createNode( string& name, StringTable& attributes)
{
    if( name.compare("InterSenseSource") == 0 )
    {       
        const string & id = attributes.get( "id" );
        for(ISTrackerVector::iterator it = trackers.begin(); it != trackers.end(); it ++ )
        {
            if( (*it)->id.compare( id ) == 0 )
                break;
        }
        if( it != trackers.end())
        {
            int station;
            int num = sscanf(attributes.get("station").c_str(), " %i", &station );
            if( num == 0 ){
                cout << "Error in converting station number !" << endl;
                return NULL;
            }
            if( station < 0 || station >= ISD_MAX_STATIONS )
            {
                cout << "Station number out of range [0,"<< ISD_MAX_STATIONS-1 << "] !" << endl;
                return NULL;
            }
            InterSenseSource * source = new InterSenseSource( station );
            (*it)->sources.push_back( source );
            cout << "Build InterSenseSource node " << endl;
            return source;
        }
        else {
            cout << "No tracker " << id << " configured !" << endl;
        }
    }
    return NULL;
}

// closes InterSense library
void InterSenseModule::close()
{
    for(ISTrackerVector::iterator it = trackers.begin(); it != trackers.end(); it ++ )
    {
        ISLIB_CloseTracker( (*it)->handle );
    }    
}

// pushes events into the tracker tree.
void InterSenseModule::pushState()
{
    for(ISTrackerVector::iterator it = trackers.begin(); it != trackers.end(); it ++ )
    {
        if((*it)->sources.size() > 0 )
        {
            ISTracker * tracker = *it;
            ISLIB_GetTrackerData( tracker->handle, &tracker->data );
            for( NodeVector::iterator si = tracker->sources.begin(); si != tracker->sources.end(); si++ )
            {
                InterSenseSource * source = ( InterSenseSource * )*si;
                ISD_STATION_DATA_TYPE * data = &tracker->data.Station[source->station];
                if( tracker->info.TrackerType == ISD_INTERTRAX_SERIES)
                {
                    float quat[4];
                    MathUtils::eulerToQuaternion(data->Orientation[2] * GradToRad,
		                             data->Orientation[1] * GradToRad,
									 data->Orientation[0] * GradToRad,								
                                     quat);
                    if( quat[0] != source->state.orientation[0] || 
                        quat[1] != source->state.orientation[1] ||
                        quat[2] != source->state.orientation[2] ||
                        quat[3] != source->state.orientation[3] )
                    {
                        source->state.orientation[0] = quat[0];
                        source->state.orientation[1] = quat[1];
                        source->state.orientation[2] = quat[2];
                        source->state.orientation[3] = quat[3];
                        source->state.timeStamp();
                        source->updateObservers( source->state );
                    }
                }
                else {
                    int changed = 0;
                    if( data->Orientation[0] != source->state.orientation[0] || 
                        data->Orientation[1] != source->state.orientation[1] ||
                        data->Orientation[2] != source->state.orientation[2] ||
                        data->Orientation[3] != source->state.orientation[3] )
                    {
                        changed = 1;
                        source->state.orientation[0] = data->Orientation[0];
                        source->state.orientation[1] = data->Orientation[1];
                        source->state.orientation[2] = data->Orientation[2];
                        source->state.orientation[3] = data->Orientation[3];
                    }
                    if( data->Position[0] != source->state.position[0] || 
                        data->Position[1] != source->state.position[1] ||
                        data->Position[2] != source->state.position[2] )
                    {
                        changed = 1;
                        source->state.position[0] = data->Position[0];
                        source->state.position[1] = data->Position[1];
                        source->state.position[2] = data->Position[2];
                    }
                    unsigned short button = 0;
                    for( int i = 0; i < ISD_MAX_BUTTONS; i++ )
                    {
                        button |= data->ButtonState[i];
                    }
                    if( button != source->state.button )
                    {
                        changed = 1;
                        source->state.button = button;
                    }
                    if( changed == 1 )
                    {
                        source->state.timeStamp();
                        source->updateObservers( source->state );
                    }
                }       
            } // for all sinks
            
        } 
    } // for all trackers       
}

