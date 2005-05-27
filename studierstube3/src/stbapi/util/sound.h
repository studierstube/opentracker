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
/** simple sound routine header file
  *
  * @author Hannes Kaufmann, Jan Prikryl
  *
  * $Id: sound.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/SbString.h>
#include <stbapi/stbapi.h>

#ifdef _SGI_SOURCE
#include <audio.h>
#endif 

/**
 Sound output routine for the Studierstube. 
 Calls the external program playaiff on the SGI to play the given sound file.
 Under Win32 the built in PlaySound routine is being called.
 It does not check if there is an available sound port (there are only 4).
 The whole soundport check and the audio routines crash the Workspace!
 Therefore I just call the playaiff routine in a brutal way, no matter
 if sound ports are available or not. I just assume that they are available.
 If SOUND is not defined, then you simply won't hear sound.
 sound files must be available in WAV (for windows) and AIFF (for SGI) format

 IMPORTANT NOTE: to play sound under Win32, you must include the
 winmm.lib into your project (go to project settings->all configurations
 ->link->general->object/library modules and enter winmm.lib

@param filePath the path and name of the file to play
*/
void sound(SbString filePath);

