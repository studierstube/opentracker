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
/** source file for ButtonFilter Node.
  *
  * @author Flo Ledermann flo@subnet.at
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/ButtonFilterNode.cxx,v 1.1 2002/01/30 11:24:16 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "ButtonFilterNode.h"

using namespace std;

// constructor method.

ButtonFilterNode::ButtonFilterNode( const char* buttonmaskstr, const char* buttonmapstr )
    : Node()
{
	int i;
	unsigned char buttonbit = 1;
	
	// initialize neutral
	buttonmask = 0xff;
	for (i=0;i<8;i++){
		buttonmap[i] = i;
	}

	// set buttonmask
	for (i=0; i<8; i++){
		if (buttonmaskstr[i] == '\0') break;
		if (!(buttonmaskstr[i] == '1')) buttonmask = buttonmask ^ buttonbit;

		buttonbit = buttonbit << 1;
	}

	// set buttonmap
	for (i=0; i<8; i++){
		if (buttonmapstr[i] == 0) break;
		if (buttonmapstr[i] >= '0' && buttonmapstr[i] <= '7') buttonmap[i] = buttonmapstr[i]-48;
		else buttonmap[i] = i;
	}

}

int ButtonFilterNode::isEventGenerator()
{
    Node * child = getChild( 0 );
	if( child != NULL )
	{
		return child->isEventGenerator();
    }
	return 0;
}

// this method is called by the EventGenerator to update it's observers.

void ButtonFilterNode::onEventGenerated( State& event, Node& generator)
{
	int i;
	unsigned char buttonbit = 1, buttonout = 0;

    lastState = event;

	lastState.button &= buttonmask;

	for (i=0; i<8; i++){
		// mask current button, shift into bit 0, shift into target position and OR into output
		buttonout = buttonout | (((lastState.button & buttonbit) >> i) << buttonmap[i]);
		// cool ;)

		buttonbit = buttonbit << 1;
	}

	lastState.button = buttonout;
	
    updateObservers( lastState );
}
