 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for XSensModule
  *
  * @author Gerhard Reitmayr
  * 
  * $Id: XSensModule.cxx 717 2004-07-27 11:54:49Z reitmayr $
  *
  * @file                                                                   */
 /* ======================================================================= */

#include <ace/Log_Msg.h>

#ifdef WIN32
#include <objbase.h>
#include <xsens/IMTObj.h>
#include <xsens/IMTObj_i.c>
#endif

#include "XSensModule.h"
#include "XSensSource.h"

//using namespace std;


namespace ot {


XSensModule::XSensModule() :
source( NULL )
{
#ifdef WIN32
	HRESULT hr;

	// first try multi-threaded. if that fails try single threaded...
	//
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if ( hr != S_OK && hr != S_FALSE )
	{
		hr = CoInitialize(NULL); 
		if ( hr != S_OK && hr != S_FALSE )
		{
			ACE_DEBUG((LM_ERROR,"XSensModule: failed to initialize COM library!\n" ));
			exit(1);
		}
	}
#endif
}

XSensModule::~XSensModule()
{
    if( source != NULL )
        delete source;

#ifdef WIN32
    CoUninitialize();
#endif
}

Node * XSensModule::createNode( const std::string & name, StringTable & attributes )
{
	if( name.compare("XSensSource") == 0 )
	{
		if( source != NULL )
		{
			ACE_DEBUG((LM_ERROR, "XSensModule: Only one GPSSource can be build !\n" ));
			return NULL;
		}
        int comport = 1;
        attributes.get("comport", &comport);        
		source = new XSensSource( comport );

        ACE_DEBUG((LM_INFO, "XSensModule: Built XSensSource for comport %i\n", comport ));

        initialized = true;
		return source;
	}    
	return NULL;	
}

void XSensModule::pushState()
{
	if( source != NULL )
	{
        source->push();
    }
}

void XSensModule::start()
{
    if( source != NULL )
    {
#ifdef WIN32
        source->pMT->MT_StartProcess();
#endif
    }
}

void XSensModule::close()
{
    if( source != NULL )
    {
#ifdef WIN32
        source->pMT->MT_StopProcess();
#endif
    }
}


}  // namespace ot
