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
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1090 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** Configuration include file for OpenTracker
  * 
  * This file sets defines that control the compilation of device drivers in
  * OpenTracker. Some device drivers require additional libraries and might
  * not be available on all platforms. These source files are bracketed by
  * conditional compile clauses to only be compiled in, if desired. Edit this
  * file to set your configuration. This should only be changed, if you
  * recompile the library, as the include files that you use for other
  * projects will also require this file.
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/Attic/config.h,v 1.1 2001/04/04 09:19:45 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/** uncomment the following line to compile support for the ARToolKit library */
// #define USE_ARTOOLKIT 1

/** uncomment the following line to compile support for CyberMouse device.
  * Support for this is only available on windows. */
// #define USE_CYBERMOUSE 1

/** uncomment the following line to compile support for InterTrax2 inertial tracker*/
// #define USE_INTERTRAX 1

/** uncomment the following line to compile support for Wacom Graphire tablet.
  * Support for this is only available on windows. */
// #define USE_WACOMGRAPHIRE 1