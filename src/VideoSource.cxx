// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      VideoSource.cxx
//  TYPE:      cxx header file
//  PROJECT:   TrackerServer
//  CONTENT:   Implementation of class 'VideoSource'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// ===========================================================================
//@START_USER1
//@END_USER1


// Master include file
#include "TrackerServer.h"


//@START_USER2
#include <AR/ar.h>
#include <AR/video.h>
#include <AR/param.h>
#ifdef WIN32
#include <iostream>    // VisualC++ has two incompatible iostreams libraries !
#else
#include <iostream.h>
#endif
#include <string>
#include <stdio.h>
#ifdef __sgi
#include <unistd.h>
#endif
//@END_USER2


// Static members


/*@NOTE_3778
Constructor method.
*/
VideoSource::VideoSource() //@INIT_3778
    : Module()
    , NodeFactory()
    , treshhold(100)
{//@CODE_3778
    ConstructorInclude();

    // Put in your own code
}//@CODE_3778


/*@NOTE_788
Destructor method.
*/
VideoSource::~VideoSource()
{//@CODE_788
    DestructorInclude();

    // Put in your own code
}//@CODE_788


/*@NOTE_4565
Called before the update cycle. Sets all markers to invalid, so they have a 
reasonable state.
*/
void VideoSource::beginUpdate()
{//@CODE_4565
    MarkerVector::iterator it = markers.begin();
    while( it != markers.end())
    {
        (*it)->getState()->isValid = 0;
	    it++;
    }
}//@CODE_4565


/*@NOTE_3592
closes the tracker source. a place for cleanup code etc.
This class also provides an empty implementation for subclasses not doing
anything here.
*/
void VideoSource::close()
{//@CODE_3592
#ifdef __sgi
    arVideoStop( did );
    arVideoCleanupDevice( did );
#endif
    arVideoClose();
}//@CODE_3592


/*@NOTE_3595
This method is called to construct a new Node. It takes the element name and
any attribute name, value pairs and returns the new Node, if it is able to 
construct the corresponding type. Otherwise it returns NULL.
*/
Node* VideoSource::createNode(char* const name, StringMap& attributes)
{//@CODE_3595
    std::string strName( name );
    if( strName.compare("Marker") == 0 )
    {
        double vertex[4][2];
        int num = sscanf( attributes["vertex"], " %lf %lf %lf %lf %lf %lf %lf %lf",
                &vertex[0][0], &vertex[0][1],
                &vertex[1][0], &vertex[1][1],
                &vertex[2][0], &vertex[2][1],
                &vertex[3][0], &vertex[3][1] );
        if( num != 8 )
        {
            cout << "Wrong number of vertices : " << num << endl;
            return NULL;
        }
        int id;
        if((id = arLoadPatt((char *) attributes["tag-file"] )) < 0 )
        {
            cout << "Error reading tag-file " << attributes["tag-file"] << endl;
            return NULL;
        }
        Marker * marker = new Marker( id, vertex );
        markers.push_back( marker );
        cout << "Build marker " << attributes["tag-file"] << endl;
        return marker;
    }
    return NULL;
}//@CODE_3595


/*@NOTE_3593
This method initializes the video system and the ARToolKit library. It expects
the following parameters :

camera-parameter filename with the camera parameters #REQUIRED

treshhold               threshhold value for image processing #IMPLIED
*/
void VideoSource::init(StringMap& attributes, const Node* localTree)
{//@CODE_3593
    int sizeX, sizeY;
    ARParam cparam, wparam;

    // checking for and reading of parameter treshhold    
    StringMap::iterator tresh = attributes.find("treshhold");
 
    if( tresh != attributes.end())
    {
        int num = sscanf(attributes["treshhold"], " %i", &treshhold );
        if( num == 0 )
        {
            treshhold = 100;
        }
        else
        {
            if( treshhold < 0 )
                treshhold = 0;
            else if( treshhold > 255 )
                treshhold = 255;
        }
    } 
 
    if( arVideoOpen() < 0 )
    {
        cout << "Error opening video source !" << endl;
        exit(1);
    }
    
#ifdef __sgi
    if( arVideoSetupDevice( did, AR_VIDEO_INTERLEAVED, AR_VIDEO_RGB_8, AR_VIDEO_1_P_1 ) < 0 )
    {
        cout << "Error setting up video source !" << endl;
        exit(1);
    }
    if( arVideoInqSize(did, &sizeX, &sizeY ) < 0 )           
#else
    if( arVideoInqSize( &sizeX, &sizeY ) < 0 )
#endif
    {
        cout << "Error querying video size !" << endl;
        exit(1);
    }
    
    if( arParamLoad((char *) attributes["camera-parameter"], 1, &wparam ) < 0 )
    {
        cout << "Error loading camera parameters !" << endl;
        exit(1);        
    }
    arParamChangeSize( &wparam, sizeX, sizeY, &cparam );
    
#ifdef __sgi
    arResampleFlag = 1;
#else
    arResampleFlag = 0;
#endif
    arInitCparam( &cparam );
    
#ifdef __sgi
    arVideoStart( did );
#endif                
    
    cout << "Module VideoSource initialized." << endl;
}//@CODE_3593


/*@NOTE_3594
This method does the main work. It graps a video frame and checks for any 
recognizable markers. Then it updates the markers accordingly.
*/
void VideoSource::update()
{//@CODE_3594
    ARUint8 * frameData;
    ARMarkerInfo * markerInfo;
    int markerNum;
    int j,k;    
    double matrix[3][4];
    float m[3][3];

#ifdef __sgi
    if((frameData = (ARUint8 *)arVideoGetImage(did)) == NULL )
    {
        sginap(1);
#else
    if((frameData = (ARUint8 *)arVideoGetImage()) == NULL )
    {
#endif
        return;
    }
    if( arDetectMarker( frameData, treshhold, &markerInfo, &markerNum ) < 0 )
    {
        return;
    }
    if( markerNum < 1 )
    {
        return;
    }
            
    MarkerVector::iterator it = markers.begin();    
    while( it != markers.end())
    {        
        k = -1;
        for( j = 0; j < markerNum; j ++ )
        {
            if( (*it)->getMarkerId() == markerInfo[j].id )
            {
                if( k == -1 )
                {
                    k = j;
                }
                else
                {
                    if( markerInfo[k].cf < markerInfo[j].cf )
                    {
                        k = j;
                    }
                }
            }
        }
        if( k != -1 )                          
        {
            if( arGetTransMat( &markerInfo[k], (double(*)[2])(*it)->getVertex(), matrix ) >= 0 )
            {
                State * state = (*it)->getState();
                state->isValid = 1;
                state->confidence = markerInfo[k].cf;
                // TODO put this method somewhere !!!
        		for( int r = 0; r < 3; r ++ )
	        	{
		            for( int s = 0; s < 3; s ++ )
		            {
        			    m[r][s] = (float)matrix[r][s];
    		        }
        		}
                MathUtils::matrixToQuaternion( m, state->orientation );
                state->position[0] = matrix[0][3];
                state->position[1] = matrix[1][3];
                state->position[2] = matrix[2][3];
                cout << "Marker " << (*it)->getMarkerId() << " updated !" << endl;
            }
        }
    	it++;
    }    
}//@CODE_3594


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_787
Method which must be called first in a constructor.
*/
void VideoSource::ConstructorInclude()
{
}


/*@NOTE_789
Method which must be called first in a destructor.
*/
void VideoSource::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
