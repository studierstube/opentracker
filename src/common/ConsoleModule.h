 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for ConsoleModule module.
  *
  * @author Gerhard Reitmayr
  * @todo a lot of documentation needed !
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/ConsoleModule.h,v 1.5 2001/03/24 23:50:02 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section consolemodule ConsoleModule
 * The ConsoleModule displays event data collected by @ref consolesink nodes.
 * It displays the data every interval'th cycle. On a windows system it just writes 
 * to the console. On a unix system it uses curses to format display.
 * It has the following attributes :
 * @li @c interval 10 number of cycles to wait between displaying data.
 * @li @c headerline "" a single line used as headerline in the display.
 *
 * An example configuration element looks like this :
 * @verbatim
<ConsoleConfig interval="10" headerline="Tracker Test 1">
    <KeyDefinition function="Move_X_plus" key="o"/>
</ConsoleConfig>@endverbatim
 */

#ifndef _CONSOLEMODULE_H
#define _CONSOLEMODULE_H

#include "../OpenTracker.h"

typedef vector<string> StringVector;

/**
 * The module and factory to drive the console output sink nodes. 
 * On a windows system it uses WIN32 API calls to clear the console and reset
 * the cursor. On a unix it uses the curses library to achieve the same 
 * effect.
 *
 * @author Gerhard Reitmayr
 */
class ConsoleModule: public Module, public NodeFactory
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
    /// headerline in display
    string headerline;
    /// angular velocity and positional velocity
    float angularSpeed, posSpeed;
    /// currently active station, must be in [0-9]
    int station;
    /// should the module quit ?
    int quit;

    /// maps the function names in the config file to indices
    static StringVector functionMap;
    /// maps key chars to indices
    vector<char> keyMap;

// Methods
protected:
    /** sets the button bit of given button on all sources that
     * are associated with station. Changes the changed flag on
     * the sources.
     * @param station the number of the station to change
     * @param button the number of the button to set ( 1 - 4, where 1 corresponds to LSB )
     */
    void setButton( int station , int button );
    /** moves the position by the given data on all sources that
     * are associated with station. Changes the changed flag on
     * the sources.
     * @param station the number of the station to change
     * @param data array of 3 floats giving the movement vector
     */
    void movePos( int station, float * data );
    /** rotates the state by the given data on all sources that
     * are associated with station. Changes the changed flag on
     * the sources.
     * @param station the number of the station to change
     * @param data array of 4 floats giving rotational quaternion
     */
    void rotate( int station, float * data );
    /** resets the data on all sources that are associated with
     * the given station. Changes the changed flag on
     * the sources.
     * @param station the number of the station to change
     */
    void reset( int station );
public:
    /** constructor method. initializes internal and static data
     * such as the functionMap and keyMap tables. */
    ConsoleModule(); 
    /** Destructor method, clears nodes member. */
    virtual ~ConsoleModule();
    /**
     * initializes the tracker module. 
     * @param attributes StringMap of elements attribute values. Should be
     *        possibly , but is not for convenience.
     * @param localTree pointer to root of configuration nodes tree
     */
    virtual void init(StringMap& attributes,  Node * localTree);
    /** This method is called to ruct a new Node. It compares
     * name to the ConsoleSink element name, and if it matches
     * creates a new ConsoleSink node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringMap containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( string& name,  StringMap& attributes);
    /**
     * checks the console for keyboard input and updates any
     * ConsoleSource nodes accordingly. This happens every cycle 
     * and all key presses recorded since are used.
     */
    virtual void pushState();
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
