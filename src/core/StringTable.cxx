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
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for StringTable class.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

#include "../tool/disable4786.h"
#include "StringTable.h"
#include <stdlib.h>
#include <stdio.h>

using namespace std;

// emtpy string to be returned, if key is not in the map

const string empty("");

// initializes the map

namespace ot {

StringTable::StringTable() : map()
{}
  
// copy constructor
StringTable::StringTable(const StringTable& table ) : map(table.map)
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

// some put and get methods

void StringTable::put(const string & key, const int value)
{
    char buffer[20];
    
    sprintf( buffer, "%i", value );
    map[key] = buffer;
}

void StringTable::put(const string & key, const float value)
{
    char buffer[20];
    
    sprintf( buffer, "%f", value );
    map[key] = buffer;
}

void StringTable::put(const string & key, const double value)
{
    char buffer[30];
    
    sprintf( buffer, "%lf", value );
    map[key] = buffer;
}

void StringTable::put(const string & key, const int * value, int len)
{
    char buffer[20];
    string strvalue;
    
    sprintf(buffer, "%i", value[0] );
    strvalue.append(buffer);
    for( int i = 1; i < len; i++ )
    {
        sprintf(buffer, " %i", value[i] );
        strvalue.append(buffer);
    }
    map[key] = strvalue;
}

void StringTable::put(const string & key, const float * value, int len)
{
    char buffer[20];
    string strvalue;
    
    sprintf(buffer, "%f", value[0] );
    strvalue.append(buffer);
    for( int i = 1; i < len; i++ )
    {
        sprintf(buffer, " %f", value[i] );
        strvalue.append(buffer);
    }
    map[key] = strvalue;
}

void StringTable::put(const string & key, const double * value, int len)
{
    char buffer[20];
    string strvalue;
    
    sprintf(buffer, "%lf", value[0] );
    strvalue.append(buffer);
    for( int i = 1; i < len; i++ )
    {
        sprintf(buffer, " %lf", value[i] );
        strvalue.append(buffer);
    }
    map[key] = strvalue;
}

int StringTable::get(const string & key, int * value, int len )
{
    StringMap::iterator it = map.find( key );
    if( it == map.end())
        return 0;    
        
    char * data = (char *)(*it).second.c_str();
    char * end = data;
    int count = 0;
    value[count++] = strtol( data, &end, 0 );    
    while( end != data && count < len){        
        data = end;
        value[count++] = strtol( data, &end, 0 );
    }
    return count;
}

int StringTable::get(const string & key, float * value, int len )
{
    StringMap::iterator it = map.find( key );
    if( it == map.end())
        return 0;
        
    char * data = (char *)(*it).second.c_str();
    char * end = data;
    int count = 0;
    value[count++] = (float)strtod( data, &end );    
    while( end != data && count < len){        
        data = end;
        value[count++] = (float)strtod( data, &end );
    }
    return count;
}

int StringTable::get(const string & key, double * value, int len )
{
    StringMap::iterator it = map.find( key );
    if( it == map.end())
        return 0;
        
    char * data = (char *)(*it).second.c_str();
    char * end = data;
    int count = 0;
    value[count++] = strtod( data, &end );    
    while( end != data && count < len){        
        data = end;
        value[count++] = strtod( data, &end );
    }
    return count;        
}

int KeyIterator::hasMoreKeys() const
{
    return((int)(it != map.end())); 
}

const string & KeyIterator::nextElement()
{
    if( hasMoreKeys()){
        const string & res = (*it).first;
        it++;
        return res;
    }
    return empty;
}

} // namespace ot
