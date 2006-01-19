 /* ========================================================================
  * Copyright (c) 2006,
  * Institute for Computer Graphics and Vision
  * Graz University of Technology
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are
  * met:
  *
  * Redistributions of source code must retain the above copyright notice,
  * this list of conditions and the following disclaimer.
  *
  * Redistributions in binary form must reproduce the above copyright
  * notice, this list of conditions and the following disclaimer in the
  * documentation and/or other materials provided with the distribution.
  *
  * Neither the name of the Graz University of Technology nor the names of
  * its contributors may be used to endorse or promote products derived from
  * this software without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
  * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
  * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
  * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
  * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for Spacemouse module.
  *
  * @author Michael Woegerbauer
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section spacemousemodule SpaceMouseModule
 * The SpaceMouseModule provides and drives @ref spacemousesource nodes that 
 * generate standard events in certain intervals. It does not use a
 * configuration element, but reserves the name 'SpaceMouseConfig'.
 */

/**
 * @page spacemouse Space Mouse Integration
 *
 * Instructions on including Modules for 3Dconnexion SpaceMouse (Plus USB) in OpenTracker:
 *
 * @par 1.Step: Install the 3Dxware sdk
 *	Installation to <3dxwaresdkpath>
 *
 *
 * @par 2.Step: Recompiling SIAPP.LIB
 *	Open "<3dxwaresdkpath>\lib\siapp\siapp32.dsw" in MS VisualStudio. \n
 *	Change Menu "Project/Settings.../All Configurations/Library/Output file name:" to ".\..\siapp.lib".\n
 * 	Remove "siapp.c" from source files list.\n
 *	Rename "<3dxwaresdkpath>\lib\siapp\src\siapp.c" to "<3dxwaresdkpath>\lib\siapp\src\siapp.cxx" .\n
 *	Add "<3dxwaresdkpath>\lib\siapp\src\siapp.cxx" to source files list.\n
 *
 * @par
 *	Changes to "<3dxwaresdkpath>\lib\siapp\src\siapp.h": \n
 *		change type of parameter "mode" in typedef of "(WINAPI *PFNSI_OPEN)" to sint32_t \n
 *		change typedef of "(WINAPI *PFNSI_GETEVENT)" to  sint32_t \n
 *		change type of parameter "flag" in typedef of "(WINAPI *PFNSI_GETEVENT)" to sint32_t \n
 *		change typedef of "(WINAPI *PFNSI_BUTTONPRESSED)" to  sint32_t \n
 *		change typedef of "(WINAPI *PFNSI_BUTTONRELEASED)" to  sint32_t \n
 *		change typedef of "(WINAPI *PFNSI_SETUIMODE)" to  sint32_t \n
 *		change return type of declaration of "SiSetInitialize()" to sint32_t \n
 *		change type of parameter "mode" in declaration of "SiOpen()" to sint32_t \n
 *		change return type of declaration of "SiGetEvent()" to sint32_t \n
 *		change type of parameter "flag" in declaration of "SiGetEvent()" to sint32_t \n
 *		change return type of declaration of "SiButtonPressed()" to sint32_t \n
 *		change return type of declaration of "SiButtonReleased()" to sint32_t \n
 * 		change return type of declaration of "SiSetUiMode()" to sint32_t \n
 * @par
 *	Changes to "<3dxwaresdkpath>\lib\siapp\src\siapp.cxx": \n
 * 		change return type of "SiSetInitialize()" to sint32_t \n
 *		change type of parameter "mode" in definition of "SiOpen()" to sint32_t \n
 *		change return type of "SiGetEvent()" to "sint32_t" \n
 *		change type of "tmpRetVal" in "SiGetEvent()" to sint32_t \n
 * 		change return type of "SiButtonPressed()" to sint32_t \n
 *		change return type of "SiButtonReleased()" to sint32_t \n
 *		change return type of "SiSetUiMode()" to "sint32_t" \n
 *		change type of "tmpRetVal" in "SiSetUiMode()" to sint32_t \n
 *
 * @par
 *	Build the library.
 *
 * @par 
 *  If any additional calls to functions of the siapp-library are needed in "SpaceMouseModule.cxx" 
 *	of OpenTracker, compare and change declarations and definitions of these functions in 
 *	"siapp.cxx" and "siapp.h" in "<3dxwaresdkpath>\lib\siapp\src\"
 *	accordingly to the "<3dxwaresdkpath>\inc\siapp.h" include-file declarations (see above)
 *	and rebuild the library.
 *
 * @par
 *	Copy "<3dxwaresdkpath>\lib\spwmath\bin\win32i\release\spwmath.lib" to "<3dxwaresdkpath>\lib".
 *
 *
 * @par 3.Step: Building OpenTracker with SpaceMouse modules
 *	Open opentracker.dsw in MS VisualStudio. \n
 *	Uncomment the following line in "<OTROOT>\config.h: \n
 *		#define USE_SPACEMOUSE 1 \n
 * 
 * @par
 *	In menu "Project/Settings..." \n
 *		add "OS_WIN32" to the preprocessor definitions in the "C/C++"-Tab, \n
 *		add "<3dxwaresdkpath>\inc" to additional include directories to 
 *           the preprocessor definitions in "C/C++"-Tab, \n
 *		add "<3dxwaresdkpath>\lib" to additional library path in the "Link"-Tab, \n
 *		add "siapp.lib", "spwmath.lib", and "user32.lib" to object/library
 *			modules in the "Link"-Tab.
 *
 * @par
 *	Build the OpenTracker library/dll.
 */

#ifndef _SPACEMOUSEMODULE_H
#define _SPACEMOUSEMODULE_H

#include "../OpenTracker.h"

#ifdef USE_SPACEMOUSE

#include <Windows.h>

/**
 * The module and factory to drive the SpaceMouseSource nodes. It constructs
 * SpaceMouseSource nodes via the NodeFactory interface and pushes events into
 * the tracker tree according to the nodes configuration.
 * The min/max values for each axis are:
 * -1 / +1 for position and
 * -90 / +90 degrees for orientation,
 * when the SpaceMouse sensitivity is set to factory defaults.
 * @author Michael Woegerbauer
 * @ingroup input
 */

namespace ot {

class OPENTRACKER_API SpaceMouseModule : public ThreadModule, public NodeFactory
{
// Members
protected:
    /// list of SpaceMouseSource nodes in the tree
    NodeVector nodes;

// Methods
public:
    /** constructor method. */
    SpaceMouseModule() : ThreadModule(), NodeFactory(), stop(0)
    {};
    /** Destructor method, clears nodes member. */
    virtual ~SpaceMouseModule();
    /** This method is called to construct a new Node. It compares
     * name to the SpaceMouseSource element name, and if it matches
     * creates a new SpaceMouseSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringTable containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( const std::string& name,  StringTable& attributes);
	/**
     * closes SpaceMouse dynamic library */
    virtual void close();
    /**
	  * opens SpaceMouse dynamic library (SIAPPDLL.DLL)
      */
    virtual void start();


	virtual
		void run();

    /// flag to stop the thread
    int stop;
    /**
     * pushes events into the tracker tree. Checks all SpaceMouseSources and
     * pushes new events, if a SpaceMouseSource fires. The events store
	 * structure with position and status of the buttons.
     */
    virtual void pushState();
	static HWND		hWndSpaceMouse;

private:
	void processMessages();

};

} // namespace ot

#endif

#endif
