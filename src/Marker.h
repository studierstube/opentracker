// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      Marker.h
//  TYPE:      h header file
//  PROJECT:   TrackerServer
//  CONTENT:   Declaration of class 'Marker'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// August 16, 2000 22:10 gerhard reitmayr removed Node and made everything TreeNodes
//     Deleted inheritance 'Node'
//     Added inheritance 'TreeNode'
// August 16, 2000 21:43 gerhard reitmayr
//     Update comment header
// ===========================================================================
#ifndef _MARKER_H
#define _MARKER_H


//@START_USER1
//@END_USER1


/*@NOTE_1596
a marker represents a single marker that is tracked by the video library.
*/
class Marker
    : public TreeNode
{

//@START_USER2
//@END_USER2

// Members
private:

protected:
    State state;
    int markerId;
    double vertex[4][2];

public:

// Methods
private:
    void ConstructorInclude();
    void DestructorInclude();

protected:

public:
    Marker(int markerId_, double vertex_[4][2]);
    virtual ~Marker();
    int getMarkerId();
    virtual State* getState();
    double** getVertex();
};

#endif


#ifdef CB_INLINES
#ifndef _MARKER_H_INLINES
#define _MARKER_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
