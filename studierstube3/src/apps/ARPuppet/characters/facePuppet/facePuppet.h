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
/* Header file defines the entry point for the application DLL.
 *
 * @author Istvan Barakonyi
 *
 * $Id: facePuppet.h 3426 2004-07-14 12:11:27Z tamer $
 * @file
 * 
/* ======================================================================= */

#ifdef WIN32
#ifdef FACEPUPPET_EXPORTS
#define FACEPUPPET_API __declspec(dllexport)
#else
#define FACEPUPPET_API __declspec(dllimport)
#endif
#else
#define FACEPUPPET_API
#endif
