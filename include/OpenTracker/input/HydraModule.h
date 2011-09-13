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
/** header file for Hydra module.
 *
 * @author Gatterer Clemens, Ivancsics Manuel
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section p5glovemodule HydraModule
 * The HydraModule provides and drives @ref hydrasource nodes. The
 * implementation was only tested under Windows because there was no Linux driver
 * available at the time of development.
 *
 * The module uses the configuration element 'HydraConfig'. This
 * element has the following attributes :
 * @li @c base = the base, currently only the value 0 is supported, as not more than one Razor Hydra can be used at the same time-
 *
 * An example configuration element looks like this :
 * @verbatim
 <HydraConfig base="0"/>@endverbatim
 *
 *
 */

#ifndef _HYDRAMODULE_H
#define _HYDRAMODULE_H

#include "../OpenTracker.h"


#ifdef USE_HYDRA
#ifdef WIN32

#include <sixense.h>
#include <sixense_math.hpp>
#include <sixense_utils/mouse_pointer.hpp>
#include <sixense_utils/derivatives.hpp>
#include <sixense_utils/button_states.hpp>
#include <sixense_utils/event_triggers.hpp>
#include <sixense_utils/controller_manager/controller_manager.hpp>

/**
 * The module and factory to drive the HydraSource nodes. It constructs
 * HydraSource nodes via the NodeFactory interface and pushes events into
 * the tracker tree according to the nodes configuration.
 * @author Clemens Gatterer, Manuel Ivancsics
 * @ingroup input
 */

namespace ot {

    class OPENTRACKER_API HydraModule : public Module, public NodeFactory
    {
        // Members
    protected:
        /// list of HydraSource nodes in the tree
        NodeVector nodes;

        int base;

		// Methods
    public:

        /** constructor method. */
        HydraModule();

        /** Destructor method, clears nodes member. */
        virtual ~HydraModule();
	/**
         * initializes the Hydra module
         * @param attributes StringTable of elements attribute values. Should be
         *        possibly , but is not for convenience.
         * @param localTree pointer to root of configuration nodes tree
         */
        virtual void init(StringTable& attributes,  ConfigNode * localTree);
        /** This method is called to construct a new Node. It compares
         * name to the HydraSource element name, and if it matches
         * creates a new HydraSource node.
         * @param name reference to string containing element name
         * @attributes reference to StringTable containing attribute values
         * @return pointer to new Node or NULL. The new Node must be
         *         allocated with new ! */
        virtual Node * createNode( const std::string& name,  const StringTable& attributes);
	/**
         * closes Hydra */
        virtual void close();
        /**
         * starts Hydra
         */
        virtual void start();
        /**
         * pushes events into the tracker tree. Checks all HydraSources and
         * pushes new events, if a HydraSource fires. The events store
	 * structure with position and status of the buttons.
         */
        virtual void pushEvent();

	protected:

    };
	OT_MODULE(HydraModule);

} // namespace ot

#endif
#endif

#endif

/* 
 * ------------------------------------------------------------
 *   End of HydraModule.h
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
