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
/** CBuffer header file
  * 
  * $Id: buffer.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _CBUFFER_H_
#define _CBUFFER_H_
#include <cstdlib>
#include <sys/types.h>
#include <stbapi/stbapi.h>
#ifndef WIN32
#include <netinet/in.h>
#else
#include <winsock.h>
#endif
#define CBUFFER_DEFAULT_INITSIZE 2048

typedef unsigned long ulong;
typedef long int32;

/**
 * Utility class to serialze various data in to a buffer. It is used extensively 
 * by various network parts of Studierstube.
 * @ingroup div
 * @ingroup sman
 * @author Gerd Hesina
 */
class STBAPI_API CBuffer
{
  public:
    CBuffer(ulong initSize=CBUFFER_DEFAULT_INITSIZE);
    CBuffer(CBuffer *initBuffer);
    CBuffer(void *data, ulong dLen, char mode=CBUFFER_STATIC);
	void setBuffer(void *data, ulong dLen, char mode=CBUFFER_STATIC);
    void setCompressionInfo(ulong info);
    ulong getCompressionInfo();
    virtual ~CBuffer();
    void addInt32(int32 theInt32);
    int32 getInt32();
    void addShort(short theShort);
    short getShort();
    void addChar(char theChar);
    char getChar();
    void addBytes(ulong dLen, void *data);
    void *getBytes(ulong dLen);
    void getRemainingBytes(void *&remainingBuffer, ulong &remainingBytes);
    void addString(char *data);
    char *getString();
    void addULong(ulong theULong);
    ulong getULong();
    void addFloat(float theFloat);
    float getFloat();
    void *getBuffer();
    ulong getSize();
    void resetBuffer();
    enum {
        CBUFFER_STATIC,
        CBUFFER_DYNAMIC
    };
  protected:
    void *buffer;
    size_t readPointer;
    size_t writePointer;
    size_t currentAllocedSize;
    size_t currentUsedSize;
    void *enlargeBuffer(ulong reqSize);
    char bufferMode;
	ulong compressionInfo;
};
#endif
