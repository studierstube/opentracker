// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      NodeFactoryContainer.cxx
//  TYPE:      h header file
//  PROJECT:   @INSERT_PROJECTNAME
//  CONTENT:   Implementation of class 'NodeFactoryContainer'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// July 22, 2000 21:35 Gerhard Reitmayr & fixed bug with char * as keys, wrong comparison function
//     Updated interface of method 'createNode'
// ===========================================================================
//@START_USER1
//@END_USER1


// Master include file
#include "TrackerServer.h"


//@START_USER2
#ifdef __sgi
#include <algo.h>
#else
#include <algorithm>
#endif
//@END_USER2


// Static members


/*@NOTE_3786
Constructor method.
*/
NodeFactoryContainer::NodeFactoryContainer() //@INIT_3786
    : NodeFactory()
{//@CODE_3786
    ConstructorInclude();

    // Put in your own code
}//@CODE_3786


/*@NOTE_1083
Destructor method.
*/
NodeFactoryContainer::~NodeFactoryContainer()
{//@CODE_1083
    DestructorInclude();

    // Put in your own code
}//@CODE_1083


/*@NOTE_1135
adds a NodeFactory to the container making nodes provided by this factory accessible.
*/
void NodeFactoryContainer::addFactory(NodeFactory& factory)
{//@CODE_1135
    NodeFactoryVector::iterator result = std::find( factories.begin(), factories.end(), &factory );
    if( result == factories.end())
    {
        factories.push_back( &factory );
    }
}//@CODE_1135


/*@NOTE_3608
This method is called to construct a new Node. It takes the element name and
any attribute name, value pairs and returns the new Node, if it is able to 
construct the corresponding type. Otherwise it returns NULL.
*/
Node* NodeFactoryContainer::createNode(char* const name, StringMap& attributes)
{//@CODE_3608
    Node* value = NULL;
    NodeFactoryVector::iterator it = factories.begin();
    while( it != factories.end() && value == NULL )
    {
        value = (*it)->createNode( name, attributes );
    	it++;
    }
    return value;
}//@CODE_3608


/*@NOTE_1138
removes a NodeFactory from the container.
*/
void NodeFactoryContainer::removeFactory(NodeFactory& factory)
{//@CODE_1138
    NodeFactoryVector::iterator result = std::find( factories.begin(), factories.end(), &factory );    
    if( result != factories.end())
    {
        factories.erase( result );
    }
}//@CODE_1138


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_1082
Method which must be called first in a constructor.
*/
void NodeFactoryContainer::ConstructorInclude()
{
}


/*@NOTE_1084
Method which must be called first in a destructor.
*/
void NodeFactoryContainer::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
