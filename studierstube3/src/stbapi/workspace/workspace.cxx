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
/** Workspace main program
  *
  * @author  Dieter Schmalstieg
  *
  * $Id: workspace.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include "StbWorkspace.h"
#include "version.h"

//----------------------------------------------------------------------------
// this function instantiates the manager-objects and initializes them
// next, it sets-up the scene-graphs for all users and starts the 
// inventor main-loop
//
// NOTE: This function is a dynamic entry point to Studierstube. Should you
//       for whatever reason change the parameters or return value, change
//       the dynamic call in `main.cxx' accordingly!

#ifdef WIN32
  int STBAPI_API startMain(int argc, char* argv[])
#else
  int main(int argc, char* argv[])
#endif
{
    printf("****************************************\n");
    printf("%s Workspace\n",STUDIERSTUBE_VERSION_STRING);
    printf("(C) %s Vienna University of Technology\n",STUDIERSTUBE_YEAR_STRING);
    printf("****************************************\n\n");

    StbWorkspace* workspace = StbWorkspace::getInstance();

    printf("SETUP: parsing options\n");
    workspace->parseOptions(argc, argv);

    printf("SETUP: init tracker\n");
    workspace->initTracker();

    printf("SETUP: init geometry\n");
    workspace->initGeometry();

    printf("SETUP: creating Workspace managers\n");
    workspace->createManagers();

    printf("SETUP: init display\n");
    workspace->initDisplay();

    printf("SETUP: setting up Workspace\n");
    if (workspace->loadApplets() < 0) 
	{
		printf("SETUP: setting up Workspace FAILED.\n");

	}
	else
	{
		workspace->createMessageCallback();

		printf("SETUP: entering main loop\n");
		workspace->mainLoop();
		printf("INFO: exiting Workspace\n");
    
		delete workspace;
	}
    printf("exit\n");

    return 0;
}

//eof

