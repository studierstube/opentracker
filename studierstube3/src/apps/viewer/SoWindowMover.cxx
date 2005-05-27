
#include "SoWindowMover.h"
#include <Inventor/actions/SoSearchAction.h>
#include <stbapi/workspace/StbWinManager.h>
#include <stbapi/workspace/SoContextKit.h>
#include <stbapi/workspace/SoContextManagerKit.h>
#include <stbapi/workspace/SoLocaleKit.h>
#include <stbapi/workspace/SoLocaleManagerKit.h>

//--initialization of kit structure in Inventor
SO_KIT_SOURCE(SoWindowMover);

void SoWindowMover::initClass(void)
{
    SO_KIT_INIT_CLASS(SoWindowMover, SoStationKit, "SoStationKit");
}

SoWindowMover::SoWindowMover()
{
    SO_KIT_CONSTRUCTOR(SoWindowMover);

    SO_KIT_DEFINE_ENUM_VALUE(SoWindowMover, PICK);
    SO_KIT_DEFINE_ENUM_VALUE(SoWindowMover, SNAP);

    SO_KIT_SET_SF_ENUM_TYPE(behaviour, SoWindowMover);

    SO_KIT_ADD_FIELD( behaviour, (PICK) );
    SO_KIT_ADD_FIELD( active, (FALSE));
    SO_KIT_ADD_FIELD( threshold, (0.05));
    SO_KIT_ADD_FIELD( application, (""));
    SO_KIT_ADD_FIELD( window, (-1));

    SO_KIT_INIT_INSTANCE();

	addEventCallback( eventCB );
    addTimeoutCallback( timeCB );
}

SoWindowMover::~SoWindowMover()
{
}

void SoWindowMover::eventCB( void * data, int station, SoStationKit * kit, SoHandle3DEventAction * h3a )
{    	
    SoWindowMover * self = (SoWindowMover *) kit;
    if( self->active.getValue() == TRUE || self->window.getValue() < 0 )
    {
        // we have no window attached, try to get one :)
        windowEntry * entry = StbWinManager::getInstance()->windowTable;
        SoWindowKit * closestWindow = NULL;
        float minDist = self->threshold.getValue();
        while( entry != NULL )
        {		
            if( entry->window != NULL )
            {
                SoWindowKit * window = entry->window;
                if( window->state.getValue() == BM_NORMAL )
                {
                    float dist = (window->getPosition() - self->position.getValue()).length();
                    if( dist < minDist )
                    {
                        closestWindow = window;
                        minDist = dist;
                    }
                }
            }
            entry = entry->next;
        }
        if( closestWindow != NULL )
        {
            SoContextKit * context = SoContextManagerKit::getInstance()->getAnyContextFromNode( closestWindow );
            if( context == NULL )
                return;
            SoGroup * group = SO_GET_PART(context, "windowGroup", SoGroup);
            int index = group->findChild( closestWindow );
            if( index == -1 )
            {
                self->window = -1;
                return;
            }
            self->application.setValue( context->getAppKit()->getName());            
            self->window = index;
            self->visible.setValue( FALSE );				
            switch( self->behaviour.getValue())
            {
            case PICK :
                // ToDo
                //  replace with fixed version.
                self->stationPosition.set1Value(0, (self->position.getValue()-closestWindow->getPosition()));
                self->stationOrientation.set1Value(0, (self->orientation.getValue() * closestWindow->getRotation().inverse()));
                printf("Picked window to kit %i with offset %f %f %f\n", station, self->stationPosition[0][0], 
                    self->stationPosition[0][1], self->stationPosition[0][2] );
                break;
            case SNAP :
                // SbVec3f pos( self->position.getValue());
                // SbRotation rot( self->orientation.getValue());
                // closestWindow->setPosition( pos );
                // closestWindow->setRotation( rot );
                self->moveWindow( closestWindow );
                printf("Snapped window to kit %i\n", station );
                break;
            }
        } 
        
    }
    // in any case, we should now have some info in these fields and do some tracking
    if( self->window.getValue() < 0 )
        return;    
    SoApplicationKit * appKit = SoContextManagerKit::getInstance()->getAppByName( self->application.getValue());
    if( appKit == NULL )
        return;
    SoContextKit * context = appKit->getContext();
    if( context->getNumWindows() <= self->window.getValue())
        return;
    SoWindowKit * window = context->getWindow( self->window.getValue());
    if( window->state.getValue() != BM_NORMAL && self->active.getValue())
    {
        self->application.setValue( "" );
        self->window.setValue( -1 );
    }
    else
    {
        self->moveWindow( window );         
    }
}

void SoWindowMover::moveWindow( SoWindowKit * window )
{
    // get the locale and then a path from the locale to the window
    SoLocaleKit * locale = SoLocaleManagerKit::getInstance()->getLocaleFromNode( window );
    if( locale == NULL )
        return;
    SoSearchAction search;
    search.setNode( window );
    search.setSearchingAll( TRUE );
    search.apply( locale );
    
    SoFullPath * path = (SoFullPath *) search.getPath();
    if( path == NULL )
        return;
    
    // get the transformation from the root 
    SbViewportRegion vp;
    SoGetMatrixAction ma(vp);
    path->ref();     
    ma.apply(path);
	SbMatrix globalToLocal = ma.getInverse();
        
    // transform the rotation part of the 3D event
    SbMatrix rotMatrix;
    orientation.getValue().getValue(rotMatrix);
    SbMatrix resMatrix;
    resMatrix = rotMatrix * globalToLocal;
    SbVec3f t,s; SbRotation rot, so;
    resMatrix.getTransform( t, rot, s, so );

    // transfrom the position part of the 3D event
    SbVec3f pos;
    globalToLocal.multVecMatrix( position.getValue(), pos);
	window->setPosition( pos );
	window->setRotation( rot );                
}

void SoWindowMover::timeCB( void * data, SoStationKit * kit )
{
    SoWindowMover * self = (SoWindowMover *) kit;	
    if( self->behaviour.getValue() == PICK )
    {
	    self->stationPosition.set1Value(0, 0,0,0 );
	    self->stationOrientation.set1Value(0, SbRotation::identity());
    }
	self->visible.setValue( TRUE );
    self->application.setValue("");
    self->window.setValue( -1 );
    printf("Time Out for %i !\n", self->stations[0]);
}