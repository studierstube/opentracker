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
/** header file for Callback module.
 *
 * @author Gerhard Reitmayr
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section callbackmodule CallbackModule
 * The Callback module provides @ref callbacknode nodes that call registered 
 * callbacks functions for any event they receive. Each node can call only
 * one function. The nodes are addressed by unique names, set in the 
 * element of that node. After reading the initialization file an application
 * can register callback functions on the read nodes. It doesn't have a 
 * configuration element but reserves the name 'CallbackConfig' for it.
 */

#ifndef _CALLBACKMODULE_H
#define _CALLBACKMODULE_H

#include "../OpenTracker.h"
#include "CallbackNode.h"


#ifndef OT_NO_CALLBACKMODULE_SUPPORT


namespace ot {

    typedef std::map<std::string, Node*> NodeMap;

    /**
     * The module and factory to drive the callback nodes. It constructs
     * CallbackNode nodes via the NodeFactory interface and registers
     * callbacks with them after parsing
     * @author Gerhard Reitmayr
     * @ingroup common
     */
    class OPENTRACKER_API CallbackModule : public Module, public NodeFactory
    {
        // Members
    protected:
        /// map of strings onto nodes
        NodeMap nodes;

        // Methods
    public:
        /** constructor method. */
        CallbackModule() : Module(), NodeFactory()
	{};
        /** Destructor method, clears nodes member. */
        virtual ~CallbackModule(){};    
        /** This method is called to construct a new Node. It compares
         * name to the TestSource element name, and if it matches
         * creates a new TestSource node.
         * @param name reference to string containing element name
         * @attributes refenrence to StringMap containing attribute values
         * @return pointer to new Node or NULL. The new Node must be
         *         allocated with new ! */
        virtual Node * createNode( const std::string& name,  StringTable& attributes);
 
        /** sets a callback on a certain node. The node is identified by its
         * unique name. Any present callback function in the node is overwritten
         * by the new one.
         * @param name the unique name of the callback node 
         * @param function the new callback function to set on the node
         * @param data pointer to data that is passed to the callback function
         */
        void setCallback( const std::string& name, CallbackFunction * function, void * data = NULL );
    };
	OT_MODULE(CallbackModule);

} // namespace ot {


#endif //OT_NO_CALLBACKMODULE_SUPPORT


#endif

/* 
 * ------------------------------------------------------------
 *   End of CallbackModule.h
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
