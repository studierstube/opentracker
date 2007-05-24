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
/** source file for StaticTransformation Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/common/StaticTransformation.h>
#include <OpenTracker/common/CommonNodeFactory.h>

namespace ot {

    // default constructor method.

    StaticTransformation::StaticTransformation()
    {
        translation[0] = 0;
        translation[1] = 0;
        translation[2] = 0;
        rotation[0] = 0;
        rotation[1] = 0;
        rotation[2] = 0;
        rotation[3] = 1;
        scale[0] = 1;
        scale[1] = 1;
        scale[2] = 1;
        confidence = 1;
        usePos = false;
        useOrient = false;
    }

    // constructor method.

    StaticTransformation::StaticTransformation(float translation_[3], float scale_[3],
                                               float rotation_[4], bool usePos_, bool useOrient_ )
        : Transformation(), usePos( usePos_ ), useOrient( useOrient_ )
    {
        for( int i = 0; i < 3; i ++ )
        {
            this->translation[i] = translation_[i];
            this->scale[i] = scale_[i];
            this->rotation[i] = rotation_[i];
        }
        this->rotation[3] = rotation_[3];

        confidence = 1;
    }

    // transforms a event.

    Event* StaticTransformation::transformEvent( Event* event )
    {
        //logPrintI("StaticTransformation::transformEvent\n");
#ifdef USE_LIVE
        lock();
#endif
        //logPrintI("transformEvent locked\n");
        // transform the position of the event
        if( usePos )
        {
            MathUtils::rotateVector( copyA2V(rotation, 4),  event->getPosition(), localEvent.getPosition() );
            localEvent.getPosition()[0] = localEvent.getPosition()[0]*scale[0] + translation[0];
            localEvent.getPosition()[1] = localEvent.getPosition()[1]*scale[1] + translation[1];
            localEvent.getPosition()[2] = localEvent.getPosition()[2]*scale[2] + translation[2];
            //logPrintI("position transformed\n");            
        }
        // transform the orientation of the event
        if( useOrient )
        {
            MathUtils::multiplyQuaternion( copyA2V(rotation, 4), event->getOrientation(), localEvent.getOrientation() );
            //logPrintI("orientation transformed\n");            
        }
        try {
            localEvent.getConfidence() = event->getConfidence() * confidence;
        } catch (std::invalid_argument) {
            logPrintE("no confidence value");
        }
        localEvent.getButton() = event->getButton();
        localEvent.copyAllButStdAttr(*event);
        localEvent.timeStamp();
#ifdef USE_LIVE
        logPrintI("about to unlock in transformEvent\n");
        unlock();
#endif
        //logPrintI("StaticTransformation::transformEvent about to return\n");
        return &localEvent;
    }


    void StaticTransformation::set_attribute(const char* _key, const char* _value) {
        lock();
        if (strcmp(_key, "rotation") == 0) {
            std::string val(_value);
            int ret = CommonNodeFactory::parseRotation(val, "quaternion", rotation);
        } else if (strcmp(_key, "translation") == 0) {
            std::string val(_value);
            int ret = CommonNodeFactory::parseVector(val, translation);
        } 
        unlock();
    }

} //namespace ot


/* 
 * ------------------------------------------------------------
 *   End of StaticTransformation.h
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
