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
/** header file for GPSInfoSource Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id: GPSInfoSource.h 712 2004-07-14 12:44:50Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section gpsinfosource GPSInfoSource
 * The GPSInfoSource node is a simple EventGenerator that outputs additional 
 * status data on the GPS signal. It encodes the type of fix in the first entry 
 * of the position, the number of satellites in the second and the hdop value in 
 * the third. It will always fire even if no there is no position fix. The fix type is 
 * 0 for no fix, 1 for uncorrected and 2 for corrected position.
 *
 * An example element looks like this :
 * @verbatim
<GPSInfoSource/>@endverbatim
 */

#ifndef _GPSINFOSOURCE_H
#define _GPSINFOSOURCE_H

#include <assert.h>

#include "../OpenTracker.h"
#include "GPSDriver.h"


namespace ot {


/**
 * A very simple EventGenerator node for outputing GPS position data.
 * @author Gerhard Reitmayr
 * @ingroup input
 */
class OPENTRACKER_API GPSInfoSource : public Node, public GPSListener  
{
public:

	 /// the state that is posted to the EventObservers
    State state;
    /// the buffer state for data from the GPS receiver
    State buffer;
    
	/** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }

    virtual void newData( const GPResult * point, const char * line, void * userData );

protected:
	/// protected constructor so it is only accessible by the module
	GPSInfoSource() {};

	friend class GPSModule;
};

inline void GPSInfoSource::newData( const GPResult * res, const char * line, void * userData )
{
    assert( userData != NULL );
    if( res->type == GPResult::GPGGA){
        GPGGA * point = (GPGGA *) res;
        GPSModule * module = (GPSModule *)userData;
        module->lock();
        buffer.timeStamp();
        buffer.position[0] = (float)point->fix;
        buffer.position[1] = (float)point->numsats;
        buffer.position[2] = (float)point->hdop;
        module->unlock();
    }
}


}  // namespace ot


#endif // !defined(_GPSINFOSOURCE_H)
