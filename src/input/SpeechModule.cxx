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
/** source file for SpeechModule module.
  *
  * @author Reinhard Steiner
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/SpeechModule.cxx,v 1.7 2003/07/18 18:23:25 tamer Exp $
  * @file                                                                   */
 /* ======================================================================= */


// Disable Debug warning for std lib classes
#ifdef WIN32
#pragma warning( disable : 4786 )
#endif

#include <cstdio>
#include <iostream>

#include "SpeechModule.h"
#include "SpeechSource.h"
#include "SpeechSet.h"
#include "SpeechCore.h"

using namespace std;

// Destructor methode
SpeechModule::~SpeechModule()
{
  m_Nodes.clear();

  if(m_Voice)
  {
    delete(m_Voice);
    m_Voice = 0;
  }
  if(m_SpeechCore)
  {
    delete(m_SpeechCore);
    m_SpeechCore = 0;
  }
}



// This methode is called to construct a new Node.
Node* SpeechModule::createNode(const string& name, StringTable& attributes)
{
  if(name.compare("SpeechRecoSource") == 0)
  {
    assert(initialized == 1);   // must be initialized

    std::string SpeechSetName, SpeechSetId;
    SpeechSetId = attributes.get("set");
    SpeechSetName = attributes.get("name");

    printf("SR: Create SpeechSource, SpeechSetName = '%s', SpeechSetId = '%s'\n", SpeechSetName.c_str(), SpeechSetId.c_str());

    SpeechSetBase *speechset = m_SpeechCore->GetSpeechSet(SpeechSetName.c_str());
    assert(speechset);

    SpeechSource *source = new SpeechSource(this, speechset);
    assert(source);


    // Search for the speechset in the config tree, and register the appropriate commands
    for(unsigned int i = 0; i < m_ConfigTree->countChildren(); ++i)
    {
      ConfigNode *SpeechSetNode = (ConfigNode*)m_ConfigTree->getChild(i);
      std::string SpeechSetId2;
      SpeechSetId2 = SpeechSetNode->getAttributes().get("id");

      if(!SpeechSetId.compare(SpeechSetId2))
      {
        for(unsigned int j = 0; j < SpeechSetNode->countChildren(); ++j)
        {
          ConfigNode *CommandNode = (ConfigNode*)SpeechSetNode->getChild(j);
          int CommandId;
          std::string CommandName;
          CommandNode->getAttributes().get("id", &CommandId);
          CommandName = CommandNode->getAttributes().get("name");
          float CommandWeight = 1.0f;
          if(CommandNode->getAttributes().get("weight").size() > 0)
            CommandWeight = atof(CommandNode->getAttributes().get("weight").c_str());
          
          printf("SR:  Register CommandId = %i, Command = '%s', Weight = %.2f\n", CommandId, CommandName.c_str(), CommandWeight);
          speechset->AddCommand(CommandName.c_str(), CommandId, CommandWeight);
        }
      }
    }

    m_Nodes.push_back(source);
    return(source);
  }
  return(NULL);
}


void SpeechModule::init(StringTable& attributes, ConfigNode *localTree)
{
    Module::init( attributes, localTree );
    
  // store the config node tree for later use
  m_ConfigTree = localTree;

  std::string Language;
  Language = localTree->getAttributes().get("language");

  printf("SR: Language = '%s'\n", Language.c_str());


  // create the sr core component
#ifdef USE_SAPISPEECH
  m_SpeechCore = new CSpeechCore;
#else
  m_SpeechCore = new SpeechCoreBase;
#endif
  assert(m_SpeechCore);

  if(!Language.compare("english"))
  {
    m_SpeechCore->Init();
  }
  else
  {
    printf("SR: ERROR: Invalid Language: '%s'\n", Language.c_str());
    m_SpeechCore->Init();
  }

  // get the standard system voice
  m_Voice = new SpeechVoiceModule;
  assert(m_Voice);


  //m_Voice->Speak("The SR system is initialized!");
}


void SpeechModule::close()
{
  if(m_Voice)
  {
    delete(m_Voice);
    m_Voice = 0;
  }
  if(m_SpeechCore)
  {
    delete(m_SpeechCore);
    m_SpeechCore = 0;
  }
}


// pushes events into the tracker tree.
void SpeechModule::pushState()
{
  if(m_SpeechCore)
  {
    assert(m_SpeechCore);
    assert(m_Voice);

    if(m_Push2Nodes.size() > 0)
    {
      for(unsigned int i = 0; i < m_Push2Nodes.size(); ++i)
      {
        SpeechSource *source = (SpeechSource*)m_Push2Nodes[i];
        source->push2();
      }
      m_Push2Nodes.clear();
    }
    else if(m_SpeechCore->ProcessRecognitionPoll())
    {
      //printf("SR: SpeechModule::pushState\n");

      for(NodeVector::iterator it = m_Nodes.begin(); it != m_Nodes.end(); it++)
      {
        SpeechSource *source = (SpeechSource*)*it;
        if(source->push())
        {
          m_Push2Nodes.push_back(source);
        }
      }
    }
  }
}


bool SpeechModule::GetCommand(DWORD p_CommandId, DWORD p_SpeechSetId, std::string &p_Command)
{
  // try to get the speech set via the SpeechSetId
  SpeechSetBase *set = m_SpeechCore->GetSpeechSet(p_SpeechSetId);
  if(!set)
  {
    printf("SR: ERROR: Invalid SpeechSet Id\n");
    return(false);
  }

  // retrieve the command via the CommandId from the speechset
  return(set->GetCommand(p_CommandId, p_Command));
}


void SpeechModule::Speak(const char *p_Sentence, bool p_Async)
{
  assert(m_Voice);
  m_Voice->Speak(p_Sentence, p_Async);
}
