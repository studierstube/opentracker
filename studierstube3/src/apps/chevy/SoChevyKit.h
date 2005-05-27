// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       SoChevyKit.h
//  TYPE:       c++ header
//  PROJECT:    Studierstube
//  CONTENT:    Demo application for Studierstube foundation class
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    ds		Dieter Schmalstieg
// ===========================================================================
//  HISTORY:
//
// ===========================================================================

#ifndef _SOCHEVYKIT_H_
#define _SOCHEVYKIT_H_

#include <Inventor/engines/SoTimeCounter.h>
#include <Inventor/engines/SoSelectOne.h>


#include <stbapi/workspace/SoContextKit.h>
#include <stbapi/interaction/SoDraggerKit.h>
#include <stbapi/widgets/SoDial.h>
#include <stbapi/widgets/SoSlider.h>
#include <stbapi/widgets/SoPushButton.h>
#include <stbapi/widgets/SoRadioButton.h>

#include "chevy.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class CHEVY_API SoChevyKit: public SoContextKit
{
    SO_KIT_HEADER(SoChevyKit);

public:
	/**
	 * static class initialization
	 */
    static void initClass();

	/**
	 * constructor
	 */
    SoChevyKit();                      //constructor

	/**
	 * override this to create a new context of a specific type
	 */
    SoContextKit* factory();
       
	/**
	 * destructor
	 */
    ~SoChevyKit();

	/**
	 * the static function that calls the member function
	 * resetScene(), registered with the push button
	 */
	static void resetSceneCB(void* data, SoButtonKit* buttonKit);

	/**
	 * the callback associated with the left radio button
	 * turns the left indicator on
	 */
	static void leftButtonCB(void* data, SoButtonKit* buttonKit);

	/**
	 * turns both indicators off
	 */
	static void middleButtonCB(void* data, SoButtonKit* buttonKit);

	/**
	 * the callback associated with the right radio button
	 * turns the right indicator on
	 */
	static void rightButtonCB(void* data, SoButtonKit* buttonKit);

	/**
	 * resets 
	 *  - the rotation of the left door, the right handle and the hood,
	 *  - the translation of the sunRoof
	 *  - the position of the transparency slider
	 *  - the position of the rgbSelector
	 */
	void resetScene();

	/**
	 * turns the left indicator on or off
	 */
    void setLeftIndicator(boolean onOff);

	/**
	 * turns the right indicator on or off
	 */
    void setRightIndicator(boolean onOff);

private:

    SoNode* createPipSheet(int uid);              //create new pip sheet
    SoWindowKit* createWindowGeometry(int index); //create window and content
    virtual void setSheetMasterMode(SoNode* pipSheet, SbBool masterMode);  //master/slave mode for pip sheet

    SoTimeCounter *leftIndicatorTimer;
    SoTimeCounter *rightIndicatorTimer;

    // car colors
    SoMaterial *bodyColor;
    SoMaterial *selectorColor;
    SoMaterial *lightColor;
    SoMaterial *leftIndicatorColor;
    SoMaterial *rightIndicatorColor;


    // car transformations
    SoRotationXYZ *leftWheelRotation;
    SoRotationXYZ *rightWheelRotation;
    SoTransform *steeringWheelTransform;

	// car parts
	SoDraggerKit *leftDoor;
	SoDraggerKit *rightHandle;
	SoDraggerKit *hood;
	SoDraggerKit *sunRoof;
  
    // pip parts
    SoDraggerKit *rgbSelector;
	SoSlider *transparencySlider;
    SoDial *lightDial;
	SoPushButton *resetButton;
    SoRadioButton *leftButton;
    SoRadioButton *middleButton;
    SoRadioButton *rightButton;

}; //class SoChevyKit

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOCHEVYKIT_H_

