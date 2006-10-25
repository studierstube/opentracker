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
/** source file for TimestampGeneratorModule module.
 *
 * @author Mathis Csisinko
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/common/TimestampGeneratorModule.h>


#ifndef OT_NO_TIMESTAMPGENERATOR_SUPPORT

#include <OpenTracker/common/TimestampGeneratorNode.h>


#include <ace/Log_Msg.h>

namespace ot {
	
    OT_MODULE_REGISTER_FUNC(TimestampGeneratorModule){
        OT_MODULE_REGISTRATION_DEFAULT(TimestampGeneratorModule, "TimestampGeneratorModule");
    }	

    Node* TimestampGeneratorModule::createNode(const std::string &name,StringTable &attributes)
    {
        if (name.compare("TimestampGenerator") == 0)
        {
            int timeOut = 1000;
			attributes.get("timeout",&timeOut);
            TimestampGeneratorNode* pNode = new TimestampGeneratorNode(timeOut);
            pNodes.push_back(pNode);

            ACE_DEBUG((LM_DEBUG,ACE_TEXT("ot:Build TimestampGenerator node\n")));
            initialized = 1;
            return pNode;
        }
        return NULL;
    }

    // pushes events into the tracker tree.

    void TimestampGeneratorModule::pushEvent()
    {
        for (NodeVector::iterator it = pNodes.begin();it != pNodes.end();++ it)
        {
            TimestampGeneratorNode* pNode = reinterpret_cast<TimestampGeneratorNode*>(*it);
            if (pNode->calcEvent())
                pNode->updateObservers(pNode->getEvent());
        }
    }

} //namespace ot


#else
#pragma message(">>> OT_NO_TIMESTAMPGENERATOR_SUPPORT")
#endif //OT_NO_TIMESTAMPGENERATOR_SUPPORT
