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
/** Event Automaton. usable for all event nodes (derived from Base3D)
 *
 *
 * @author Gottfried Eibner
 *
 * $Id: EventAutomaton.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */


/// default values for automaton
const int AUT_NO_TRANSITION = -1;   /// no transition available
const int AUT_DONT_CARE     = -1;   /// don't care field content 

/** AutomatonCallback
  * each transition can call a callback routine */
typedef void AutomatonCallback(int transitionIdx, void* data, void* event);

#define STB_MAX_INPUT 5
#define STB_MAX_OUTPUT 5

/** @struct
  * Struct for automaton's transition.
  * @param id number of transition
  * @param sourceState state automaton is in
  * @param targetState state automaton changes to
  * @param input[] vector of input conditions that trigger a transition
  * @param output[] vector of output conditions that describe state change,
  * and things happening during the change
  * @param callback pointer to a callback routine (NULL if none) */
struct AutomatonTransition
{
    int id;
    int sourceState;             // state where we start from
    int targetState;             // state where we move to
    int input[STB_MAX_INPUT];        // vector of input conditions
    int output[STB_MAX_OUTPUT];      // vector of output conditions
    AutomatonCallback* callback; // gets called when transition is executed
};

/** 
  * EventAutomaton is a state machine to describe state change behaviours
  * and can be used for all event nodes (derived from Base3D).
  * usage example of EventAutomaton: new 3D window state automaton
  * (SoWindowKit's automaton)
  *
  * @code
  *  enum STATES {
  *      STATE_OUTSIDE, STATE_TV, STATE_TB, STATE_SB, STATE_VV, STATE_TBSEL, STATE_SBSEL
  *  };
  *
  *  numStates = 7
  *
  *  enum INPUTS {
  *      IN_EVENT, // values: PRESS, RELEASE, MOVE, MOVE_BUT
  *      IN_VOLUME    // volume pen is currently in 
  *  }
  *
  *  numInputs = 2
  *
  *  enum OUTPUTS {
  *      OUT_GRABBER,    // GRABBER_SET, GRABBER_RELEASE, GRABBER_DONT_CARE
  *      OUT_HIGHLIGHT,  // NO_HILIGHT, HIGHLIGHT, DRAG_HIGHLIGHT
  *      OUT_HANDLED,    // bool = set handled
  *      OUT_VOLUME      // volume we could move
  *  }
  *
  *  numOutputs = 4
  *
  *  transitions = ... // describe automaton's behaviour 
  * @endcode
  * @ingroup event
  */
class EventAutomaton
{
public:
    /** constructor
     * build an instance of the event automaton with given fields
     * and transition vector (desrcribing the automaton's behaviour) */
    EventAutomaton(
        int aNumStates,     /// number of states
        int aNumTransitions,/// number of transitions
        int aNumInputs,     /// number of fields counting for input conditions
        int aNumOutputs,    /// number of fields describing output conditions
        AutomatonTransition *aTransition, /// vector of transitions describing
        /// the automaton's behaviour
        int start           /// start state
    );

    /** Execute automaton and return new state or AUT_NO_TRANSITION if none.
     * The automaton search for an applicable transition with given state
     * (currentState) and given input vector.
     * All possible transition that can force a state change are these with
     * sourceState == currentState.
     * All applicable transition are these where their input vector equals
     * the given input vector.
     * The first applicalbe transition in the event automaton description
     * (transition vector) will be triggered.
     * @param input input vector with input conditions
     * @param output output vector with output conditions after the state
     * change.
     * @param data user data for callback routine
     * @param event event data for callback routine */
    int exec(                          
        int* input,
        int* output,
        void* data = (void*)0,
        void* event = (void*)0
    );

    /// return current state
    inline int getState(void) { return currentState; }

private:
    /** find an applicable transition in the transition vector
     * Current state equals transition source state.
     * Check all input fields of the given input vector with the transition's
     * input vector.
     * @param idx number of transition to check for
     * @param input input vector with input conditions */
    int applicableTransition(
        int idx,
        int* input
        );

    /** if an applicable transition was found apply it.
     * Trigger the state change and write transition's output fields into the
     * output vector. Call callback routine if specified with given data and
     * event.
     * @param idx number of transition to apply
     * @param output output vector to write output conditions into
     * @param data user data for callback routine
     * @param event event data for callback routine */
    void applyTransition(
        int idx,
        int* output,
        void* data,
        void* event
        );
  
    int currentState;                  /// current state (node)
    int numStates;                     /// number of states
    int numTransitions;                /// number of transitions
    int numInputs;                     /// number of used input fields
    int numOutputs;                    /// number of used output fields
    AutomatonTransition *transition;   /// vector of transitions
};
