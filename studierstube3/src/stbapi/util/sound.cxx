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
/** simple sound routine
  *
  * @author Hannes Kaufmann, Jan Prikryl
  *
  * $Id: sound.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */


#include "sound.h"

#ifdef WIN32
  #include <windows.h>
#endif

void
sound( SbString filePath )
{
#ifdef SOUND
#ifdef WIN32
  // Play sound in Windows environment
  const char *fileName = filePath.getString();
  PlaySound(fileName, NULL, (SND_FILENAME|SND_NODEFAULT|SND_ASYNC|SND_NOSTOP));
#else
#ifdef _SGI_SOURCE
  const char *fileName = filePath.getString();
  systemCommand = (char *) alloca(filePath.getLength() + 100);
  sprintf(systemCommand, "playaiff %s &", fileName);
  system(systemCommand);
#else
  const char *fileName = filePath.getString();
  systemCommand = (char *) alloca(filePath.getLength() + 100);
  sprintf(systemCommand, "play %s &", fileName);
  system(systemCommand);
#endif
#endif
#endif
}

//eof
