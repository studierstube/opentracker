// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      CommonNodeFactory.h
//  TYPE:      h header file
//  PROJECT:   TrackerServer
//  CONTENT:   Declaration of class 'CommonNodeFactory'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// August 16, 2000 21:43 gerhard reitmayr
//     Update comment header
// ===========================================================================
#ifndef _COMMONNODEFACTORY_H
#define _COMMONNODEFACTORY_H


//@START_USER1
//@END_USER1


/*@NOTE_1056
This class provides the general node types found in the config file such as
Transformation, VirtualTransformation, Filters etc.
*/
class CommonNodeFactory
    : public NodeFactory
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
    MaxConfidenceFilter* buildMaxConfidenceFilter(StringMap& attributes);
    Transformation* buildTransformation(StringMap& attributes);
    VirtualTransformation* buildVirtualTransformation(StringMap& attributes);

public:
    CommonNodeFactory();
    virtual ~CommonNodeFactory();
    virtual TreeNode* createNode(char* const name, StringMap& attributes);
};

#endif


#ifdef CB_INLINES
#ifndef _COMMONNODEFACTORY_H_INLINES
#define _COMMONNODEFACTORY_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
