 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for Context class
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Context.h,v 1.1 2000/12/11 10:46:41 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _CONTEXT_H
#define _CONTEXT_H

#include <vector>
#include <string>

using namespace std;

#include "Node.h"
#include "Module.h"
#include "NodeFactoryContainer.h"
#include "ConfigurationParser.h"

/// A vector of module pointers to store them
typedef std::vector<Module *> ModuleVector;

/**
 * This class represents one context. It keeps its own modules and tracker tree. Furthermore
 * it implements the main loop driving the tracker.
 * @author Gerhard Reitmayr
 */
class Context
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

// Methods
public:
   /** a ructor method. It instantiates all known modules and factories and
    * adds them to its local containers. */
    Context();
   /** destructor method clears containers */
    virtual ~Context();
   /** adds a new newfactory to the NodeFactoryContainer.
    * @param newfactory reference to the new factory */
    void addFactory(NodeFactory& newfactory);
    /** adds a module to the contexts container.
     * @param name the element name of the modules configuration element
     * @param module reference to the module */
    void addModule(string& name, Module& module);
    /** calls close on all modules to close any resources.*/
    void close();
    /** This method initialies the context with a tracker tree described by
     * a configuration file. It parses the file and builds the tree.
     * @param filename the path and name of the configuration file */
    void parseConfiguration(string& filename);
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
};

#endif
