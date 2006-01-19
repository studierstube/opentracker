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
/** header file for State class.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _STATE_H
#define _STATE_H

#include "../dllinclude.h"
#include "OSUtils.h"

/**
 * A single state passing through the tree. It encodes the full information
 * of a tracked object. Not all information has to be used at a certain time.
 * Every node should define, what information is set and to what values.
 *
 * @todo the plan is to make this generic and not just put any fixed members
 *   on it. Maybe just extend it ? We will see.
 * @author Gerhard Reitmayr
 * @ingroup core
 */

namespace ot {

class OPENTRACKER_API State
{

public:
    /// the position value of the state
    float position[3];
    /** orientation value stored as unit quaternion. The vector component are
     * the first three entries and the scalar component is the forth entry.
     */
    float orientation[4];
    /// button states, each bit represents a button
    unsigned short button;
    /// confidence value, in [0,1]
    float confidence;
    /// timestamp of the state in milliseconds since 1.1.1970
    double time;
    /// typesafe null reference, to be used instead of a NULL pointer
    static State null;

public:
   /** constructor taking some values
     * @param time_ timestamp
     * @param isValid_ valid or not ?
     * @param confidence_ good or bad measurement */
    State(double time_ = 0.0, float confidence_ = 0);    
    /** copy constructor, copies the values of another state in the new
     * @param other the state to copy from */
    State( const State & other );
   /** copies the value of another state object into
     * this object.
     * @param other reference to the other state */
    State & operator= (const State& other);
    /** timestamps a state to the current time */
    void timeStamp();
};

// constructor method.

inline State::State(double time_, float confidence_)
    : confidence(confidence_)
    , time( time_ )
{
    position[0] = 0;
    position[1] = 0;
    position[2] = 0;
    orientation[0] = 0;
    orientation[1] = 0;
    orientation[2] = 0;
    orientation[3] = 1;
    button = 0;
}

// copies another state into this one

inline State& State::operator=(const State& other)
{
    if( this == &other ) return *this;
    button = other.button;
    confidence = other.confidence;
    orientation[0] = other.orientation[0];
    orientation[1] = other.orientation[1];
    orientation[2] = other.orientation[2];
    orientation[3] = other.orientation[3];
    position[0] = other.position[0];
    position[1] = other.position[1];
    position[2] = other.position[2];
    time = other.time;
    return *this;
}

// copy constructor

inline State::State( const State & other )
{
    *this = other;
} 

// timestamp a state to current time
inline void State::timeStamp()
{
    time = OSUtils::currentTime();
}

} // namespace ot

#endif
