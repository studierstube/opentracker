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
/** header file for ConsoleModule module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/ConsoleModule.h,v 1.12 2001/06/08 16:56:06 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section consolemodule ConsoleModule
 * The ConsoleModule is a basic console based input / output module. It generates
 * input events via @ref consolesource nodes based on keyboard strokes and 
 * displays event data collected by @ref consolesink nodes.
 * It displays the data every interval'th cycle. On a windows system it just writes 
 * to the console. On a unix system it uses curses to format the display.
 * It cannot yet read all keys, as I'm still fighting with the Windows / 
 * courses API :).
 * The configuration element is called @c ConsoleConfig and has the following attributes :
 * @li @c interval 10 number of cycles to wait between displaying data.
 * @li @c headerline "" a single line used as headerline in the display.
 *
 * Moreover the keys for simulating tracking events can be configured freely.
 * The module supports 10 stations, numbered from 0 to 9. The input allows
 * to move the stations position in all 3 directions and to rotate around
 * all 3 axes. The velocities are displayed on top of the screen and can
 * be changed. The button bits 0 to 3 can be set, the keys strokes toggle
 * the state of the buttons, so you don't have to keep them pressed
 * all the time. Finally a station can
 * be reset to identity. To command several stations, the active station
 * has to be switched.
 
 * The @c KeyDefinition configuration element is used to
 * set the keys assigned to each function. It has the following attributes :
 * @li @c function name of the function ( see the following table )
 * @li @c key to assign the function to
 *
 * There are three ways to specifiy a key : 
 * @li use one of the predefined key names for arrow keys, function keys etc.
 * @li use a hexadecimal expression giving the keycode as returned by Windows
 *     or curses. This looks like @c 0xHHHH for some key.
 * @li directly use the character printed on the key. This does only work for
       simple characters.
 *
 * The following table lists the supported key names :
 * @verbatim
Name            Key

down            cursor down
up              cursor up
left            cursor left
right           cursor right
home            Pos1 ( aka Home )
end             End
page_down       page down
page_up         page up
backspace       backspace ( <- )
F0              F0 ( if present )
F1 .. F12       F1 througth F12          
insert          Insert
enter           Enter or Return
escape          Escape
@endverbatim
 * Note that the curses definition where taken from an SGI keyboard and
 * the keycodes returned by the curses library. Only the keypad ( numpad )
 * keys work correctly, the ones in the middle are send to the shell ?!
 *
 * The following table lists all possible functions and their default keys :
 * @verbatim
Function        Default Key     Description
Move_X_plus     up              moves in direction X +
Move_X_minus    down            moves in X -
Move_Y_plus     left            moves in Y +
Move_Y_minus    right           moves in Y -
Move_Z_plus     page_up         moves in Z +
Move_Z_minus    page_down       moves in Z -
Rot_X_plus      e               rotates positive around X
Rot_X_minus     d               rotates negative around X
Rot_Y_plus      r               rotates positive around Y
Rot_Y_minus     f               rotates negative around Y
Rot_Z_plus      t               rotates positive around Z
Rot_Z_minus     g               rotates negative around Z
Accelerate      y               increases velocities
Brake           x               reduces velocities
Button_1        ' '             sets button bit 0
Button_2        ,               sets button bit 1
Button_3        .               sets button bit 2
Button_4        -               sets button bit 3
Station_0       0               activates Station 0
Station_1       1               activates Station 1
Station_2       2               activates Station 2
Station_3       3               activates Station 3
Station_4       4               activates Station 4
Station_5       5               activates Station 5
Station_6       6               activates Station 6
Station_7       7               activates Station 7
Station_8       8               activates Station 8
Station_9       9               activates Station 9
Reset           w               resets current station
Quit            q               signals to quit OpenTracker
@endverbatim
 * 
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
#include "ConsoleSource.h"
#include "ConsoleSink.h"

/**
 * The module and factory to drive the console output sink nodes. 
 * On a windows system it uses WIN32 API calls to clear the console and reset
 * the cursor. On a unix it uses the curses library to achieve the same 
 * effect.
 *
 * @ingroup common
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

    /// maps key chars to indices
    vector<int> keyMap;

protected:

// Methods
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
    virtual Node * createNode( string& name,  StringTable& attributes);
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
