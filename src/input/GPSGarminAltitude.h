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
/** header file for GPSGarminAltitude Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section gpsgarminaltitude GPSGarminAltitude
 * The GPSGarminAltitude node is a simple EventGenerator that outputs GPS 
 * altitude data from a Garmin device interpreting the propriatory $PGRMZ string.
 * The altitude is given as a vector with the altitude encoded in the positive Y 
 * direction in meters. It is managed by the @ref gpsmodule, see there for
 * more information on how to configure GPS support.
 *
 * An example element looks like this :
 * @verbatim
<GPSGarminAltitude/>@endverbatim
 */

#ifndef _GPSGARMINALTITUDE_H
#define _GPSGARMINALTITUDE_H

#include "../OpenTracker.h"
#include "GPSDriver.h"


namespace ot {


/**
 * A very simple EventGenerator node for outputing GPS direction and velocity data.
 * @author Gerhard Reitmayr
 * @ingroup input
 */
class OPENTRACKER_API GPSGarminAltitude : public Node, public GPSListener  
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
	GPSGarminAltitude() {};

	friend class GPSModule;
};

inline void GPSGarminAltitude::newData( const GPResult * res, const char * line, void * userData )
{
    assert( userData != NULL );
    if( res->type == GPResult::PGRMZ){
        PGRMZ * point = (PGRMZ *) res;
        GPSModule * module = (GPSModule *)userData;
        if( !module->driver->hasFix() )
            return;
        module->lock();
        buffer.timeStamp();
        buffer.position[0] = 0;
        // 1 feet = 0.3048 meter, by google
        buffer.position[1] = (float)(point->altitude * 0.3048);
        buffer.position[0] = 0;
        buffer.confidence = (float)(1 / module->driver->getHdop());
        module->unlock();
    }
}


}  // namespace ot


#endif // !defined(_GPSGARMINALTITUDE_H)
