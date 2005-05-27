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
//  PROJECT:    Studierstube
//  CONTENT:    Definition of SoAudio node
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    jp      Jan Prikryl
// ===========================================================================
//  HISTORY:
//
//  05-Sep-00 17:46:22  jp      last modification
//  04-Sep-00 10:59:15  jp      created
// ===========================================================================
//
//  PURPOSE:
//
//  This node makes it possible to play sound files and sound effects.
//
//  SYNTAX:
//
//  SoAudio {
//    fileName         "soundfile_without_extension"
//    trigger          FALSE/TRUE
//    risingEdgeActive TRUE/FALSE
//  }
//
//  REMARKS:
//
//  In order to suppport all architectures, sound files have to exist in .wav
//  and .aiff versions. `risingEdgeActive' termines which edge of input signal
//  connected to `trigger' actually starts the playback. Its default value
//  starts the playback when trigger changes from 0 to 1.

#ifndef _SOAUDIO_H_
#define _SOAUDIO_H_

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/nodes/SoSubNode.h>

#include <stbapi/stbapi.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * @ingroup util
 */
class STBAPI_API SoAudio : public SoNode {

   SO_NODE_HEADER(SoAudio);

 public:
   // Fields:
   SoSFString     fileName;
   SoSFBool       trigger;
   SoSFBool       risingEdgeActive;
   // Initializes this class for use in scene graphs. This
   // should be called after database initialization and before
   // any instance of this node is constructed.
   static void    initClass();

   // Constructor
   SoAudio();

 protected:
   // These implement supported actions. The only actions that
   // deal with materials are the callback and GL render
   // actions. We will inherit all other action methods from
   // SoNode.
   virtual void   GLRender(SoGLRenderAction *action);
   virtual void   callback(SoCallbackAction *action);

   // This implements generic traversal of SoAudio node, used in
   // both of the above methods.
   virtual void   doAction(SoAction *action);

 private:
   // Destructor. Private to keep people from trying to delete
   // nodes, rather than using the reference count mechanism.
   // Makes newer GCC complaining about destructor not being
   // avaliable as public function.
   virtual ~SoAudio();
   // Used to determine rising edge (0->1) of signal given by
   // "playing" field
   int      isPlaying;
   // If TRUE, sound will be played. Used to inhibit playing
   // sounds at beginning when risingEdgeActive is FALSE and
   // trigger is 0.
   int      testActive;
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SOSoAudio_H_
