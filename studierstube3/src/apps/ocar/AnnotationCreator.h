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
* PROJECT: ocar
* ======================================================================== */
/**
* ContextKit for standalone and shared display of annotation
*
* @author Gerhard Reitmayr
*
* $Id: AnnotationCreator.h 3426 2004-07-14 12:11:27Z tamer $
* @file                                                                   */
/* ======================================================================= */


#ifndef _ANNOTATIONCREATOR_H_
#define _ANNOTATIONCREATOR_H_

#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFPath.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFEngine.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodekits/SoSubKit.h>

#include "ocar.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**	
 * This class displays annotation information.
 * @author Gerhard Reitmayr
 * @ingroup ocar
 */
class OCAR_API AnnotationCreator : public SoBaseKit  
{
    SO_KIT_HEADER(AnnotationCreator);
    
public:

    static void initClass(void);    
    
    AnnotationCreator();        

    /// switch for hidden parts that must not be traversed
    SO_KIT_CATALOG_ENTRY_HEADER(hidden);
    /// a scene graph describing a marker 
    SO_KIT_CATALOG_ENTRY_HEADER(markerTemplate);
    /// the master list of all icons, indices correspond to the indexMap engines fields
    SO_KIT_CATALOG_ENTRY_HEADER(icons);
    /// our mini tuple space, each annotation app creates a group node in there and adds its markers to it
    SO_KIT_CATALOG_ENTRY_HEADER(markers);

    /** @name Configuration data fields
     *  hold configuration data that is not expected to change at runtime. 
     * In a future distributed model with database servers, these could be 
     * the interfaces to update the clients.
     */
    /*@{*/

    /// reference to the id to index map, necessary to update the state of the index engine
    SoSFEngine indexMap;

    /// user annotation list box keywords for additional selection of these items
    SoMFString markerKeywords;

    /// another map engine to do the user annotation selections
    SoSFEngine markerMap;

    /// the user name for marker data
    SoSFString name;

    /*@}*/
        
    /** @name Input data fields
     * These will only be read by the
     * navigation application and trigger updates in the state.
     */
    /*@{*/

    /// current 3D eye cursor position, only valid if isPicking is TRUE !
    SoSFVec3f cursorPosition;

    /// normal at the picked point, only valid if isPicking is TRUE !
    SoSFVec3f pickNormal;

    /// flag to denote if raypicker is picking something
    SoSFBool isPicking;

    /// the current icon template node to use
    SoSFNode iconTemplate;

    /// the current material template node to use
    SoSFNode colorTemplate;

    /// the current state of the button of the picking device, only valid if isPicking is TRUE !
    SoSFInt32 pickButton;


    /*@}*/
        
    /** @name Output data fields
     * These will only be written to by the
     * navigation application. Writing to these fields does not do anything.
     */
    /*@{*/

    SoMFString selectedWords;

    SoSFString selectedBuilding;

    SoSFInt32 selectedIcon;

    /*@}*/        

protected:
	virtual ~AnnotationCreator();

    /** This will detach/attach the fieldSensor.
     * It is called at the end of the constructor (to attach).
     * and at the start/end of SoBaseKit::readInstance()
     * and on the new copy at the start/end of SoBaseKit::copy().
     * Returns the state of the node when this was called.
     */
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

    SoFieldSensor buttonSensor;
    SoNodeSensor markerSensor;
    int oldButton;

    static void buttonChanged( void * data, SoSensor * sensor );
    static void markerChanged( void * data, SoSensor * sensor );

    /// creates a marker icon using the currently set templates
    void createMarker(void);

    /** adds a new marker and takes care of setting up all the engines correctly
     * now put the result into the scene graph, need to do two things here :
     * @li add to the icons 
     * @li set appropriate keywords and id for the new marker. 
     *
     * To do so it assumes a few things :
     * @li the def name of the passed node is a unique id going to be used in the maps
     * @li there exists an Info node with def name USER that contains the users name which 
     *     is used for the maps
     * @li there exists an Info node with def name COLOR that contains the color name which
     *     is used for the maps
     */
    void addMarker( SoNode * marker );
};

#endif // !defined(_ANNOTATIONCREATOR_H_)
