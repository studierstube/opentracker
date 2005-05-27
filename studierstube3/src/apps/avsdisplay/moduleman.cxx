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


#include "moduleman.h"
#include <stdio.h>

//---------------------------------------------------------------------------
// Constructor
//
//initializes the container....
//
ModuleManager::ModuleManager()
{
  bag = new Container(sizeof(int)); //key = int!
}

//---------------------------------------------------------------------------
// Destructor 
//
// deletes the container....
//
ModuleManager::~ModuleManager()
{
  delete bag;
}

//---------------------------------------------------------------------------
// getNumModules() 
//
// get the Number of Modules curretnly stored in the database
//
int ModuleManager::getNumModules()
{
  return bag->getNumItems();
}

//---------------------------------------------------------------------------
// findModule(int id,int set) 
//
// Searches the Module Database for the Module with the id id,
// Parameter:
//   if set is TRUE the current Module is the searches, 
//   if set is FALSE the current Module will not be changed.
// returns:
//   if the Module was found the method returns the Module, 
//   if not NULL will be returned
//
Module* ModuleManager::findModule(int id)
{
  return (Module*)bag->find(&id);
}

//---------------------------------------------------------------------------
// addModule(Module* newModule) 
//
// Adds the Module newModule into the ModuleDatabase.
// Parameter:
//   newModule: the Pointer to the Module which has to be added
// returns:
//   returns TRUE if it has been added, FALSE otherwhise
//
int ModuleManager::addModule(Module* newModule)
{
  if (newModule==NULL) return FALSE;
  void* res = bag->insert(newModule,&newModule->id);
  return(res==NULL);
}

//---------------------------------------------------------------------------
// removeModule(int id) 
//
// Removes the Module with the id id from the Module database.
// The Module itself will not be deleted.
// Paramter:
//   the ID of the Module which has to be removed...
// returns:
//  a Pointer to the removed Module
//  If the Modulemanager could not delete the Module NULL will be returned.
//
Module* ModuleManager::removeModule(int id)
{
  return (Module*)bag->remove(&id);
}

//---------------------------------------------------------------------------
// reset() 
//
// Resets the Modulemanager, sets the currentModule to the first Module in the
// database.
//
void ModuleManager::reset()
{
  bag->reset();
}

//---------------------------------------------------------------------------
// reset() 
//
// sets the current Module to the next Module in the database.
// returns:
//   the Pointer to the next Module in the database.
//   If there is no more Module available, the method returns NULL.
//
Module* ModuleManager::getNextModule()
{
  return (Module*)bag->getNextItem();
}

