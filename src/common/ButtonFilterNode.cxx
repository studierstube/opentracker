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
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

// this will remove the warning 4786
#include "../tool/disable4786.h"

#include "ButtonFilterNode.h"
#include <iostream>

#include <ace/Log_Msg.h>


#define DEBUG_BUTTONFILTER 
#if defined(DEBUG_BUTTONFILTER) && defined(_DEBUG) 
#define DEBUG_CODE(a) {a;}
#else
#define DEBUG_CODE(a) {}
#endif

using namespace std;

// constructor method.

namespace ot {

ButtonFilterNode::ButtonFilterNode(const char* buttonmaskstr, const char* buttonmapstr, const char * invertstr, 
								   const char * validtransstr , const char * radiobuttonsstr, const char *validTransSetButtonStr )
    : Node()
{
	int i;
	unsigned char buttonbit = 1;
	
	// initialize neutral
	buttonmask = 0xff;
    invert = 0;
	for (i=0;i<8;i++){
		buttonmap[i] = i;
	}

	// set buttonmask
	for (i=0; i<8; i++){
		// check end of string
		if (buttonmaskstr[i] == '\0') break;
		if (!(buttonmaskstr[i] == '1')) buttonmask = buttonmask ^ buttonbit;

		buttonbit = buttonbit << 1;
	}

	// set buttonmap
	for (i=0; i<8; i++){
		// check end of string 
		if (buttonmapstr[i] == '\0') break;
		
		if (buttonmapstr[i] >= '0' && buttonmapstr[i] <= '7') buttonmap[i] = buttonmapstr[i]-48;
		else buttonmap[i] = i;
	}

    // set invertmask
    buttonbit = 1;
    for (i=0; i<8; i++){
		// check end of string 
        if (invertstr[i] == '\0') break;
        if (invertstr[i] == '1') invert = invert ^ buttonbit;
        
        buttonbit = buttonbit << 1;
	}

	// set validtrans
	for (i=0; i< 8; i++)
	{
		validtrans[i] = -1;
		validTransSetButton[i] = -1;
		radiobuttons[i] = 0;
	}

	if (validtransstr!= NULL && radiobuttonsstr != NULL)
	{
		if (strlen(validtransstr) == 8 && strlen(radiobuttonsstr) == 8)
		{
			// these could be processed in one loop, but readiblity improves with 2 extra loops
			// set validtrans  
			for (i=0; i< 8; i++)
			{
				if (validtransstr[i] >= '0' && validtransstr[i] <= '7') 
				{
					int vtId = validtransstr[i] - 48;
					validtrans[i] = vtId; // set position to given id 
				}

				if (strlen(validTransSetButtonStr) > (unsigned int)i)
				{
					if (validTransSetButtonStr[i] >= '0' && validTransSetButtonStr[i] <= '7') 
					{
						int vtId = validTransSetButtonStr[i] - 48;
						validTransSetButton[i] = vtId; // set position to given id 
					} else { validTransSetButton[i] = -1; }
									
				}
			}
			
			// set radiobuttons 
			for (i=0; i< 8; i++)
			{
				if (radiobuttonsstr[i] >= '0' && radiobuttonsstr[i] <= '7') 
				{
					int radioButtonId = radiobuttonsstr[i] - 48;
					radiobuttons[i] = radioButtonId; // set position in radio button with id given in string
				}
				else
				{
					radiobuttons[i] = -1; // set position in radio button with not a radiobutton
				}

			}
		}
		else
		{
			//cerr << "ButtonFilterNode error. validtrans && radiobuttons must be exact 8 chars long. SETTINGS IGNORED !!" << endl;
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:ButtonFilterNode error. validtrans && radiobuttons must be exact 8 chars long. SETTINGS IGNORED !!\n")));
		}
	}
}

int ButtonFilterNode::isEventGenerator()
{    
	return 1;
}

// this method is called by the EventGenerator to update it's observers.
#define COUT_BINARY(bina) { for (int bi=0; bi <8; bi++) cout << (((bina)>>bi) & 1); }
void ButtonFilterNode::onEventGenerated( State& event, Node& generator)
{
	int i;
	unsigned char buttonbit = 1, buttonout = 0;

    lastState = event;
	DEBUG_CODE(cout << endl;);
	DEBUG_CODE(cout << "got button                  :"; COUT_BINARY(lastState.button); cout << endl;);
    
	unsigned char deleteButtonBitsNotValidTrans = 0x00; // delete no bits
	unsigned char validTransSetButtonMask = 0x00; // change no bits
	// just for debug:
	unsigned char validTransBits = 0x00; 
	unsigned char validTransSetButtonBit = 0x00; 
	// go through all 8 bits and check if 
	for (i=0; i < 8; i++)
	{
		
			//just for debug: 
		if (validTransSetButton[i] > -1) validTransSetButtonBit |= 1<< 1;

		if (validtrans[i] > -1) // validtrans bit is set
		{
			//just for debug: 
			validTransBits |= 1 <<i;

			// it is a validtrans bit
			unsigned char bit = (lastState.button >> i) &1;
	
			if (bit) // if vt is set -> set all buttonbits (that belong to the vt) to 0
			{
				deleteButtonBitsNotValidTrans |= (1<<i); // vt must be set to 0 !!
				for (int j =0 ; j< 8; j++)
				{
					if (radiobuttons[j] == validtrans[i]) 
						deleteButtonBitsNotValidTrans |= (1<<j);
				}
			} // vt is 0 -> vt transmission signal !!, change nothing button will show as 1
			else 
			{	 // fix for broken hardware :<
				// set all bits where the validtrans id and the validTransSetButton mask match
				// validtrans: 3-------  setbuttononvalidtrans: -33----- would set buttonbits 1-2 if valid trans bit 0 is cleared
				for (int j =0 ; j< 8; j++)
				{
					if (validTransSetButton[j] == validtrans[i]) 
						validTransSetButtonMask |= (1<<j);
				}
			}
		}
	}
	
	// invert and AND
	// if there are no deleteButtonBitsNotValidTrans (0x00) nothing is changed, all '1' within the deleteButtonBitsNotValidTrans
	// will set the lastState.button to zero

	DEBUG_CODE(cout << "validTransBits              :"; COUT_BINARY(validTransBits); cout << endl;);
	DEBUG_CODE(cout << "set to 0 buttonbits         :"; COUT_BINARY(deleteButtonBitsNotValidTrans); cout << endl;);

	deleteButtonBitsNotValidTrans = ~deleteButtonBitsNotValidTrans;
	lastState.button &= deleteButtonBitsNotValidTrans;
	DEBUG_CODE(cout << "buttonbits after no VT      :"; COUT_BINARY(lastState.button); cout << endl;);

	DEBUG_CODE(cout << "validTransSetButtonBit      :"; COUT_BINARY(validTransSetButtonBit); cout << endl;);
	
	lastState.button |= validTransSetButtonMask;
	DEBUG_CODE(cout << "set buttonbits on VT        :"; COUT_BINARY(validTransSetButtonMask); cout << endl;);
	DEBUG_CODE(cout << "buttonbits  after VT        :"; COUT_BINARY(lastState.button); cout << endl;);

	lastState.button ^= invert;
	DEBUG_CODE(cout << "invert with                 :"; COUT_BINARY(invert); cout << endl;);
	DEBUG_CODE(cout << "buttonbits after invert     :"; COUT_BINARY(lastState.button); cout << endl;);

	lastState.button &= buttonmask;
	DEBUG_CODE(cout << "buttonmask                  :"; COUT_BINARY(buttonmask); cout << endl;);
	DEBUG_CODE(cout << "buttonbits after buttonmask :"; COUT_BINARY(lastState.button); cout << endl;);

	
	for (i=0; i<8; i++){
		// simple stuff
		// get current bit (0 / 1) and move it to new position
		buttonout |= ((lastState.button>>i) & 1) << buttonmap[i];
		
	    // complicated stuff:
		// mask current button, shift into bit 0, shift into target position and OR into output
		//buttonout = buttonout | (((lastState.button & buttonbit) >> i) << buttonmap[i]);
		// cool ;)
		// buttonbit = buttonbit << 1;
	}

	lastState.button = buttonout;

	DEBUG_CODE(cout << "buttonbits after buttonmap  :"; COUT_BINARY(lastState.button); cout << endl;);

    updateObservers( lastState );
}

} // namespace ot
