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
 * @file   QtMousePosSink.cxx
 * @author Christian Pirchheim
 *
 * @brief  Implementation of class @c QtMousePosSink
 *
 * $Id: OpenTracker.h 900 2006-01-19 16:47:43Z spiczak $
 */

#include <dllinclude.h>
#if USE_OTQT

#include "QtMousePosSink.h"
#include "OTQtMath.h"

namespace ot {

//--------------------------------------------------------------------------------
void QtMousePosSink::onEventGenerated(State & event, Node & generator) {

  // switch 'acquire pending event' (default: yes)
  bool acquire = true;

  // basic check: position unchanged ?!
  acquire = false;
  for (int i = 0; i < 3; i++) {
    if (event.position[i] != curr_event_.position[i]) {
      acquire = true;
      break;
    }
  }
  // within position threshold sphere ?!
  if (acquire && (state_ & POS_THRESH_FILTER))
    acquire = !isInsidePosThreshSphere(event);

  // acquire tracking event
  if (acquire)
    acquireEvent(event);
  // pass original event to parent nodes
  updateObservers(event);

//   ///// 3D position position threshold filtering

//   if ((state_ & POS_THRESH_FILTER) &&
//       (OTQtMath::distance(event.position, curr_state_.position) <= POS_THRESH_RADIUS))
//   {
//     updateObservers(event);
//     return;
//   }

//   ///// check incoming tracking event

//   for (int i = 0; i < 3; i++) {
//     if (event.position[i] != curr_state_.position[i]) {

//       // acquire tracking event
//       prev_state_ = curr_state_;
//       curr_state_ = event;
//       // set pending flag
//       state_ |= EVENT_PENDING;
//       break;
//     }
//   }

//   updateObservers(event);
}

} // namespace ot

#endif // USE_OTQT