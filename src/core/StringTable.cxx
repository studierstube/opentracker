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
/** source file for StringTable class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/StringTable.cxx,v 1.1 2001/04/09 07:12:18 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "StringTable.h"

// emtpy string to be returned, if key is not in the map

const string empty("");

// initializes the map

StringTable::StringTable() : map()
{}
  
// clears the map

StringTable::~StringTable()
{
    map.clear();
}

// returns a value to a given key

const string & StringTable::get( const string & key )
{
    StringMap::iterator it = map.find( key );
    if( it == map.end())
        return empty;
    return (*it).second;
}

// stores a key, value pair

void StringTable::put( const string & key, const string & value )
{
    map[key] = value;
}

// removes a key, value pair

void StringTable::remove( const string & key )
{
    StringMap::iterator it = map.find( key );
    if( it == map.end())
        return;
    map.erase( it );
}

// tests for a given key

int StringTable::containsKey( const string & key )
{
    StringMap::iterator it = map.find( key );
    if( it == map.end())
        return 0;
    return 1;
}

// returns the size of the table

unsigned StringTable::size()
{
    return map.size();
}
