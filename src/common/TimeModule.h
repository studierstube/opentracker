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
/** header file for Time Module.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section timemodule TimeModule
 * The TimeModule controls the timing of the OpenTracker main loop. It does 
 * not provide any functionality related to nodes and events. There are two
 * operating modes. In @e Sleep mode the TimeModule sleeps after each turn
 * of the main loop a specified amount of time. In @e Framerate mode it tries
 * to achieve a certain framerate of loops per second. Both modes are limited
 * by the underlying implementation of OSUtils. That means that sleeps shorter
 * then 1/100 of a second are not possible, which obvious implications for the
 * @e Sleep mode. The @e Framerate mode works by inserting short sleeps after
 * a number of loops to achieve the desired overall framerate ! 
 *
 * The configuration element is called @c TimeConfig and has the following attributes :
 * @li @c rate float giving the desired frames per second (i.e. 100 )
 * @li @c sleep number of milliseconds to sleep each loop
 * @li @c display false either true or false, displays the framerate at the end.
 * If both @c rate and @c sleep are specified the @c rate attribute takes precedence and the 
 * @e Framerate mode is used. If no configuration element is present, the module
 * is not activated and the main loop runs at full speed.
 *
 * An example configuration element looks like this :
 * @verbatim
<TimeConfig rate="187"/>@endverbatim
 */

#ifndef _TIMEMODULE_H
#define _TIMEMODULE_H

#include "../OpenTracker.h"

/**
 * This class implements the TimeModule functionality. It sleeps the required time
 * in the stop() method, which is executed after a loop to check for stopping
 * conditions. At this point it should not interfere with any event processing.
 * @author Gerhard Reitmayr
 * @ingroup common
 */

namespace ot {

class OPENTRACKER_API TimeModule : public Module
{
// Members
protected:
    // number of milliseconds to sleep in @e Sleep mode
   int sleep, 
    // flag for displaying of frame rate
       display;
   // start time to calculate the frame rate in @e Framerate mode
   double startTime, 
   // the desired frame rate in frames / millisecond for computational purposes
       rate, 
   // counting frames
       count;

// Methods
public:
    /** constructor method. */
    TimeModule() : Module(), sleep(0), display(0), rate(0)
    {};
  
    /**
     * initializes the tracker module. Parses any @c sleep or @c rate attributes
     * @param attributes StringTable of elements attribute values. Should be
     *        possibly , but is not for convenience.
     * @param localTree pointer to root of configuration nodes tree
     */
    void init( StringTable & attributes,  ConfigNode * localTree);
  
    /** starts the module, stores startTime if in @i Framerate mode. */
    void start();
    
    /** test for stopping the main loop. Here the actual sleeping is done
     * if necessary.
     * @return always 0*/
    int stop();
    
    /** closes the module, prints out the framerate, if display is 1. */
    void close();
};

} // namespace ot

#endif
