/* ========================================================================
 * Copyright (c) 2008,
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
/** header file for VideoLocationModule module.
 *
 * @author Markus Sareika
 *
 * $Header$
 * @file                                                                   */
/* ======================================================================= */


#ifndef _VIDEOLOCATIONMODULE_H
#define _VIDEOLOCATIONMODULE_H

#include "../dllinclude.h"

#include <vector>
#include <map>

#include "../OpenTracker.h"
#include "../core/VideoUser.h"


#ifdef USE_VIDEOLOCATION


namespace ot
{

  class VideoLocationModule;
  typedef std::vector<Node::Ptr> NodeVector;


  /**
   * The module and factory to drive the source nodes. It constructs
   * Source nodes via the NodeFactory interface and pushes events into
   * the tracker tree according to the nodes configuration.
   */
  class OPENTRACKER_API VideoLocationModule : public ThreadModule, public NodeFactory, public VideoUser
    {
      // Members
    protected:

      /// list of TestSource nodes in the tree
      NodeVector sources;


      // Methods
    public:
     
      /** constructor method. */
      VideoLocationModule();

      /** Destructor method, clears nodes member. */
      virtual ~VideoLocationModule();

      virtual Node * createNode( const std::string& name, const StringTable& attributes);
      
      /**
       * pushes events into the tracker tree. 
       */
      virtual void pushEvent(); 

      void newVideoFrame(const unsigned char* image, int width, int height, PIXEL_FORMAT format);

    };

	OT_MODULE(VideoLocationModule);

} // namespace ot


#endif //#ifdef USE_VIDEOLOCATION

#endif // _VIDEOLOCATIONMODULE_H
