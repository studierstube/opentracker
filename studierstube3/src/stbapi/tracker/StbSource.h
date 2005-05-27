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
/** header file for StbSource Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id: StbSource.h 4055 2004-11-26 10:41:36Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __STBSOURCE_H__
#define __STBSOURCE_H__

#include <string>
#include <Inventor/sensors/SoFieldSensor.h>
#include <OpenTracker.h>
#include <stbapi/stbapi.h>

using namespace ot;

class TrackerOT;

/**
 * This class implements a node that receives data from various OpenInventor fields
 * (part of one or several nodes) and generates new events based on this data. It tries 
 * to obtain references to the necessary fields at runtime, even if the fields are removed
 * and replaced by different ones. It also will generate events, even if not all fields
 * are initialized and found yet. The module handling this node is implemented in TrackerOT.
 *
 * The corresponding OpenTracker configuration element has the following attributes :
 * @li @c node if present this is the name of the OpenInventor node to refer the fields to
 * @li @c position the field name of a SoSFVec3f field that contains the position.
 * @li @c orientation the field name of a SoSFRotation field that contains the orientation.
 * @li @c button the field name of a SoSFInt32 field that contains button information.
 * @li @c confidence the field name of a SoSFFloat field that contains confidence information.
 * If the node field is empty the field names must be prefixed by a node name seperated
 * with a '.'(dot). Then each field come from another node !
 *
 * An example element looks like this :
 * @verbatim
<StbSource node="MyNode" position="translation" orientation="rotation"/>@endverbatim
 *
 * @author Gerhard Reitmayr
 *
 * @todo performance while testing for the OIV node could be improved by storing 
 * computed strings etc.
 * @ingroup tracking
 */
class STBAPI_API StbSource : public Node
{

protected:

	/// flag to store, if new data is available and should be processed
	bool changed;

	/// flag if not all fields are found yet and further setup is necessary
	bool setup;

	/// stores the name of the node if any
	std::string node;
	/// stores the name of the position field to use. It must be of type SoSFVec3f.
	std::string position;
	/// stores the name of the orientation field to use. It must be of type SoSFRotation.
	std::string orientation;
	/// stores the name of the button field to use. It must be of the type SoSFInt32.
	std::string button;
	/** stores the name of the confidence field to use. It must be of the type SoSFFloat and will
	  * be clamped to the interval [0,1]. 
	  */
	std::string confidence;

	/// field sensor for position field
	SoFieldSensor positionSensor;
	/// field sensor for orientation field
	SoFieldSensor orientationSensor;
	/// field sensor for button field
	SoFieldSensor buttonSensor;
	/// field sensor for confidence field
	SoFieldSensor confidenceSensor;

	/// state used for OpenTracker event processing
	State state;

    /** constructor gets all the data and initializes the field sensors to the right callback functions
	 * and data object as this.
	 */
    StbSource( const std::string & node_, const std::string & position_, const std::string & orientation_,
			   const std::string & button_, const std::string & confidence_ ) 
			: Node(), changed( false ), setup( false ), node( node_ ), position( position_),
			  orientation( orientation_ ),  button( button_), confidence( confidence_ )
    {
		positionSensor.setFunction( positionCB );
		positionSensor.setDeleteCallback( deleteCB, this );
		positionSensor.setData( this );

		orientationSensor.setFunction( orientationCB );
		orientationSensor.setDeleteCallback( deleteCB, this );
		orientationSensor.setData( this );

		buttonSensor.setFunction( buttonCB );
		buttonSensor.setDeleteCallback( deleteCB, this );
		buttonSensor.setData( this );

		confidenceSensor.setFunction( confidenceCB );
		confidenceSensor.setDeleteCallback( deleteCB, this );
		confidenceSensor.setData( this );
	};
	
	/// the delete callback to be notified if a field is about to be removed
	static void deleteCB( void * data, SoSensor * sensor);
	
	/// position callback, updates the state variable and sets changed to true
	static void positionCB( void * data, SoSensor * sensor);

	/// orientation callback, updates the state variable and sets changed to true
	static void orientationCB( void * data, SoSensor * sensor);

	/// button callback, updates the state variable and sets changed to true
	static void buttonCB( void * data, SoSensor * sensor);

	/// confidence callback, updates the state variable and sets changed to true
	static void confidenceCB( void * data, SoSensor * sensor);

	/** checks whether any fields not yet connected to have become available.
	 * If all fields configured are bound to sensors, it sets setup to be true.
	 * The delete callback will setup to false again. 
	 */
	void checkForNodes();

	/** static helper function that takes a field name of the form 
	 * 'nodeName.fieldName' and tries to get a field of the name
	 * 'fieldName' from a node of the name 'nodeName'.
	 * If returns NULL, if no such field can be found.
	 */
	static SoField * getFieldByName( const std::string & longName );

public:
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator();

	/** this is callbed by the tracker module TrackerOT in the mainloop.
     * It tests, whether any setup is necessary and tries to do further 
	 * setup. Then it checks for new data and pushes that into the tracker
	 * graph.
	 */
	void pushState()
	{
		if( !setup )
		{
			checkForNodes();
		}
		if( changed )
		{
			updateObservers( state );
			changed = false;
		}
	};

	friend class TrackerOT;
};

#endif
