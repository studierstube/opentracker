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
/**
 * @file   QtMouseWheelSink.cxx
 * @author Christian Pirchheim
 *
 * @brief  Implementation of class @c QtMouseWheelSink
 *
 * $Id: OpenTracker.h 900 2006-01-19 16:47:43Z spiczak $
 */

#include <dllinclude.h>
#if USE_OTQT

#include "QtMouseWheelSink.h"
#include "OTQtLog.h"

namespace ot {

//--------------------------------------------------------------------------------
void QtMouseWheelSink::onEventGenerated( State& event, Node& generator)
{
  OTQT_DEBUG("QtMouseWheelSink::onEventGenerated(): event.button = %hx\n", event.button);

  if (event.button != curr_event_.button) {
    // acquire tracking event
    acquireEvent(event);
  }
  // pass original event to parent nodes
  updateObservers(event);
}

//--------------------------------------------------------------------------------
bool QtMouseWheelSink::wheelForwardMove() const
{
  return (((prev_event_.button >> 0) & 0x0001) == 0 &&
          ((curr_event_.button >> 0) & 0x0001) == 1);

}

//--------------------------------------------------------------------------------
bool QtMouseWheelSink::wheelBackwardMove() const
{
  return (((prev_event_.button >> 1) & 0x0001) == 0 &&
          ((curr_event_.button >> 1) & 0x0001) == 1);
}


} // namespace ot

#endif // USE_OTQT

