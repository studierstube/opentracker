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
/** header file for ICubeXSource.
 *
 * @author Eduardo Veas
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/tool/midi.h>
#include <vector>
#include <map>
#include <string>

namespace ot{


  /// helper class 
  template<class T>
  class SyncQueue{
  private:
    std::queue<T> msgQueue;
    ACE_Thread_Mutex _mutex;  
  
    ACE_Thread_Mutex _sigMutex;

    ACE_Condition<ACE_Thread_Mutex> haveDataSignal;

  public:
    //delete all the messages in the queue
    void cleanupAll(){
      _mutex.acquire();
      while(!msgQueue.empty()){
	delete msgQueue.front();
	msgQueue.pop();
      }
      _mutex.release();
   
    };

  public:
    SyncQueue():_mutex("SyncQueueMutex"),_sigMutex("SyncQueueHaveData"),
		haveDataSignal(_sigMutex, USYNC_THREAD, "SyncQueueDataSignal", NULL){
    };

    ~SyncQueue(){
      removeAll();
    };
  
    void push(T msg){
      _mutex.acquire();
      msgQueue.push(msg);

      haveDataSignal.broadcast();
      _mutex.release();
    };

    T front(){
      T result=0;
      _mutex.acquire();
      if (!msgQueue.empty()){
	result = msgQueue.front();
	msgQueue.pop();
      }
      _mutex.release();
      return result;
    };

    void removeAll(){
      _mutex.acquire();
      while(!msgQueue.empty()){
	//      delete msgQueue.front();
	msgQueue.pop();
      }
      _mutex.release();
    };
    bool isEmpty(){
      bool result = true;
      _mutex.acquire();
      result = msgQueue.empty();
      _mutex.release();
      return result;
    };

    void waitForData(){
      _sigMutex.acquire();
      haveDataSignal.wait();
      _sigMutex.release();
    }
  
  };


  class ixMdig;
  class ixMidiSocket;
  class MidiMsg;
  class ICubeXSource: public Node{
  protected:
    ixMdig * mdig;
    ixMidiSocket * socket;
    bool changed;
    Event event;

    friend class ICubeXModule;
    ICubeXSource();
    void configure(StringTable & config);

    void start();
    void stop();

  public:
    typedef SyncQueue<MidiMsg *> MsgQueue;
    MsgQueue mQueue;
    virtual ~ICubeXSource();
    virtual int isEventGenerator();
    void pushEvent();

    ixMdig * getDevice();
  };
  


} // namespace ot
