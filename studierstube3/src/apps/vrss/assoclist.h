/* =========================================================================
 *  (C) 2000,2001 vr|vis vienna
 * =========================================================================
 *  PROJECT: Studierstube VR Slide Show system
 * ========================================================================= */
/** 
 * Very simple associative list for SoGroup nodes.
 *  This file shall probably go away and we shall use the STL version of 
 *  associative lists. I was not able to get the STL code to run under IRIX
 *  though.
 * 
 *  @author Jan Prikryl
 * 
 *  $Id: assoclist.h 3426 2004-07-14 12:11:27Z tamer $
 *  @file
 *  @ingroup VRSS
 */
/* ======================================================================== */
/*@{*/

#ifndef _VRSS_ASSOCLIST_H_INCLUDED_
#define _VRSS_ASSOCLIST_H_INCLUDED_

#include <Inventor/nodes/SoGroup.h>

/**
 * Pair holding the SoGroup pointer and the corresponding key value.
 * The GroupPair forms an element of an associative array: In our case,
 * `key' is the filename of SoFile node that has been turned into
 * SoGroup. The pointer to this SoGroup is stored as `value'.
 */
typedef struct {
    char    * key;       /**< String that is used as an associative key. */
    SoGroup * value;     /**< Pointer to existing corresponding SoGroup. */
} GroupPair;

/**
 * Class implementing the SoGroup associative list.
 * The key values of the associative list are file names of SoFile nodes
 * that have been replaced by SoGroups in the process of nuking SoFile
 * nodes.  
 */
class GroupAssocList 
{
public:
    /**
     * A constructor.
     * Implicitely allocates some place for the associative array. Later
     * reallocations are on-demand.
     */ 
    GroupAssocList();
    /**
     * A destructor.
     * Frees the allocated memory and all associative array elements. Also
     * deallocated all the memory that has been allocated for key strings.
     */ 
    ~GroupAssocList();
    /**
     * Adds a pair to the associative list.
     * Duplicates the key string, reallocates the underlying array if
     * necessary.
     * @param newKey   Key string, typically a path name of SoFile group.
     * @param newValue Pointer to the SoGroup replacement of the SoFile.
     */
    void AddPair(const char * newKey, SoGroup * newValue);
    /**
     * Case sensitive search for the given key string.
     * The routine linearly searches the associative list for the given
     * key.
     * @param searchKey The key (SoFile path in our case) to search for.
     * @return The SoGroup node corresponding to the search string or NULL
     * in case that the search string does not match any of the existing
     * keys. 
     */ 
    SoGroup * SearchFor(const char * searchKey);
private:
    /**
     * Length of the array.
     * Starts at 0, will increase after every call to AddPair().
     */
    unsigned int len;         /**< length */
    /**
     * Number of allocated GroupPairs.
     * Will be increased in AddPair() after len >= allocated.
     */
    unsigned int allocated;   /**< number of allocated elements */
    /**
     * Base of the GroupPair array.
     * Initialized by the malloc() in constructor. May change after
     * realloc() in AddPair().
     */
    GroupPair * base;         /**< pointer to the base of the array */
};

#endif

/*@}*/
/* ===========================================================================
    End of assoclist.h
=========================================================================== */
