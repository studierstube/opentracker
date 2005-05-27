
#include "SoAppControl.h"
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/SbBox.h>
#include <stbapi/workspace/SoApplicationKit.h>
#include <stbapi/workspace/distrManager/StbDistrManager.h>
#include <stbapi/resource/SoUserManagerKit.h>
#include <stbapi/workspace/SoContextManagerKit.h>
#include <stbapi/workspace/SoLocaleManagerKit.h>
#include <stbapi/workspace/SoLocaleKit.h>

SO_KIT_SOURCE(SoAppControl);

// class initialisation for OpenInventor

void SoAppControl::initClass(void)
{
    SO_KIT_INIT_CLASS(SoAppControl, SoStationKit, "SoStationKit");   
}

SoAppControl::SoAppControl()
: suppressionTimer( 0.0 )
{
    SO_KIT_CONSTRUCTOR(SoAppControl);  
        
    SO_KIT_ADD_FIELD( locale, (""));
	SO_KIT_ADD_FIELD( application, (""));
	SO_KIT_ADD_FIELD( becomeMaster, (FALSE));

    SO_KIT_DEFINE_ENUM_VALUE( Stop, NEVER );
    SO_KIT_DEFINE_ENUM_VALUE( Stop, ALWAYS );
    SO_KIT_DEFINE_ENUM_VALUE( Stop, SLAVE );
    SO_KIT_DEFINE_ENUM_VALUE( Stop, MASTER );
    SO_KIT_SET_SF_ENUM_TYPE( shouldStop, Stop );
	SO_KIT_ADD_FIELD( shouldStop, (NEVER));

    SO_KIT_ADD_FIELD( min, (-100, -100, -100 ));
    SO_KIT_ADD_FIELD( max, (100, 100, 100 ));
        
    SO_KIT_INIT_INSTANCE();

	addEventCallback( SoAppControl::eventCB, this );
	addTimeoutCallback( SoAppControl::timeoutCB, this );
}

SoAppControl::~SoAppControl()
{

}

void SoAppControl::handle3DEvent(SoHandle3DEventAction *h3a)
{    
    SoLocaleKit * localeKit = SoLocaleManagerKit::getInstance()->getLocale( locale.getValue().getString());
    if( localeKit == NULL )
        return;
    SbXfBox3f box( min.getValue(), max.getValue());
    SbMatrix trans;
    trans.setTransform( localeKit->getMover()->translation.getValue(), 
                        localeKit->getMover()->rotation.getValue(),
                        SbVec3f(1,1,1), 
                        SbRotation(SbVec3f(1,0,0),0));
    box.setTransform( trans );
    SbVec3f pos = h3a->getEvent()->getTranslation();
    if( box.intersect( pos ))    
        SoStationKit::handle3DEvent( h3a );
 }

void SoAppControl::eventCB( void *userData, int stationId, 
					SoStationKit *stationKit, SoHandle3DEventAction *action)
{
	SoAppControl * self = (SoAppControl *) userData;
	// try maximal every 10 seconds to load the application
    SoApplicationKit * kit = SoContextManagerKit::getInstance()->getAppByName( self->application.getValue());
	// did we do our job already ?
	if( kit != NULL )
    {
    //    printf("App %s node %x found !\n", kit->getName().getString(), kit );
		return;
    }
	if( SbTime::getTimeOfDay() - self->suppressionTimer < SbTime( 10.0 ))
    {
    //    printf("suppression hit !\n");
		return;
    }
	self->suppressionTimer = SbTime::getTimeOfDay();
	// printf("attaching to application %s\n", self->application.getValue().getString());
	StbDistrManager::getInstance()->attachApplication( self->application.getValue().getString(), self->locale.getValue(),
		SoUserManagerKit::getInstance()->getUserKitSequentially(0)->userID.getValue());
	if( self->becomeMaster.getValue())
	{
		SoTimerSensor * timer = new SoTimerSensor( getMasterCB, self );
		timer->setBaseTime( SbTime::getTimeOfDay() + 2.0);
        timer->setInterval( 5.0 );
        timer->schedule();
	}
}

void SoAppControl::timeoutCB( void *userData, SoStationKit *stationKit)
{	
	SoAppControl * self = (SoAppControl *) userData;
	// printf("Timeout for app %s\n", self->application.getValue().getString());

    SoApplicationKit * kit = NULL;
	switch(self->shouldStop.getValue())
    {
    case NEVER : return;
    case ALWAYS : break;
    case SLAVE :
    case MASTER :
        kit = SoContextManagerKit::getInstance()
                                 ->getAppByName( self->application.getValue());
        if( kit != NULL )
            if( kit->getContext() != NULL )
                if( kit->getContext()->getMasterMode() == 
                    (self->shouldStop.getValue() == MASTER) )
                    break;
        // but we try again, maybe something has changed
        self->timer.setTimeFromNow( self->timeOut.getValue());
        self->timer.schedule();
        return;
    default :
        assert(false);
    }
    kit = SoContextManagerKit::getInstance()
                                 ->getAppByName( self->application.getValue());
    if( kit == NULL )
        return;
	SoContextManagerKit::getInstance()->stopApplication(
		kit->getContext()->appID.getValue());
}

void SoAppControl::getMasterCB( void * data, SoSensor * sensor )
{
	SoAppControl * self = (SoAppControl *) data;
	// printf("Mastercallback for app %s\n", self->application.getValue().getString());
	
    SoApplicationKit * kit = SoContextManagerKit::getInstance()->getAppByName( self->application.getValue());
	if( kit == NULL )
		return;
	if( !kit->getContext()->getMasterMode())
		kit->getContext()->getDivObject()->getMaster.setValue();
	else
		sensor->unschedule();
}