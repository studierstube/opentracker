// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      Station.h
//  TYPE:      h header file
//  PROJECT:   TrackerServer
//  CONTENT:   Declaration of class 'Station'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// August 10, 2000 10:22 Gerhard Reitmayr
//     Update comment header
// ===========================================================================
#ifndef _STATION_H
#define _STATION_H


//@START_USER1
//@END_USER1


/*@NOTE_3147
represents a trackerstation. it is also part of the tree but will be polled
for new updates. It drives the propagation of the states through the tree and
updates the tree that way.
*/
class Station
    : public TreeNode
{

//@START_USER2
//@END_USER2

// Members
private:

protected:
    char name[256];
    int stationNumber;

public:

// Methods
private:
    void ConstructorInclude();
    void DestructorInclude();

protected:

public:
    Station(int stationNumber_, const char* name_);
    virtual ~Station();
    char* getName();
    virtual State* getState();
    int GetStationNumber() const;
};

#endif


#ifdef CB_INLINES
#ifndef _STATION_H_INLINES
#define _STATION_H_INLINES

/*@NOTE_3545
Returns the value of member 'stationNumber'.
*/
inline int Station::GetStationNumber() const
{//@CODE_3545
    return stationNumber;
}//@CODE_3545



//@START_USER3
//@END_USER3

#endif
#endif
