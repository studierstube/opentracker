 /* ========================================================================
  * (C) 2000,2001 vr|vis vienna
  * ========================================================================
  * PROJECT: Studierstube VR Slide Show system
  * ======================================================================== */
/** Implementation of very simple associative list.
  * This file shall probably go away and we shall use the STL version of 
  * associative lists. I was not able to get the STL code to run under IRIX
  * though.
  *
  * @author Jan Prikryl
  *
  * $Id: assoclist.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file
  * @ingroup VRSS
  */
 /* ======================================================================== */
/*@{*/

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef WIN32
#include <windows.h>
#define strdup _strdup
#endif

#include "assoclist.h"

GroupAssocList::GroupAssocList()
{
    len = 0;
    allocated = 100;
    base = (GroupPair *) malloc(allocated * sizeof(GroupPair));
}

void
GroupAssocList::AddPair(const char * newKey, SoGroup * newValue)
{
    GroupPair pair;

    if ( len >= allocated )
    {
        allocated *= 2;
        base = (GroupPair *) realloc(base, allocated * sizeof(GroupPair));
        assert (base != NULL);
    }

    /** newValue must not be NULL */
    assert ( newValue != NULL);

    /** newKey is a pointer to some existing string; make a private copy */
    pair.key   = strdup (newKey);
    pair.value = newValue;

    base[len++] = pair;
}

SoGroup *
GroupAssocList::SearchFor(const char * searchKey)
{
    unsigned int i;

    /** linear search - not very efficient but the list is not
        supposed to be very long */
    for (i = 0 ; i < len ; i++)
    {
        if (!strcmp (searchKey, base[i].key)) return base[i].value;
    }

    return NULL ; 
}

GroupAssocList::~GroupAssocList()
{
    if ( base != NULL)
    {
        while (len > 0) free(base[--len].key);
        free(base);
    }
}

/*@}*/
/* ===========================================================================
    End of assoclist.cxx
=========================================================================== */
