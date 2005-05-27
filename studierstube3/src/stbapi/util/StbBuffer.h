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
/** StbBuffer header file
  * 
  * $Id: StbBuffer.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _STBBUFFER_H_
#define _STBBUFFER_H_
#include <stbapi/util/buffer.h>
#include <Inventor/SbBasic.h>

class SoNode;

/**
 * extended version of CBuffer to include methods to write and read node
 * data. This avoids a lot of copying and memory management.
 * @ingroup div
 * @ingroup sman
 * @author Gerhard Reitmayr
 */
class STBAPI_API StbBuffer : public CBuffer
{
  public:
    StbBuffer(ulong initSize=CBUFFER_DEFAULT_INITSIZE);
    StbBuffer(CBuffer *initBuffer);
    StbBuffer(void *data, ulong dLen, char mode=CBUFFER_STATIC);
    virtual ~StbBuffer();
    void addNode( SoNode *, SbBool binary = TRUE, SbBool withImages = TRUE );
    SoNode * getNode();    
};
#endif
