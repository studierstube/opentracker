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
/** header file for SpeechCore.
  *
  * @author Reinhard Steiner
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/SpeechCore.h,v 1.2 2002/12/23 15:03:49 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */



#if !defined(__SPEECHCORE_H)
#define __SPEECHCORE_H

#include "../../config.h"

#include "SpeechDef.h"
#include "SpeechSet.h"

/**
 * This class represents the core component interface of the SR and a simple 
 * non functional implementation.
 * The registerable commands/words are managed via SpeechSet큦. Each command/word group
 * is stored in a SpeechSet. Speech sets could be created/deleted on the fly.
 * The speech core recognizes via a polling mechanism. After polling, the result are available at
 * the appropriate SpeechSet. All results could be queried via the SpeechSet큦 or the
 * SpeechCore itself.
 * 
 * This class represents the abstract interface a command set/group and a simple
 * non functional implementation. Any real implementation should be derived from 
 * this one. Currently there is a big overlap between the SAPI implementation
 * and this base class, because the refactoring was done rather quickly. It
 * might also be interesting to move more of the SAPI interface to this 
 * super class.
 *
 * @author Reinhard Steiner
 * @ingroup input 
 */
class SpeechCoreBase
{
// protected static data members
protected:
  static DWORD s_GrammarId;               /// Unique Grammar Id counter


// protected data members
protected:
  bool m_Initialized;                     /// wether speech core is initialized or not

  unsigned m_NextRuleId;                     /// unique Rule Id counter for the speech sets
  std::vector<SpeechSetBase*> m_SpeechSets;  /// a array with all speech sets of this core object


// protected init & destroy methods
protected:
	/// initializes all class members
	virtual void Initialize(void)
	{
		m_Initialized = false;
		m_NextRuleId = 1;
	};

public:
	/// destroys (cleanup) all class members
	virtual void Destroy(void)
	{
		try
		{
			for(int i = m_SpeechSets.size() - 1; i >= 0; --i)
				delete(m_SpeechSets[i]);
			m_SpeechSets.clear();
		}
		catch(...) {}
		m_Initialized = false;
	};

	// constructor & destructor
public:
	SpeechCoreBase()
	{
		Initialize();
	}

	virtual ~SpeechCoreBase()
	{
		Destroy();
	}


// public methods
public:
	/// Initialize the speech core with a appropriate language
	virtual void Init(void)
	{
		Destroy();
		Initialize();
		m_Initialized = true;
	};


  /// get/create a SpeechSet with the appropriate name
  virtual SpeechSetBase * GetSpeechSet(const char *p_Name, bool p_Create = true);

  /// get a SpeechSet with the appropriate id
  virtual SpeechSetBase* GetSpeechSet(DWORD p_Id);


  /// remove a specified SpeechSet
  virtual void RemoveSpeechSet(const char *p_Name);

  /// remove a specified SpeechSet
  virtual void RemoveSpeechSet(DWORD p_Id);


  /// find a (active) SpeechSet witch recognizes this command
  virtual SpeechSetBase* FindSpeechSet(const char *p_Command, bool p_Active = true);

  /// find a (active) SpeechSet witch recognizes this command
  virtual SpeechSetBase* FindSpeechSet(DWORD p_CommandId, bool p_Active = true);

  /// Poll the recognize events, look if some commands have been recognized.
  virtual bool ProcessRecognitionPoll()
  {
	  return false;
  };

  friend class SpeechSetBase;
};

#ifdef USE_SAPISPEECH

#include "SpeechInc.h"

/**
 * This class represents the core component of the SR.
 * The registerable commands/words are managed via SpeechSet큦. Each command/word group
 * is stored in a SpeechSet. Speech sets could be created/deleted on the fly.
 * The speech core recognizes via a polling mechanism. After polling, the result are available at
 * the appropriate SpeechSet. All results could be queried via the SpeechSet큦 or the
 * SpeechCore itself.
 *
 * @author Reinhard Steiner
 * @ingroup input
 */
class CSpeechCore : public SpeechCoreBase
{
// protected static data members
protected:
  static DWORD s_GrammarId;               /// Unique Grammar Id counter


// protected data members
protected:
  bool m_Initialized;                     /// wether speech core is initialized or not

  CComPtr<ISpRecognizer>  m_RecoEngine;   /// Pointer to our recognition engine instance
  CComPtr<ISpRecoContext> m_RecoCtxt;     /// Pointer to our recognition context
  CComPtr<ISpRecoGrammar> m_CmdGrammar;   /// Pointer to our grammar object

  DWORD m_NextRuleId;                     /// unique Rule Id counter for the speech sets
  std::vector<CSpeechSet*> m_SpeechSets;  /// a array with all speech sets of this core object


// protected init & destroy methods
protected:
  void Initialize();    /// initializes all class members
public:
  void Destroy();       /// destroys (cleanup) all class members


// constructor & destructor
public:
  CSpeechCore()
  {
    Initialize();
  }

  virtual ~CSpeechCore()
  {
    Destroy();
  }


// public methods
public:  

  void Init(void)
  {
	  Init(MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));
  };

  /// Initialize the speech core with a appropriate language
  void Init(LANGID p_LanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));


  /// get/create a SpeechSet with the appropriate name
  SpeechSetBase* GetSpeechSet(const char *p_Name, bool p_Create = true);

  /// get a SpeechSet with the appropriate id
  SpeechSetBase* GetSpeechSet(DWORD p_Id);


  /// remove a specified SpeechSet
  void RemoveSpeechSet(const char *p_Name);

  /// remove a specified SpeechSet
  void RemoveSpeechSet(DWORD p_Id);


  /// find a (active) SpeechSet witch recognizes this command
  SpeechSetBase* FindSpeechSet(const char *p_Command, bool p_Active = true);

  /// find a (active) SpeechSet witch recognizes this command
  SpeechSetBase* FindSpeechSet(DWORD p_CommandId, bool p_Active = true);


  /// Send a windows notify message, when any sr occures
  void NotifyWindowMessage(HWND p_hWnd, UINT p_Msg = WM_USER+1, WPARAM p_wParam = 0, LPARAM p_lParam = 0);

  /// Poll the recognize events, look if some commands have been recognized.
  bool ProcessRecognitionPoll();

  /// Get the next recognized command, in any SpeechSet
  bool GetReco(std::string &p_Result, std::string &p_SpeechSet);


public:
  /// Convert a standard string to a wide string
  static void StrToWide(const char *p_String, std::wstring &p_WideString);
  /// Convert a wide string to a standard string
  static void WideToStr(const WCHAR *p_WideString, std::string &p_String);



  friend class CSpeechSet;
};

#endif //#ifdef USE_SAPISPEECH

#endif //#if !defined(__SPEECHCORE_H)
