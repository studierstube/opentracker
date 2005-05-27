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
//  PROJECT:    Studierstube
//  CONTENT:    window related defines for workspace (non-public)
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    hw      Hermann Wurnig
// ===========================================================================
//  HISTORY:
//
//  11-dec-98 10:00:00  hw      last modification
//  11-dec-98 10:00:00  hw      created
// ===========================================================================
#ifndef __WINDEF_H__
#define __WINDEF_H__
 
#include <Inventor/SbLinear.h>
#include <Inventor/SbString.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoTransform.h>

#include "appdef.h"

// MSG-NUMBERS btw. 1000 and 2000 are reserved for applications

typedef struct _winDragDropMessage
{
	int	type;
	int userID;
	int releaseEvent;
	void *data;
	SoNode *repr;
	SoTransform *penXf;
	SbMatrix objectXfMatrix;

} winDragDropMessage;


// message number definitions

//#define USER_HAS_DROPPED_DATA_INTO_CLIPBOARD 402			 
										 // rec: window-manager
										 // data: winDropDataMessage
#define REMOVE_PEN_DRAG_GEOMETRY 403	 // called after d&d stopped
										 // data: userID

//#define REMOVE_OBJECT_FROM_CLIPBOARD 404 // called after obj taken
										 // data: separator nodeID


#define RM_CHANGE_WCS_FOR_USER		500	// rec: -10, 
										// data: resWCSChangeMessage

#define WM_START_DRAG_DROP_ACTION	301 // rec: -20, data: winDropMessage
#define WM_CLOSE_WINDOW				302 // rec: -20, data: window


#endif
