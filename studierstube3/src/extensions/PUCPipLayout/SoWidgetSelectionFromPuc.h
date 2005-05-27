/* ========================================================================
  * Copyright (C) 2000  Vienna University of Technology
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
  * For further information please contact Dieter Schmalstieg under
  * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: Studierstube
  * ======================================================================== */

 /* ======================================================================= */
/** Header file for API definitions of SoWidgetSelectionFromPuc
  *
  * @author Valerie Maquil
  *
  * $Id: SoWidgetSelectionFromPuc.h 3565 2004-08-02 14:47:20Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef SoWidgetSelectionFromPuc_H
#define SoWidgetSelectionFromPuc_H

#ifdef _WIN32
#include <SoWinEnterScope.h>
#endif

#include "PucPipLayout.h"

#include <Inventor/fields/SoMFString.h>

#include <stbapi/interaction/SoBehaviorKit.h>

#include <extensions/PUC/SoPucCommand.h>
#include <extensions/PUC/SoPucStateBool.h>
#include <extensions/PUC/SoPucStateInt.h>
#include <extensions/PUC/SoPucStateFixed.h>
#include <extensions/PUC/SoPucStateFloat.h>
#include <extensions/PUC/SoPucStateEnumerated.h>
#include <extensions/PUC/SoPucStateString.h>

#ifdef _WIN32
#include <SoWinLeaveScope.h>
#endif

#define NO_LABEL 0
#define SMALL_LABEL 1
#define LONG_LABEL 2

#include <string>

/**
 * implements static methods transforming a puc state into a widget and setting all the connection between them
 * 
 * @ingroup puc
 */
class PUC_PIP_LAYOUT_API SoWidgetSelectionFromPuc
{


public:
	///constructor
    SoWidgetSelectionFromPuc();
	///static method which commutes a SoPucCommand into a SoPushButton and chooses the label with the specified size
	static SoBehaviorKit *getPushButtonForCommand(SoPucCommand *state, int size);
	///static method which commutes a SoPucStateBool into a SoToggleButton  and chooses the label with the specified size
	static SoBehaviorKit *getToggleButtonForStateBool(SoPucStateBool *state, int size);

	///static method which commutes a SoPucStateInt into a SoIncrementalSlider  and chooses the label with the specified size
	static SoBehaviorKit *getSliderForStateInt(SoPucStateInt *state, int size);
	///static method which commutes a SoPucStateInt into a SoTextListBox  and chooses the label with the specified size
	static SoBehaviorKit *getTextListBoxForStateInt(SoPucStateInt *state, int numCols, int numRows, int navPlace, int size);

	///static method which commutes a SoPucStateFixed into a SoIncrementalSlider and chooses the label with the specified size
	static SoBehaviorKit *getSliderForStateFixed(SoPucStateFixed *state, int size);

	///static method which commutes a SoPucStateString into a SoTextBox and chooses the label with the specified size
	static SoBehaviorKit *getTextBoxForStateString(SoPucStateString *state, int size);

	///static method which commutes a SoPucStateFixed into a SoTextListBox and chooses the label with the specified size
	static SoBehaviorKit *getTextListBoxForStateFixed(SoPucStateFixed *state, int numCols, int numRows, int navPlace, int size);


	///static method which commutes a SoPucStateFloat into a SoIncrementalSlider and chooses the label with the specified size
	static SoBehaviorKit *getSliderForStateFloat(SoPucStateFloat *state, int size);

	///static method which commutes a SoPucStateEnumerated into a SoTextListBox and chooses the label with the specified size
	static SoBehaviorKit *getTextListBoxForStateEnumerated(SoPucStateEnumerated *state, int numCols, int numRows, int navPlace, int size);

	///static method which extracts ativeIf and tooltip information from a puc state
	static void addActiveIfAndTooltip(SoBehaviorKit *widget, SoPucElementBase *state);


protected:
	///destructor
	virtual ~SoWidgetSelectionFromPuc();
	
	///gets from the label list the label with a certain size
	static const SbString &getLabelIndex(const SoMFString &labels, int size);
	
	///used to return empty label string
	static const SbString emptyString;
};

//----------------------------------------------------------------------------

#endif // SoWidgetSelectionFromPuc_H