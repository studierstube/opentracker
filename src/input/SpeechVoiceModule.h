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
/** header file for SpeechVoiceModule.
  *
  * @author Reinhard Steiner
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/SpeechVoiceModule.h,v 1.4 2003/01/13 10:51:49 bara Exp $
  * @file                                                                   */
 /* ======================================================================= */


#if !defined(__SPEECHVOICEMODULE_H)
#define __SPEECHVOICEMODULE_H

#include "../OpenTracker.h"

#include "SpeechDef.h"

/**
 * This class represents the core component of the TTS.
 * Multiple instances of this class could speak in parallel (not at Win9x).
 * Multiple voices (as installed) are available, and could be changed at runtime.
 * The default voice is the windows standard voice (as in the control panel defined).
 * Text could be spoken synchron or asynchron.
 */
class SpeechVoiceModule
{
// protected data members
protected:
  SpeechVoiceBase *m_Voice;                              // Voice


// protected init & destroy methods
protected:
  void Initialize();    /// initializes all class members
  void Destroy();       /// destroys (cleanup) all class members


// constructor & destructor
public:
  SpeechVoiceModule()
  {
    Initialize();
  }

  virtual ~SpeechVoiceModule()
  {
    Destroy();
  }


// public methods
public:
  /// get the total number of voices
  DWORD GetNumVoices();

  /// get the voice name with this id
  const char* GetVoiceName(DWORD p_Id, std::string &p_Name);


  /// set the current voice by id
  void SetVoice(DWORD p_Id);
  /// set the current voice by name
  void SetVoice(const char *p_Name);


  /// Speak something
  void Speak(const char *p_Sentence, bool p_Async = true);
};

#endif //#if !defined(__SPEECHVOICEMODULE_H)
