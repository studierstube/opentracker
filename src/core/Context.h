 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for Context class
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Context.h,v 1.10 2001/06/08 16:57:07 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _CONTEXT_H
#define _CONTEXT_H

#include "../dllinclude.h"

#include "Node.h"
#include "Module.h"
#include "NodeFactoryContainer.h"
#include "ConfigurationParser.h"

/// A vector of module pointers to store them
typedef std::vector<Module *> ModuleVector;

/**
 * This class represents one context. It keeps its own modules and tracker tree. Using
 * a ConfigurationParser it builds the tree from a given configuration file. Any 
 * factories and modules need to be added before the parsing happens, otherwise
 * the nodes can not be parsed and the modules will not be initialized with
 * their respective configuration elements. 
 *
 * Finally it implements the main loop driving the tracker. This consists of
 * the following basic algorithm described in pseudo code here :
 *@verbatim
start();
while( !stop())
{
    pushState();
    pullState();
}
close();@endverbatim
 * The whole loop is implemented in the member function run, which can be used
 * instead of implementing it yourself.
 *
 * @author Gerhard Reitmayr
 * @ingroup core
 */
class OPENTRACKER_API Context
{

protected:
    /// A vector of the modules this context works with
    ModuleVector modules;
    /// Pointer to the root node of the local tracker tree
    Node* rootNode;
    /// A NodeFactoryContainer used by the ConfigurationParser to create new nodes.
    NodeFactoryContainer factory;
    /// Pointer to a local ConfigurationParser used to build the tracker tree.
    ConfigurationParser * parser;
    /// flag to remember whether the Context is responsible for cleaning up the modules.
    int cleanUp;

// Methods
public:
   /** a constructor method. 
    * @param init If init is not equal to 0, it instantiates all known modules and 
    *        factories, adds them to its local containers and also takes care of 
    *        removing them again in the destructor.*/
    Context( int init = 0 );
   /** destructor method clears containers and removes any modules instantiated in
    * the default setup, if cleanUp is st. */
    virtual ~Context();
   /** adds a new newfactory to the NodeFactoryContainer.
    * @param newfactory reference to the new factory */
    void addFactory(NodeFactory& newfactory);
    /** adds a module to the contexts container.
     * @param name the element name of the modules configuration element
     * @param module reference to the module */
    void addModule(const string& name, Module& module);
    /** calls close on all modules to close any resources.*/
    void close();
    /** This method initialies the context with a tracker tree described by
     * a configuration file. It parses the file and builds the tree.
     * @param filename the path and name of the configuration file */
    void parseConfiguration(const string& filename);
    /** calls pullState on all modules to get data out again.*/
    void pullStates();
    /** This method calls pushState on all modules to get new data into the
     * shared data tree. */
    void pushStates();
    /** This method implements the main loop and runs until it is stopped
     * somehow. Then it calls close() on all modules. */
    void run();
    /** calls start on all modules to do some initialization. */
    void start();
    /** tests whether the mainloop should stop, by asking all modules whether
     * they need to stop.*/
    int stop();
    /** returns the current time in milliseconds since ... */
    static double currentTime();
};

#endif
