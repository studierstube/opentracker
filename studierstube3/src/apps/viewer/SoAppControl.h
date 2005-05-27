
#ifndef _SOAPPCONTROL_H_
#define _SOAPPCONTROL_H_

#include "viewer.h"
#include <stbapi/tracker/SoStationKit.h>
#include <Inventor/fields/SoSFName.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFVec3f.h>

class VIEWER_API SoAppControl : public SoStationKit  
{

	SO_KIT_HEADER(SoAppControl);

public:
	SoAppControl();
	virtual ~SoAppControl();
	static void initClass(void);

    enum { NEVER = 0, ///< will never try to stop the application
           ALWAYS,    ///< will always try to stop the application
           SLAVE,     ///< will only stop the application, if it is a slave
           MASTER     ///< will only stop the application, if it is a master
    } Stop;

	/// the locale to share the app in ...
	SoSFName locale;
	
	/// the name of the application to share,
	SoSFName application;

	/// try to become master after sharing goes through
	SoSFBool becomeMaster;

	/// upon timeout
	SoSFEnum shouldStop;

    /// minimum corner of area of interest
    SoSFVec3f min;

    /// maximum corner of area of interest
    SoSFVec3f max;

    virtual void handle3DEvent(SoHandle3DEventAction *h3a);

protected:
    
	static void eventCB( void *userData, int stationId, 
		SoStationKit *stationKit, SoHandle3DEventAction *action);

	static void timeoutCB( void *userData, SoStationKit *stationKit);

	static void getMasterCB( void * data, SoSensor * sensor );

	SbTime suppressionTimer;
};

#endif // _SOAPPCONTROL_H_
