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
