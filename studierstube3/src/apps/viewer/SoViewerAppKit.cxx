/* ========================================================================
 * Copyright (C) 2000,2001,2002  Vienna University of Technology
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: viewer
 * ======================================================================== */
/**
 * Source file for StbViewerApp a simple test application
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoViewerAppKit.cxx 3426 2004-07-14 12:11:27Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#pragma warning(disable:4786)

#include <assert.h>
#include <string>

using namespace std;

#include <Inventor/nodes/SoSphere.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoOneShotSensor.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoInfo.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/util/ivio.h>
#include <stbapi/resource/SoPipSheetKit.h>
#include <stbapi/workspace/StbWorkspace.h>
#include <stbapi/tracker/SoStationKit.h>
#include <stbapi/workspace/distrManager/StbDistrManager.h>
#include <stbapi/workspace/SoLocaleKit.h>
#include <stbapi/workspace/SoLocaleManagerKit.h>
#include <stbapi/resource/SoUserManagerKit.h>
//#include <stbapi/misc/SoMultiSwitch.h>
//#include <stbapi/tracker/SoPartitionKit.h>
#include "SoViewerAppKit.h"
// #include "SoWindowMover.h"
#include "SoAppControl.h"

//--initialization of kit structure in Inventor
SO_KIT_SOURCE(SoViewerAppKit);

void SoViewerAppKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoViewerAppKit, SoContextKit, "SoContextKit");
//    SoWindowMover::initClass();
	SoAppControl::initClass();
}

SoViewerAppKit::SoViewerAppKit()
{
    SO_KIT_CONSTRUCTOR(SoViewerAppKit);

    SO_KIT_ADD_CATALOG_ENTRY(toAuxGeom,       SoGroup,     TRUE, this,    \x0, TRUE );

    SO_KIT_INIT_INSTANCE();


}

SoViewerAppKit::~SoViewerAppKit()
{
}

/*
SbBool switchCB( void * userdata, SoMultiSwitch * switchNode, 
                 SoAction * action, int number, SoNode * child )
{
    printf("Switch CB with action %s, node %i\n", action->getTypeId().getName().getString(), number );
    // return number % 2;
    return TRUE;
}

void fieldCB( void * userdata, SoSensor * sensor )
{
    SoFieldSensor * fs = (SoFieldSensor *) sensor;
    printf("activeChild is %i\n", ((SoSFInt32*)fs->getAttachedField())->getValue() );
}
*/

void buttonCB( void * userdata, So3DButton * button )
{
	/*
	SoLocaleManagerKit::getInstance()->createLocale( "faust" );
	StbDistrManager::getInstance()->joinLocale( "faust" );
	SoNodeList list;
    int userId = SoUserManagerKit::getInstance()->getUserKitSequentially(0)->userID.getValue();
    SoContextManagerKit::getInstance()->loadApplication( "test.iv", list, userId, "faust" );
	StbDistrManager::getInstance()->attachApplication( "test", "faust", userId );
	*/
}

SbBool SoViewerAppKit::checkPipGeometry()
{
	SoSearchAction search;
	SoNode * pip = templatePipSheet.getValue();
	if( pip != NULL )
	{
		search.setName("SHARE_LOCALE");
		search.apply( pip );
		SoFullPath * path = (SoFullPath *) search.getPath();
		if( path != NULL )
		{
			So3DButton * button = (So3DButton *) path->getTail();
			button->addReleaseCallback( buttonCB, this );
		}
	}
	return TRUE;
}

void SoViewerAppKit::startUp( void * data, SoSensor * sensor )
{

    SoViewerAppKit * self = (SoViewerAppKit *) data;

    if (self->toAuxGeom.getValue() != NULL) {
		printf("SoViewerAppKit: toAuxGeom set!!\n");
		StbWorkspace::getInstance()->preRoot->addChild(self->toAuxGeom.getValue());
		self->toAuxGeom.setValue(NULL);
	}
	else {
		printf("SoViewerAppKit: toAuxGeom NOT set!!\n");
	}

	SoGroup * windows = (SoGroup*)self->windowGroup.getValue();
    if( windows->getNumChildren() > 0 )
    {
        SoSearchAction search;
        search.setType( SoInfo::getClassTypeId());
        search.setName( "CREATE" );
        search.apply( windows );
        
        if( search.getPath() != NULL )
        {
            SoInfo * info = (SoInfo *)search.getPath()->getTail();

            string create = info->string.getValue().getString();
            string locale;
            int start = 0;
            int end = create.find(' ');
            while( end != string::npos )
            {
                locale = create.substr( start, end - start );
                if( locale.length() > 0 )
                {
                    int token = locale.find('_');
                    int station = -1;
                    if( token != string::npos )
                    {
                        station = atoi( locale.substr( token+1, locale.length() - token - 1).c_str());
                        locale = locale.substr(0, token );
                    }
                    SoLocaleKit * localeKit = SoLocaleManagerKit::getInstance()
                        ->createLocale( locale.c_str());
                    if( station != -1 )
                        localeKit->station.setValue( station );
                    printf("Viewer: Created locale %s - %d\n", locale.c_str(), station);
                }
                start = end+1;
                end = create.find(' ', start );
            }
            locale = create.substr( start, create.length() - start);
            int token = locale.find('_');
            int station = -1;
            if( token != string::npos )
            {
                station = atoi( locale.substr( token+1, locale.length() - token - 1).c_str());
                locale = locale.substr(0, token );
            }
            SoLocaleKit * localeKit = SoLocaleManagerKit::getInstance()
                ->createLocale( locale.c_str());
            if( station != -1 )
                localeKit->station.setValue( station );
            printf("Viewer: Created locale %s - %d\n", locale.c_str(), station);
        }

        search.setName( "JOIN" );
        search.apply( windows );
        if( search.getPath() != NULL )
        {
            SoInfo * info = (SoInfo *)search.getPath()->getTail();

            string create = info->string.getValue().getString();
            string locale;
            int start = 0;
            int end = create.find(' ');
            while( end != string::npos )
            {
                locale = create.substr( start, end - start );
                if( locale.length() > 0 )
                {
                    StbDistrManager::getInstance()->joinLocale(locale.c_str());
                    printf("Viewer: joined locale %s\n", locale.c_str());
                }
                start = end+1;
                end = create.find(' ', start );
            }
            locale = create.substr( start, create.length() - start);
            if( locale.length() > 0 )
            {
                StbDistrManager::getInstance()->joinLocale( locale.c_str());
                printf("Viewer: joined locale %s\n", locale.c_str());
            }
        }

        search.reset();
        search.setType( SoInfo::getClassTypeId());
        search.setInterest( SoSearchAction::ALL );
        search.apply( windows );

        SoPathList list = search.getPaths();
        for( int i = 0; i < list.getLength(); i++ )
        {
            SoInfo * info = (SoInfo *)list[i]->getTail();
            int userId = SoUserManagerKit::getInstance()->getUserKitSequentially(0)->userID.getValue();
            SoNodeList nodeList;
            if( SoLocaleManagerKit::getInstance()->getLocale(info->getName()) != NULL )
            {
                printf("Viewer: starting in locale %s\n", info->getName().getString());
                SbName & name = info->getName();
                string create = info->string.getValue().getString();
                string app;
                int start = 0;
                int end = create.find(' ');
                while( end != string::npos )
                {
                    app = create.substr( start, end - start );
					if( app.length() > 0 )
                    {
						if( app[0] == '_' ) // attach only
						{
							app = app.substr( 1, app.length() - 1 );
							StbDistrManager::getInstance()->attachApplication( app.c_str(), name, userId );
						}
						else                    
							SoContextManagerKit::getInstance()->loadApplication( app.c_str(), nodeList, userId, name );
                        printf("  %s\n", app.c_str());
                    }
                    start = end+1;
                    end = create.find(' ', start );
                }
                app = create.substr( start, create.length() - start);
                if( app.length() > 0 )
                {
					if( app[0] == '_' ) // attach only
					{
						app = app.substr( 1, app.length() - 1 );
						StbDistrManager::getInstance()->attachApplication( app.c_str(), name, userId );
					}
					else
						SoContextManagerKit::getInstance()->loadApplication( app.c_str(), nodeList, userId, name );
                    printf("  %s\n", app.c_str());
                }
            }
        }        
    }
}

SbBool SoViewerAppKit::checkWindowGeometry()
{
    SoOneShotSensor * oneShot = new SoOneShotSensor( SoViewerAppKit::startUp, this );
    oneShot->schedule();
	return TRUE;
}
