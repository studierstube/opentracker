/* ========================================================================
 * Copyright (c) 2006,
 * Institute for Computer Graphics and Vision
 * Graz University of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Graz University of Technology nor the names of
 * its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

//using namespace std;

// emtpy string to be returned, if key is not in the map

const std::string empty("");

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

    const std::string & StringTable::get( const std::string & key )
    {
        StringMap::iterator it = map.find( key );
        if( it == map.end())
            return empty;
        return (*it).second;
    }

    // stores a key, value pair

    void StringTable::put( const std::string & key, const std::string & value )
    {
        map[key] = value;
    }

    // removes a key, value pair

    void StringTable::remove( const std::string & key )
    {
        StringMap::iterator it = map.find( key );
        if( it == map.end())
            return;
        map.erase( it );
    }

    // tests for a given key

    int StringTable::containsKey( const std::string & key )
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

    void StringTable::put(const std::string & key, const int value)
    {
        char buffer[20];

        sprintf( buffer, "%i", value );
        map[key] = buffer;
    }

    void StringTable::put(const std::string & key, const float value)
    {
        char buffer[20];

        sprintf( buffer, "%f", value );
        map[key] = buffer;
    }

    void StringTable::put(const std::string & key, const double value)
    {
        char buffer[30];

        sprintf( buffer, "%lf", value );
        map[key] = buffer;
    }

    void StringTable::put(const std::string & key, const int * value, int len)
    {
        char buffer[20];
        std::string strvalue;

        sprintf(buffer, "%i", value[0] );
        strvalue.append(buffer);
        for( int i = 1; i < len; i++ )
        {
            sprintf(buffer, " %i", value[i] );
            strvalue.append(buffer);
        }
        map[key] = strvalue;
    }

    void StringTable::put(const std::string & key, const float * value, int len)
    {
        char buffer[20];
        std::string strvalue;

        sprintf(buffer, "%f", value[0] );
        strvalue.append(buffer);
        for( int i = 1; i < len; i++ )
        {
            sprintf(buffer, " %f", value[i] );
            strvalue.append(buffer);
        }
        map[key] = strvalue;
    }

    void StringTable::put(const std::string & key, const double * value, int len)
    {
        char buffer[20];
        std::string strvalue;

        sprintf(buffer, "%lf", value[0] );
        strvalue.append(buffer);
        for( int i = 1; i < len; i++ )
        {
            sprintf(buffer, " %lf", value[i] );
            strvalue.append(buffer);
        }
        map[key] = strvalue;
    }

    int StringTable::get(const std::string & key, int * value, int len )
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

    int StringTable::get(const std::string & key, float * value, int len )
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

    int StringTable::get(const std::string & key, std::vector<float> & vector, int len )
    {
        StringMap::iterator it = map.find( key );
        if( it == map.end())
            return 0;

        float *array = (float*)malloc(len * sizeof(float));
        int count = get(key, array, len);
        copyA2V(array, len, vector);
        free(array);
        return count;
    }

    int StringTable::get(const std::string & key, double * value, int len )
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

    const std::string & KeyIterator::nextElement()
    {
        if( hasMoreKeys()){
            const std::string & res = (*it).first;
            it++;
            return res;
        }
        return empty;
    }

} // namespace ot

/* 
 * ------------------------------------------------------------
 *   End of StringTable.cxx
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
