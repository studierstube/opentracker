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
/** CBuffer implementation
  * 
  * $Id: buffer.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <cstdio>
#include <cstring>

#include "buffer.h"

CBuffer::CBuffer(ulong initSize)
{
  // alloc buffer with initSize bytes
  buffer=malloc(initSize);
  currentAllocedSize=initSize;
  currentUsedSize=0;
  readPointer=0;
  writePointer=0;
  bufferMode=CBUFFER_DYNAMIC;
  compressionInfo=0;
}

CBuffer::CBuffer(CBuffer *initBuffer)
{
  currentAllocedSize=initBuffer->getSize();
  buffer=malloc(currentAllocedSize);
  memcpy(buffer,initBuffer->getBuffer(),currentAllocedSize);
  currentUsedSize=currentAllocedSize;
  readPointer=0;
  writePointer=0;
  bufferMode=CBUFFER_DYNAMIC;
  compressionInfo=0;
}

CBuffer::CBuffer(void *data, ulong dLen, char mode)
{
  buffer=data;
  currentUsedSize=currentAllocedSize=dLen;
  readPointer=0;
  writePointer=0;
  bufferMode=mode;
  compressionInfo=0;
}

void CBuffer::setBuffer(void *data, ulong dLen, char mode)
{
  buffer=data;
  currentUsedSize=currentAllocedSize=dLen;
  readPointer=0;
  writePointer=0;
  bufferMode=mode;
  compressionInfo=0;
}

void CBuffer::resetBuffer()
{
    currentUsedSize=0;
    readPointer=0;
    writePointer=0;
    compressionInfo=0;
}

CBuffer::~CBuffer()
{
  if((buffer)&&(bufferMode==CBUFFER_DYNAMIC))
      free(buffer);
}

void CBuffer::addInt32(int32 theInt32)
{
  int32 help32[1];
  ulong requiredSize=currentUsedSize+sizeof(int32);
  help32[0]=htonl(theInt32);
  if(requiredSize>currentAllocedSize)
  {
    // we have to realloc now.
    buffer=enlargeBuffer(requiredSize);
  }
  currentUsedSize+=sizeof(int32);
  memcpy((char*)buffer+writePointer, (void*)help32, sizeof(int32));
  writePointer+=sizeof(int32);
}

int32 CBuffer::getInt32()
{
  int32 help32[1];
  
  if(((readPointer+sizeof(int32))>currentAllocedSize)||
     ((readPointer+sizeof(int32))>currentUsedSize)
    )
  {
    // tried to read over the buffer bound or currentUsedSize
    return 0;
  }
  memcpy((void*)help32, (char*)buffer+readPointer, sizeof(int32));
  readPointer+=sizeof(int32);
  return(ntohl(help32[0]));
}

void CBuffer::addShort(short theShort)
{
  short help[1];
  ulong requiredSize=currentUsedSize+sizeof(short);
  help[0]=htons(theShort);
  if(requiredSize>currentAllocedSize)
  {
    // we have to realloc now.
    buffer=enlargeBuffer(requiredSize);
  }
  currentUsedSize+=sizeof(short);
  memcpy((char*)buffer+writePointer, (void*)help, sizeof(short));
  writePointer+=sizeof(short);
}

short CBuffer::getShort()
{
  short help[1];
  
  if(((readPointer+sizeof(short))>currentAllocedSize)||
     ((readPointer+sizeof(short))>currentUsedSize)
    )
  {
    // tried to read over the buffer bound or currentUsedSize
    return 0;
  }
  memcpy((void*)help, (char*)buffer+readPointer, sizeof(short));
  readPointer+=sizeof(short);
  return(ntohs(help[0]));
}



void CBuffer::addChar(char theChar)
{
  char help[1];
  ulong requiredSize=currentUsedSize+sizeof(char);

  help[0]=theChar;
  if(requiredSize>currentAllocedSize)
  {
    // we have to realloc now.
    buffer=enlargeBuffer(requiredSize);
  }
  currentUsedSize+=sizeof(char);
  memcpy((char*)buffer+writePointer, (void*)help, sizeof(char));
  writePointer+=sizeof(char);
}

char CBuffer::getChar()
{
  char help[1];
  if(((readPointer+sizeof(char))>currentAllocedSize)||
     ((readPointer+sizeof(char))>currentUsedSize)
    )
  {
    // tried to read over the buffer bound or currentUsedSize
    return 0;
  }
  memcpy((void*)help, (char*)buffer+readPointer, sizeof(char));
  readPointer+=sizeof(char);
  return(help[0]);
}

void CBuffer::addBytes(ulong dLen, void *data)
{
  ulong requiredSize=currentUsedSize+dLen;

  if(requiredSize>currentAllocedSize)
  {
    // we have to realloc now.
    buffer=enlargeBuffer(requiredSize);
  }
  currentUsedSize+=dLen;
  memcpy((char*)buffer+writePointer, data, dLen);
  writePointer+=dLen;
}

void CBuffer::addString(char *data)
{
  ulong dLen = strlen(data) + 1;
  ulong requiredSize=currentUsedSize+dLen;

  if(requiredSize>currentAllocedSize)
  {
    // we have to realloc now.
    buffer=enlargeBuffer(requiredSize);
  }
  currentUsedSize+=dLen;
  memcpy((char*)buffer+writePointer, data, dLen);
  writePointer+=dLen;
}

void *CBuffer::getBytes(ulong dLen)
{
  void *help;
  if(((readPointer+dLen)>currentAllocedSize)||
     ((readPointer+dLen)>currentUsedSize)
    )
  {
    // tried to read over the buffer bound or currentUsedSize
    return 0;
  }
  help=(void*)((char*)buffer+readPointer);
  readPointer+=dLen;
  return(help);
}

void CBuffer::getRemainingBytes(void *&remainingBuffer, ulong &remainingBytes)
{
  remainingBuffer=(void*)((char*)buffer+readPointer);
  remainingBytes=currentUsedSize-readPointer;
  readPointer=currentUsedSize;
}

char *CBuffer::getString()
{
  char *help;
  int dLen=strlen((char*)((char*)buffer+readPointer))+1;

  if(((readPointer+dLen)>currentAllocedSize)||
     ((readPointer+dLen)>currentUsedSize)
    )
  {
    // tried to read over the buffer bound or currentUsedSize
    return 0;
  }
  help=(char*)((char*)buffer+readPointer);
  readPointer+=dLen;
  return(help);
}


void CBuffer::addULong(ulong theULong)
{
  ulong help[1];
  ulong requiredSize=currentUsedSize+sizeof(ulong);

  help[0]=htonl(theULong);
  if(requiredSize>currentAllocedSize)
  {
    // we have to realloc now.
    buffer=enlargeBuffer(requiredSize);
  }
  currentUsedSize+=sizeof(ulong);
  memcpy((char*)buffer+writePointer, (void*)help, sizeof(ulong));
  writePointer+=sizeof(ulong);
}

ulong CBuffer::getULong()
{
  ulong help[1];
  if(((readPointer+sizeof(ulong))>currentAllocedSize)||
     ((readPointer+sizeof(ulong))>currentUsedSize)
    )
  {
    // tried to read over the buffer bound or currentUsedSize
    return 0;
  }
  memcpy((void*)help, (char*)buffer+readPointer, sizeof(ulong));
  readPointer+=sizeof(ulong);
  return(ntohl(help[0]));
}

void CBuffer::addFloat(float theFloat)
{
  ulong requiredSize=currentUsedSize+4;
  union 
  { 
      float f;
      long int l;
  } convert;
  convert.f=theFloat;
  convert.l=htonl(convert.l);

  if(requiredSize>currentAllocedSize)
  {
    // we have to realloc now.
    buffer=enlargeBuffer(requiredSize);
  }
  currentUsedSize+=4;
  memcpy((char*)buffer+writePointer, (void*)&convert, 4);
  writePointer+=4;
}

float CBuffer::getFloat()
{
  float theFloat;

  if(((readPointer+4)>currentAllocedSize)||
     ((readPointer+4)>currentUsedSize)
    )
  {
    // tried to read over the buffer bound or currentUsedSize
    return 0.0f;
  }
  memcpy((void*)&theFloat, (char*)buffer+readPointer, sizeof(float));
  union 
  { 
      float f;
      long int l;
  } convert;
  convert.f=theFloat;
  convert.l=ntohl(convert.l);
  readPointer+=sizeof(float);
  return convert.f;
}

void *CBuffer::enlargeBuffer(ulong reqSize)
{
  if(reqSize>currentAllocedSize*2)
    currentAllocedSize=reqSize*2;
  else
    currentAllocedSize*=2;
  return(realloc((void*)buffer, currentAllocedSize));
}

void CBuffer::setCompressionInfo(ulong info)
{
	compressionInfo=info;
}

ulong CBuffer::getCompressionInfo()
{
    return compressionInfo;
}

void *CBuffer::getBuffer()
{
    return buffer;
}

ulong CBuffer::getSize()
{
    return currentUsedSize;
}
