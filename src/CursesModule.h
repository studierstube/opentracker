// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      CursesModule.h
//  TYPE:      h header file
//  PROJECT:   TrackerServer
//  CONTENT:   Declaration of class 'CursesModule'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// August 16, 2000 23:58 gr, added exit code test
//     Added method 'close'
// August 16, 2000 23:53 gr, added curses based display
//     Added method 'DestructorInclude'
//     Added method 'ConstructorInclude'
//     Added method 'stop'
//     Added method 'init'
//     Added method 'endUpdate'
//     Added method 'createNode'
//     Added method '~CursesModule'
//     Added inheritance 'Module'
//     Added inheritance 'NodeFactory'
//     Added member 'headerLine'
//     Added member 'nodes'
// ===========================================================================
#ifndef _CURSESMODULE_H
#define _CURSESMODULE_H


//@START_USER1
//@END_USER1


/*@NOTE_7199
A simple output module, based on curses. It uses output nodes to
grab state information from anywhere in the three. It only initializes 
the screen used for outputting tracker information, if it appears in
the configuration section.
*/
class CursesModule
    : public NodeFactory
    , public Module
{

//@START_USER2
//@END_USER2

// Members
private:

protected:
    CursesOutputVector nodes;
    char* headerLine;

public:

// Methods
private:
    void ConstructorInclude();
    void DestructorInclude();

protected:
 
public:
    virtual ~CursesModule();
    virtual void close();
    virtual TreeNode* createNode(char* const name, StringMap& attributes);
    virtual void endUpdate();
    virtual void init(StringMap& attributes, const TreeNode* localTree);
    virtual int stop();
};

#endif


#ifdef CB_INLINES
#ifndef _CURSESMODULE_H_INLINES
#define _CURSESMODULE_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
