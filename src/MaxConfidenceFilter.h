// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      MaxConfidenceFilter.h
//  TYPE:      h header file
//  PROJECT:   TrackerServer
//  CONTENT:   Declaration of class 'MaxConfidenceFilter'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// August 25, 2000 13:44 gr added NetworkSource classes
//     Update comment header
// ===========================================================================
#ifndef _MAXCONFIDENCEFILTER_H
#define _MAXCONFIDENCEFILTER_H


//@START_USER1
//@END_USER1


/*@NOTE_329
gets the states from all its children and returns the first one with the maximal 
confidence value. Note that there may be several with the same maximal
confidence value, therefore the above definition is deterministic. It will only use
valid states in its computation. If no valid state is found the last state will be returned.
*/
class MaxConfidenceFilter
    : public TreeNode
{

//@START_USER2
//@END_USER2

// Members
private:

protected:

public:

// Methods
private:
    void ConstructorInclude();
    void DestructorInclude();

protected:

public:
    MaxConfidenceFilter();
    virtual ~MaxConfidenceFilter();
    virtual State* getState();
};

#endif


#ifdef CB_INLINES
#ifndef _MAXCONFIDENCEFILTER_H_INLINES
#define _MAXCONFIDENCEFILTER_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
