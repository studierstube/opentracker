/******************************************************************************
*									      *
*	File:     rtcm.h						      *
*	Author:   Wolfgang Rupprecht <wolfgang@capsicum.wsrcc.com>	      *
*	Created:  Thu Feb  4 14:17:29 PST 1999				      *
*	Contents: forward decls for John Sagers rtcm decoder                  *
*									      *
*	Copyright (c) 1999 Wolfgang Rupprecht.				      *
*	All rights reserved.						      *
*									      *
*	$Id: rtcm.h,v 1.1 2003/03/27 18:26:02 reitmayr Exp $								      *
******************************************************************************/

#include <ace/OS.h>

void            initialise(void);
int             preamble(void);
void            state_change(u_int s);
u_int           parity_ok(void);
int             find_sync(u_char b);
void            next_word(void);
int             filled_word(u_char b);
void            printcor(void);
void            printref(void);
void            printba(void);
void            printspec(void);
void            printnull(void);
void            printconh(void);
void            new_frame(void);
void            buffer(u_int w);
void            frame_start(void);
void            find_start(void);
void            fill_frame(void);
void            status_byte(u_char b);
void            data_byte(u_char b);
void            new_byte(u_char b);
void            printdatum(void);

/* end */
