 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for Module class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Module.h,v 1.3 2001/01/29 17:16:44 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _MODULE_H
#define _MODULE_H

/**
 * @page module How To Implement a Module
 *
 * A module is the basic element of functionality in the OpenTracker frame
 * work. It implements anything that is needed by the application. All module
 * classes are derived from Module. They override several of the virtual
 * methods according to their needs.
 *
 * @section lifetime The Life of a Module
 *
 * A module is going throught several steps during a program run. First it
 * is instantiated at some point, usually during initialisation. The global
 * function ::initializeContext is such a point, it instantiates all known
 * modules and adds them to a Context.
 * 
 * In the next step the modules Module::init method is called. The parameters are
 * data from the configuration section of the configuration file. They
 * are a StringMap containing the attributes of the configuration element
 * and any subtrees of children nodes, stored in ConfigNode objects.
 *
 * After parsing of the configuration file is finished and the DAG
 * structure of nodes has been built, the method Module::start is called on all
 * modules. This is a good place for any initialisations that depend on
 * configuration parameters and instantiated nodes.
 *
 * Now the main loop is entered and for all modules the methods
 * Module::pushState, Module::pullState and Module::stop are called. The first
 * allows the generation of new state update events and introduction into the
 * tree via EventGenerator nodes. The second is to retrieve states from the
 * shared DAG structure via other interfaces such as the EventQueue or
 * TimeDependend interface. 
 *
 * Module::stop tests whether a module wants to exit the main loop and
 * stop the program. If so the main loop is stopped and Module::close
 * is called on all modules. Then the program exits.
 *
 * @section implement Implementing a Module
 *
 * To implement a new module, decide about what your module will do in
 * each of these phases and which methods you need to override. Then
 * derive you module from the class Module and you are done. To use
 * in the standard contexts, instantiate a module of your class in
 * ::initializeContext and add it to the context passed to the method.
 * Then your module is available to the simple standalone program.
 *
 * @section thread A multithreaded Module
 *
 * If your module has some tasks that require more time than the
 * event loop should take, you are advised to implement your extension
 * using multiple threads. A simple helper class is ThreadModule that
 * takes care of starting an additional worker thread for your module
 * and provides synchronisation methods. Override the ThreadModule::run
 * method and implement your tasks. Then use ThreadModule::lock and
 * ThreadModule::unlock to synchronize access to your data between
 * the work thread and the main loop calling pushState or pullState.
 *
 * @section node The NodeFactory, Module and Context Triangle
 *
 * The three classes typically are in a close relation. The 
 * NodeFactory is an interface that allows a parser to create nodes
 * for elements by delegating the actuall parsing of element names
 * and attributes to the NodeFactory that knows about the node.
 *
 * Because the NodeFactory is the place a node is created, it is
 * simple to keep track of new nodes by implementing the NodeFactory
 * in the same class as the Module. This way a module can very
 * easily find out about the nodes it is interested in.
 *
 * A Context pulls all the different functionality together. It 
 * contains a ConfigurationParser object and stores the modules
 * and nodefactories needed for the application. Furthermore it
 * implements the main loop. The global helper function
 * ::initializeContext keeps the Context class general by keeping
 * the actual module instantiations outside of the class.
 */

#include <map>

using namespace std;

#include "Node.h"

/**
 * maps a string to another string. Mostly used to map element attributes
 * to values.
 */
typedef std::map<string, string> StringMap;

/**
 * Module is an abstract super class for all OpenTracker modules. A module
 * is associated with a configuration element. If the configuration element
 * is present, it is initialized with init. Then it is started. During the
 * main loop in Context, pushState and pullState, and stop are periodically
 * called. When the Context exits its main loop it calls close on all methods.
 *
 * This class provides empty implementations for all its methods instead of
 * pure virtual functions, so that classes inheriting from it don't have to
 * implement all methods in a trivial way.
 * @author Gerhard Reitmayr
 * @ingroup core
 */
class Module
{
//members
protected:
    /// check if this module appeared in the configuration section
    int initialized;
//methods
public:
    /// constructor method
    Module(): initialized(0)
    {};
    /**
     * closes the module. A place for cleanup code etc.
     * This class provides an empty implementation for subclasses not doing
     * anything here. */
    virtual void close()
    {};
    /**
     * initializes the tracker module. This class provides an implementation
     * that sets the initialization flag to true. Subclasses should call this
     * method if they override this method. It takes the attributes of the
     * element configuring this module and a local tree consisting of the
     * children of the element. This tree must be build of Nodes.
     * @param attributes StringMap of elements attribute values. Should be
     *        possibly , but is not for convenience.
     * @param localTree pointer to root of configuration nodes tree
     */
    virtual void init(StringMap& attributes,  Node * localTree)
    {
        initialized = 1;
    };
    /**
     * pulls state information out of the tracker tree. It enables the module
     * to query any EventQueue or TimeDependend node in the shared memory. It
     * is called after pushState was executed on each module.
     */
    virtual void pullState()
    {};
    /**
     * pushes state information into the tracker tree. It enables the module
     * to push new data into the tree by updating EventGenerator nodes and
     * thereby triggering an event.
     */
    virtual void pushState()
    {};
    /**
     * This method is called after initialisation is finished and before the
     * main loop is started. It allows the module to finish any setups that
     * need to be done before entering the main loop. */
    virtual void start()
    {};
    /**
     * tests whether the module wants the tracker main loop to stop.
     * @return 1 if main loop should stop, 0 otherwise. */
    virtual int stop()
    {
        return 0;
    };
    /**
     * tests whether the module was initialized or not.
     * @return 1 of the module was initialized, 0 otherwise. */
    int isInitialized()
    {
        return initialized;
    };
};

#endif
