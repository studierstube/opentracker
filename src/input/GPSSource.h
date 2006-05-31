 /* ========================================================================
  * Copyright (c) 2006,
  * Institute for Computer Graphics and Vision
  * Graz University of Technology
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are
  * met:
  *
  * Redistributions of source code must retain the above copyright notice,
  * this list of conditions and the following disclaimer.
  *
  * Redistributions in binary form must reproduce the above copyright
  * notice, this list of conditions and the following disclaimer in the
  * documentation and/or other materials provided with the distribution.
  *
  * Neither the name of the Graz University of Technology nor the names of
  * its contributors may be used to endorse or promote products derived from
  * this software without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
  * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
  * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
  * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
  * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for GPSSource Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section gpssource GPSSource
 * The GPSSource node is a simple EventGenerator that outputs GPS position data encoded
 * in the position part of the event. This data is in Latitue, Longitute, Height 
 * in the WGS84 coordinate system. The confidence value is the inverse of the PDOP value. 
 * It is managed by the @ref gpsmodule, see there for more information on how to configure GPS support.
 * Use a filter after that to transform the data into your local coordinate system. The node
 * has no further attributes besides the ID attribute.
 *
 * An example element looks like this :
 * @verbatim
<GPSSource/>@endverbatim
 */

#ifndef _GPSSOURCE_H
#define _GPSSOURCE_H

#include <assert.h>

#include "../OpenTracker.h"
#include "GPSDriver.h"

/**
 * A very simple EventGenerator node for outputing GPS position data.
 * @author Gerhard Reitmayr
 * @ingroup input
 */

namespace ot {

class OPENTRACKER_API GPSSource : public Node, public GPSListener  
{
public:

	 /// the event that is posted to the EventObservers
    Event event;
    /// the buffer event for data from the GPS receiver
    Event buffer;
    
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
	GPSSource() {};

	friend class GPSModule;
};

inline void GPSSource::newData( const GPResult * res, const char * line, void * userData )
{
    ACE_ASSERT( userData != NULL );
    if( res->type == GPResult::GPGGA){
        GPGGA * point = (GPGGA *) res;
        if( point->fix == 0)
            return;
        GPSModule * module = (GPSModule *)userData;
        module->lock();
        buffer.timeStamp();
        buffer.getPosition()[0] = (float)(point->lat * MathUtils::GradToRad);
        buffer.getPosition()[1] = (float)(point->lon * MathUtils::GradToRad);
        buffer.getPosition()[2] = (float)(point->altitude);
        buffer.getConfidence() = (float)(1 / point->hdop);
        module->unlock();
    }
}

} // namespace ot

#endif // !defined(_GPSSOURCE_H)
