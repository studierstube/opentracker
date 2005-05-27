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
/** StbBuffer implementation
  *
  * $Id: StbBuffer.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <stbapi/util/StbBuffer.h>
#include <stbapi/util/ivio.h>

StbBuffer::StbBuffer(ulong initSize)
: CBuffer( initSize )
{
}

StbBuffer::StbBuffer(CBuffer *initBuffer)
: CBuffer( initBuffer )
{
}

StbBuffer::StbBuffer(void *data, ulong dLen, char mode)
: CBuffer( data, dLen, mode )
{
}

StbBuffer::~StbBuffer()
{
}

void StbBuffer::addNode( SoNode * node, SbBool binary, SbBool withImages )
{
    currentUsedSize = writeBuffer(node, &buffer, &currentAllocedSize, currentUsedSize, binary, withImages );
    writePointer = currentUsedSize;
}

SoNode * StbBuffer::getNode()
{
    SoNode * result = readNodeFromMemory((const char *) buffer + readPointer,currentUsedSize-readPointer);
    readPointer = currentUsedSize;
    return result;
}
