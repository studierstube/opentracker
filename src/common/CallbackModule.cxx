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
/** source file for Callback module.
 *
 * @author Gerhard Reitmayr
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include "../tool/disable4786.h"

#include <stdlib.h>
#include "CallbackModule.h"

#include <iostream>

//using namespace std;

#include <ace/Log_Msg.h>
#include "../tool/OT_ACE_Log.h"

// called to construct a new Node.


#ifndef OT_NO_CALLBACKMODULE_SUPPORT


namespace ot {

  Node * CallbackModule::createNode( const std::string& name, StringTable& attributes)
  {
    if( name.compare("Callback") == 0 )
      {
        const std::string & nameVal = attributes.get("name");
        NodeMap::iterator it = nodes.find( nameVal );
        if( it == nodes.end())
	  {
            CallbackNode * node = new CallbackNode( nameVal );
            nodes[nameVal] = node;
	    LOG_ACE_INFO("ot:Build Callback node %s.\n", nameVal.c_str());
            return node;
	  }
      } 
    return NULL;
  }

  //  sets a callback on a certain node.

  void CallbackModule::setCallback( const std::string& name, CallbackFunction * function, void * data )
  {
    NodeMap::iterator it = nodes.find( name );
    if( it != nodes.end())
      {
        ((CallbackNode *)(*it).second)->function = function;
        ((CallbackNode *)(*it).second)->data = data;
      }
  }

} // namespace ot


#else
#pragma message(">>> OT_NO_CALLBACKMODULE_SUPPORT")
#endif //OT_NO_CALLBACKMODULE_SUPPORT


/* 
 * ------------------------------------------------------------
 *   End of CallbackModule.cxx
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
