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
/** header file for SpeechDef.
  *
  * @author Reinhard Steiner
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/SpeechDef.h,v 1.1 2002/12/10 17:23:44 kaufmann Exp $
  * @file                                                                   */
 /* ======================================================================= */


#if !defined(__SPEECHDEF_H)
#define __SPEECHDEF_H


#include <assert.h>

#include <vector>
#include <queue>
#include <string>


typedef unsigned long       DWORD;


// Speech Wide String
//typedef std::wstring Speechwstring;

// Speech String
//typedef std::string Speechstring;

/// String array with std components
typedef std::vector<std::string> StringArray;

/// String queue with std components
typedef std::queue<std::string> StringQueue;

/// Speech Exception
typedef std::string CSpeechException;


struct SSpeechCommand
{
  DWORD m_CommandId;
  std::string m_Command;
  std::string m_Seperator;
  float m_Weight;
};


class CSpeechCore;      /// SR core
class CSpeechSet;       /// SR command set
class CSpeechVoice;     /// TTS voice
class SpeechVoiceModule;/// TTS voice wrapper



#endif //#if !defined(__SPEECHDEF_H)
