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
/** Common include file for OpenTracker classes.
  * This file declares and includes the central classes of the
  * OpenTracker framework. It is typically needed in all header files.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _OPENTRACKER_H
#define _OPENTRACKER_H

/**
 * @mainpage OpenTracker
 *
 * OpenTracker is a new hardware and event processing software for
 * VR and AR applications. It is designed to be extensible and
 * easily configureable. It is distributed under the Lesser Gnu Public License.
 * See the file LICENSE.txt for details.
 * 
 * Its functionality is divided into modules that implement specific functions
 * like device drivers, network interfaces etc. This simplifies adding new
 * functionality. For a list of currently implemented modules see the 
 * @ref module_ref page.
 *
 * Modules exchange data via a shared memory that is structured into a tree.
 * Different nodes of the tree are managed and observed by different modules.
 * The nodes exchange data via three interfaces :
 * @li EventGenerator is the interface of a node sending event updates ( events )
 * @li EventQueue is the interface of a node holding a queue of past events
 * @li TimeDependend is the interface of a node providing a event for any moment
 *     in time. 
 *
 * A node may implement several interfaces. However parent child relationships
 * are only allowed betweem nodes that understand each other. This is specified
 * in the configuration file format DTD. 
 *
 * The following links point to a reference of all modules and nodes and
 * how their configuration elements are specified :
 * @li @ref module_ref - reference of modules
 * @li @ref Nodes - reference of implemented nodes
 * @li @ref transform_nodes - reference for geometric transformation nodes 
 *
 * More information on building, using and extending OpenTracker can be found 
 * here :
 * @li <a href="../install.html">Installation Guide</a> - How to compile and
 * install OpenTracker.
 * @li <a href="../integrationguide.html">Integration Guide</a> - How to use
 * OpenTracker in your own programs.
 * @li <a href="../progguide.html">Programmers Guide</a> - How to extend 
 * OpenTracker with your own modules.
 * @li <a href="../userguide.html">Users Guide</a> - How to configure and
 * run OpenTracker.
 * @li @ref config - Lists different configuration options to include various
 *                   device drivers and other libraries.
 *
 * Additional information regarding various modules can be found here :
 * @li @ref spacemouse - How to integrate the SpaceMouse support
 * 
 * Also see a list of @ref changes between each version and a list of @ref contributions 
 * we would like to thank. 
 *
 * @author Gerhard Reitmayr
 * @date 2001, 2002, 2003
 */


#include "dllinclude.h"

#include "core/ThreadModule.h"
#include "core/NodeFactory.h"
#include "core/Context.h"
#include "core/MathUtils.h"
#include "core/StringTable.h"
#include "core/OSUtils.h"
#include "core/NodePort.h"

namespace ot {

#define OPENTRACKER_VERSION_MAJOR 1
#define OPENTRACKER_VERSION_MINOR 3
  
  enum OPENTRACKER_VERSION {
    VERSION_MAJOR = OPENTRACKER_VERSION_MAJOR,
    VERSION_MINOR = OPENTRACKER_VERSION_MINOR
  };

  /**
   * a Vector of Node pointers. Very useful to implement a simple
   * container of Nodes such as a parent node, or to keep pointers
   * to several nodes around.
   */
  typedef std::vector<Node *> NodeVector;

  /** initializes a context by instantiating the available modules and factories
   * and registering them with the given context. So for each passed context 
   * there is a new set of objects instantiated. This leaves the programmer with
   * the responsibility of deleting these objects again ! Any compile time definitions 
   * go in here, to define which modules are compiled in and can be instantiated.
   * Also the modules configuration element names are defined here.
   *
   * @param context reference of the context to be initialized.
   */
  OPENTRACKER_API void  initializeContext( Context & context );

} // namespace ot

#endif
