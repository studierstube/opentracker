 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for State class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/State.h,v 1.2 2001/01/03 14:46:36 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _STATE_H
#define _STATE_H

#ifndef WIN32
#include <unistd.h>
#include <sys/time.h>
#else
#include <windows.h>
#include <sys/timeb.h>
#include <time.h>
#endif
#include <sys/types.h>

/**
 * a single state passing through the tree. It encodes the full information
 * of a tracked object. Not all information has to be used at a certain time.
 *
 * @todo the plan is to make this generic and not just put any fixed members
 *   on it. Maybe just extend it ? We will see.
 * @author Gerhard Reitmayr
 */
class State
{

public:
    /// the position value of the state
    float position[3];
    /// orientation value stored as unit quaternion
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
    : time( time_ )
    , confidence(confidence_)
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
#ifndef WIN32  // IRIX and Linux code
#ifdef _SGI_SOURCE
    struct timeval tp;
    gettimeofday(&tp);
    time = (double)(tp.tv_sec)*1000.0 + (double)(tp.tv_usec)*0.001;
#else //LINUX code
    struct timeval tp;
    gettimeofday(&tp,NULL);
    time = (double)(tp.tv_sec)*1000.0 + (double)(tp.tv_usec)*0.001;
#endif
#else  // WIN code
    struct _timeb timeBuffer;
    _ftime(&timeBuffer);
    time = (double)(timeBuffer.time)*1000.0 + (double)timeBuffer.millitm;
#endif
}

#endif


