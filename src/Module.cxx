// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      Module.cxx
//  TYPE:      cxx header file
//  PROJECT:   TrackerServer
//  CONTENT:   Implementation of class 'Module'
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
//@START_USER1
//@END_USER1


// Master include file
#include "TrackerServer.h"


//@START_USER2
//@END_USER2


// Static members


/*@NOTE_3798
Constructor method.
*/
Module::Module() //@INIT_3798
    : initialized(0)
{//@CODE_3798
    ConstructorInclude();

    // Put in your own code
}//@CODE_3798


/*@NOTE_735
Destructor method.
*/
Module::~Module()
{//@CODE_735
    DestructorInclude();

    // Put in your own code
}//@CODE_735


/*@NOTE_3849
called before the update cycle. Any preparation stuff is happening here.
*/
void Module::beginUpdate()
{//@CODE_3849
}//@CODE_3849


/*@NOTE_805
closes the tracker source. a place for cleanup code etc.
This class also provides an empty implementation for subclasses not doing
anything here.
*/
void Module::close()
{//@CODE_805
}//@CODE_805


/*@NOTE_3851
after the update cycle. Any conclusion is done here. See update for more
information
*/
void Module::endUpdate()
{//@CODE_3851
}//@CODE_3851


/*@NOTE_739
initializes the tracker source. This class also provides an implementation that sets the initialization flag to true.
Subclasses should call this method if they override this method. It takes the attributes of the element configuring
this module and a local tree consisting of the children of the element. This tree has to
be made up of Nodes.
*/
void Module::init(StringMap& attributes, const TreeNode* localTree)
{//@CODE_739
    initialized = 1;
}//@CODE_739


/*@NOTE_7198
should the tracker be stopped ?
*/
int Module::stop()
{//@CODE_7198
    return 0;
}//@CODE_7198


/*@NOTE_1589
updates the tracker source. This method is where most work is done. It should do
whatever the source needs to do to update any nodes in the tree with new data.

It may seem overkill to have three methods called per update cycle. However the
different functionality of the modules might call for that. For example, any sources 
should do their work in update, whereas the NetworkModule will gather the
stations updates in endUpdate to be sure that the new data was processed.
This makes the modules independed of the order of their invocations.
*/
void Module::update()
{//@CODE_1589
}//@CODE_1589


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_734
Method which must be called first in a constructor.
*/
void Module::ConstructorInclude()
{
}


/*@NOTE_736
Method which must be called first in a destructor.
*/
void Module::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
