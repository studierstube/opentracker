// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      CursesModule.cxx
//  TYPE:      cxx header file
//  PROJECT:   TrackerServer
//  CONTENT:   Implementation of class 'CursesModule'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// August 18, 2000 23:05 gr, added setting of init flag
//     Updated code of method 'init'
// August 18, 2000 23:04 gr, changed names and added init flag
//     Update comment header
// ===========================================================================
//@START_USER1
//@END_USER1


// Master include file
#include "TrackerServer.h"


//@START_USER2
#include <string>
#ifdef WIN32
#include <iostream>    // VisualC++ has two incompatible iostreams libraries !
#else
#include <iostream.h>
#endif
#include <curses.h>
#include <unistd.h>
//@END_USER2


// Static members


/*@NOTE_7201
Destructor method.
*/
CursesModule::~CursesModule()
{//@CODE_7201
    DestructorInclude();

    // Put in your own code
}//@CODE_7201


/*@NOTE_7228
closes the tracker source. a place for cleanup code etc.
This class also provides an empty implementation for subclasses not doing
anything here.
*/
void CursesModule::close()
{//@CODE_7228
    endwin();
}//@CODE_7228


/*@NOTE_7218
This method is called to construct a new Node. It takes the element name and
any attribute name, value pairs and returns the new Node, if it is able to 
construct the corresponding type. Otherwise it returns NULL.
*/
TreeNode* CursesModule::createNode(char* const name, StringMap& attributes)
{//@CODE_7218
    std::string strName( name );
    if( strName.compare("CursesOutput") == 0 )        
    {
        CursesOutput * output = new CursesOutput((char *) attributes["comment"] );
	// cout << "Build output node" << endl;
        nodes.push_back( output );
	return output;
    }
    return NULL;

}//@CODE_7218


/*@NOTE_7221
after the update cycle. Any conclusion is done here. See update for more
information
*/
void CursesModule::endUpdate()
{//@CODE_7221
    if( nodes.empty())
        return;
    
    // clear the screen and write header info
    // clear();
    move(1,1);
    printw("%s\n\n",headerLine);
    for( CursesOutputVector::iterator it = nodes.begin(); it != nodes.end(); it ++)
    {
	CursesOutput * output = (*it);
	State * state = output->getState();
        printw(" %s : ",output->getComment());
        if( state != NULL )
        {
            printw((state->isValid)?("Valid\n"):("Invalid\n"));
            printw("  Pos : %f %f %f\n",state->position[0], 
                   state->position[1], state->position[2]);
            printw("  Rot : %f %f %f %f\n", state->orientation[0], 
                   state->orientation[1], state->orientation[2],
                   state->orientation[3] );
            printw("  Buttons : %hx\n", state->button );
            printw("  confidence : %f\n", state->confidence );
        }
        else
        {
            printw("NULL !\n");    
        }
        printw("\n");
    }
    printw("Press any key to stop");
    refresh();
}//@CODE_7221


/*@NOTE_7222
initializes the tracker source. This class also provides an empty implementation for
subclasses not doing anything here. It takes the attributes of the element configuring
this module and a local tree consisting of the children of the element. This tree has to
be made up of Nodes.
*/
void CursesModule::init(StringMap& attributes, const TreeNode* localTree)
{//@CODE_7222
    Module::init( attributes, localTree );
    
    initscr();
    cbreak();
    noecho();
    nonl();
    intrflush(stdscr,FALSE);
    keypad(stdscr,TRUE);
    nodelay(stdscr, TRUE);
    leaveok(stdscr, TRUE);                    //switch off the cursor display

    // set the headerline from the configuration file
    headerLine = (char *)attributes["headerline"];
}//@CODE_7222


/*@NOTE_7225
should the tracker be stopped ?
*/
int CursesModule::stop()
{//@CODE_7225
    // check for user input
    if( getch() != ERR )
    { 
        return 1;
    }
    return 0;
}//@CODE_7225


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_7200
Method which must be called first in a constructor.
*/
void CursesModule::ConstructorInclude()
{
}


/*@NOTE_7202
Method which must be called first in a destructor.
*/
void CursesModule::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
