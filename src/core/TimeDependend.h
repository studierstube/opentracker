 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** Header file for TimeDependend interface.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Attic/TimeDependend.h,v 1.1 2000/12/11 10:46:41 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _TIMEDEPENDEND_H
#define _TIMEDEPENDEND_H

/**
 * Interface for classes that implement time dependend functions.
 * @author Gerhard Reitmayr
 */
class TimeDependend
{
public:
    /**
     * returns the state of the time dependend function at a given
     * point in time.
     * @param time point in time in milliseconds since 1.1.1970
     * @return reference to the state value */
    virtual State& getStateAtTime(double time)  = 0;
};

#endif

