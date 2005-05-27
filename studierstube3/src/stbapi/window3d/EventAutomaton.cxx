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
/** EventAutomaton.cxx  - implementation of class EventAutomaton.
 * EventAutomaton is a state machine to describe state change behaviours
 * and can be used for all event nodes (derived from Base3D).
 * 
 * @author Dieter Schmalstieg, Gottfried Eibner
 *
 * $Id: EventAutomaton.cxx 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#include "EventAutomaton.h"
#include <cstdio>

 /*-------------------------------------------------------------------------*/
/** constructor
  * build an instance of the event automaton with given fields
  * and transition vector (desrcribing the automaton's behaviour) */
 /*-------------------------------------------------------------------------*/
EventAutomaton::EventAutomaton(
    int aNumStates,
    int aNumTransitions,
    int aNumInputs,
    int aNumOutputs,
    AutomatonTransition *aTransition,
    int start)
:
    currentState(start),
    numStates(aNumStates),
    numTransitions(aNumTransitions),
    numInputs(aNumInputs),
    numOutputs(aNumOutputs),
    transition(aTransition)
{
    // nil
}

 /*-------------------------------------------------------------------------*/
/** find an applicable transition in the transition vector
  * Current state equals transition source state.
  * Check all input fields of the given input vector with the transition's
  * input vector.
  * @param idx number of transition to check for
  * @param input input vector with input conditions */
 /*-------------------------------------------------------------------------*/
int
EventAutomaton::applicableTransition(int idx, int* input)
{
    int okay = (currentState == transition[idx].sourceState)
        || transition[idx].sourceState==AUT_DONT_CARE;
    if(okay)
    {
        for(int i=0; (i<numInputs) && (okay); i++)
        {
            okay &= (input[i]==transition[idx].input[i]
                 || transition[idx].input[i]==AUT_DONT_CARE);
        }
    }
    return okay;
}

 /*-------------------------------------------------------------------------*/
/** if an applicable transition was found apply it.
  * Trigger the state change and write transition's output fields into the
  * output vector. Call callback routine if specified with given data and
  * event.
  * @param idx number of transition to apply
  * @param output output vector to write output conditions into
  * @param data user data for callback routine
  * @param event event data for callback routine */
 /*-------------------------------------------------------------------------*/
void
EventAutomaton::applyTransition(int idx, int* output, void* data, void* event)
{
    currentState = transition[idx].targetState;
    for(int i=0; i<numOutputs; i++)
        output[i] = transition[idx].output[i];
    if(transition[idx].callback)
    {
        transition[idx].callback(idx, data, event);
    }
}

 /*-------------------------------------------------------------------------*/
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
 /*-------------------------------------------------------------------------*/
int
EventAutomaton::exec(int* input, int* output, void* data, void* event)
{
    for(int i=0; i<numTransitions; i++)
    {
        if(applicableTransition(i, input))
        {
            applyTransition(i, output, data, event);
            return i;
        }
    }
    return AUT_NO_TRANSITION;
}

 /*-------------------------------------------------------------------------*/
/** usage example of EventAutomaton: new 3D window state automaton
  * (SoWindowKit's automaton)
  *
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
  *  transitions = ... // describe automaton's behaviour */
 /*-------------------------------------------------------------------------*/
