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
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1090 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for SpeechCore.
  *
  * @author Reinhard Steiner
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/SpeechCore.cxx,v 1.3 2003/01/09 04:14:13 tamer Exp $
  * @file                                                                   */
 /* ======================================================================= */


// Disable Debug warning for std lib classes
#ifdef WIN32
#pragma warning( disable : 4786 )
#endif

#include "SpeechCore.h"
#include "SpeechSet.h"

using namespace std;

DWORD SpeechCoreBase::s_GrammarId = 1;

SpeechSetBase * SpeechCoreBase::GetSpeechSet(const char *p_Name, bool p_Create)
{
  if(!m_Initialized)
    return(NULL);

  for(int i = 0; i < m_SpeechSets.size(); ++i)
  {
    if(!strcmp(m_SpeechSets[i]->GetName(), p_Name))
      return(m_SpeechSets[i]);
  }

  if(!p_Create)
    return(NULL);

  SpeechSetBase *set = new SpeechSetBase(p_Name, m_NextRuleId++, this);
  if(!set) throw CSpeechException("no memory");
  m_SpeechSets.push_back(set);
  return(set);
}


SpeechSetBase* SpeechCoreBase::GetSpeechSet(DWORD p_Id)
{
  if(!m_Initialized)
    return(NULL);

  for(int i = 0; i < m_SpeechSets.size(); ++i)
  {
    if(m_SpeechSets[i]->GetId() == p_Id)
      return(m_SpeechSets[i]);
  }
  return(NULL);
}


void SpeechCoreBase::RemoveSpeechSet(const char *p_Name)
{
  if(!m_Initialized)
    return;

  for(std::vector<SpeechSetBase*>::iterator i = m_SpeechSets.begin(); i < m_SpeechSets.end(); ++i)
  {
    if(!strcmp((*i)->GetName(), p_Name))
    {
      delete(*i);
      m_SpeechSets.erase(i);
    }
  }
}


void SpeechCoreBase::RemoveSpeechSet(DWORD p_Id)
{
  if(!m_Initialized)
    return;

  for(std::vector<SpeechSetBase*>::iterator i = m_SpeechSets.begin(); i < m_SpeechSets.end(); ++i)
  {
    if((*i)->GetId() == p_Id)
    {
      delete(*i);
      m_SpeechSets.erase(i);
    }
  }
}


SpeechSetBase* SpeechCoreBase::FindSpeechSet(const char *p_Command, bool p_Active)
{
  if(!m_Initialized)
    return(NULL);

  for(int i = 0; i < m_SpeechSets.size(); ++i)
  {
    if(!p_Active || m_SpeechSets[i]->IsActive())
      if(m_SpeechSets[i]->IsCommandRegistered(p_Command))
        return(m_SpeechSets[i]);
  }
  return(NULL);
}


SpeechSetBase* SpeechCoreBase::FindSpeechSet(DWORD p_CommandId, bool p_Active)
{
  if(!m_Initialized)
    return(NULL);

  for(int i = 0; i < m_SpeechSets.size(); ++i)
  {
    if(!p_Active || m_SpeechSets[i]->IsActive())
      if(m_SpeechSets[i]->IsCommandIdRegistered(p_CommandId))
        return(m_SpeechSets[i]);
  }
  return(NULL);
}

#ifdef USE_SAPISPEECH

DWORD CSpeechCore::s_GrammarId = 1;

void CSpeechCore::Initialize()
{
  m_Initialized = false;
  m_NextRuleId = 1;
}


void CSpeechCore::Destroy()
{
  try
  {
    for(int i = m_SpeechSets.size() - 1; i >= 0; --i)
      delete(m_SpeechSets[i]);
    m_SpeechSets.clear();
  }
  catch(...) {}

  try
  {
    // Release grammar, if created
    if(m_CmdGrammar)
    {
      m_CmdGrammar.Release();
    }
    // Release recognition context, if created
    if(m_RecoCtxt)
    {
      m_RecoCtxt->SetNotifySink(NULL);
      m_RecoCtxt.Release();
    }
    // Release recognition engine instance, if created
    if(m_RecoEngine)
    {
      m_RecoEngine.Release();
    }
  }
  catch(...) {}

  m_Initialized = false;
}


void CSpeechCore::Init(LANGID p_LanguageId)
{
  Destroy();
  Initialize();


  HRESULT hr = S_OK;
  
  // create a recognition engine
  hr = m_RecoEngine.CoCreateInstance(CLSID_SpSharedRecognizer);
  if(FAILED(hr))
  {
    Destroy();
    throw CSpeechException("Unable to create Recognizer");
  }
  
  
  // create the command recognition context
  hr = m_RecoEngine->CreateRecoContext(&m_RecoCtxt);
  if(FAILED(hr))
  {
    Destroy();
    throw CSpeechException("Unable to create Recognition Context");
  }
  
  
  // Tell SR what types of events interest us.  Here we only care about command recognition.
  hr = m_RecoCtxt->SetInterest(SPFEI(SPEI_RECOGNITION), SPFEI(SPEI_RECOGNITION));
  if(FAILED(hr))
  {
    Destroy();
    throw CSpeechException("Unable to set event Interest");
  }
  
  
  // Create our grammar
  hr = m_RecoCtxt->CreateGrammar(CSpeechCore::s_GrammarId++, &m_CmdGrammar);
  if(FAILED(hr))
  {
    Destroy();
    throw CSpeechException("Unable to create Grammar");
  }
  
  
  // Set language
  if(p_LanguageId != MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT))
  {
    hr = m_CmdGrammar->ResetGrammar(p_LanguageId);
    if(FAILED(hr))
    {
      Destroy();
      throw CSpeechException("Unable to set Language");
    }
  }

  m_Initialized = true;
}



SpeechSetBase* CSpeechCore::GetSpeechSet(const char *p_Name, bool p_Create)
{
  if(!m_Initialized)
    return(NULL);

  for(int i = 0; i < m_SpeechSets.size(); ++i)
  {
    if(!strcmp(m_SpeechSets[i]->GetName(), p_Name))
      return(m_SpeechSets[i]);
  }

  if(!p_Create)
    return(NULL);

  CSpeechSet *set = new CSpeechSet(p_Name, m_NextRuleId++, this);
  if(!set) throw CSpeechException("no memory");
  m_SpeechSets.push_back(set);
  return(set);
}


SpeechSetBase* CSpeechCore::GetSpeechSet(DWORD p_Id)
{
  if(!m_Initialized)
    return(NULL);

  for(int i = 0; i < m_SpeechSets.size(); ++i)
  {
    if(m_SpeechSets[i]->GetId() == p_Id)
      return(m_SpeechSets[i]);
  }
  return(NULL);
}


void CSpeechCore::RemoveSpeechSet(const char *p_Name)
{
  if(!m_Initialized)
    return;

  for(std::vector<CSpeechSet*>::iterator i = m_SpeechSets.begin(); i < m_SpeechSets.end(); ++i)
  {
    if(!strcmp((*i)->GetName(), p_Name))
    {
      delete(*i);
      m_SpeechSets.erase(i);
    }
  }
}


void CSpeechCore::RemoveSpeechSet(DWORD p_Id)
{
  if(!m_Initialized)
    return;

  for(std::vector<CSpeechSet*>::iterator i = m_SpeechSets.begin(); i < m_SpeechSets.end(); ++i)
  {
    if((*i)->GetId() == p_Id)
    {
      delete(*i);
      m_SpeechSets.erase(i);
    }
  }
}


SpeechSetBase* CSpeechCore::FindSpeechSet(const char *p_Command, bool p_Active)
{
  if(!m_Initialized)
    return(NULL);

  for(int i = 0; i < m_SpeechSets.size(); ++i)
  {
    if(!p_Active || m_SpeechSets[i]->IsActive())
      if(m_SpeechSets[i]->IsCommandRegistered(p_Command))
        return(m_SpeechSets[i]);
  }
  return(NULL);
}


SpeechSetBase* CSpeechCore::FindSpeechSet(DWORD p_CommandId, bool p_Active)
{
  if(!m_Initialized)
    return(NULL);

  for(int i = 0; i < m_SpeechSets.size(); ++i)
  {
    if(!p_Active || m_SpeechSets[i]->IsActive())
      if(m_SpeechSets[i]->IsCommandIdRegistered(p_CommandId))
        return(m_SpeechSets[i]);
  }
  return(NULL);
}


void CSpeechCore::NotifyWindowMessage(HWND p_hWnd, UINT p_Msg, WPARAM p_wParam, LPARAM p_lParam)
{
  // Let SR know that window we want it to send event information to, and using what message
  HRESULT hr = m_RecoCtxt->SetNotifyWindowMessage(p_hWnd, p_Msg, p_wParam, p_lParam);
  if(FAILED(hr))
    throw CSpeechException("Unable to set notify window message");
}


bool CSpeechCore::ProcessRecognitionPoll()
{
  if(!m_Initialized)
    return(false);

  bool Recognized = false;
  CSpEvent event;  // Event helper class
  
  // Loop processing events while there are any in the queue
  while(event.GetFrom(m_RecoCtxt) == S_OK)
  {
    // Look at recognition event only
    switch(event.eEventId)
    {
    case SPEI_RECOGNITION:
      {
        HRESULT hr;
        ISpRecoResult *RecoResult = event.RecoResult();
        WCHAR   *ppszCoMemText;

        hr = RecoResult->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, &ppszCoMemText, NULL);

        string RecoText;  // convert to ANSI string
        CSpeechCore::WideToStr(ppszCoMemText, RecoText);
        ::CoTaskMemFree(ppszCoMemText);

        CSpeechSet* set = reinterpret_cast<CSpeechSet *>(FindSpeechSet(RecoText.c_str()));
        if(set)
        {
          set->Recognize(RecoText.c_str());
        }

        Recognized = true;
      }      
    }
  }
  return(Recognized);
}


bool CSpeechCore::GetReco(string &p_Result, string &p_SpeechSet)
{
  if(!m_Initialized)
    return(false);

  for(int i = 0; i < m_SpeechSets.size(); ++i)
  {
    if(m_SpeechSets[i]->GetReco(p_Result))
    {
      p_SpeechSet = m_SpeechSets[i]->GetName();
      return(true);
    }
  }
  return(false);
}


void CSpeechCore::StrToWide(const char *p_String, wstring &p_WideString)
{
  int len = strlen(p_String) + 1;
  p_WideString.resize(len);
  
  MultiByteToWideChar(CP_ACP, 0, p_String, -1, &p_WideString[0], len-1);
  p_WideString[len-1] = 0;
}


void CSpeechCore::WideToStr(const WCHAR *p_WideString, string &p_String)
{
  int len = wcslen(p_WideString)+1;
  p_String.resize(len);
  
  WideCharToMultiByte(CP_ACP, 0, p_WideString, -1, &p_String[0], len-1, NULL, NULL);
  p_String[len-1] = 0;
}


#endif //ifdef USE_SAPISPEECH