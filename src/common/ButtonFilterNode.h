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
/** header file for ButtonFilter Node.
  *
  * @author Flo Ledermann flo@subnet.at
  * 
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section buttonfilternode ButtonFilter
 * The Button Filter allows the user to filter and re-map the individual buttons of an event.
 *
 * WATCH OUT !! Button bits are leastsignificant first invert (10000000) with buttonin(00000000) will result in (00000001) !!
 * sequence :
 * validtrans, radiobuttons, invertstr, buttonmask, buttonmap 
 *
 * The element has the following attributes :
 * 
 * @li @c validtrans (8bit) specify valid transmission bit for radio pen     (1_2_____) means radio pen 1 has VT on bit 0, radio pen 2 has VT on bit 2  
 * @li @c radiobuttons (8bit) specify bits for radio pen (ref to validtrans) (_1_22___) means bit 1 is for radio-pen 1, bit 3&4  is for radio-pen 2 (has two buttons)
 * @li @c buttonmask binary mask to enable the individual buttons, e.g. "11110000" enables buttons 0-3
 * @li @c buttonmap 8-digit string, assigning an output number to every individual button input, e.g.
 * "01324567" swaps buttons 2 and 3. Multiple assignments ar ORed together, e.g "00001111" means button 
 * 0=true if one (or more) of buttons 0-3 is pressed, button 1=true if one of buttons 4-7 is pressed.
 * @li @c invert binary mask for inverting individual buttons. 0 stands for leave alone, 1 for invert. 
 *        this is applied before the button mapping !
 *
 * An example element looks like this :
 * @verbatim
<ButtonFilter buttonmask="11110000" buttonmap="32107777" invert="00100000">
    <ParButtonSource dev="0x378" DEF="buttons"/>
</ButtonFilter>@endverbatim
 * outputs buttons 0-3 in reverse order, ignoring the others. (note that button 7 is never TRUE because buttons
 * 4-7 are masked out.
 */

#ifndef _BUTTONFILTERNODE_H
#define _BUTTONFILTERNODE_H

#include "../OpenTracker.h"

/**
 * The Button Filter allows the user to filter and re-map the individual buttons of an event.
 * @author Flo Ledermann flo@subnet.at
 * @author Thomas Psik tomp@ims.tuwien.ac.at
 *
 *
 * ButtonFilter description
 * WATCH OUT !! Button bits are leastsignificant first invert (10000000) with buttonin(00000000) will result in (00000001) !!
 * sequence :
 * validtrans, radiobuttons, invertstr, buttonmask, buttonmap 
 * 
 * validtrans (8bit) specify valid transmission bit for radio pen     (-----2-1) means radio pen 1 has VT on bit 0, radio pen 2 has VT on bit 2  
 * radiobuttons (8bit) specify bits for radio pen (ref to validtrans) (---22-1-) means bit 1 is for radiopen 1, bit 3&4  is for radiopen 2 (has two buttons)
 * buttonmask (8bit) set bit to 0 if you want to disable the button (00000010) will enable only button 1
 * buttonmap  (8bit) rearrange buttons (01234567) is identity mapping (02134567) will change order of button 2 and 1
 * invertstr  (8bit) invert single button (00000010) button 1 is pressed if value reads 0
 * @ingroup common
 */

namespace ot {

class OPENTRACKER_API ButtonFilterNode
    : public Node
{
// Members
protected:
    /// last saved state variable
    State lastState;

	// supply 8 buttons
	signed char validtrans[8];  
	signed char radiobuttons[8];
	signed char validTransSetButton[8];
	unsigned char buttonmap[8];
	unsigned char buttonmask;
    unsigned char invert;

// Methods
protected:

    /** constructor method
     */
    ButtonFilterNode( const char* buttonmask, const char* buttonmap, const char * invertstr, const char * validtrans , const char * radiobuttons, const char *setValidTrans);

public:

    /** tests for EventGenerator interface being present. Returns the
     * result of the childs implementation of this method.
     * @return 1 if child implements EventGenerator, 0 otherwise */
    virtual int isEventGenerator() ;

    /**
     * this method is called by the EventGenerator to update it's observers.
     */
    virtual void onEventGenerated( State& event, Node& generator);

    friend class CommonNodeFactory;
};

} // namespace ot

#endif
