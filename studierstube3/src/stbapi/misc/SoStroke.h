/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/**
 * header file for SoStroke geometry kit
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoStroke.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SOSTROKE_H_
#define _SOSTROKE_H_

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <stbapi/stbapi.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoComputeBoundingBox;

/**
 * This class provides a stroke geometry similar to strokes in vector drawing
 * programs. It takes a line defined as points and any geometry and will stretch the
 * geometry to fit onto each segment of the line.  
 * @author Gerhard Reitmayr
 * @ingroup misc
 * @todo poly line implementation, right now it only uses the first segment.
 */
class STBAPI_API SoStroke : public SoBaseKit  
{
    SO_KIT_HEADER(SoStroke);
    
    SO_KIT_CATALOG_ENTRY_HEADER(root);
    SO_KIT_CATALOG_ENTRY_HEADER(transform);
    SO_KIT_CATALOG_ENTRY_HEADER(geometry);
    
public:

    static void initClass(void);

    SoStroke();

    virtual ~SoStroke();

    SoMFVec3f point;

    enum Scale { UNIFORM, DIRECTION };

    SoSFEnum scale;

protected:
    SoSFVec3f min;
    SoSFVec3f max;
    SoSFVec3f center;

    /** This will detach/attach the fieldSensor.
     * It is called at the end of the constructor (to attach).
     * and at the start/end of SoBaseKit::readInstance()
     * and on the new copy at the start/end of SoBaseKit::copy().
     * Returns the state of the node when this was called.
     */
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

    static void parameterChanged( void * data, SoSensor * sensor );

    SoFieldSensor bboxSensor;
    SoFieldSensor pointSensor;
    SoFieldSensor scaleSensor;

    SoComputeBoundingBox * computeBB;
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // !defined(_SOSTROKE_H_)
