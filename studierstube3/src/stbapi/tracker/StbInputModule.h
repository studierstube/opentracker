/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** header file for StbInputModule class.
  *
  * @author Gerhard Reitmayr, Gerd Hesina
  *
  * $Id: StbInputModule.h 4055 2004-11-26 10:41:36Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __STBINPUTMODULE_H__
#define __STBINPUTMODULE_H__

#include <stbapi/stbapi.h>
#include <Inventor/events/SoEvent.h>

#include <OpenTracker.h>

using namespace ot;

class StbKeyboardSource;
class StbMouseSource;
class SoStudierstubeViewer;

/**
 * This class provides an OpenTracker module to interface the windows events
 * from the SoStudierstubeViewer with OpenTracker. Key events from the application 
 * window can be used to drive source nodes. Idea of connection to the different
 * viewer implementations from the KeyboardHandler by Gerd Hesina. It supports 
 * the StbKeyboadSource, StbMouseSource and StbSource nodes. Its configuration
 * element has the following attributes :
 * @li @c keyevents off on|off this flag configures whether the module lets
 *        keyboard event pass to the general OpenInventor event handler or not. 
 *        The default behaviour is to suppress used key events.
 * @li @c mouseevents on on|off this flag configures whether the module lets
 *        mouse events pass to the general OpenInventor event handler. If you
 *        turn this off standard Inventor mouse manipulation will not work 
 *        anymore.
 *
 * The definition of keys follows the implementation of the ConsoleModule in
 * OpenTracker. The following documentation is copied from there.
 *
 * The keys for simulating tracking events can be configured freely.
 * The module supports 10 stations, numbered from 0 to 9. The input allows
 * to move the stations position in all 3 directions and to rotate around
 * all 3 axes. The velocities are displayed on top of the screen and can
 * be changed. The button bits 0 to 3 can be set, the keys strokes toggle
 * the state of the buttons, so you don't have to keep them pressed
 * all the time. Finally a station can
 * be reset to identity. To command several stations, the active station
 * has to be switched.
 *
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
pad0            Keypad 0
pad1            Keypad 1
pad2            Keypad 2
pad3            Keypad 3
pad4            Keypad 4
pad5            Keypad 5
pad6            Keypad 6
pad7            Keypad 7
pad8            Keypad 8
pad9            Keypad 9
pad+            Keypad plus
pad/            Keypad divide
pad*            Keypad multiply
pad-            Keypad minus
padenter        Keypad enter
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
<StbInputConfig passevents="on">
    <KeyDefinition function="Move_X_plus" key="o"/>
</StbInputConfig>@endverbatim
 *
 * @author Gerhard Reitmayr
 * @ingroup tracking
 */
class STBAPI_API StbInputModule : public Module, public NodeFactory
{

protected:
    /// list of key symbols as ints to provide faster lookup
    enum keys { MOVE_X_PLUS = 1,
           MOVE_X_MINUS = 2,
           MOVE_Y_PLUS = 3,
           MOVE_Y_MINUS = 4,
           MOVE_Z_PLUS = 5,
           MOVE_Z_MINUS = 6,
           ROT_X_PLUS = 7,
           ROT_X_MINUS = 8,
           ROT_Y_PLUS = 9,
           ROT_Y_MINUS = 10,
           ROT_Z_PLUS = 11,
           ROT_Z_MINUS = 12,
           ACCELL = 13,
           BRAKE = 14,
           BUTTON_1 = 15,
           BUTTON_2 = 16,
           BUTTON_3 = 17,
           BUTTON_4 = 18,
           STATION_0 = 20,
           STATION_1 = 21,
           STATION_2 = 22,
           STATION_3 = 23,
           STATION_4 = 24,
           STATION_5 = 25,
           STATION_6 = 26,
           STATION_7 = 27,
           STATION_8 = 28,
           STATION_9 = 29,
           RESET = 30,
           QUIT = 31 };
           
    /// indices for different motion and button states        
    enum states { X = 1, Y, Z, RX, RY, RZ, B1, B2, B3, B4 };              

    /// maps the function names in the config file to indices
    static std::vector<std::string> functionMap;

    /** maps the function key names in the config file to key codes. 
        This provides a platform independend keycode map (aaarg) */
    static std::map<std::string,int> keyCodeMap;

    /// maps key chars to indices
    std::vector<int> keyMap;
    /// list of StbKeyboardSource nodes
    std::map<int, StbKeyboardSource *> keySources;
	/// list of StbMouseSource nodes
    std::map<int, StbMouseSource *> mouseSources;
    /// angular velocity and positional velocity
    float angularSpeed, posSpeed;
    /// currently active station, must be in [0-9]
    int station;
    /// time of last update
    double lastTime;
    /// state vector to describe current motion
    std::vector<int> states;
    /// store flags to indicate different changes to the state
    bool changed,moving;
    /// stores the keyevents flag
    bool keyEvents;
    /// stores the mouseevents flag
    bool mouseEvents;
    
    /** handles any key down events, the passed value is allready the decoded
     * function of the actually pressed key. 
     * @param key the function number (see implementation) 
     */
    void handleKeyDown( int key );
    
     /** handles any key up events, the passed value is allready the decoded
     * function of the actually pressed key. 
     * @param key the function number (see implementation) 
     */
    void handleKeyUp( int key );
    
public:
    /**
     * the constructor
     */
    StbInputModule(); 

    /** destructor deposes of any data structures. */
    virtual ~StbInputModule();
    
    /**
     * initializes the input module.
     * @param attributes StringTable of elements attribute values. Should be
     *        possibly , but is not for convenience.
     * @param localTree pointer to root of configuration nodes tree
     */
    virtual void init(StringTable& attributes, ConfigNode * localTree);


    /** creates various Studierstube input and output nodes such as
     * StbKeyboardSource, StbMouseSource and StbSource.
     * @param name the name of the element
     * @param attributes StringMap containing the attribute values
     * @return a new StbSink node or NULL
     */
    virtual Node * createNode( const std::string& name,  StringTable& attributes);

    /**
     * checks the console for keyboard input and updates any
     * ConsoleSource nodes accordingly. This happens every cycle 
     * and all key presses recorded since are used.
     */
    virtual void pushState();

    /** static method that is registered with the SoStudierstubeViewer to handle
     * keyboard events. It takes different form depening on the OpenInventor / Windows
     * system used. It decodes the key and passes it to the handleKeyDown or
     * handleKeyUp methods respectively. A static pointer to the current
     * StbInputModule is used to get at the data.
     */
    static SbBool processSoEvent(const SoEvent * event, SoStudierstubeViewer * const viewer);
};

#endif
