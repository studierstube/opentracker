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
/** header file for StringTable class and helper classes.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/StringTable.h,v 1.5 2001/08/18 20:01:42 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _STRINGTABLE_H
#define _STRINGTABLE_H

#include "../dllinclude.h"

#include<string>
#include<map>

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
 
    /** 
     * sets an int value, the value is converted to a string and stored
     * under the given key.
     * @param key key string of the entry
     * @param value the integer value to store
     */
    void put(const std::string & key, const int value);
    /** 
     * sets a float value, the value is converted to a string and stored
     * under the given key.
     * @param key key string of the entry
     * @param value the floating point value to store
     */
    void put(const std::string & key, const float value);
    /** 
     * sets a double value, the value is converted to a string and stored
     * under the given key.
     * @param key key string of the entry
     * @param value the double precision value to store
     */
    void put(const std::string & key, const double value);

    /** 
     * sets an array of int values. It is converted to a string where the
     * values are separated by spaces and stored
     * under the given key.
     * @param key key string of the entry
     * @param value pointer to the array of integer values to store
     * @param len the length of the array
     */    
    void put(const std::string & key, const int * value, int len);
    /** 
     * sets an array of float values. It is converted to a string where the
     * values are separated by spaces and stored
     * under the given key.
     * @param key key string of the entry
     * @param value pointer to the array of floating point values to store
     * @param len the length of the array
     */
    void put(const std::string & key, const float * value, int len);
    /** 
     * sets an array of double values. It is converted to a string where the
     * values are separated by spaces and stored
     * under the given key.
     * @param key key string of the entry
     * @param value pointer to the array of double values to store
     * @param len the length of the array
     */
    void put(const std::string & key, const double * value, int len);

    /**
     * parses a stored entry into an array of integers. It assumes that the
     * integers are separated by spaces. It returns the number of actually
     * converted integers.
     * @param key the key of the entry to parse
     * @param value pointer to the array of integers to store the parsed values 
     * @param len the length of the array, default 1 to use it for a single int only
     * @return number of actually parsed values
     */
    int get(const std::string & key, int * value, int len = 1 );
    /**
     * parses a stored entry into an array of floats. It assumes that the
     * floats are separated by spaces. It returns the number of actually
     * converted floats.
     * @param key the key of the entry to parse
     * @param value pointer to the array of integers to store the parsed values 
     * @param len the length of the array, default 1 to use it for a single float only
     * @return number of actually parsed values
     */
    int get(const std::string & key, float * value, int len = 1 );
    /**
     * parses a stored entry into an array of doubles. It assumes that the
     * doubles are separated by spaces. It returns the number of actually
     * converted doubles.
     * @param key the key of the entry to parse
     * @param value pointer to the array of doubles to store the parsed values 
     * @param len the length of the array, default 1 to use it for a single double only
     * @return number of actually parsed values
     */
    int get(const std::string & key, double * value, int len = 1 );

    friend class KeyIterator;
};

/**
 * This is a little helper class that iterates through the keys in a StringTable.
 * You cannot reset the Iterator. 
 * @author Gerhard Reitmayr
 * @ingroup core
 */
class OPENTRACKER_API KeyIterator {
protected:
    /// iterator to keep track of the KeyIterator
    StringMap::const_iterator it;
    /// the actual StringMap we are working with
    StringMap & map;
public:
    /**
     * constructor to create a new KeyIterator for a given StringTable. It 
     * initializes the iterator and sets it to the first key.
     * @param table the StringTable to iterate through
     */
    KeyIterator( StringTable & table ) :
        map( table.map ), it( table.map.begin())
    {
    }
    /**
     * tests if there are more keys to see.
     * @return 0 if no more keys are left, 1 otherwise
     */
    int hasMoreKeys() const; 
    /**
     * returns the key at the current location and advances the iterator by one.
     * @return the key string at the current location.
     */
    const std::string & nextElement();
};

#endif
