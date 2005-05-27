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
 * header file for SoScreenCapture
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoScreenCapture.h 3425 2004-07-14 12:09:17Z tamer $
 *
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SOSCREENCAPTURE_H_
#define _SOSCREENCAPTURE_H_

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoSFImage.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <stbapi/stbapi.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * This class provides a simple way to do two things. Firstly, to capture the
 * current image on the screen, therefore requiring no additional offscreen
 * rendering or other tricks. And secondly, to write the contents of an image
 * to a file. It depends on the Qt library to write out the images and will
 * not do anything, if Studierstube is not linked with it.
 *
 * Capturing the screen is triggered by sending an event to the trigger field.
 * The contents of the capture are then available in the screen field. Furthermore
 * if writeImage is enabled, every change to the screen field will be written
 * to a file. The file is written in PNG format. The filename consists of
 * the value of the field baseName and a running counter stored in the field
 * count. After an image is written to file, count is increased by one.
 *
 * File format :
@verbatim
SoScreenCapture {
	SoSFTrigger trigger
	SoSFImage   screen
	SoSFString  baseName   ""
	SoSFInt32   count      0
	SoSFBool    writeImage TRUE
}
@endverbatim
 *
 * @author Gerhard Reitmayr
 * @ingroup util
 */
class STBAPI_API SoScreenCapture : public SoNode
{

SO_NODE_HEADER(SoScreenCapture);

public:

    /// trigger input field, triggers screen capture
    SoSFTrigger trigger;
	/// contains the last captured image or any other image set
    SoSFImage screen;
	/// the basename of the files written
    SoSFString baseName;
    /// counter that is appended to the basename
    SoSFInt32 count;
    /// flag whether images should be written to files
    SoSFBool writeImage;

    /** static method to initialize class for OpenInventor. It also
     * initializes SoGroupSwitchElement and enables it for all necessary
     * actions.
     */
    static void initClass();

    /// constructor
    SoScreenCapture();

protected:

    /// destructor
    virtual ~SoScreenCapture();

    static void triggerFired( void * data, SoSensor * sensor );
    static void imageChanged( void * data, SoSensor * sensor );

    SoFieldSensor triggerSensor;
    SoFieldSensor imageSensor;
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif
