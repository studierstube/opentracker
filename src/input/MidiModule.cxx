/* ========================================================================
 * Copyright (c) 2006,
 * Institute for Computer Graphics and Vision
 * Graz University of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Graz University of Technology nor the names of
 * its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/** source file for ICubeXModule.
 *
 * @author Eduardo Veas
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/OpenTracker.h>

#ifdef USE_MIDI

#  include <OpenTracker/input/ICubeXModule.h>
#  include <OpenTracker/input/ICubeXSource.h>
#  include <OpenTracker/tool/midi.h>
#  include <vector>


namespace ot{

  OT_MODULE_REGISTER_FUNC(ICubeXModule){
    OT_MODULE_REGISTRATION_DEFAULT(MidiModule, "ICubeXConfig");
  }



  void ICubeXModule::cleanUpSrcs(){
    for (ICubeXSrcDict::iterator i = srcDict.begin(); i != srcDict.end(); i++){
      ICubeXSource * src = (*i);
      delete src;
    }
  }

  ICubeXModule::ICubeXModule(){
    /* store a description of the capabilities for all available devices */
    unsigned int  devcount = midiInDevCount();
    for (unsigned int i = 0; i < devcount; i++){
      MIDIINPUTCAPS * caps = new MIDIINPUTCAPS;
      
      unsigned int err = midiInDevCaps(i, caps, (unsigned int)sizeof(*caps));
      
      if (err == MIDINOERROR){
	inDevDict[(caps->mPname)] = i;
      } else {
	printf("MIDIMODULE:: error retrieving capabilities for input device id %u\n", i);
      }
      
    }
  };

 ICubeXModule::~ICubeXModule(){
   printf("ICubeXMODULE::Destroyed\n");
    cleanUpSrcs();
  };
    
  void ICubeXModule::init(StringTable & attributes, ConfigNode * localTree){

  };
  
  void ICubeXModule::close(){
    
  };
  
  void ICubeXModule::start(){
    for (ICubeXSrcDict::iterator i= srcDict.begin(); i != srcDict.end(); i++){
      (*i)->startRecording();
    }
    
  };
  
  void ICubeXModule::addNode(const Node *node){
    Node * tmp = const_cast<Node *> (node);
    srcDict.push_back( dynamic_cast<ICubeXSource *>(tmp));
  };
  void ICubeXModule::removeNode(const Node*){};
  
  // NodeFactory interface
  Node * ICubeXModule::createNode( const std::string & name, StringTable & attributes){
    Node * result = 0;
	//printf("MIDIMODULE::CREATENODE creating a node %s\n", name.c_str());
    if (name.compare ("ICubeXSource") == 0){
      std::string device = attributes.get("device");
	//  printf("MIDIMODULE::CREATENODE using device %s\n", device.c_str());
      if (device.compare("")!=0){
	ICubeXInCapsDict::iterator search = inDevDict.find(device);
	if (search != inDevDict.end()){
	  unsigned long devid = search->second;
	  printf("MIDIMODULE::CREATENODE using devId %u\n", devid);
	  
	  ICubeXSource * node = new ICubeXSource(devid);
	  addNode(node);
	  result = node;
	  
	}
	  }
	}

    return result;
  };
  
  void ICubeXModule::pushEvent(){
	  for (ICubeXSrcDict::iterator i = srcDict.begin(); i != srcDict.end(); i++){
		ICubeXSource * src = (*i);
		src->pushEvent();
	  }
};
}; // namespace ot

#endif //USE_MIDI
