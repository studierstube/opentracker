/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** SoClassLoader node class implementation
 *
 * @author  Andreas Zajic
 *
 * $Id: SoClassLoader.cxx 3481 2004-07-19 16:25:10Z reitmayr $
 * @file SoClassLoader.cxx                                                 */
/* ======================================================================= */

#include <Inventor/SbPList.h>
#include "SoClassLoader.h"

#include <iostream>
#include <sstream>

using namespace std;

SO_NODE_SOURCE(SoClassLoader);

//initClass
void
SoClassLoader::initClass()
{
   SO_NODE_INIT_CLASS(SoClassLoader, SoNode, "Node");
}

// Constructor
SoClassLoader::SoClassLoader()
   :
   objectHandle(NULL)
{
   SO_NODE_CONSTRUCTOR(SoClassLoader);

   SO_NODE_ADD_FIELD(className,(""));
   SO_NODE_ADD_FIELD(fileName,(""));

#ifndef WIN32
   // initialise libltdl
   if (lt_dlinit())
   {
	  printf("ERROR: Initialisation of ltdl failed!\n");
   }
#endif
}

// Destructor
SoClassLoader::~SoClassLoader()
{
   // printf("Classloader destructed\n");
}

// loadclass()
SbBool
SoClassLoader::loadClass( const SbStringList & directories )
{
   SbBool result = FALSE;
   int i;
   SbString file;

   for (i = 0; i < directories.getLength() && result == FALSE; i++)
   {
	  if (directories[i]->getLength() != 0)
	  {
		 file = *(directories[i]);
		 file += "/";
	  }
	  else file = "";
	  
	  file += fileName.getValue();
	  result = loadClass(className.getValue(), file);
   }

   if (result == FALSE)
   {
	  fprintf(stderr, "ERROR: Could not load class %s from '%s' !\n", 
			  className.getValue().getString(),
			  fileName.getValue().getString());
	  exit(1);
   }
   return result;
}
    

// loadClass(SbString classname, SbString filename)
SbBool
SoClassLoader::loadClass(SbString classname, SbString filename)
{
   // load the shared library without OS-specific extension
   if (!linkDll(filename))
	  return FALSE;

   // find the Entrypoint(s) in the Dll
   if (!findEntryPoint(classname))
	  return FALSE;
    
   // call classname::initClass() to register Class in OIV
   if (!initClassFunc)
   {
	  fprintf(stderr, "ERROR: No %s::initClass() in '%s'!\n", className.getValue().getString(),
			  fileName.getValue().getString());
	  return FALSE;
   } else {
	  if (!callInitClass())
		 return FALSE;
   }
   // everything ok     
   return TRUE;
}



// linkDll
SbBool
SoClassLoader::linkDll(SbString filename)
{

// try to open the shared object
#if defined(WIN32)
   // windows is not handled by ldtld so add the extension
#ifdef _DEBUG   
    // test with d.dll for debug version.
    SbString name = filename;
    name += "d.dll";
    objectHandle = LoadLibrary(name.getString());
    if( !objectHandle)
    {
        name = filename;
        name += ".dll";
        objectHandle = LoadLibrary(name.getString());
    }
#else
    filename+=".dll";
    objectHandle = LoadLibrary(filename.getString());
#endif // _DEBUG
#else
   objectHandle = lt_dlopenext(filename.getString());
#endif

   if (!objectHandle)
   {
#if 0
	  fprintf(stderr, "ERROR: Library '%s' not found!\n", filename.getString());
#ifndef WIN32
	  fprintf(stderr, "lt_dlopenext(): %s\n", lt_dlerror());
#endif
#endif
	  return FALSE;
   }
	
   return TRUE;
}



// find entrypoint(s) in dll/so
SbBool
SoClassLoader::findEntryPoint(SbString classname)
{
   
#if defined(WIN32)
   SbString funcStr;
      
   funcStr+="?initClass@";
   funcStr+=classname;
   funcStr+="@@SAXXZ";

   initClassFunc = (void (*)()) GetProcAddress(objectHandle, funcStr.getString());
#elif defined(_SGI_SOURCE)
/*
  initFunc = (void (*)(int, StbCommunicator *)) 
  dlsym(objectHandle,"Stb_init__GiP15StbCommunicator");
  // try to find the entry-point of the Stb_message-function

  messageHandler = dlsym(objectHandle,"Stb_getMessage__GPviT2T1");
*/
#else
   ostringstream oss;
   oss << "_ZN" << string(classname.getString()).length() 
	   << classname.getString() << "9" << "initClass" << "Ev";
    
   char *error;

   initClassFunc = (void (*)()) lt_dlsym(objectHandle, oss.str().c_str());
        
   if (!initClassFunc)
   {
      cerr << "Error in SoClassloader::findEntryPoint(\"" 
		   << endl 
		   << "  " << classname.getString() << "\") : " << lt_dlerror() << endl
		   << "looking for symbol: " << oss.str() << endl;

      return FALSE;    
   }
#endif
    
   if (!initClassFunc)
   {
	  fprintf(stderr, "ERROR: No %s::initClass in '%s'!\n", 
			  className.getValue().getString(),
			  fileName.getValue().getString());
	  return FALSE;
   }
   return TRUE;
}


// call initClass() of the loaded dll
SbBool
SoClassLoader::callInitClass()
{
   if (!initClassFunc)
	  return FALSE;

   // check if the type exists and rather ignore the class to load than crash
   // in flames and smoke !
   if(SoType::fromName(className.getValue()) != SoType::badType())
   {
       printf("WARNING: SoClassLoader type %s already exists! No initialisation done."
              " If you think this is a problem, check your class loaders !\n", className.getValue().getString());
       return TRUE;
   }

   (*initClassFunc)();
    
   fprintf(stderr,"INFO: %s::initClass() called\n",className.getValue().getString());
   return TRUE;
}


// readInstance
SbBool
SoClassLoader::readInstance(SoInput *in, unsigned short flags)
{
   if (!(SoFieldContainer::readInstance(in, flags))) return FALSE;

   if (!loadClass( in->getDirectories() )) return FALSE;

   fprintf(stderr,"INFO: %s successfully read from OIV-File\n",
		   className.getValue().getString());
   return TRUE;
}

