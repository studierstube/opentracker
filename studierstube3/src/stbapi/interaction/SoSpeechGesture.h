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
/** class definition of SoSpeechGesture
  * 
  * SoSpeechGesture is used to detect, if a speech command occured
  *
  * @author  Steiner Reinhard
  *
  * $Id: SoSpeechGesture.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/SbBox.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoMFName.h>

#include <stbapi/event3d/So3DEvent.h>
#include <stbapi/event3d/Sb3DEventHistory.h>
#include <stbapi/interaction/SoGesture.h>

/**
 * A SoSpeechGesture ocurrs, when on the Station occurs a
 * speech event that is equal to a registered command.
 * @ingroup widgets
 * @ingroup interaction
 */
class STBAPI_API SoSpeechGesture : public SoGesture
{
  SO_NODE_HEADER(SoSpeechGesture);

public:
  SoMFName SpeechCommands;    // command this Gesture should recognize
  SoSFBool actionBeginSpeech; // if true, use the begin speech event, otherwise use the end speech event

public:
  static void initClass();

protected:
  void initSoSpeechGesture();

public:
  SoSpeechGesture();

  SoSpeechGesture(const char *p_Command);

public:
	/// has a speech event occured?
	SbBool detect(const Sb3DEventHistory &history, const SbXfBox3f &bbox);

  void AddCommand(const char *p_Command);
};

