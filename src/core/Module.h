 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for Module class.
  *
  * @author Gerhard Reitmayr
  *
  * @todo put in this file a section on how to create a new module for
  * another device or node service etc.
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Module.h,v 1.1 2000/12/11 10:46:41 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _MODULE_H
#define _MODULE_H

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
