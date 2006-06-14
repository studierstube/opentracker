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
/** header file for TargusModule.
 *
 * @author Bernhard Reitinger
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section targusmodule TargusModule
 * The TargusModule provides the @ref targussource nodes that 
 * generate button press events for the wireless device. This
 * class relies on the code provided by xbindkeys (http://hocwp.free.fr/xbindkeys/xbindkeys.html)
 */

#ifndef _TARGUSMODULE_H
#define _TARGUSMODULE_H

// this is a linux implementation !!!
#ifndef WIN32

#include "../OpenTracker.h"
#include "../core/StringTable.h"
#include "../misc/XKeys.h"

#include <X11/Xlib.h>
#include <iostream>
#include <string>

#define USE_TARGUSMODULE
#ifdef USE_TARGUSMODULE

/**
 * The module and factory to drive the TargusSource node.
 * @author Bernhard Reitinger
 * @ingroup input
 */

namespace ot {

    class OPENTRACKER_API TargusModule : public Module, public NodeFactory
    {
        // Members
    protected:
        /// list of TargusSource nodes in the tree
        NodeVector nodes;

        // Methods
    public:
        /** constructor method. */
        TargusModule() : Module(), NodeFactory()
        {};

        /** Destructor method, clears nodes member. */
        virtual ~TargusModule();

        /** This method is called to construct a new Node. It compares
         * name to the TargusSource element name, and if it matches
         * creates a new TargusSource node.
         * @param name reference to string containing element name
         * @attributes refenrence to StringTable containing attribute values
         * @return pointer to new Node or NULL. The new Node must be
         *         allocated with new ! */
        virtual Node * createNode( const std::string& name,  StringTable& attributes);

        /**
         * closes the X keyboard source */
        virtual void close();

        /**
         * opens the X keyboard source
         */
        virtual void start();

        /**
         * pushes events into the tracker tree. Checks all TargusSources and
         * pushes new events, if a TargusSource fires. The events store
         * structure with position and status of the buttons.
         */
        virtual void pushEvent();

    protected:
        // holds the display pointer
        Display *display;

        // Stores all information about the key mappings
        XKeys *xkeys;
    };

} // namespace ot

#endif

// WIN32
#endif

#endif

/* 
 * ------------------------------------------------------------
 *   End of TargusModule.h
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
