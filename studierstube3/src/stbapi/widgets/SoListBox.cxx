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
/** implementation of listbox widget
  *
  * @author  Gerhard Reitmayr
  *
  * $Id: SoListBox.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <cassert>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoAsciiText.h>
#include <Inventor/actions/SoSearchAction.h>

#include <stbapi/widgets/SoPushButton.h>
#include <stbapi/widgets/SoToggleButton.h>
#include <stbapi/util/ivio.h>

#include "SoListBox.h"


char * SoListBox::onString = 
"Separator {" 
"	 SoAbortGroup { abortActions BBOX "
"      Separator {"
"        Material { diffuseColor 0.9 0.9 0.9 }"
"        Transform { translation 0 0.1 0.25 rotation 1 0 0 -1.5707963 }"
"		 AntiSquish { sizing Y }"
#ifdef __COIN__
"        Translation { translation 0 0 0.1 }"
"        PolygonOffset { units -2 }"
#else
"        Translation { translation 0 0 0.1 }"
#endif
"		 FontStyle { size 0.5 family SANS }"
"		 AsciiText { justification CENTER }"
"    } }"
"	 Material { diffuseColor 0.5 0.5 0.5 }"
"    ShapeHints {"
"        vertexOrdering  COUNTERCLOCKWISE"
"        creaseAngle 1.36136"
"    }"
"    Coordinate3 {"
"        point [ -0.5 -0.5 -0.5,"
"                -0.5 0.0 -0.5,"
"                0.5 0.0 -0.5,"
"                0.5 -0.5 -0.5,"
"                -0.5 0.0 0.5,"
"                -0.5 -0.5 0.5,"
"                0.5 0.0 0.5,"
"                0.5 -0.5 0.5,"
"                -0.36444 0.1 -0.348644,"
"                -0.363032 0.1 0.354509,"
"                0.353259 0.1 0.350574,"
"                0.354633 0.1 -0.344963,"
"                -0.3 0.1 -0.3,"
"                -0.3 0.1 0.3,"
"                0.3 0.1 0.3,"
"                0.3 0.1 -0.3 ]"
"    }"
"    IndexedFaceSet {       "
"        coordIndex  [ 12, 11, 8, -1, 15, 11, 12, -1,"
"              15, 14, 11, -1, 14, 10, 11, -1,"
"              14, 9, 10, -1, 13, 9, 14, -1,"
"              13, 8, 9, -1, 12, 8, 13, -1,"
"              8, 11, 1, -1, 11, 2, 1, -1,"
"              11, 10, 2, -1, 10, 6, 2, -1,"
"              10, 4, 6, -1, 9, 4, 10, -1,"
"              9, 1, 4, -1, 8, 1, 9, -1,"
"              7, 5, 3, -1, 5, 0, 3, -1,"
"              6, 7, 2, -1, 7, 3, 2, -1,"
"              5, 4, 1, -1, 0, 5, 1, -1,"
"              6, 5, 7, -1, 4, 5, 6, -1,"
"              0, 2, 3, -1, 0, 1, 2, -1 ]"
"    }"
"    IndexedFaceSet {"
"        coordIndex  [ 12, 13, 14, -1, 12, 14, 15, -1 ]"
"    }"
"}";

char * SoListBox::offString = 
"Separator {"
"	 SoAbortGroup { abortActions BBOX"
"      Separator {"
"        Material { diffuseColor 0.9 0.9 0.9 }"
"        Transform { translation 0 0.5 0.25 rotation 1 0 0 -1.5707963 }"
"		 AntiSquish { sizing Y }"
#ifdef __COIN__
"        Translation { translation 0 0 0.1 }"
"        PolygonOffset { units -2 }"
#else
"        Translation { translation 0 0 0.1 }"
#endif
"		 FontStyle { size 0.5 family SANS }"
"		 AsciiText { justification CENTER }"
"    } }"
"	 Material { diffuseColor 0.5 0.5 0.5 }"
"    ShapeHints {"
"        vertexOrdering  COUNTERCLOCKWISE"
"        creaseAngle 1.36136"
"    }"
"    Coordinate3 {"
"        point [ -0.5 -0.5 -0.5,"
"                -0.5 0.4 -0.5,"
"                0.5 0.4 -0.5,"
"                0.5 -0.5 -0.5,"
"                -0.5 0.4 0.5,"
"                -0.5 -0.5 0.5,"
"                0.5 0.4 0.5,"
"                0.5 -0.5 0.5,"
"                -0.36444 0.5 -0.348644,"
"                -0.363032 0.5 0.354509,"
"                0.353259 0.5 0.350574,"
"                0.354633 0.5 -0.344963,"
"                -0.3 0.5 -0.3,"
"                -0.3 0.5 0.3,"
"                0.3 0.5 0.3,"
"                0.3 0.5 -0.3 ]"
"    }"
"    IndexedFaceSet {       "
"        coordIndex  [ 12, 11, 8, -1, 15, 11, 12, -1,"
"              15, 14, 11, -1, 14, 10, 11, -1,"
"              14, 9, 10, -1, 13, 9, 14, -1,"
"              13, 8, 9, -1, 12, 8, 13, -1,"
"              8, 11, 1, -1, 11, 2, 1, -1,"
"              11, 10, 2, -1, 10, 6, 2, -1,"
"              10, 4, 6, -1, 9, 4, 10, -1,"
"              9, 1, 4, -1, 8, 1, 9, -1,"
"              7, 5, 3, -1, 5, 0, 3, -1,"
"              6, 7, 2, -1, 7, 3, 2, -1,"
"              5, 4, 1, -1, 0, 5, 1, -1,"
"              6, 5, 7, -1, 4, 5, 6, -1,"
"              0, 2, 3, -1, 0, 1, 2, -1 ]"
"    }"
"    IndexedFaceSet {"
"        coordIndex  [ 12, 13, 14, -1, 12, 14, 15, -1 ]"
"    }"
"}";

char * SoListBox::onHlString = 
"Separator {"
"	 SoAbortGroup { abortActions BBOX"
"      Separator {"
"        Material { diffuseColor 0.9 0.9 0.9 }"
"        Transform { translation 0 0.1 0.25 rotation 1 0 0 -1.5707963 }"
"		 AntiSquish { sizing Y }"
#ifdef __COIN__
"        Translation { translation 0 0 0.1 }"
"        PolygonOffset { units -2 }"
#else
"        Translation { translation 0 0 0.1 }"
#endif
"		 FontStyle { size 0.5 family SANS }"
"		 AsciiText { justification CENTER }"
"    } }"
"	 Material { diffuseColor 0.8 0.8 0.5 }"
"    ShapeHints {"
"        vertexOrdering  COUNTERCLOCKWISE"
"        creaseAngle 1.36136"
"    }"
"    Coordinate3 {"
"        point [ -0.5 -0.5 -0.5,"
"                -0.5 0.0 -0.5,"
"                0.5 0.0 -0.5,"
"                0.5 -0.5 -0.5,"
"                -0.5 0.0 0.5,"
"                -0.5 -0.5 0.5,"
"                0.5 0.0 0.5,"
"                0.5 -0.5 0.5,"
"                -0.36444 0.1 -0.348644,"
"                -0.363032 0.1 0.354509,"
"                0.353259 0.1 0.350574,"
"                0.354633 0.1 -0.344963,"
"                -0.3 0.1 -0.3,"
"                -0.3 0.1 0.3,"
"                0.3 0.1 0.3,"
"                0.3 0.1 -0.3 ]"
"    }"
"    IndexedFaceSet {       "
"        coordIndex  [ 12, 11, 8, -1, 15, 11, 12, -1,"
"              15, 14, 11, -1, 14, 10, 11, -1,"
"              14, 9, 10, -1, 13, 9, 14, -1,"
"              13, 8, 9, -1, 12, 8, 13, -1,"
"              8, 11, 1, -1, 11, 2, 1, -1,"
"              11, 10, 2, -1, 10, 6, 2, -1,"
"              10, 4, 6, -1, 9, 4, 10, -1,"
"              9, 1, 4, -1, 8, 1, 9, -1,"
"              7, 5, 3, -1, 5, 0, 3, -1,"
"              6, 7, 2, -1, 7, 3, 2, -1,"
"              5, 4, 1, -1, 0, 5, 1, -1,"
"              6, 5, 7, -1, 4, 5, 6, -1,"
"              0, 2, 3, -1, 0, 1, 2, -1 ]"
"    }"
"    IndexedFaceSet {"
"        coordIndex  [ 12, 13, 14, -1, 12, 14, 15, -1 ]"
"    }"
"}";

char * SoListBox::offHlString = 
"Separator {"
"	 SoAbortGroup { abortActions BBOX" 
"      Separator {"
"        Material { diffuseColor 0.9 0.9 0.9 }"
"        Transform { translation 0 0.5 0.25 rotation 1 0 0 -1.5707963 }"
"		 AntiSquish { sizing Y }"
#ifdef __COIN__
"        Translation { translation 0 0 0.1 }"
"        PolygonOffset { units -2 }"
#else
"        Translation { translation 0 0 0.1 }"
#endif
"		 FontStyle { size 0.5 family SANS }"
"		 AsciiText { justification CENTER }"
"    } }"
"	 Material { diffuseColor 0.8 0.8 0.5 }"
"    ShapeHints {"
"        vertexOrdering  COUNTERCLOCKWISE"
"        creaseAngle 1.36136"
"    }"
"    Coordinate3 {"
"        point [ -0.5 -0.5 -0.5,"
"                -0.5 0.4 -0.5,"
"                0.5 0.4 -0.5,"
"                0.5 -0.5 -0.5,"
"                -0.5 0.4 0.5,"
"                -0.5 -0.5 0.5,"
"                0.5 0.4 0.5,"
"                0.5 -0.5 0.5,"
"                -0.36444 0.5 -0.348644,"
"                -0.363032 0.5 0.354509,"
"                0.353259 0.5 0.350574,"
"                0.354633 0.5 -0.344963,"
"                -0.3 0.5 -0.3,"
"                -0.3 0.5 0.3,"
"                0.3 0.5 0.3,"
"                0.3 0.5 -0.3 ]"
"    }"
"    IndexedFaceSet {       "
"        coordIndex  [ 12, 11, 8, -1, 15, 11, 12, -1,"
"              15, 14, 11, -1, 14, 10, 11, -1,"
"              14, 9, 10, -1, 13, 9, 14, -1,"
"              13, 8, 9, -1, 12, 8, 13, -1,"
"              8, 11, 1, -1, 11, 2, 1, -1,"
"              11, 10, 2, -1, 10, 6, 2, -1,"
"              10, 4, 6, -1, 9, 4, 10, -1,"
"              9, 1, 4, -1, 8, 1, 9, -1,"
"              7, 5, 3, -1, 5, 0, 3, -1,"
"              6, 7, 2, -1, 7, 3, 2, -1,"
"              5, 4, 1, -1, 0, 5, 1, -1,"
"              6, 5, 7, -1, 4, 5, 6, -1,"
"              0, 2, 3, -1, 0, 1, 2, -1 ]"
"    }"
"    IndexedFaceSet {"
"        coordIndex  [ 12, 13, 14, -1, 12, 14, 15, -1 ]"
"    }"
"}";

SO_KIT_SOURCE(SoListBox);

void SoListBox::initClass()
{
    SO_KIT_INIT_CLASS(SoListBox, SoBaseKit, "BaseKit");
}

SoListBox::SoListBox()
{
    SO_KIT_CONSTRUCTOR(SoListBox);

    isBuiltIn = TRUE;

	SO_KIT_ADD_CATALOG_ENTRY(geometrySwitch, SoSwitch, FALSE, this, \x0, FALSE );
	SO_KIT_ADD_CATALOG_ENTRY(on, SoSeparator, TRUE, geometrySwitch, \x0, TRUE );
	SO_KIT_ADD_CATALOG_ENTRY(off, SoSeparator, TRUE, geometrySwitch, \x0, TRUE );
	SO_KIT_ADD_CATALOG_ENTRY(onHl, SoSeparator, TRUE, geometrySwitch, \x0, TRUE );
	SO_KIT_ADD_CATALOG_ENTRY(offHl, SoSeparator, TRUE, geometrySwitch, \x0, TRUE );    
    SO_KIT_ADD_CATALOG_ENTRY(navigation, SoSeparator, FALSE, this, \x0, FALSE );
    SO_KIT_ADD_CATALOG_ENTRY(xfUp, SoTransform, FALSE, navigation, \x0, TRUE );
    SO_KIT_ADD_CATALOG_ENTRY(up, SoPushButton, FALSE, navigation, \x0, TRUE );
    SO_KIT_ADD_CATALOG_ENTRY(xfDown, SoTransform, FALSE, navigation, \x0, TRUE );
    SO_KIT_ADD_CATALOG_ENTRY(down, SoPushButton, FALSE, navigation, \x0, TRUE );
    SO_KIT_ADD_CATALOG_LIST_ENTRY(buttons, SoGroup, FALSE, this, \x0, SoSeparator, TRUE);

    SO_KIT_INIT_INSTANCE();

    SO_KIT_ADD_FIELD( values, (""));
    SO_KIT_ADD_FIELD( selected, (""));
    SO_KIT_ADD_FIELD( selectedIndices, (-1));
    SO_KIT_ADD_FIELD( multipleSelections, (FALSE));
	SO_KIT_ADD_FIELD( width, (1));
   	SO_KIT_ADD_FIELD( height, (1));
    SO_KIT_ADD_FIELD( depth, (1));
    SO_KIT_ADD_FIELD( navButtonWidth, (0.2));
    SO_KIT_ADD_FIELD( spacing, (0.02));
    SO_KIT_ADD_FIELD( numButtons, (5));
    SO_KIT_ADD_FIELD( displayIndex, (0));
    SO_KIT_ADD_FIELD( interval, (0.5));    

    values.deleteValues(0, -1);
    selected.deleteValues(0, -1);
    selectedIndices.deleteValues(0, -1);

	// setup geometry parts
	SoSeparator * onGeo = readMemory( onString );
	assert( onGeo != NULL );
	setPart( "on", onGeo );
	SoSeparator * offGeo = readMemory( offString );
	assert( offGeo != NULL );
	setPart( "off", offGeo );	
	SoSeparator * onHlGeo = readMemory( onHlString );
	assert( onHlGeo != NULL );
	setPart( "onHl", onHlGeo );	
	SoSeparator * offHlGeo = readMemory( offHlString );
	assert( offHlGeo != NULL );
	setPart( "offHl", offHlGeo );

	SoPushButton * button = SO_GET_PART(this, "up", SoPushButton);
	assert( button );
	button->setPart("onGeometry", SO_GET_PART(this, "on", SoSeparator )->copy());
	button->setPart("offGeometry", SO_GET_PART(this, "off", SoSeparator )->copy());
	button->setPart("highlightedOnGeometry", SO_GET_PART(this, "onHl", SoSeparator )->copy());
	button->setPart("highlightedOffGeometry", SO_GET_PART(this, "offHl", SoSeparator )->copy());
	button = SO_GET_PART(this, "down", SoPushButton);
	assert( button );
	button->setPart("onGeometry", SO_GET_PART(this, "on", SoSeparator )->copy());
	button->setPart("offGeometry", SO_GET_PART(this, "off", SoSeparator )->copy());
	button->setPart("highlightedOnGeometry", SO_GET_PART(this, "onHl", SoSeparator )->copy());
	button->setPart("highlightedOffGeometry", SO_GET_PART(this, "offHl", SoSeparator )->copy());			

	// create sensors
    valuesSensor.setData( this );
    valuesSensor.setFunction( SoListBox::valuesChanged );
    
    widthSensor.setData( this );
    widthSensor.setFunction( SoListBox::configChanged );
    heightSensor.setData( this );
    heightSensor.setFunction( SoListBox::configChanged );
    depthSensor.setData( this );
    depthSensor.setFunction( SoListBox::configChanged );
    navButtonSensor.setData( this );
    navButtonSensor.setFunction( SoListBox::configChanged );
    spacingSensor.setData( this );
    spacingSensor.setFunction( SoListBox::configChanged );
    numButtonsSensor.setData( this );
    numButtonsSensor.setFunction( SoListBox::configChanged );
	selectedSensor.setData( this );
	selectedSensor.setFunction( SoListBox::selectedChanged );
	selectedIndexSensor.setData( this );
	selectedIndexSensor.setFunction( SoListBox::selectedIndexChanged );

    navTimer.setData( this );
    navTimer.setFunction( SoListBox::navTimerFired );

    setUpConnections(TRUE, TRUE);
}

SbBool SoListBox::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	// todo take master / slave status into account
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    if (onOff) {
        // We connect AFTER base class.
        SoBaseKit::setUpConnections(onOff, doItAlways);
		// never traverse the children of the geometry switch !
		((SoSwitch *)geometrySwitch.getValue())->whichChild.setValue( -1 );
        // setup the widget
        adaptToConfig();
        // setup listeners
        valuesSensor.attach( &values );
        widthSensor.attach( &width );
        heightSensor.attach( &height );
        depthSensor.attach( &depth );
        navButtonSensor.attach( &navButtonWidth );
        spacingSensor.attach( &spacing );
        numButtonsSensor.attach( &numButtons );
		selectedSensor.attach( &selected );
		selectedIndexSensor.attach( &selectedIndices );
        
        SoPushButton * button = SO_GET_PART( this, "up", SoPushButton );
        button->setPressCallback( SoListBox::navPressed, this );
        button->setReleaseCallback( SoListBox::navReleased, this );
        button = SO_GET_PART( this, "down", SoPushButton );
        button->setPressCallback( SoListBox::navPressed, this );
        button->setReleaseCallback( SoListBox::navReleased, this );        

        navTimer.setInterval(interval.getValue());        
    }
    else {
        // We disconnect BEFORE base class.        
        SoPushButton * button = SO_GET_PART( this, "up", SoPushButton );
        button->removePressCallback();
        button->removeReleaseCallback();
        button = SO_GET_PART( this, "down", SoPushButton );
        button->removePressCallback();
        button->removeReleaseCallback();
        // Disconnect the field sensors.
        valuesSensor.detach();
        widthSensor.detach();
        heightSensor.detach();
        depthSensor.detach();
        navButtonSensor.detach();
        spacingSensor.detach();
        numButtonsSensor.detach();
		selectedSensor.detach();
		selectedIndexSensor.detach();

        SoBaseKit::setUpConnections(onOff, doItAlways);
    }
    return !(connectionsSetUp = onOff);
}

// this method will relayout the listbox and recreate the buttons
void SoListBox::adaptToConfig(void)
{
    // calculate button sizes
    float buttonW = width.getValue() - spacing.getValue() - navButtonWidth.getValue();
    float buttonD = (depth.getValue() - (numButtons.getValue() - 1)* spacing.getValue())/numButtons.getValue();

    float navW = navButtonWidth.getValue();
    float navD = (depth.getValue() - spacing.getValue()) / 2;

	float w2 = width.getValue() / 2;
	float d2 = depth.getValue() / 2;
	float s2 = spacing.getValue() / 2;

    // set up button
    SoTransform * xf = (SoTransform *)xfUp.getValue();
    xf->translation.setValue( w2 - navW / 2, 0, - d2 + navD/2 );
    SoPushButton * button = SO_GET_PART( this, "up", SoPushButton );
    button->width.setValue( navW );
    button->depth.setValue( navD );
    button->height.setValue( height.getValue());

    // set down button
    xf = (SoTransform *)xfDown.getValue();
    xf->translation.setValue( 0, 0, d2 + s2);
    button = SO_GET_PART( this, "down", SoPushButton );
    button->width.setValue( navW );
    button->depth.setValue( navD );
    button->height.setValue( height.getValue());
    
    // clear out buttons
    SoNodeKitListPart * list = (SoNodeKitListPart*)buttons.getValue();
    while( list->getNumChildren() != 0 )
        list->removeChild( 0 );

    // rebuild buttons
    for( int i = 0; i < numButtons.getValue(); i++ )
    {
        SoToggleButton * toggle = new SoToggleButton;
		toggle->ref();
        toggle->width.setValue( buttonW );
        toggle->depth.setValue( buttonD );
        toggle->height.setValue( height.getValue());
        SbString name("_LISTBOX_");
		name += SbString(i);
        toggle->setName( name );
        toggle->setPressCallback( SoListBox::buttonPressed, this );
        toggle->setReleaseCallback( SoListBox::buttonPressed, this );

		// we clone the geometry !
		toggle->setPart("onGeometry", SO_GET_PART(this, "on", SoSeparator )->copy());
		toggle->setPart("offGeometry", SO_GET_PART(this, "off", SoSeparator )->copy());
		toggle->setPart("highlightedOnGeometry", SO_GET_PART(this, "onHl", SoSeparator )->copy());
		toggle->setPart("highlightedOffGeometry", SO_GET_PART(this, "offHl", SoSeparator )->copy());
											
        SoTransform * transform = new SoTransform;
        transform->translation.setValue( -w2 + buttonW/2, 0, i * (buttonD + spacing.getValue()) - d2 + buttonD/2);
        SoSeparator * separator = new SoSeparator;
        separator->ref();
        separator->addChild( transform );
        separator->addChild( toggle );
        list->addChild( separator );		
        separator->unref();
		toggle->unref();
    }                                                    

    displayIndex.setValue( 0 );
	selectedSensor.detach();
    selected.deleteValues(0, -1);
	selectedSensor.attach( &selected );
	selectedIndexSensor.detach();
    selectedIndices.deleteValues(0, -1);    
	selectedIndexSensor.attach( &selectedIndices );

    buttons.setDefault( TRUE );
    adaptToState();
}

// this method will set the button state to the current listbox state
void SoListBox::adaptToState(void)
{
    int i;
    for( i = 0; i < numButtons.getValue(); i++ )
    {
        if( i + displayIndex.getValue() < values.getNum() )
        {
            SbBool state = (selected.find( values[i+displayIndex.getValue()]) != -1);
            setButton( i, values[i+displayIndex.getValue()], state );
        }
        else
        {
            setButton( i, "", FALSE );
        }        
    }
}

void SoListBox::setButton( int index, SbString string, SbBool state )
{
    // name we look for
	SbString name("_LISTBOX_");
	name += SbString(index);        

    // search for button with name = index, probably faster then search action
	SoNodeKitListPart * list = (SoNodeKitListPart *)buttons.getValue();
	SoToggleButton * button = NULL;
	int i;
	for( i = 0; i < list->getNumChildren() && button == NULL; i++ )
	{
		SoSeparator * sep = (SoSeparator *)list->getChild(i);
		assert( sep->getNumChildren() > 1);
		SoNode * candidate = sep->getChild(1);
		assert( candidate != NULL );
		if( candidate->getName() == name.getString() && candidate->isOfType( SoToggleButton::getClassTypeId()))
			button = (SoToggleButton *)candidate;
	}
	assert( button != NULL );
    // search for SoText node, now we need a search action, because we don't know about structure
	SoSearchAction sa;
	sa.setType(SoAsciiText::getClassTypeId());
	sa.setSearchingAll( TRUE );
	sa.setInterest( SoSearchAction::ALL );

	sa.apply( button );
	SoPathList pathList = sa.getPaths();		
    // set text and state
	if( pathList.getLength() != 0)
	{
		for( i = 0; i < pathList.getLength(); i++ )
		{
			SoFullPath * path = (SoFullPath *) pathList[i];
			assert( path->getTail()->isOfType(SoAsciiText::getClassTypeId()));
			SoAsciiText * AsciiText = (SoAsciiText *) path->getTail();
			AsciiText->string.setValue( string );
		}
	}
	button->on.setValue( state );
}

SoListBox::~SoListBox()
{

}

void SoListBox::valuesChanged( void * data, SoSensor * sensor  )
{
	assert( data != NULL );
    SoListBox * self = (SoListBox *)data;    
    self->displayIndex.setValue( 0 );

	self->selectedSensor.detach();
    self->selected.deleteValues(0, -1);
	self->selectedSensor.attach( &self->selected );

	self->selectedIndexSensor.detach();
    self->selectedIndices.deleteValues(0, -1);    
	self->selectedIndexSensor.attach( &self->selectedIndices );
    
	self->adaptToState();
}

void SoListBox::configChanged( void * data, SoSensor * sensor  )
{
	assert( data != NULL );
    ((SoListBox *)data)->adaptToConfig();
}

void SoListBox::buttonPressed( void * data, SoButtonKit* buttonKit )
{
	assert( data != NULL );
    SoListBox * self = (SoListBox *)data;
    // todo change selected !
	const char * name = buttonKit->getName().getString();
	int index;
	int res = sscanf(name, "_LISTBOX_%i", &index );
	assert( res == 1 );
	assert( 0 <= index && index <= self->numButtons.getValue());
	index += self->displayIndex.getValue();
	if( index >= self->values.getNum())
	{
		buttonKit->on.setValue( FALSE );
		// buttonKit->adaptGeometryToState();
		return;
	}
	
	self->selectedSensor.detach();
	self->selectedIndexSensor.detach();
	if( self->multipleSelections.getValue() )
	{
		if( buttonKit->on.getValue())
		{
			self->selectedIndices.find( index, TRUE );
			self->selected.find( self->values[index], TRUE );
		}
		else
		{
			int found = self->selectedIndices.find( index );
			if( found != -1 )
				self->selectedIndices.deleteValues( found, 1 );
			found = self->selected.find( self->values[index] );
			if( found != -1 )
				self->selected.deleteValues( found, 1 );
		}
	}
	else
	{
		if( buttonKit->on.getValue())
		{
			self->selectedIndices.setValue( index );
			self->selected.setValue( self->values[index] );
			self->adaptToState();
		}
		else
		{
			self->selectedIndices.deleteValues(0, -1);
			self->selected.deleteValues(0, -1);
		}
	}
	self->selectedSensor.attach( &self->selected );
	self->selectedIndexSensor.attach( &self->selectedIndices );
}

void SoListBox::navPressed( void * data, SoButtonKit * buttonKit )
{
	assert( data != NULL );
    SoListBox * self = (SoListBox *)data;
    if( self->up.getValue() == buttonKit ) // up pressed
        self->navigatingUp = TRUE;
    else                                   // down pressed
        self->navigatingUp = FALSE;

    // do the first press
    SoListBox::navTimerFired( data, NULL );
    self->navTimer.schedule();
}

void SoListBox::navReleased( void * data, SoButtonKit * buttonKit )
{
    assert( data != NULL );
    SoListBox * self = (SoListBox *)data;
    if( self->navTimer.isScheduled())
        self->navTimer.unschedule();
}

void SoListBox::navTimerFired( void * data, SoSensor * sensor )
{
    assert( data != NULL );
    SoListBox * self = (SoListBox *)data;
    if( self->navigatingUp == TRUE ) // up pressed
    {
        if( self->displayIndex.getValue() > 0 )
        {
            self->displayIndex.setValue( self->displayIndex.getValue() - 1 );
            self->adaptToState();            
        }
    }
    else                                   // down pressed
    {
        if( self->displayIndex.getValue() < ( self->values.getNum() - self->numButtons.getValue()))
        {
            self->displayIndex.setValue( self->displayIndex.getValue() + 1 );
            self->adaptToState();
        }
    }
}

void SoListBox::selectedChanged( void * data, SoSensor * sensor )
{
	assert( data != NULL );
	SoListBox * self = (SoListBox *)data;

	self->selectedSensor.detach();
	self->selectedIndexSensor.detach();
	// do some sanity checks and adapt selectedIndices to the same state
    if( self->multipleSelections.getValue() == FALSE
        && self->selected.getNum() > 1)
    {
        self->selected.setValue(self->selected[0]);
    }
	self->selectedIndices.setNum(self->selected.getNum());
	int i, count;
	int32_t * indices = self->selectedIndices.startEditing();
	for( i = 0, count = 0; i < self->selected.getNum(); )
	{
		indices[count] = self->values.find( self->selected[i] );
		if( indices[count] != -1 )
		{
			count++;
			i++;
		}
		else
		{
			self->selected.deleteValues(i, 1);
		}
	}
	self->selectedIndices.finishEditing();
	self->selectedIndices.setNum(count);

	self->selectedSensor.attach( &self->selected );
	self->selectedIndexSensor.attach( &self->selectedIndices );

    self->adaptToState();    
}

void SoListBox::selectedIndexChanged( void * data, SoSensor * sensor )
{
	assert( data != NULL );
	SoListBox * self = (SoListBox *)data;

	self->selectedSensor.detach();
	self->selectedIndexSensor.detach();
	// similar to selectedChanged but easier, because we only need to check
	// for boundaries !
    if( self->multipleSelections.getValue() == FALSE 
        && self->selectedIndices.getNum() > 1 )
    {
        self->selectedIndices.setValue(self->selectedIndices[0]);
    }
	self->selected.setNum( self->selectedIndices.getNum());
	int i, count;
	SbString * strings = self->selected.startEditing();
	for( i = 0, count = 0; i < self->selectedIndices.getNum(); )
	{
		if( self->selectedIndices[i] >= 0 && self->selectedIndices[i] < self->values.getNum())
		{
			strings[count] = self->values[self->selectedIndices[i]];
			count++;
			i++;
		}
		else
		{
			self->selectedIndices.deleteValues(i,1);
		}
	}
	self->selected.finishEditing();
	self->selected.setNum( self->selectedIndices.getNum());

	self->selectedSensor.attach( &self->selected );
	self->selectedIndexSensor.attach( &self->selectedIndices );

    self->adaptToState();    
}
