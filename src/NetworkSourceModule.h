// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      NetworkSourceModule.h
//  TYPE:      h header file
//  PROJECT:   TrackerServer
//  CONTENT:   Declaration of class 'NetworkSourceModule'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// August 25, 2000 16:51 gr
//     Added method 'convertFloatsNToHl'
//     Update comment header
// August 25, 2000 16:40 gr fixed typedef typo
//     Updated member 'sources'
//     Update comment header
// ===========================================================================
#ifndef _NETWORKSOURCEMODULE_H
#define _NETWORKSOURCEMODULE_H


//@START_USER1
#include "zpthread.h"
#include "NetworkModule.h"
//@END_USER1


/*@NOTE_12979
This class provides a module to read data sent to a multicast group using the
flexible network protocol. As with the NetworkModule to output to a multicast group,
it is so far only capable of managing one multicast group. 

Reading data form the network group is happening in a special thread to enhance
performance.
*/
class NetworkSourceModule
    : public Module
    , public NodeFactory
{

//@START_USER2
protected:
	Semaphore * semid;
	NetworkTrackerDataRecord data;
//@END_USER2

// Members
private:

protected:
    int socket;
    long threadPID;
    NetworkSourceBufferVector sources;
    char* multicast;
    char* port;

public:

// Methods
private:
    void ConstructorInclude();
    void DestructorInclude();

protected:
    static void entryThread(void* self);
    void run();

public:
    NetworkSourceModule();
    virtual ~NetworkSourceModule();
    virtual void beginUpdate();
    virtual void close();
    void convertFloatsNToHl(float* floats, float* result, int num);
    virtual TreeNode* createNode(char* const name, StringMap& attributes);
    virtual void init(StringMap& attributes, const TreeNode* localTree);
    virtual void update();
};

#endif


#ifdef CB_INLINES
#ifndef _NETWORKSOURCEMODULE_H_INLINES
#define _NETWORKSOURCEMODULE_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
