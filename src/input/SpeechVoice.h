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
/** header file for SpeechVoice.
  *
  * @author Reinhard Steiner
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/SpeechVoice.h,v 1.2 2002/12/23 15:03:49 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */


#if !defined(__SPEECHVOICE_H)
#define __SPEECHVOICE_H

#include "../../config.h"

#include "SpeechDef.h"

/**
 * This class represents the core component interface of the TextToSpeech (TTS) system
 * an a simple non functional implementation.
 *
 * @author Reinhard Steiner
 * @ingroup input 
 */
class SpeechVoiceBase
{
// protected init & destroy methods
protected:
	/// initializes all class members
  virtual void Initialize()
  {
  };    
  /// destroys (cleanup) all class members
  virtual void Destroy()
  {
  };


// constructor & destructor
public:
  SpeechVoiceBase()
  {
    Initialize();
  }

  virtual ~SpeechVoiceBase()
  {
    Destroy();
  }


// public methods
public:
  /// get the total number of voices
  virtual DWORD GetNumVoices()
  {
	  return 0;
  }

  /// get the voice name with this id
  virtual const char* GetVoiceName(DWORD p_Id, std::string &p_Name)
  {
	  return NULL;
  }

  /// set the current voice by id
  virtual void SetVoice(DWORD p_Id)
  {
  };
  /// set the current voice by name
  virtual void SetVoice(const char *p_Name)
  {
  };

  /// Speak something
  virtual void Speak(const char *p_Sentence, bool p_Async = true)
  {
  };
};

#ifdef USE_SAPISPEECH

#include "SpeechInc.h"

/**
 * This class represents the core component of the TTS.
 * Multiple instances of this class could speak in parallel (not at Win9x).
 * Multiple voices (as installed) are available, and could be changed at runtime.
 * The default voice is the windows standard voice (as in the control panel defined).
 * Text could be spoken synchron or asynchron.
 *
 * @author Reinhard Steiner
 * @ingroup input 
 */
class CSpeechVoice : public SpeechVoiceBase
{
// protected data members
protected:
  CComPtr<ISpVoice> m_Voice;                          /// TTS - Voice

  StringArray m_VoiceName;                            /// all voice names
  std::vector<CComPtr<ISpObjectToken> > m_VoiceToken; /// all voice tokens


// protected init & destroy methods
protected:
  void Initialize();    /// initializes all class members
  void Destroy();       /// destroys (cleanup) all class members


// constructor & destructor
public:
  CSpeechVoice()
  {
    Initialize();
  }

  virtual ~CSpeechVoice()
  {
    Destroy();
  }


// protected methods
protected:
  /// Retrieve all available voices
  void GetVoices();


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


#endif //ifdef USE_SAPISPEECH

#endif //#if !defined(__SPEECHVOICE_H)
