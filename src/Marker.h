// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      Marker.h
//  TYPE:      h header file
//  PROJECT:   @INSERT_PROJECTNAME
//  CONTENT:   Declaration of class 'Marker'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// July 22, 2000 18:56 Gerhard Reitmayr & updated some methods now returning pointers instead of references
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
    : public Node
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
