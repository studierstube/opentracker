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
/** header file for HydraSource Node.
 *
 * @author Gatterer Clemens, Ivancsics Manuel
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section hydrasource HydraSource
 * The HydraSource node is an EventGenerator that outputs the
 * current position and orientation of one Razer Hydra Controller. Additionally it outputs the Status of the joystick, the trigger and the buttons of the controller.
 * It is driven by the @ref hydramodule.
 *
 * It has the following output
 * @li @c position = The X, Y and Z position of the controller.
 * @li @c orientation = The current rotation angles for the controller in quaternion form.
 * @li @c buttons = A bit vector describing the state of the controller buttons. See below for the bit field descriptions. This value can be AND'ed with one of the button macros to check the state of a given button. See the Notes section for a list of these macros.
 * @li @c trigger = The status of the analog trigger. 0 is unpressed, 255 is fully pressed.
 * @li @c joystickX = The horizontal position of the joystick. 0 is full left, 127 is centered, 255 is full right.
 * @li @c joystickY = The vertical position of the joystick. 0 is full down, 127 is centered, 255 is full up.
 *
 * It has the following attributes
 * @li @c controller = 0 or 1, indicating which of the two controllers shall be read. 
 * An example element looks like this :
 * @verbatim
 <HydraSource controller="0"/>@endverbatim
 *
 */

#ifndef _HYDRASOURCE_H
#define _HYDRASOURCE_H

#include "../OpenTracker.h"
#include <sixense.h>
#include <sixense_math.hpp>
#include <sixense_utils/mouse_pointer.hpp>
#include <sixense_utils/derivatives.hpp>
#include <sixense_utils/button_states.hpp>
#include <sixense_utils/event_triggers.hpp>
#include <sixense_utils/controller_manager/controller_manager.hpp>


#ifdef USE_HYDRA
#ifdef WIN32



namespace ot {

    /**
     * This class implements a simple source that sets its valid flag in
     * regular intervals and updates any EventObservers. 
	 * @author Clemens Gatterer, Manuel Ivancsics 
     * @ingroup input
     */
    class OPENTRACKER_API HydraSource : public Node
    {
        friend class HydraModule;

		// Members
    private: 
        /// the event that is posted to the EventObservers
        Event event;
	    /// controller to be tracked
		int controller;
		int base;


        // Methods
    protected:
        /** simple constructor, sets members to initial values */
		HydraSource(int,int);

        /** tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1 */
        virtual int isEventGenerator()
        {
            return 1;
        }

        Event & getEvent();
    };

}  // namespace ot

#endif
#endif

#endif

/* 
 * ------------------------------------------------------------
 *   End of HydraSource.h
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
