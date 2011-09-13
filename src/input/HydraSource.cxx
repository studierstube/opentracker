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
/** source file for HydraSource Node.
 *
 * @author Gatterer Clemens, Ivancsics Manuel
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/input/HydraSource.h>

#ifdef USE_HYDRA
#ifdef WIN32

namespace ot {

	HydraSource::HydraSource(int base, int controller) : Node(), base(base), controller(controller)
	{
		;
	}

    Event & HydraSource::getEvent()
	{
		std::vector<float> quaternion(4);
		std::vector<float> vec3(3);
		sixenseControllerData acd;
		sixenseSetActiveBase(base);

		if( sixenseIsControllerEnabled( controller ) ) {
			sixenseGetNewestData(controller, &acd );
			
			//add buttons to event
			event.getButton() = (unsigned short)acd.buttons;
			//add trigger to event
			event.setAttribute("trigger", acd.trigger);

			//add joystick x and y orientation to event
			event.setAttribute("joystickX", acd.joystick_x);
			event.setAttribute("joystickY", acd.joystick_y);
			
			//add controller position and rotation to event
			vec3[0]=acd.pos[0];
			vec3[1]=acd.pos[1];
			vec3[2]=acd.pos[2];
			event.getPosition() = vec3;
			quaternion[0]=acd.rot_quat[0];
			quaternion[1]=acd.rot_quat[1];
			quaternion[2]=acd.rot_quat[2];
			quaternion[3]=acd.rot_quat[3];
			event.getOrientation() = quaternion;
		}
		
		return event;
	}

}  // namespace ot

#endif
#endif
