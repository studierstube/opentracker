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
/** Header file for CORBAUtils class, a collection of common utility functions.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: CORBAUtils.h 1151 2006-06-13 12:31:43Z  $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _CORBAUTILS_H
#define _CORBAUTILS_H

#include "../dllinclude.h"

#include <OT_CORBA.hh>
#include "../core/Event.h"

/**
 * This class implements some corba-related functions
 * @author Joseph Newman
 * @ingroup network
 */

namespace ot {

    class OPENTRACKER_API CORBAUtils
    {

    public:
        static void convertToCORBAEvent(Event& ot_event, OT_CORBA::Event& corba_event);

        static void convertFromCORBAEvent(Event& ot_event, const OT_CORBA::Event& corba_event);
        
        static CORBA::Object_ptr getObjectReference(CORBA::ORB_ptr orb, CosNaming::NamingContextExt::StringName_var string_name);
        
        static void bindObjectReferenceToName(CORBA::ORB_ptr orb, CORBA::Object_var obj, CosNaming::NamingContextExt::StringName_var string_name);
        
        static CosNaming::NamingContextExt_var getRootContext(CORBA::ORB_ptr orb);
        
        static CosNaming::NamingContext_var getContextFromName(CosNaming::NamingContext_var root_context, CosNaming::Name_var name);

        static CosNaming::NamingContext_var getContext(CosNaming::NamingContext_var root_context, CosNaming::Name_var name);

        static PortableServer::ObjectId_var getObjectId(CORBA::ORB_ptr orb, CosNaming::NamingContextExt::StringName_var string_name);
    };
            
} // namespace ot

#endif

/* 
 * ------------------------------------------------------------
 *   End of OSUtils.h
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
