// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      VideoSource.h
//  TYPE:      h header file
//  PROJECT:   TrackerServer
//  CONTENT:   Declaration of class 'VideoSource'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// August 16, 2000 22:10 gerhard reitmayr removed Node and made everything TreeNodes
//     Updated interface of method 'init'
// August 16, 2000 21:43 gerhard reitmayr
//     Update comment header
// ===========================================================================
#ifndef _VIDEOSOURCE_H
#define _VIDEOSOURCE_H


//@START_USER1
//@END_USER1


/*@NOTE_786
finally our video source. It uses the ARToolKit library to track a number of markers in a
video image. It sets up the video library and acts as a NodeFactory for Marker nodes.
It also keeps a vetor of the created nodes to update them.

It also implements Source to provide the basic functionality of a tracker source.
*/
class VideoSource
    : public Module
    , public NodeFactory
{

//@START_USER2
//@END_USER2

// Members
private:

protected:
    MarkerVector markers;
    int did;
    int treshhold;

public:

// Methods
private:
    void ConstructorInclude();
    void DestructorInclude();

protected:

public:
    VideoSource();
    virtual ~VideoSource();
    virtual void beginUpdate();
    virtual void close();
    virtual TreeNode* createNode(char* const name, StringMap& attributes);
    virtual void init(StringMap& attributes, const TreeNode* localTree);
    virtual void update();
};

#endif


#ifdef CB_INLINES
#ifndef _VIDEOSOURCE_H_INLINES
#define _VIDEOSOURCE_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
