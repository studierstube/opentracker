// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      NetworkSource.h
//  TYPE:      h header file
//  PROJECT:   TrackerServer
//  CONTENT:   Declaration of class 'NetworkSource'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// August 25, 2000 13:44 gr added NetworkSource classes
//     Added method 'DestructorInclude'
//     Added method 'ConstructorInclude'
//     Added method 'getStationNumber'
//     Added method 'getState'
//     Added method '~NetworkSource'
//     Added method 'NetworkSource'
//     Added inheritance 'TreeNode'
//     Added member 'stationNumber'
//     Added member 'state'
// ===========================================================================
#ifndef _NETWORKSOURCE_H
#define _NETWORKSOURCE_H


//@START_USER1
//@END_USER1


/*@NOTE_12984
This class is a source for tracker states coming from a network multicast group. It
implements the node handled by the NetworkSourceModule.
*/
class NetworkSource
    : public TreeNode
{

//@START_USER2
//@END_USER2

// Members
private:

protected:
    State state;
    int stationNumber;

public:

// Methods
private:
    void ConstructorInclude();
    void DestructorInclude();

protected:

public:
    NetworkSource(int stationNumber_);
    virtual ~NetworkSource();
    virtual State* getState();
    int getStationNumber() const;
};

#endif


#ifdef CB_INLINES
#ifndef _NETWORKSOURCE_H_INLINES
#define _NETWORKSOURCE_H_INLINES

/*@NOTE_13003
Returns the value of member 'stationNumber'.
*/
inline int NetworkSource::getStationNumber() const
{//@CODE_13003
    return stationNumber;
}//@CODE_13003



//@START_USER3
//@END_USER3

#endif
#endif
