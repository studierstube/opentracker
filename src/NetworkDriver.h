// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      NetworkDriver.h
//  TYPE:      h header file
//  PROJECT:   @INSERT_PROJECTNAME
//  CONTENT:   Declaration of class 'NetworkDriver'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// July 22, 2000 19:12 Gerhard Reitmayr & changed reference to pointer
//     Updated interface of method 'init'
// July 22, 2000 18:56 Gerhard Reitmayr & updated some methods now returning pointers instead of references
//     Update comment header
// ===========================================================================
#ifndef _NETWORKDRIVER_H
#define _NETWORKDRIVER_H


//@START_USER1

// definitions for the network packet format

typedef struct
{
    short int headerId;         // Id (magic number)
    short int revNum;           // Revision Number
    short int maxStationNum;    // Number of stations dealt with by the server
    short int headerLength;     // Length of the header (from the start of the
                                // record; Offset to the first station data)
    short int numOfStations;    // Number of station packets to follow
    short int commentLength;    // Length of the comment
    char data[10000];           // Holds the following data:
                                // n bytes of comment

                                // Data per Station:
                                // short int number of the station
                                // short int format (Quaternion, Euler, Matrix)
                                // short int button states (binary coded)
                                // short int bytes per station (incl. this header)
                                // short int length of the name of the station
                                // n bytes name of the station
                                // position and orientation according to format
} NetworkTrackerDataRecord;
//@END_USER1


/*@NOTE_3202
drives the network connection of the tracker. It keeps the stations and polls them 
for updates. Therefore it also acts as the NodeFactory for the stations. Any network
code goes into this class as well.
*/
class NetworkDriver
    : public NodeFactory
    , public Module
{

//@START_USER2
protected:
    NetworkTrackerDataRecord data;
//@END_USER2

// Members
private:

protected:
    StationVector stations;
    int socket;

public:

// Methods
private:
    void ConstructorInclude();
    void DestructorInclude();

protected:

public:
    NetworkDriver();
    virtual ~NetworkDriver();
    virtual void beginUpdate();
    virtual void close();
    void convertFloatsHToNl(float* floats, float* result, int num);
    virtual Node* createNode(char* const name, StringMap& attributes);
    virtual void endUpdate();
    virtual void init(StringMap& attributes, const Node* localTree);
};

#endif


#ifdef CB_INLINES
#ifndef _NETWORKDRIVER_H_INLINES
#define _NETWORKDRIVER_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
