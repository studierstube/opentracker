// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      Module.h
//  TYPE:      h header file
//  PROJECT:   @INSERT_PROJECTNAME
//  CONTENT:   Declaration of class 'Module'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// July 22, 2000 19:12 Gerhard Reitmayr & changed reference to pointer
//     Updated interface of method 'init'
// July 22, 2000 18:56 Gerhard Reitmayr & updated some methods now returning pointers instead of references
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
    virtual void init(StringMap& attributes, const Node* localTree);
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
