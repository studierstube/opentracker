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
/** header file for StringTable class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/StringTable.h,v 1.2 2001/04/18 16:38:18 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _STRINGTABLE_H
#define _STRINGTABLE_H

#include "../dllinclude.h"

#include<string>
#include<map>

using namespace std;

typedef map<string,string> StringMap;


/**
 * This class implements a string to string mapping similar to a Hashtable in
 * Java. It is used to work around a problem in VC++ with passing STL maps 
 * between methods in different dlls. This way it cleans up the API, because
 * now all dependencies on the STL are removed from the OpenTracker API.
 * @author Gerhard Reitmayr
 * @ingroup core
 */ 
class OPENTRACKER_API StringTable {
protected:
    /// the underlying STL map implementing the functionality
    StringMap map;
public:
    /** constructor */
    StringTable();
    /** destructor */
    ~StringTable();
    /** returns a value stored in the map
     * @param key the key the value is for 
     * @returns the string value or an empty string, if the key is not found */
    const string & get( const string & key );
    /** stores a key value pair in the table, overwritting a possible prior 
     * value
     * @param key the key to store it under
     * @param value the value to store */
    void put( const string &key, const string &value );
    /**
     * removes a key value pair from the table
     * @param key the key to the pair */
    void remove( const string &key );
    /**
     * tests, whether the table contains a certain key
     * @param the key to test for */
    int containsKey( const string & );
    /** returns the number of pairs stored 
     * @returns unsigned with the number of pairs */
    unsigned size();
};

#endif
