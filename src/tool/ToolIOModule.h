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
/** header file for special module handling ConsoleSink / ConsoleSource nodes
  * instead of the usuall ConsoleModule.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _TOOLIOMODULE_H
#define _TOOLIOMODULE_H

#include <OpenTracker.h>
#include <QTable.h>

/** This module substitutes the usuall ConsoleModule in an OpenTracker Context
 * and takes over its nodes and configuration. It does not implement ConsoleSources
 * with keyboard events, but will at some state provide a special user interface
 * to do so. Until then it will only output stuff.
 * @author Gerhard Reitmayr
 * @ingroup tool
 */
class ToolIOModule : public Module, public NodeFactory 
{
    // Members
protected:
    /// list of ConsoleSink nodes in the tree
    NodeVector sinks;
    /// list of ConsoleSource nodes  in the tree
    NodeVector sources;
    /// current cycle count, for computing when to print out the state again
    int cycle;
    /// cycle interval to use for printing out states
    int interval;
    /// should the module quit ?
    int quit;
    /// the table displaying the output information
    QTable * output;

public:
    /** constructor method. initializes internal and static data
     * such as the functionMap and keyMap tables. */
    ToolIOModule( QTable * out_ ); 
    /** Destructor method, clears nodes member. */
    virtual ~ToolIOModule();
    /**
     * initializes the tracker module. 
     * @param attributes StringTable of elements attribute values. Should be
     *        possibly , but is not for convenience.
     * @param localTree pointer to root of configuration nodes tree
     */
    virtual void init(StringTable& attributes, ConfigNode * localTree);
    /** This method is called to ruct a new Node. It compares
     * name to the ConsoleSink element name, and if it matches
     * creates a new ConsoleSink node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringTable containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( const std::string& name, StringTable& attributes);
    /**
     * checks the console for keyboard input and updates any
     * ConsoleSource nodes accordingly. This happens every cycle 
     * and all key presses recorded since are used.
     */
    // virtual void pushState();
    /**
     * reads out the ConsoleSink nodes current state an prints it
     * to the console. This is done only each length cylce.
     */
    virtual void pullState();
    /**
     * On Unix platforms initializes curses. This method is called after 
     * initialisation is finished and before the main loop is started.*/
    virtual void start();
    /**
     * On Unix platforms closes curses.*/
    virtual void close();
    /**
     * tests whether a key was pressed, if so it stops.
     * @return 1 if main loop should stop, 0 otherwise. */
    virtual int stop();
};

#endif