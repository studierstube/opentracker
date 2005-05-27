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
/**
 * appdef.h header file
 *
 * @author Hermann Wurnig
 *
 * $Id: appdef.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef __APPDEF_H__
#define __APPDEF_H__

#include <Inventor/nodes/SoTransform.h>

class SoWindowKit;
class So3DEvent;
class SoHandle3DEventAction;

typedef SbBool WindowMessageCB(void *, int, SoWindowKit *, int userID, 
							   So3DEvent *, SoPath *);


#define BROADCAST_ADDR			-99


typedef struct
{
	int message;
	int receiver;
	void *data;
	int deliveryCount;
} deliveryRecord;


typedef struct _winDropDataMessage
{
	int	type;
	int userID;
	void *data;
	SbVec3f pos;
	SbRotation rotation;
	SoTransform *objectToPenXf;
	SoWindowKit *window;

} winDropDataMessage;

typedef struct _winFocusMessage
{
	int userID;
	SoWindowKit *window;

} winFocusMessage;



#define WM_RESIZE					100 // data: window
#define WM_CLOSEW					101	// data: window
#define WM_MAXIMIZE					104 // data: window
#define WM_NORMALIZE				105 // data: window
#define WM_ICONIFY					106 // data: window
#define WM_WINDOW_GOT_FOCUS 		107 // data: winFocusMessage
#define WM_WINDOW_LOST_FOCUS 		108 // data: winFocusMessage

#define CB_WINDOW_WILL_GET_FOCUS 	109


#define DRAG_MOVE_ACTION_IN_WINDOW  400 // rec: app of window
										// data:
#define USER_HAS_DROPPED_DATA       401	// rec: app of window
										// data: winDropDataMessage

// drag&drop release event definitions

#define DRAG_BUTTON0_UP    0
#define DRAG_BUTTON0_DOWN  1
#define DRAG_BUTTON1_UP    2
#define DRAG_BUTTON1_DOWN  3
#define DRAG_BUTTON2_UP    4
#define DRAG_BUTTON2_DOWN  5
#define DRAG_BUTTON3_UP    6
#define DRAG_BUTTON3_DOWN  7
#define DRAG_BUTTON4_UP    8
#define DRAG_BUTTON4_DOWN  9
#define DRAG_BUTTON5_UP    10
#define DRAG_BUTTON5_DOWN  11
#define DRAG_BUTTON6_UP    12
#define DRAG_BUTTON6_DOWN  13
#define DRAG_BUTTON7_UP    14
#define DRAG_BUTTON7_DOWN  15

#define DRAG_PRIMARY_BUTTON_UP 		0
#define DRAG_PRIMARY_BUTTON_DOWN 	1
#define DRAG_SECONDARY_BUTTON_UP 	2
#define DRAG_SECONDARY_BUTTON_DOWN 	3

#endif
