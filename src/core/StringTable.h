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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/StringTable.h,v 1.3 2001/07/16 21:43:52 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _STRINGTABLE_H
#define _STRINGTABLE_H

#include "../dllinclude.h"

#include<string>
#include<map>

// using namespace std;

typedef std::map<std::string,std::string> StringMap;


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
    /** copy constructor, builds a new table by copying another
     * @param table StringTable to copy from
     */
    StringTable( const StringTable & table );
    /** destructor */
    ~StringTable();
    /** returns a value stored in the map
     * @param key the key the value is for 
     * @returns the string value or an empty string, if the key is not found */
    const std::string & get( const std::string & key );
    /** stores a key value pair in the table, overwritting a possible prior 
     * value
     * @param key the key to store it under
     * @param value the value to store */
    void put( const std::string &key, const std::string &value );
    /**
     * removes a key value pair from the table
     * @param key the key to the pair */
    void remove( const std::string &key );
    /**
     * tests, whether the table contains a certain key
     * @param the key to test for */
    int containsKey( const std::string & );
    /** returns the number of pairs stored 
     * @returns unsigned with the number of pairs */
    unsigned size();
 
    void put(const std::string & key, const int value);
    void put(const std::string & key, const float value);
    void put(const std::string & key, const double value);

    void put(const std::string & key, const int * value, int len);
    void put(const std::string & key, const float * value, int len);
    void put(const std::string & key, const double * value, int len);

    int get(const std::string & key, int * value, int len );
    int get(const std::string & key, float * value, int len );
    int get(const std::string & key, double * value, int len );

};

#endif
