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
/** header file for TrackerOT class.
  *
  * @author Gerhard Reitmayr
  *
  * $Id: TrackerOT.h 4055 2004-11-26 10:41:36Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __TRACKEROT_H__
#define __TRACKEROT_H__

#include <Inventor/SbLinear.h>
#include <stbapi/stbapi.h>
#include <stbapi/tracker/StbInputModule.h>
#include <OpenTracker.h>

using namespace ot;

class StbSink;
class StbSource;
struct StationEventInfo;

/**
 * This class interfaces the OpenTracker library with the Studierstube tracking
 * mechanism. It provides the Tracker interface to the Studierstube. Internally
 * it manages an OpenTracker context. This context is initialized from a file 
 * set by a command line option, that is passed to the class via the 
 * TrackerBuilder. It also poses as a NodeFactory  and Module for the StbSink nodes,
 * which store data intended for Studierstube tracker stations, and the StbSource
 * nodes which route data from arbitrary OpenInventor fields into the tracker graph.
 *
 * The Context lifecycle is mapped to the Tracker lifecycle in the following way:
 * @li The TrackerOT constructor parses the configuration file and builds the
 *     tracker tree.
 * @li activate calls start on the Context to start the modules.
 * @li deactivate calls close on the Context to close the modules.
 * @li every call to getState runs a single loop, then copies new data to the
 *     TrackerState structure.
 *
 * @author Gerhard Reitmayr
 * @ingroup tracking
 */
class STBAPI_API TrackerOT : public Module, public NodeFactory
{
	typedef std::vector<StbSink *> SinkVector;
	typedef std::vector<StbSource *> SourceVector;
	typedef std::vector<StationEventInfo *> StationEventInfoVector;

public:
    /**
     * the constructor instantiates a Context and parses the configuration file
     * to create the Tracker tree structure.
     * @param file pointer to char array containing the path and filename of 
     *        the configuration file.
     */
    TrackerOT( const char * file ); 

    /** destructor deposes of any data structures. */
    virtual ~TrackerOT();

    /** calls start on the OpenTracker context to start the modules. */
    void activate();

    /** calls close on the OpenTracker context to stop and close the modules. */
    void deactivate();

    /** executes the OpenTracker main loop once, then copies any new data from the
     * StbSink nodes to the passed vector of StationsStates. This is a special 
     * interface for the OpenTracker tracker to allow more stations to be used.
     */
    const std::vector<StationEventInfo *> & getStateVector();

    /** returns the maximal number of stations available. */
    int getMaxStationNumber();

    /** creates a new StbSink node. It tests name for 'StbSink' and creates a
     * new StbSink name with the station number from the station attribute.
     * @param name the name of the element
     * @param attributes StringMap containing the attribute values
     * @return a new StbSink node or NULL
     */
    Node * createNode( const std::string& name,  StringTable& attributes);

	/**
     * pushes state information from StbSources into the tracker tree.
     */
    virtual void pushState();

    /**
     * This method is called after initialisation is finished and before the
     * main loop is started. TrackerOT tries to push the sources to initialization
	 * before the main loop starts. */
    virtual void start();

    /** for extended use of the OpenTracker data return a pointer to the
     * OpenTracker Context here.
     * @return pointer to Context */
    Context * getContext();

    /** returns whether a certain station should emit events or not. This 
     * subclass of Tracker actually implements this method.
     * @param which the number of the station to get information about
     * @return 0 if no events should be generated, 1 otherwise.
     */
    SbBool isEventEnabled( int which );

    /** returns filename of configuration file 
     *  @return filename of configuration file 
     */
    const char *getFilename();

protected:
    /**
     * add the data of an StbSink to the changed stations vector.
     */
    void addChangedStation( StationEventInfo * stationInfo );

    /// flag whether tracker is active or not
    int isActive;
    /// list of StbSink nodes
    SinkVector sinks;
	/// list of StbSource nodes
	SourceVector sources;
    /// Actual Context doing most of the work
    Context context;
    /// maximum defined station number
    int maximum;
    /// filename of configuration file
    std::string filename;
    /// this module deals with the keyboard input
    StbInputModule eventHandler;
    /// accumulates the changed stations and their new data during a single cycle
    StationEventInfoVector changedStations;

    friend class StbSink;
};

#endif

