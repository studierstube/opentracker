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
/** source file for SpaceDeviceSource Node.
  *
  * @author Mathis Csisinko
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

#include <OpenTracker/input/SpaceDeviceSource.h>

#ifdef USE_SPACEDEVICE
#ifdef WIN32
#ifndef SPACEDEVICE_DEPRECATED

#include <opentracker\core\MathUtils.h>

namespace ot {

	SpaceDeviceSource::SpaceDeviceSource(HANDLE hDevice): Node(),modified(false),event(),hDevice(hDevice)
    {
    }

    void SpaceDeviceSource::setButtonEvent(unsigned short button)
	{
        event.getButton() = button;
        event.timeStamp();
        modified = true;
	}

    void SpaceDeviceSource::setPosition(float x,float y,float z)
	{
		event.getPosition()[0] = x;
		event.getPosition()[1] = y;
		event.getPosition()[2] = z;
        event.timeStamp();
        modified = true;
	}

	void SpaceDeviceSource::setOrientation(float yaw,float pitch,float roll)
	{
		float q[4];
		MathUtils::eulerToQuaternion(yaw,pitch,roll,q);
		for (size_t i = 0;i < sizeof(q) / sizeof(float);i ++)
			event.getOrientation()[i] = q[i];
	}

    bool SpaceDeviceSource::calcEvent()
    {
        bool result = modified;
        modified = false;
        return result;
    }

} // namespace ot

#endif
#endif
#endif