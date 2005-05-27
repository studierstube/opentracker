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
// ===========================================================================
//  NAME:       container.h
//  TYPE:       c++ header
//  PROJECT:    Studierstube
//  CONTENT:    Container: generic list/hashtable with iterator
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    ds      Dieter Schmalstieg
// ===========================================================================
//  HISTORY:
//
//  08-aug-97 09:49:41  ds      last modification
//  08-aug-97 09:49:41  ds      created
// ===========================================================================

#ifndef _CONTAINER_H_
#define _CONTAINER_H_

#include <string.h>
#include <stbapi/stbapi.h>

//Container:
//a class to store any kind of data. combination of a list and a hash table
//features are insert+remove in o(1), list iterator
//operates with void pointers for data and key
//subclasses may overload hash function and compare function

typedef unsigned int uint32;

const int defaultHashTableSize = 37;
const int defaultKeyLen = sizeof(int);

typedef void* VoidPTR;

struct Item;

class Container
{
public:
    STBAPI_API Container(int aKeyLen = defaultKeyLen, 
              int aHashTableSize = defaultHashTableSize);
      //keyLen = length of key in byte, if 0 then key taken as string
    VoidPTR STBAPI_API insert(void* data, void* key);//return 0 on ok,data ptr if exists
    VoidPTR STBAPI_API remove(void* key);       //return data ptr if ok, 0 if not exists
    VoidPTR STBAPI_API find(void* key);         //return data ptr or 0 if not exists
    void STBAPI_API reset();                  //reset iterator
    VoidPTR STBAPI_API getNextItem();           //get next item from iterator
    VoidPTR STBAPI_API getFirst();              //reset iterator and get first item
    int STBAPI_API getNumItems();             //number of items in container
    STBAPI_API ~Container();                  //destructor

protected:
    uint32 Crc32(void* key);
    int compare(void* key1, void* key2);
    Item* find(void* key, uint32 crc, int hashVal);

    int hashTableSize;
    int keyLen;
    Item** hash;
    Item* iter;
    int iterHashVal;
    int count;
    static const uint32 crcTable[];
};//Container

#endif//_CONTAINER_H_

