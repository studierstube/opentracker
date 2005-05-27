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
/** dbg - implementation
  *
  * @author  Markus Krutz, Gerd Hesina
  *
  * $Id: dbg.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <cstdio>
#include <cstdarg>
#include <cstdlib>

#include "dbg.h"

//#define DEBUGOUT_ENABLED
//#define DEBUGOUT_WINDOW

FILE *debug_fp = NULL;

#ifdef DEBUGOUT_ENABLED
#ifdef DEBUGOUT_WINDOW
int dbgout(const char *fmt, ...) {
	if (debug_fp == NULL) {
		debug_fp = fopen("resources.dat","wb");
		char command[1000];
		sprintf(command,"xterm -display marille:0.0 -title \"Res-Manager\" -e debug/debugout resources.dat &\n");
		system(command);
	}
	va_list ap;
	int rc;
	
	va_start(ap, fmt);
	rc = vfprintf(debug_fp, fmt, ap);
	va_end(ap);
	
	return rc;
}
#else
int dbgout(const char *fmt, ...) {
	va_list ap;
	int rc;
	
	va_start(ap, fmt);
	rc = vprintf(fmt, ap);
	va_end(ap);
	
	return rc;
}
#endif
#else
int dbgout(const char *, ...) {
	return 0;
}
#endif
