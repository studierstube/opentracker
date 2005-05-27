// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       moduleman
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    ModuleManager: management of an Module Database 
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  21-apr-99 08:00:00  peci      created
// ===========================================================================

#ifndef _MODULEMAN_H_
#define _MODULEMAN_H_

#include <stbapi/util/container.h>
#include "module.h"

#ifndef TRUE
#define TRUE (1==1)
#endif

#ifndef FALSE
#define FALSE (1==0)
#endif

class ModuleManager
{
public:
  //Constructor
  ModuleManager();                             

  //Destructor
  ~ModuleManager();                            

  //Searches the Module Database for the Module with the id id,
  //if the Module was found the method returns the Module, 
  //if not NULL will be returned
  Module* findModule(int id);   

  //Adds the Module newModule into the ModuleDatabase.
  //returns TRUE if it has been added, FALSE otherwhise
  int addModule(Module* newModule);

  //Removes the Module with the id id from the Module database and return
  //a Pointer to the removed Module. The Module itself will not be deleted.
  //If the Modulemanager could not delete the Module NULL will be returned.
  Module* removeModule(int id);

  //Resets the Modulemanager, sets the currentModule to the first Module in the
  //database.
  void reset();

  //sets the current Module to the next Module in the database
  //and returns its Pointer. If there is no more Module available, the method
  //returns NULL.
  Module* getNextModule();

  //get the Number of Modules curretnly stored in the database
  int getNumModules();

private:
  //container, database where the modules are stored.....
  Container* bag;

}; //class ModuleManager



#endif//_MODULEMAN_H_
