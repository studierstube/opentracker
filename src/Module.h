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
// August 25, 2000 13:44 gr added NetworkSource classes
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
    int initialized;

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
    virtual int stop();
    virtual void update();
    int isInitialized() const;
};

#endif


#ifdef CB_INLINES
#ifndef _MODULE_H_INLINES
#define _MODULE_H_INLINES

/*@NOTE_7714
Returns the value of member 'initialized'.
*/
inline int Module::isInitialized() const
{//@CODE_7714
    return initialized;
}//@CODE_7714



//@START_USER3
//@END_USER3

#endif
#endif
