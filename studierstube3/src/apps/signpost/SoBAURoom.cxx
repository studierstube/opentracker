/* ========================================================================
  * Copyright (C) 2000  Vienna University of Technology
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
  * PROJECT: Studierstube
  * ======================================================================== */ 

 /* ======================================================================= */
/** SignPost BAU
  *
  * @author Michael Knapp <knapp@cg.tuwien.ac.at>
  *
  * $Id: SoBAURoom.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */


#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/actions/SoSearchAction.h>

#include <stbapi/context/SoContextSwitch.h>
#include <stbapi/context/SoNodeContextReport.h>
#include <stbapi/context/SoContextSeparator.h>
#include <float.h>

#include "SoBAURoom.h"

SO_KIT_SOURCE(SoBAURoom);

//----------------------------------------------------------------------------

void
SoBAURoom::initClass(void)
{
    SO_KIT_INIT_CLASS(SoBAURoom, SoBaseKit, "BaseKit");
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoBAURoom::SoBAURoom()
{
    SO_KIT_CONSTRUCTOR(SoBAURoom);

	SO_KIT_ADD_FIELD(roomName, ("***"));
	SO_KIT_ADD_FIELD(portalNames, (""));
	SO_KIT_ADD_FIELD(neighbors, (""));
	SO_KIT_ADD_FIELD(portalPositions, (0, 0, 0));
	SO_KIT_ADD_FIELD(floorCoordinates, (NULL));

    floorCoordinates.deleteValues(0);

	SO_KIT_ADD_CATALOG_ENTRY(room,          SoContextSeparator,        FALSE, this,    \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(roomCoords,    SoCoordinate3,      FALSE, room,    \x0, TRUE);

    SO_KIT_ADD_CATALOG_ENTRY(allAppearance,  SoNodeContextReport,    FALSE, room,  \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(wallSeparator,     SoSeparator,        FALSE, room,    \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(floorSeparator,    SoSeparator,        FALSE, room,    \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(ceilingSeparator,  SoSeparator,        FALSE, room,    \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(portalSeparator,   SoSeparator,        FALSE, room,    \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(specialSeparator,  SoSeparator,        FALSE, room,    \x0, FALSE);

	SO_KIT_ADD_CATALOG_ENTRY(wallAppearance,       SoNodeContextReport,    FALSE,    wallSeparator,     \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(floorAppearance,      SoNodeContextReport,    FALSE,    floorSeparator,    \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(ceilingAppearance,    SoNodeContextReport,    FALSE,    ceilingSeparator,  \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(portalAppearance,     SoNodeContextReport,    FALSE,    portalSeparator,   \x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(specialAppearance,    SoNodeContextReport,    FALSE,    specialSeparator,  \x0, TRUE);

	SO_KIT_ADD_CATALOG_ENTRY(wallFaceSet,       SoIndexedFaceSet,    FALSE,    wallSeparator,     \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(floorFaceSet,      SoIndexedFaceSet,    FALSE,    floorSeparator,    \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(ceilingFaceSet,    SoIndexedFaceSet,    FALSE,    ceilingSeparator,  \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(portalFaceSet,     SoIndexedFaceSet,    FALSE,    portalSeparator,   \x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(specialFaceSet,    SoIndexedFaceSet,    FALSE,    specialSeparator,  \x0, TRUE);

	SO_KIT_ADD_CATALOG_ENTRY(portalGeometryContextSwitch,     SoContextSwitch,    FALSE,    portalSeparator,   \x0, TRUE);

    SO_KIT_INIT_INSTANCE();

    setUpConnections( TRUE, TRUE );
}

//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------

SoBAURoom::~SoBAURoom()
{

}

//----------------------------------------------------------------------------
SbBool 
SoBAURoom::setUpConnections(SbBool onoff, SbBool doitalways)
{
    if ((!doitalways) && (connectionsSetUp == onoff)) return onoff;
    if (onoff) 
    {
        // We connect AFTER base class.
        SoBaseKit::setUpConnections(onoff, doitalways);

        SoContextSwitch * cs = (SoContextSwitch *)getPart("portalGeometryContextSwitch", TRUE);
        cs->index.setValue("BAU.portal.switch");
        cs->defaultChild.setValue(-3);

        SO_GET_PART(this, "allAppearance", SoNodeContextReport)->index.setValue("BAU.all");
        SO_GET_PART(this, "wallAppearance", SoNodeContextReport)->index.setValue("BAU.wall");
        SO_GET_PART(this, "floorAppearance", SoNodeContextReport)->index.setValue("BAU.floor");
        SO_GET_PART(this, "ceilingAppearance", SoNodeContextReport)->index.setValue("BAU.ceiling");
        SO_GET_PART(this, "portalAppearance", SoNodeContextReport)->index.setValue("BAU.portal");
        SO_GET_PART(this, "specialAppearance", SoNodeContextReport)->index.setValue("BAU.special");
        
        allAppearance.enableNotify(FALSE);
        floorAppearance.enableNotify(FALSE);
        wallAppearance.enableNotify(FALSE);
        ceilingAppearance.enableNotify(FALSE);
        portalAppearance.enableNotify(FALSE);
        specialAppearance.enableNotify(FALSE);

        wallSeparator.enableNotify(FALSE);
        floorSeparator.enableNotify(FALSE);
        ceilingSeparator.enableNotify(FALSE);
        portalSeparator.enableNotify(FALSE);
        specialSeparator.enableNotify(FALSE);
    }
    else 
    {
        // We disconnect BEFORE base class.
        // do your own special setup here !


        SoBaseKit::setUpConnections(onoff, doitalways);
    }
    return !(connectionsSetUp = onoff);

}

//----------------------------------------------------------------------------

bool 
SoBAURoom::inRoom(const SbVec3f &pt)
{
    for (int n=0; n<floorCoordinates.getNum(); n++)
    {
        SoMFVec3f &points = ((SoCoordinate3 *)floorCoordinates[n])->point;

	    float x, y, h, xa, ya, ha, xn, yn, hn, ymin, ymax, xbelow, xabove, hbelow, habove;
	    int below = 0;
	    int above = 0;
        int numpoints = points.getNum();

	    pt.getValue(x, h, y); 

	    xa = points[numpoints-1][0];
	    ya = points[numpoints-1][2];
	    ha = points[numpoints-1][1];
        xbelow = -FLT_MAX;
        xabove = +FLT_MAX;
	    for (int i=0; i<numpoints; i++)
	    {
		    xn = points[i][0];
		    yn = points[i][2];
		    hn = points[i][1];
		    if (ya > yn)
		    {
			    ymin = yn;
			    ymax = ya;
		    }
		    else
		    {
			    ymin = ya;
			    ymax = yn;
		    }
		    if ((y >= ymin) && (y <= ymax))
		    {
			    float xd = xn - xa;
			    float yd = yn - ya;
			    float hd = hn - ha;
			    float xx = xa + ((y - ya) / yd) * xd;
			    float hh = ha + ((y - ya) / yd) * hd;
			    if (xx < x) 
                {
                    below++;
                    if (xx > xbelow) 
                    {
                        hbelow = hh;
                        xbelow = xx;
                    }
                }
			    if (xx > x) 
                {
                    above++;
                    if (xx < xabove) 
                    {
                        habove = hh;
                        xabove = xx;
                    }
                }
		    }

		    xa = xn;
		    ya = yn;
		    ha = hn;
	    }
    //printf("%s: B:%i G:%i - (%f,%f) - h:%f\n", roomname.getValue().getString(), lower, greater, x, y, h);
	    if ((below & above & 1) == 1) 
        {
            float xd = xabove - xbelow;
            float hd = habove - hbelow;
            float hh = hbelow + ((x - xbelow) / xd) * hd;
            return ((h > hh-0.5) && (h < hh+2.5));
        }
    }
    return false;
}

int 
SoBAURoom::getPortalIndex(const SbString &neighborname)
{
    return portalNames.find(neighborname);
}


//----------------------------------------------------------------------------
//eof
//----------------------------------------------------------------------------

