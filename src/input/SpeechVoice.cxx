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
/** source file for SpeechVoice.
  *
  * @author Reinhard Steiner
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */


// this will remove the warning 4786
#include "../tool/disable4786.h"

#include "SpeechVoice.h"
#include "SpeechCore.h"

#ifdef USE_SAPISPEECH
//using namespace std;

namespace ot {

void CSpeechVoice::Initialize()
{
  HRESULT hr = S_OK;
  hr = m_Voice.CoCreateInstance(CLSID_SpVoice);
  if(FAILED(hr))
    throw CSpeechException("Unable create Voice");

  m_Voice->SetPriority(SPVPRI_OVER);

  GetVoices();
}


void CSpeechVoice::Destroy()
{
  // Release all voice tokens
  for(unsigned int i = 0; i < m_VoiceToken.size(); ++i)
    m_VoiceToken[i].Release();

  // Release voice, if created
  if(m_Voice)
  {
    m_Voice.Release();
  }
}



DWORD CSpeechVoice::GetNumVoices()
{
  return(m_VoiceName.size());
}


const char* CSpeechVoice::GetVoiceName(DWORD p_Id, string &p_Name)
{
  if(!m_Voice)
    return(NULL);

  if(p_Id >= m_VoiceName.size())
    throw CSpeechException("Voice id out of bounds");
  p_Name = m_VoiceName[p_Id];
  return(p_Name.c_str());
}


void CSpeechVoice::SetVoice(DWORD p_Id)
{
  if(!m_Voice)
    return;

  if(p_Id >= m_VoiceName.size())
    throw CSpeechException("Voice id out of bounds");
  m_Voice->SetVoice(m_VoiceToken[p_Id]);
}


void CSpeechVoice::SetVoice(const char *p_Name)
{
  if(!m_Voice)
    return;

  for(unsigned int i = 0; i < m_VoiceName.size(); ++i)
    if(!strcmp(m_VoiceName[i].c_str(), p_Name))
    {
      SetVoice(i);
      break;
    }
}


void CSpeechVoice::Speak(const char *p_Sentence, bool p_Async)
{
  if(!m_Voice)
    return;

  wstring wSentence;
  CSpeechCore::StrToWide(p_Sentence, wSentence);
  
  if(p_Async)
    m_Voice->Speak(&wSentence[0], SPF_ASYNC, NULL);
  else
    m_Voice->Speak(&wSentence[0], 0, NULL);
}


void CSpeechVoice::GetVoices()
{
  if(!m_Voice)
    return;

  HRESULT                      hr = S_OK;
  CComPtr<ISpObjectToken>      VoiceToken;
  CComPtr<IEnumSpObjectTokens> Enum;
  ULONG                        Count = 0;
    
  
  // Enumerate the available voices 
  hr = SpEnumTokens(SPCAT_VOICES, NULL, NULL, &Enum);
  if(FAILED(hr))
    throw CSpeechException("Unable to enum Speakers");
  
  // Get the number of voices
  hr = Enum->GetCount(&Count);
  if(FAILED(hr))
    throw CSpeechException("Unable to count Speakers");
  
  // Obtain a list of available voice tokens, set the voice to the token, and call Speak
  for(unsigned int i = 0; i < Count; ++i)
  {
    VoiceToken.Release();
    hr = Enum->Item(i, &VoiceToken);
    //hr = Enum->Next(1, &VoiceToken, NULL);

    if(SUCCEEDED(hr))
    {
      WCHAR *ppszCoMemText;      
      hr = VoiceToken->GetStringValue(NULL, &ppszCoMemText);
      
      if(SUCCEEDED(hr))
      {
        string Speaker;  // convert to ANSI string
        CSpeechCore::WideToStr(ppszCoMemText, Speaker);
        ::CoTaskMemFree(ppszCoMemText);
        m_VoiceName.push_back(Speaker);
        m_VoiceToken.push_back(VoiceToken);
      }
    }
  }
}

} // namespace ot

#endif //ifdef USE_SAPISPEECH
