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
/** source file for SpeechSet.
  *
  * @author Reinhard Steiner
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/SpeechSet.cxx,v 1.3 2003/01/09 04:14:13 tamer Exp $
  * @file                                                                   */
 /* ======================================================================= */


// Disable Debug warning for std lib classes
#ifdef WIN32
#pragma warning( disable : 4786 )
#endif

#include "SpeechSet.h"
#include "SpeechCore.h"

using namespace std;

DWORD SpeechSetBase::GetId()
{
  assert(m_SpeechCore);

  return(m_Id);
}


const char* SpeechSetBase::GetName()
{
  assert(m_SpeechCore);

  return(m_Name.c_str());
}


bool SpeechSetBase::IsCommandRegistered(const char *p_Command)
{
  for(int i = 0; i < m_RegisteredCommands.size(); ++i)
  {
    if(!strcmp(m_RegisteredCommands[i].m_Command.c_str(), p_Command))
      return(true);
  }
  return(false);
}


bool SpeechSetBase::IsCommandIdRegistered(DWORD p_CommandId)
{
  for(int i = 0; i < m_RegisteredCommands.size(); ++i)
  {
    if(m_RegisteredCommands[i].m_CommandId == p_CommandId)
      return(true);
  }
  return(false);
}


long SpeechSetBase::GetCommandId(const char *p_Command)
{
  for(int i = 0; i < m_RegisteredCommands.size(); ++i)
  {
    if(!strcmp(m_RegisteredCommands[i].m_Command.c_str(), p_Command))
      return(m_RegisteredCommands[i].m_CommandId);
  }
  return(-1);     // no command found
}


bool SpeechSetBase::GetCommand(DWORD p_CommandId, std::string &p_Command)
{
  for(int i = 0; i < m_RegisteredCommands.size(); ++i)
  {
    if(m_RegisteredCommands[i].m_CommandId == p_CommandId)
    {
      p_Command = m_RegisteredCommands[i].m_Command;
      return(true);
    }
  }
  p_Command = ""; // no command found
  return(false);
}


void SpeechSetBase::AddCommand(const char *p_Command, DWORD p_CommandId, float p_Weight)
{
  assert(m_SpeechCore);

  // no need for 2 times the same command
  if(IsCommandRegistered(p_Command))
    return;

  if(p_CommandId == -1)
    p_CommandId = m_RegisteredCommands.size() + 1;

  SSpeechCommand command;
  command.m_CommandId = p_CommandId;
  command.m_Command = p_Command;
  command.m_Seperator = " ";
  command.m_Weight = p_Weight;
  m_RegisteredCommands.push_back(command);
}


void SpeechSetBase::RemoveCommand(const char *p_Command)
{
  for(std::vector<SSpeechCommand>::iterator i = m_RegisteredCommands.begin(); i < m_RegisteredCommands.end(); ++i)
  {
    if(!strcmp(i->m_Command.c_str(), p_Command))
    {
      m_RegisteredCommands.erase(i);
      return;
    }
  }
}


void SpeechSetBase::RemoveCommand(DWORD p_CommandId)
{
  for(std::vector<SSpeechCommand>::iterator i = m_RegisteredCommands.begin(); i < m_RegisteredCommands.end(); ++i)
  {
    if(i->m_CommandId == p_CommandId)
    {
      m_RegisteredCommands.erase(i);
      return;
    }
  }
}

void SpeechSetBase::Activate()
{
  if(IsActive())
    return;
  m_Active = true;
}

void SpeechSetBase::Deactivate()
{
  if(!IsActive())
    return;
  m_Active = false;
}

bool SpeechSetBase::IsActive()
{
  return(m_Active);
}

#ifdef USE_SAPISPEECH

	
CSpeechSet::CSpeechSet(const char *p_Name, DWORD p_Id, CSpeechCore *p_SpeechCore)
: SpeechSetBase( p_Name, p_Id, p_SpeechCore)
{
    Initialize();
    m_Name = p_Name;
    m_Id = p_Id;
    m_SpeechCore = p_SpeechCore;
}

void CSpeechSet::Initialize()
{
  m_SpeechCore = 0;
  m_Active = true;
}


void CSpeechSet::Destroy()
{
  if(m_SpeechCore)
  {
    HRESULT hr = S_OK;
    SPSTATEHANDLE hStateTravel;
    wstring wName;

    CSpeechCore::StrToWide(GetName(), wName);

    // remove rule
    hr = m_SpeechCore->m_CmdGrammar->GetRule(&wName[0], m_Id, SPRAF_TopLevel | SPRAF_Active, TRUE, &hStateTravel);
    if(SUCCEEDED(hr))
      m_SpeechCore->m_CmdGrammar->ClearRule(hStateTravel);
  }

  m_SpeechCore = 0;
  m_Active = false;
}


DWORD CSpeechSet::GetId()
{
  assert(m_SpeechCore);

  return(m_Id);
}


const char* CSpeechSet::GetName()
{
  assert(m_SpeechCore);

  return(m_Name.c_str());
}


bool CSpeechSet::IsCommandRegistered(const char *p_Command)
{
  for(int i = 0; i < m_RegisteredCommands.size(); ++i)
  {
    if(!strcmp(m_RegisteredCommands[i].m_Command.c_str(), p_Command))
      return(true);
  }
  return(false);
}


bool CSpeechSet::IsCommandIdRegistered(DWORD p_CommandId)
{
  for(int i = 0; i < m_RegisteredCommands.size(); ++i)
  {
    if(m_RegisteredCommands[i].m_CommandId == p_CommandId)
      return(true);
  }
  return(false);
}


long CSpeechSet::GetCommandId(const char *p_Command)
{
  for(int i = 0; i < m_RegisteredCommands.size(); ++i)
  {
    if(!strcmp(m_RegisteredCommands[i].m_Command.c_str(), p_Command))
      return(m_RegisteredCommands[i].m_CommandId);
  }
  return(-1);     // no command found
}


bool CSpeechSet::GetCommand(DWORD p_CommandId, std::string &p_Command)
{
  for(int i = 0; i < m_RegisteredCommands.size(); ++i)
  {
    if(m_RegisteredCommands[i].m_CommandId == p_CommandId)
    {
      p_Command = m_RegisteredCommands[i].m_Command;
      return(true);
    }
  }
  p_Command = ""; // no command found
  return(false);
}


void CSpeechSet::AddCommand(const char *p_Command, DWORD p_CommandId, float p_Weight)
{
  assert(m_SpeechCore);

  HRESULT hr = S_OK;
  SPSTATEHANDLE hStateTravel;
  wstring wName, wCommand, wSeperator;

  // no need for 2 times the same command
  if(IsCommandRegistered(p_Command))
    return;

  CSpeechCore::StrToWide(GetName(), wName);
  CSpeechCore::StrToWide(p_Command, wCommand);
  CSpeechCore::StrToWide(" ", wSeperator);

  // create (if rule does not already exist) top-level Rule, defaulting to Active
  hr = m_SpeechCore->m_CmdGrammar->GetRule(&wName[0], m_Id, SPRAF_TopLevel | SPRAF_Active, TRUE, &hStateTravel);
  if(FAILED(hr))
  {
    Destroy();
    throw CSpeechException("Unable to get Rule");
  }
  
  // add a command to the rule
  hr = m_SpeechCore->m_CmdGrammar->AddWordTransition(hStateTravel, NULL, &wCommand[0], &wSeperator[0], SPWT_LEXICAL, p_Weight, NULL);
  if(FAILED(hr))
  {
    Destroy();
    throw CSpeechException("Unable to add Word");
  }
    
  // Must Commit the grammar changes before using the grammar.
  hr = m_SpeechCore->m_CmdGrammar->Commit(0);
  if (FAILED(hr))
  {
    Destroy();
    throw CSpeechException("Unable to Commit");
  }

  // Activate the Rule
  hr = m_SpeechCore->m_CmdGrammar->SetRuleState(&wName[0], NULL, SPRS_ACTIVE);
  if (FAILED(hr))
  {
    Destroy();
    throw CSpeechException("Unable to activate rule");
  }

  if(p_CommandId == -1)
    p_CommandId = m_RegisteredCommands.size() + 1;

  SSpeechCommand command;
  command.m_CommandId = p_CommandId;
  command.m_Command = p_Command;
  command.m_Seperator = " ";
  command.m_Weight = p_Weight;
  m_RegisteredCommands.push_back(command);
}


void CSpeechSet::RemoveCommand(const char *p_Command)
{
  for(std::vector<SSpeechCommand>::iterator i = m_RegisteredCommands.begin(); i < m_RegisteredCommands.end(); ++i)
  {
    if(!strcmp(i->m_Command.c_str(), p_Command))
    {
      m_RegisteredCommands.erase(i);
      RebuildRule();
      return;
    }
  }
}


void CSpeechSet::RemoveCommand(DWORD p_CommandId)
{
  for(std::vector<SSpeechCommand>::iterator i = m_RegisteredCommands.begin(); i < m_RegisteredCommands.end(); ++i)
  {
    if(i->m_CommandId == p_CommandId)
    {
      m_RegisteredCommands.erase(i);
      RebuildRule();
      return;
    }
  }
}


void CSpeechSet::RebuildRule()
{
  HRESULT hr = S_OK;
  SPSTATEHANDLE hStateTravel;
  wstring wName, wCommand, wSeperator;

  CSpeechCore::StrToWide(GetName(), wName);
  
  // create (if rule does not already exist) top-level Rule, defaulting to Active
  hr = m_SpeechCore->m_CmdGrammar->GetRule(&wName[0], m_Id, SPRAF_TopLevel | SPRAF_Active, TRUE, &hStateTravel);
  if(FAILED(hr))
  {
    Destroy();
    throw CSpeechException("Unable to get Rule");
  }
  
  // remove rule
  hr = m_SpeechCore->m_CmdGrammar->ClearRule(hStateTravel);
  if(FAILED(hr))
  {
    Destroy();
    throw CSpeechException("Unable to clear Rule");
  }

  for(int i = 0; i < m_RegisteredCommands.size(); ++i)
  {
    CSpeechCore::StrToWide(m_RegisteredCommands[i].m_Command.c_str(), wCommand);
    CSpeechCore::StrToWide(m_RegisteredCommands[i].m_Seperator.c_str(), wSeperator);

    // add a command to the rule
    hr = m_SpeechCore->m_CmdGrammar->AddWordTransition(hStateTravel, NULL, &wCommand[0], &wSeperator[0], SPWT_LEXICAL, m_RegisteredCommands[i].m_Weight, NULL);
    if(FAILED(hr))
    {
      Destroy();
      throw CSpeechException("Unable to add Word");
    }
  }
    
  // Must Commit the grammar changes before using the grammar.
  hr = m_SpeechCore->m_CmdGrammar->Commit(0);
  if (FAILED(hr))
  {
    Destroy();
    throw CSpeechException("Unable to Commit");
  }

  if(IsActive())
    Activate();
  else
    Deactivate();
}


void CSpeechSet::Recognize(const char *p_String)
{
  m_RecogizedCommands.push(p_String);
}


bool CSpeechSet::IsReco()
{
  assert(m_SpeechCore);
  return(m_RecogizedCommands.size() > 0);
}


bool CSpeechSet::GetReco(string &p_Result)
{
  assert(m_SpeechCore);

  if(m_RecogizedCommands.size() > 0)
  {
    p_Result = m_RecogizedCommands.front();
    m_RecogizedCommands.pop();
    return(true);
  }

  return(false);
}


void CSpeechSet::Activate()
{
  if(IsActive())
    return;

  HRESULT hr = S_OK;
  wstring wName;
  CSpeechCore::StrToWide(GetName(), wName);

  // Activate the Rule
  hr = m_SpeechCore->m_CmdGrammar->SetRuleState(&wName[0], NULL, SPRS_ACTIVE);
  if (FAILED(hr))
  {
    Destroy();
    throw CSpeechException("Unable to activate rule");
  }

  m_Active = true;
}


void CSpeechSet::Deactivate()
{
  if(!IsActive())
    return;

  HRESULT hr = S_OK;
  wstring wName;
  CSpeechCore::StrToWide(GetName(), wName);

  // Activate the Rule
  hr = m_SpeechCore->m_CmdGrammar->SetRuleState(&wName[0], NULL, SPRS_INACTIVE);
  if (FAILED(hr))
  {
    Destroy();
    throw CSpeechException("Unable to deactivate rule");
  }

  m_Active = false;
}


bool CSpeechSet::IsActive()
{
  return(m_Active);
}


#endif //ifdef USE_SAPISPEECH

