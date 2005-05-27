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
/** source file for SoSpeechGesture class.
  *
  * @author Steiner Reinhard
  *
  * $Id: SoSpeechGesture.cxx 4055 2004-11-26 10:41:36Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */


//#define USE_SAPISPEECH

#ifndef WIN32
#include <ctype.h>
#endif 

#include <stbapi/interaction/SoSpeechGesture.h>
#include <stbapi/event3d/Stb3DEventGenerator.h>
#include <stbapi/tracker/TrackerOT.h>
#include <input/SpeechModule.h>
#include <cctype>

using namespace ot;

SO_NODE_SOURCE(SoSpeechGesture);



void SoSpeechGesture::initClass()
{
  SO_NODE_INIT_CLASS(SoSpeechGesture, SoGesture, "SoGesture");
}



SoSpeechGesture::SoSpeechGesture()
{
  initSoSpeechGesture();
}


SoSpeechGesture::SoSpeechGesture(const char *p_Command)
{
  initSoSpeechGesture();

  AddCommand(p_Command);
}


void SoSpeechGesture::initSoSpeechGesture()
{
  SO_NODE_CONSTRUCTOR(SoSpeechGesture);

  SO_NODE_ADD_FIELD(SpeechCommands, (""));
  SO_NODE_ADD_FIELD(actionBeginSpeech, (TRUE));
}


bool CmpNoCase(const std::string &str1, const std::string &str2)
{
  int i = 0;
  while(str1[i] && str2[i] && toupper(str1[i]) == toupper(str2[i])) ++i;
  return((str1[i] - str2[i]) == 0);
}


SbBool SoSpeechGesture::detect(const Sb3DEventHistory &history, const SbXfBox3f &bbox)
{
	So3DEvent *currentEvent = history.getCurrentEvent();

  // ignore event, if it station is blocked
  if(currentEvent == NULL || isBlocked(currentEvent->getStation()) || currentEvent->getType() != So3DEvent::ET_BUTTON0_EVENT)
    return(false);

  if(currentEvent != NULL)
  {
    TrackerOT *tracker = (TrackerOT*)Stb3DEventGenerator::getTracker();
    Context *context = tracker->getContext();
    SpeechModule *SRModule = (SpeechModule*)context->getModule("SpeechRecoConfig");
    assert(SRModule != NULL);
    
    // if there is button 0 activated, then this is a begin speech event
    if(currentEvent->getButton(0) != (actionBeginSpeech.getValue()==TRUE))
      return(false);

    unsigned long CommandId = (unsigned long)currentEvent->getTranslation()[0];
    unsigned long SpeechSetId = (unsigned long)currentEvent->getTranslation()[1];
    
    std::string Command;
    if(SRModule->GetCommand(CommandId, SpeechSetId, Command))
    {
      /*
      if(actionBeginSpeech.getValue()==TRUE)
        printf("-> SpeechBeginEvent: CommandId = %i, SpeechSetId = %i, Command: '%s'\n", (int)CommandId, (int)SpeechSetId, Command.c_str());
      else
        printf("-> SpeechEndEvent: CommandId = %i, SpeechSetId = %i, Command: '%s'\n", (int)CommandId, (int)SpeechSetId, Command.c_str());
      */

      for(int i = 0; i < SpeechCommands.getNum(); ++i)
      {
        SbName cmd;
        cmd = SpeechCommands[i];
        std::string stdcmd(cmd.getString());
        if(CmpNoCase(Command, stdcmd))
        { // Correct Speech command detected!
          if(actionBeginSpeech.getValue() == TRUE)  // TTS this command
            SRModule->Speak(Command.c_str());
          return(true);
        }
      }
    }
    else
      printf("Invalid Speech Event: CommandId = %i, SpeechSetId = %i\n", (int)CommandId, (int)SpeechSetId);
  }
  return(false);
}


void SoSpeechGesture::AddCommand(const char *p_Command)
{
  SpeechCommands.setValue(p_Command);
}
