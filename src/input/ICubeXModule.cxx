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

#ifdef USE_ICUBEX

#include <ace/Thread.h>
#include <ace/Thread_Mutex.h>
#include <ace/Condition_T.h>
#include <queue>

#  include <OpenTracker/input/ICubeXCodes.h>
#  include <OpenTracker/input/ICubeXModule.h>
#  include <OpenTracker/input/ICubeXSource.h>
#  include <OpenTracker/input/ICubeXSink.h>
#  include <OpenTracker/input/ICubeXSensor.h>
#  include <OpenTracker/core/OtLogger.h>
#  include <OpenTracker/tool/midi.h>
#  include <OpenTracker/tool/midiCodes.h>
#  include <exception>
#  include <vector>




namespace ot{

  OT_MODULE_REGISTER_FUNC(ICubeXModule){
    OT_MODULE_REGISTRATION_DEFAULT(ICubeXModule, "ICubeXConfig");
  }


  class ixMidiSocket: public MidiInHandler, public MidiIn, public MidiOut{
  public:
    typedef SyncQueue<MidiMsg *> ixMsgQueue;
    typedef std::map<unsigned char, ICubeXSource *>SourceDict;
    ixMsgQueue mQueue;

  protected:
    // whether host or standalone
    unsigned char mode;
    SourceDict sources;

    std::vector<MIDIBUFFER*> buffers;

    // number of bytes each input buffer will have
    unsigned long bufferSize;
    // number of input buffers to allocate
    unsigned long bufferCount;


    void initBuffers(){
      if (buffers.size() != bufferCount){
	for (unsigned long i = 0; i < bufferCount; i++){
	  MIDIBUFFER * buf = new MIDIBUFFER;
	  midiInitBuffer(bufferSize, buf);
	  MidiIn::addBuffer(buf);
	  buffers.push_back(buf);
	}
      }
    };

    void releaseBuffers(){
      for (std::vector<MIDIBUFFER*>::iterator i = buffers.begin();
	   i != buffers.end(); i++){
	MIDIBUFFER * buf = (*i);
	midiInUnPrepareBuffer(buf);
	midiReleaseBuffer(buf);
      }
    
    };

  public:
    ixMidiSocket():MidiIn(),MidiOut(), bufferSize(256), bufferCount(1){};

    virtual ~ixMidiSocket(){
      releaseBuffers();
      MidiIn::close();
      MidiOut::close();
    };

    bool hasSource(unsigned char devid){
      SourceDict::iterator search = sources.find(devid);
      if (search != sources.end())
	return true;
      return false;
    }

      ICubeXSource * getSource(unsigned char devid){
          SourceDict::iterator search = sources.find(devid);
          if (search != sources.end())
              return search->second;
          return 0;
      }

    void addSource(ICubeXSource * src, StringTable & attributes){
      if (src){
	unsigned char id = src->getId();
	sources[id] = src;
        
	checkConnection(*src);
	if (! isHostMode()){
	  // it is stand alone, 
	  const std::string & config = attributes.get("config");
	  if (config.compare("mdig")==0 || config.compare("")==0){
	    requestConfiguration( *(src));
	  } else if (config.substr(config.size()-5).compare(".icx")){
	    loadConfigFile(*(src), config);
	  }
	} else {
	  // implement the configuration of an mdig in host mode
	}
	const std::string & status = attributes.get("running_status");
	if (status.compare("on") == 0 || status.compare("") ==0 ){
	  // use running status
	  setRunningStatus(*(src), 0x01);
	} else {
	  // no running status (whaaaaaaa)
	  setRunningStatus(*(src), 0x00);
	}
	
	const std::string midithru = attributes.get("midi_thru");
	if (midithru.compare("off") == 0 || midithru.compare("")){
	  setMidiThru( * (src), 0x00);
	} else
        setMidiThru( * (src), 0x01);
      }
    }

    bool isHostMode(){
      return (mode == 0x00);
    }


    void configure (StringTable & config){
      std::string indev, outdev;
      indev  = config.get("indevice");
      outdev = config.get("outdevice");
      setPort(indev.c_str(), outdev.c_str());

      std::string attr = config.get("mode");
      if (attr.compare("host")==0 )
	mode = 0x00;
      else
	mode = 0x01;
      
    };

    void setPort(const char * inDevName, const char * outDevName){
      // configure the input port
        //        logPrintI("ixMidiPort opening ports %s %s\n", inDevName, outDevName);
      MidiIn::openStr(inDevName);
      MidiIn::setHandler(this);

      // configure the output port
      MidiOut::openStr(outDevName);
    };

    void start(){
      initBuffers();
      logPrintI("ixMidiSocket start recording\n");
      MidiIn::startRecording();
    };

    void stop(){
      MidiIn::stopRecording();
      // cleanup all buffers
      releaseBuffers();
    };

      void setRunningStatus(ICubeXSource & mdig, unsigned char data){
          MidiMsg ixmsg;
          mdig.makeOneByteMsg(ixmsg, MIDI_RUNNING_STATUS, data);
          sendSysExMessage(&ixmsg.msg[0], ixmsg.msg.size());
      }

      void setMidiThru(ICubeXSource & mdig, unsigned char data){
          MidiMsg ixmsg;
          mdig.makeOneByteMsg(ixmsg, MIDI_THRU, data);
          sendSysExMessage(&ixmsg.msg[0], ixmsg.msg.size());
      }


    void checkConnection(ICubeXSource & mdig){


      MidiMsg ixmsg;
      // mute the device so it only answers to us
      mdig.makeOneByteMsg(ixmsg, SET_MUTE, 0x01);
      sendSysExMessage(&ixmsg.msg[0], ixmsg.msg.size());
      
      mdig.makeCmdMsg(ixmsg, DUMP_VERSION);
      sendSysExMessage(&ixmsg.msg[0], ixmsg.msg.size());

      mdig.makeCmdMsg(ixmsg, DUMP_MODE);
      sendSysExMessage(&ixmsg.msg[0], ixmsg.msg.size());

      /*    makeOneByteMsg(ixmsg, SET_ID, 0x01);
	    sendSysExMessage(&ixmsg.msg[0], ixmsg.msg.size());

	    makeOneByteMsg(ixmsg, SET_MODE, 0x01);
	    sendSysExMessage(&ixmsg.msg[0], ixmsg.msg.size());
      */
      // unmute
      mdig.makeOneByteMsg(ixmsg, SET_MUTE,  0x00);
      sendSysExMessage(&ixmsg.msg[0], ixmsg.msg.size());
    
    };

    void requestConfiguration(ICubeXSource & mdig){
      MidiMsg ixmsg;
      // mute the device so it only answers to us
      mdig.makeOneByteMsg(ixmsg, SET_MUTE, 0x01);
      sendSysExMessage(&ixmsg.msg[0], ixmsg.msg.size());
      
      mdig.makeOneByteMsg(ixmsg, DUMP_NAME, mdig.getConfigNr());
      sendSysExMessage(&ixmsg.msg[0], ixmsg.msg.size());

      for (unsigned char i = 0; i < 8; i++){
	mdig.makeTwoByteMsg(ixmsg, DUMP_CONFIG, mdig.getConfigNr(), i);
	sendSysExMessage(&ixmsg.msg[0], ixmsg.msg.size());
      }
      
      mdig.makeOneByteMsg(ixmsg, SET_MUTE,  0x00);
      sendSysExMessage(&ixmsg.msg[0], ixmsg.msg.size());
      
    };

    void loadConfigFile(ICubeXSource & mdig, std::string filename){};



    ////////////////MidiInHandler/////////////////////////////////////
    /* receive a short message */
    virtual void handleShortMsg( unsigned long msg, unsigned long timestamp ) {
      unsigned char byte1, byte2, byte3;
      
      //      logPrintW("ICubeXSource:got short msg\n");

      MidiMsg::unpackShortMsg(msg, byte1, byte2, byte3);
      
      MidiMsg * mMsg = new MidiMsg;
      mMsg->timestamp = timestamp;
      mMsg->msg.push_back(byte1);
      mMsg->msg.push_back(byte2);
      mMsg->msg.push_back(byte3);

      if (! isHostMode()){
          //          logPrintW("Standalone mode, using default source\n");
          // there is only one source
          ICubeXSource * src = sources[0];
          src->mQueue.push(mMsg);
      }
      //      mQueue.push(mMsg);
      
    };
    /* do something when an error from short messages*/
    virtual void onShortMsgError( unsigned long msg, unsigned long timestamp) {
      
    };



    /* receive a long message */
    virtual void handleLongMsg(  unsigned long msg, unsigned long timestamp) {
        //      logPrintW("ICUBEXSOCKET::got a message\n");
      MIDIBUFFER * buf = (MIDIBUFFER *)((LPMIDIHDR)msg)->dwUser;

      unsigned long size = midiBufferGetBytesRecorded(buf);
      unsigned char * ptr  = midiBufferGetPtr (buf);
      // the device is being closed, release the buffers
      if (size ==0){
	midiInUnPrepareBuffer(buf);
	// and quit
	return;
      }

     

      // proceed normally otherwise
      
      
      unsigned char devid = ptr[2];
      SourceDict::iterator search = sources.find(devid);

      if (search == sources.end()){
          logPrintW("Got Message from unknown device id %u\n", devid);
          return;
      }
      
      ICubeXSource * ixSource = search->second;
      unsigned char msgType   = ptr[3];
      //      logPrintW("using source %u to decode %u\n", (ixSource->getDevice() ->id), msgType);
      switch (msgType){
	case VERSION:
            ixSource->setVersion(ptr);
	  break;
	case MODE:
            mode = ptr[4];
            break;
	case NAME:
            ixSource->setName(ptr);
	  break;
	case CONFIG:
        case DUMP_CONFIG:
	  int index;
	  index = (int)ptr[5];
          ixSource ->setSensorConfig(index, ptr);
	  break;
	default:
	  
	  break;
	}
     
      MidiMsg * mMsg = new MidiMsg;
      std::vector<unsigned char> &out = mMsg->msg;
      mMsg->timestamp = timestamp;
      bool ended = (*(ptr + (buf->hdr.dwBytesRecorded -1)) == 0xF7);
      
      if (ended){
          while (size --){
              out.push_back((*ptr));
              ptr++;
          }
          
      } else {
	// there is no message yet because the buffer was full, 
	// the end of the message should be in the next message... 
	// the first part should be stored somewhere to make sure the 
	// message can be put together again
      }
      // place the msg in the queue for external processing
      ixSource->mQueue.push(mMsg);
      
      // enqueue the buffer once again
      midiInQueueBuffer(buf); 
      
    };
    /* do something when an error from long messages */
    virtual void onLongMsgError( unsigned long msg, unsigned long timestamp) {};
    
  };


  /// MODULE IMPL


  void ICubeXModule::cleanUpSrcs(){
    for (ICubeXSourceDict::iterator i = srcs.begin(); i != srcs.end(); i++){
      ICubeXSource * src = (*i);
      delete src;
    }
  }

  void ICubeXModule::cleanUpSinks(){
    for (ICubeXSinkDict::iterator i = sinks.begin(); i != sinks.end(); i++){
      ICubeXSink * src = (*i);
      delete src;
    }
  }

  void ICubeXModule::cleanUpSensors(){
      // SENSORS are now cleaned up by their respective sources Edu
      /*    for (ICubeXSensorDict::iterator i = sensors.begin(); i != sensors.end(); i++){
      ICubeXSensor * src = (*i);
      delete src;
      }*/
  }


  void  ICubeXModule::cleanUpSockets(){
    for(SocketDict::iterator i = sockets.begin(); i != sockets.end(); i++){
      ixMidiSocket * sock = i->second;
      delete sock;
    }
  }
  
  ICubeXModule::ICubeXModule(){
        
  };

  ICubeXModule::~ICubeXModule(){
    cleanUpSockets();
    cleanUpSrcs();
    cleanUpSensors();
    cleanUpSinks();
  };
    
  void ICubeXModule::init(StringTable & attributes, ConfigNode * localTree){
    Module::init( attributes, localTree );
    
    // StringTable & localAttrib = localTree->getAttributes();
    // I think I dont have any local attributes
    for (unsigned i = 0; i< localTree->countChildren() ; i++){
      ConfigNode * socketConfig = (ConfigNode *)localTree->getChild(i);
      StringTable & socketAttr  = socketConfig->getAttributes();
      const std::string & id    = socketAttr.get("socket_id");
      
      SocketDict::iterator search = sockets.find(id);
      if (search == sockets.end()){
	// this is a new socket
	ixMidiSocket * ixSocket = new ixMidiSocket;
	try {
	  ixSocket->configure(socketAttr);
	  
	  sockets[id] = ixSocket;
          ixSocket->start();
	} catch(std::exception & e){
	  logPrintE("ICubeXModule could not initialize socket_id %s: %s\n",id.c_str(), e.what() );
	}
      } else {
	logPrintW("ICubeXModule already initialized socket id %s\n", id.c_str());
      }
    }
    
  };
  
  void ICubeXModule::close(){
    for (SocketDict::iterator i= sockets.begin(); i != sockets.end(); i++){
      i->second->stop();
    }
    
  };
  
  void ICubeXModule::start(){
      //for (SocketDict::iterator i= sockets.begin(); i != sockets.end(); i++){
      //    i->second->start();
      //}
    
  };
  
  void ICubeXModule::addNode(const Node *node){
      //Node * tmp = const_cast<Node *> (node);
      //if (tmp->type.compare("ICubeXSource"));
      //cubes.push_back( dynamic_cast<ICubeXSource *>(tmp));
  };

  void ICubeXModule::removeNode(const Node*){};
  
  // NodeFactory interface
  Node * ICubeXModule::createNode( const std::string & name, StringTable & attributes){
    Node * result = 0;
    //printf("MIDIMODULE::CREATENODE creating a node %s\n", name.c_str());
    if (name.compare ("ICubeXSink") == 0){
      const std::string & port = attributes.get("socket_id");
      SocketDict::iterator search = sockets.find(port);
      if (search != sockets.end()){
	int devid = 0;
	ixMidiSocket * socket = search->second;
	if (socket->isHostMode()){
            logPrintE("Device is hostmode\n");
	  // if it is hostmode we need an id
	  if (attributes.get("devid", &devid) != 1){
	    logPrintE("Error creating ICubeXSinK: Host mode requires an id for each device\n");
	    return 0;
	  }
	}
	ICubeXSink * node = new ICubeXSink;
	node->outPort = socket;
	node->devid = (unsigned char)devid;
        node->type  = "ICubeXSink";
	result = node;
        sinks.push_back(node);
	//addNode(result);

      } else {
	logPrintE("Error creating ICubeXSink: The socket_id %s does not exist\n", port.c_str());
      }
     
    } else if(name.compare ("ICubeXSource") == 0){
      const std::string & port = attributes.get("socket_id");
      SocketDict::iterator search = sockets.find(port);
      int devid = 0;
      if (search != sockets.end()){
	ixMidiSocket * socket = search->second;
	if (socket->isHostMode()){
	  // if it is hostmode we need an id
	  if (attributes.get("devid", &devid) != 1){
	    logPrintE("Error creating ICubeXSinK: Host mode requires an id for each device\n");
	    return 0;
	  }
	  
	}
	
        if (! (socket->hasSource((unsigned char)devid))){
	  ICubeXSource * node = new ICubeXSource;
	  node->configure(attributes);
          node->type = "ICubeXSource";
          srcs.push_back(node);
          //          node->socket = socket;
	  socket->addSource(node, attributes);
          addNode(node);
          result = node;
        } else
            logPrintE("Error creating ICubeXSource: There is already an mdig with devid %d for socket %s\n", devid, port.c_str());
      } else {
	logPrintE("Error creating ICubeXSource: The socket_id %s does not exist\n", port.c_str());
      }
    } else if (name.compare("ICubeXSensor") == 0){
        const std::string & socket_id = attributes.get("socket_id");
        int source_id ;
        int port;
        if (socket_id.compare("") != 0 ){
            SocketDict::iterator search = sockets.find(socket_id);
            if (search != sockets.end()){
                if (attributes.get("dev_id", & source_id) != 0){
                    ixMidiSocket * socket = search->second;
                    ICubeXSource * src = socket->getSource((unsigned char) source_id);
                    if (src != 0){
                        if (attributes.get("port", &port)!= 0){
							ICubeXSensor * sensor = src->getSensor(port);
							if (sensor != 0){				
	/*
							ICubeXSensor * sensor = new ICubeXSensor;
                            sensor->port = (unsigned char) port;
                            sensor->type = "ICubeXSensor";
                            src->addSensor(sensor);
							*/
							   sensor->configure(attributes);
                               sensors.push_back(sensor);
                               result = sensor;
                               addNode(sensor);
							}else
								logPrintE("Error creating ICubeXSensor: invalid port number. Did you use a number between 0 and %u ?\n",ICubeXSource::numberOfPorts);
                        } else
                            logPrintE("Error creating ICubeXSensor: you need to provide a port number. (The port in your Mdig to which the sensor is connected)\n");
                    
                   }else
                        logPrintE("Error creating ICubeXSensor: dev_id %u not found in socket_id %s. You need to declare an ICubeXSource in order to create sensors\n", (unsigned char)source_id, socket_id.c_str());

                } else
                    logPrintE("Error creating ICubeXSensor: need a dev_id to create a sensor\n");

            } else
                logPrintE("Error creating ICubeXSensor: socket_id is unknown\n");
        } else
            logPrintE("Error creating ICubeXSensor: need a socket_id to create a sensor\n");

    }
    return result;
  };
	   
  void ICubeXModule::pushEvent(){
    for (ICubeXSourceDict::iterator i = srcs.begin(); i != srcs.end(); i++){
      ICubeXSource * src = (*i);
      src->pushEvent();
    }
    for (ICubeXSensorDict::iterator i = sensors.begin(); i != sensors.end(); i++){
      ICubeXSensor * src = (*i);
      src->pushEvent();
    }

  };


  ICubeXSink::ICubeXSink():devid(0), outPort(0){};

  ICubeXSink::~ICubeXSink(){};
  int ICubeXSink::isEventGenerator() {return 1;};
  void ICubeXSink::onEventGenerated(Event & event, Node & generator){
    // search the event for an attribute of type MidiMsg
    int size = event.getSize();
    for (int i = 0; i< size; i++){
      std::string name = event.getAttributeName(i);
      try {
	MidiMsg & theMsg = event.getAttribute((MidiMsg*)NULL, name);
	// check for sysEx message      
	if (theMsg.isSysEx()){
	  outPort->sendSysExMessage(&theMsg.msg[0], (unsigned long)theMsg.msg.size());
	} else {
	  // its short, whooo 
	  outPort->sendShortMessage(theMsg.packMsg());
	}
      }catch(std::exception & e){
	// catch all invalid_argument exceptions
      }
    }
    //in the end, resend the message
    updateObservers(event);
    
  };



  const unsigned int ICubeXSource::numberOfPorts=8;

  ICubeXSource::ICubeXSource():id(0x00), confignr(0x01){
	for (int i = 0; i < ICubeXSource::numberOfPorts; i++){
		ICubeXSensor * sensor = new ICubeXSensor;
		sensor->port = (unsigned char) i;
		sensors.push_back(sensor);
	}
  };

  ICubeXSource::~ICubeXSource(){
      for (SensorArray::iterator it = sensors.begin(); it != sensors.end(); it ++){
          ICubeXSensor * sensor = (*it);
          delete sensor;
          
      }
  };


  void ICubeXSource::start(){

  }

  void ICubeXSource::stop(){

  }

  ICubeXSensor * ICubeXSource::getSensor( int index ){
	  if ((index < ICubeXSource::numberOfPorts)  &&  ( index >= 0))
		  return sensors[index];
	  return 0;
  };

  void ICubeXSource::configure(StringTable & config){

  }



  int ICubeXSource::isEventGenerator(){ return 1;};

    unsigned char convertToIxCommand(unsigned char cmd){
        unsigned char result = END_OF_SYSTEM_EX;
        switch (cmd){
            case NOTE_OFF:
                result = 0;
                break;
            case NOTE_ON:
                result = 1;
                break;
            case KEY_PRESSURE:
                result = 2;
                break;
            case CONTROL_CHANGE:
                result = 3;
                break;
            case PROGRAM_CHANGE:
                result = 4;
                break;
            case AFTER_TOUCH:
                result = 5;
                break;
            case PITCH_BEND:
                result = 6;
                break;
        }
        return result;
    };

  void ICubeXSource::pushEvent(){
      MidiMsg * msg = mQueue.front();
      //      logPrintE("Queue msg = %p", msg);
      if (msg != 0){
          event.setAttribute("midiMsg", *msg);
          

              //find out which sensor is using
              if (! msg->isSysEx()){
                  unsigned char cmd = msg->getCommand();
                  unsigned char chl = msg->getChannel();
                  unsigned char ixcmd = convertToIxCommand(cmd);

				  //logPrintE("SMS \n\tcmd 0x%X  ixcmd %u\n\t chl %u\n", cmd, ixcmd,chl); 
 
                  for (int i = 0 ; i< (int)sensors.size(); i++){
                      unsigned char sensorChn = (getSensorConfig(i) [0]) & ((unsigned char)CHANNEL_MASK);
                      unsigned char sensorMappingType = ((getSensorConfig(i) [0]) & ~((unsigned char)CHANNEL_MASK)) >> 4;
                      if (sensorChn == chl && sensorMappingType == ixcmd){
                          if (ixcmd == 0 || ixcmd == 1 || ixcmd == 2){
							  if ((getSensorConfig(i)[1])  == (msg->msg[1])){ 
								  sensors[i] ->haveData((unsigned long)(msg->msg[2]));
							  }

                          }else if(ixcmd == 3) {
							  if ((getSensorConfig(i)[1])  == (msg->msg[1])){ 
								  sensors[i] ->haveData((unsigned long)(msg->msg[2]));
							  }
                          }else{
                              // this is the sensor
							  sensors[i] ->haveData( (unsigned long)(msg->msg[1]) );
             
                          }
                      }
                  }
              }
          delete msg;

          updateObservers(event);
      }
  };

  void ICubeXSource::addSensor(ICubeXSensor * sensor){
      sensors.push_back(sensor);
  }


void ICubeXSource::asHostMode(unsigned char idd){ 
      mode = 0x00;
      id   = idd;
    };
    bool ICubeXSource::isHostMode(){return socket->isHostMode();};
    bool ICubeXSource::isStandAloneMode(){return !(socket->isHostMode());};
    unsigned char ICubeXSource::getId(){return id;};
    unsigned char ICubeXSource::getConfigNr(){return confignr;};
	unsigned char ICubeXSource::getRunningStatus(){return running_status;};
	unsigned char ICubeXSource::getMidiThru(){return midi_thru;};
    float ICubeXSource::getFirmwareVersion(){
      return (version[0]/10);
    };
    float ICubeXSource::getHardwareVersion(){
      return ((version[1]/10) + (version[2]/1000));
    };
    unsigned int ICubeXSource::getSerial(){
      unsigned int result = version[3];
      result <<= 7;
      result |= version[4];
      return result;
    };

      void ICubeXSource::setVersion( unsigned char * ptr ){
          for (int i = 0; i < sizeof (version); i++){
              version[i] = ptr[i+4];
          }
          logPrintI("MDIG is FwVersion %f HwVersion %f Serial %u\n", getFirmwareVersion(), getHardwareVersion(), getSerial());
      }
 
      void ICubeXSource::setSensorConfig(int index, unsigned char * ptr){
		  if (index > ICubeXSource::numberOfPorts)
			  for (int i = 0; i < sizeof(default_config); i++){
				default_config[i] = ptr[i+5];		
			  }
		  else {
	  for (int i = 0; i < sizeof(sensors[index]->config); i++){
              sensors[index]->config[i]  = ptr[i+6];
	  }
		  }    
      }

      unsigned char * ICubeXSource::getSensorConfig(int index){
          if (index < sizeof(sensors))
              return &(sensors[index]->config[0]);
          return 0;
      }

    
      void ICubeXSource::setName(unsigned char * ptr){
	  for (int i = 0; i < sizeof(name); i++){
	    name[i]  = ptr[i+5];
	  }
          logPrintI("MDIG got name %s\n", name);
        
      }

    void ICubeXSource::clear(MidiMsg & mMsg) {
      mMsg.msg.clear();
    };

    void ICubeXSource::makeHeader(MidiMsg & mMsg){
      mMsg.msg.push_back(0xF0);
      mMsg.msg.push_back(0x7D);
      mMsg.msg.push_back(id);
    };

    void ICubeXSource::makeFooter(MidiMsg & mMsg){
      mMsg.msg.push_back(0xF7);
    };

    void ICubeXSource::makeCmdMsg(MidiMsg & mMsg, unsigned char cmd){
      clear(mMsg);
      makeHeader(mMsg);
      mMsg.msg.push_back(cmd);
      makeFooter(mMsg);
    };
    void ICubeXSource::makeOneByteMsg(MidiMsg & mMsg, unsigned char cmd, unsigned char data1){
      clear(mMsg);
      makeHeader(mMsg);
      mMsg.msg.push_back(cmd);
      mMsg.msg.push_back(data1);
      makeFooter(mMsg);
    };

    void ICubeXSource::makeTwoByteMsg(MidiMsg & mMsg, unsigned char cmd, unsigned char data1, unsigned char data2){
      clear(mMsg);
      makeHeader(mMsg);
      mMsg.msg.push_back(cmd);
      mMsg.msg.push_back(data1);
      mMsg.msg.push_back(data2);
      makeFooter(mMsg);
    };

/*    void configure(StringTable & attributes){

    };
*/

 ICubeXSensor::ICubeXSensor():changed(false){};
 void ICubeXSensor::configure(StringTable & config){
 
 };
 ICubeXSensor::~ICubeXSensor(){};
 int ICubeXSensor::isEventGenerator(){return 1;};
 void ICubeXSensor::pushEvent(){
     if (changed){
         updateObservers(event);
         changed = false;
     }
 };

 void ICubeXSensor::haveData(unsigned long data){
     event.setAttribute("data", data);
     changed =true;
 };

}; // namespace ot

#endif //USE_MIDI

/* 
 * ------------------------------------------------------------
 *   End of ICubeXModule.cxx
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
