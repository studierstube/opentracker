// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      Module.h
//  TYPE:      h header file
//  PROJECT:   TrackerServer
//  CONTENT:   Declaration of class 'Module'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// August 16, 2000 22:10 gerhard reitmayr removed Node and made everything TreeNodes
//     Updated interface of method 'init'
// August 16, 2000 21:43 gerhard reitmayr
//     Update comment header
// ===========================================================================
#ifndef _MODULE_H
#define _MODULE_H


//@START_USER1
//@END_USER1


/*@NOTE_733
abstract super class for all tracker modules. These might be sources of 
tracker data, or the NetworkDriver or other things.
*/
class Module
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
    Module();
    virtual ~Module();
    virtual void beginUpdate();
    virtual void close();
    virtual void endUpdate();
    virtual void init(StringMap& attributes, const TreeNode* localTree);
    virtual void update();
};

#endif


#ifdef CB_INLINES
#ifndef _MODULE_H_INLINES
#define _MODULE_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
