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
//
// Debug helper
//
// if DEBUG is defined the printfD function acts like a ordinary printf, 
// otherwise no output is generated (it is made a comment)

#ifndef _DEBUG_H_
#define _DEBUG_H_

#undef printMsg

#ifdef DEBUG_ON
#define printMsg fprintf
#else
#define printMsg //
#endif

#undef printRes

#ifdef DEBUG_ON
#define printRes fprintf
#else
#define printRes //
#endif

#undef printWin

#ifdef DEBUG_ON
#define printWin fprintf
#else
#define printWin //
#endif

#undef printApp

#ifdef DEBUG_ON
#define printApp fprintf
#else
#define printApp //
#endif

#endif//_DEBUG_H_
