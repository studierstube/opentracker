// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       SoChevyKit.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube
//  CONTENT:    "Red and Blue" Demo
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    ds      Dieter Schmalstieg
// ===========================================================================
//  HISTORY:
//
// ===========================================================================

#include <Inventor/nodes/SoSphere.h>
#include <Inventor/engines/SoCompose.h>
#include <Inventor/engines/SoSelectOne.h>


#include <stbapi/widgets/So3DButton.h>
#include <stbapi/util/ivio.h>
#include <stbapi/workspace/SoContextKitSub.h>
#include <stbapi/resource/SoPipSheetKit.h>
#include <stbapi/interaction/SoDraggerKit.h>


#include "SoChevyKit.h"

//initialization of kit structure in Inventor
SO_KIT_SOURCE(SoChevyKit);

//initialization of context in Studierstube
CONTEXT_APPLICATION(SoChevyKit);

//----------------------------------------------------------------------------
// static class initialization
//----------------------------------------------------------------------------

void
SoChevyKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoChevyKit, SoContextKit, "SoContextKit");
}

//----------------------------------------------------------------------------
// virtual constructor
//----------------------------------------------------------------------------

SoContextKit*
SoChevyKit::factory()
{
    return new SoChevyKit;
}

/**
 * constructor
 */

SoChevyKit::SoChevyKit()
{
    SO_KIT_CONSTRUCTOR(SoChevyKit);
    SO_KIT_INIT_INSTANCE();

    bodyColor = NULL;
}

/**
 * destructor
 */

SoChevyKit::~SoChevyKit()
{
    destructor();
}


void SoChevyKit::resetSceneCB(void* data, SoButtonKit* buttonKit)
{
    //this is a static function so we don't know which instance
    //thus pointer to context is passed in as <data> and we must cast it
    SoChevyKit* self = (SoChevyKit*)data;

    //get pointer to window content
    // SoSeparator* winRoot = self->getWindow(0)->getClientVolumeSeparator();
    self->resetScene();


}

void SoChevyKit::leftButtonCB(void* data, SoButtonKit* buttonKit)
{
    //this is a static function so we don't know which instance
    //thus pointer to context is passed in as <data> and we must cast it
    SoChevyKit* self = (SoChevyKit*)data;

    self->setLeftIndicator(true);
    self->setRightIndicator(false);
}

void SoChevyKit::rightButtonCB(void* data, SoButtonKit* buttonKit)
{
    //this is a static function so we don't know which instance
    //thus pointer to context is passed in as <data> and we must cast it
    SoChevyKit* self = (SoChevyKit*)data;

    self->setLeftIndicator(false);
    self->setRightIndicator(true);
}

void SoChevyKit::middleButtonCB(void* data, SoButtonKit* buttonKit)
{
    //this is a static function so we don't know which instance
    //thus pointer to context is passed in as <data> and we must cast it

    SoChevyKit* self = (SoChevyKit*)data;

    self->setLeftIndicator(false);
    self->setRightIndicator(false);
}

void SoChevyKit::setLeftIndicator(boolean on)
{

	// stopping the timeCounter and reseting it does not
	// yield the desired results

    leftIndicatorTimer->max = on;



}

void SoChevyKit::setRightIndicator(boolean on)
{
	// stopping the timeCounter and reseting it does not
	// yield the desired results

	rightIndicatorTimer->max = on;

}

/**
 * Reads the pip sheet and sets the button callbacks
 */

SoNode* SoChevyKit::createPipSheet(int uid)
{
    //read pip sheet from file "sheet.iv"
    comm->setContextAliasName(getName(),"R&B");
    SoSeparator *newPipSheet = readFile("sheet.iv",comm->workingDirectory);

    // get the RGB Selector and its current color from the PIP Sheet
    newPipSheet->ref();
    selectorColor  = (SoMaterial*) findNode(newPipSheet,"SELECTOR_COLOR");
    newPipSheet->unrefNoDelete();

    newPipSheet->ref();
    rgbSelector  = (SoDraggerKit*) findNode(newPipSheet,"RGB_SELECTOR");
    newPipSheet->unrefNoDelete();

    newPipSheet->ref();
    lightDial = (SoDial*) findNode(newPipSheet,"LIGHT_DIAL");
    newPipSheet->unrefNoDelete();

    newPipSheet->ref();
    transparencySlider = (SoSlider*) findNode(newPipSheet,"TRANSPARENCY_SLIDER");
    newPipSheet->unrefNoDelete();

    newPipSheet->ref();
    resetButton = (SoPushButton*) findNode(newPipSheet,"RESET_BUTTON");
    newPipSheet->unrefNoDelete();

    newPipSheet->ref();
    rightButton = (SoRadioButton*) findNode(newPipSheet,"RIGHT_BUTTON");
    newPipSheet->unrefNoDelete();

    newPipSheet->ref();
    middleButton = (SoRadioButton*) findNode(newPipSheet,"MIDDLE_BUTTON");
    newPipSheet->unrefNoDelete();

    newPipSheet->ref();
    leftButton = (SoRadioButton*) findNode(newPipSheet,"LEFT_BUTTON");
    newPipSheet->unrefNoDelete();

    return newPipSheet;
}
 
/**
 * createWindowGeometry
 * create window and attach scene graph to it. scene graph contains
 * a car the color of which can be set by controls on pip
 */

SoWindowKit* SoChevyKit::createWindowGeometry(int index)
{
    //create a default window
    WindowCreate wc;
    SoWindowKit::defaultWindow(wc);
    SoWindowKit* windowKit = comm->createWindow(NULL, &wc, NULL, NULL);
    windowKit->size = SbVec3f(0.7,0.7,0.7);
	windowKit->maximize();

    SoSeparator* clientVolume = windowKit->getClientVolumeSeparator();
    clientVolume->addChild(readFile("carParts.iv",comm->workingDirectory));

    // get the car's current color from the pip sheet
    clientVolume->ref();
    bodyColor  = (SoMaterial*) findNode(clientVolume,"CAR_COLOR");
    clientVolume->unrefNoDelete();

    clientVolume->ref();
    lightColor  = (SoMaterial*) findNode(clientVolume,"LIGHT_COLOR");
    clientVolume->unrefNoDelete();

    clientVolume->ref();
    leftDoor = (SoDraggerKit*) findNode(clientVolume,"LEFT_DOOR");
    clientVolume->unrefNoDelete();

    clientVolume->ref();
    rightHandle = (SoDraggerKit*) findNode(clientVolume,"RIGHT_HANDLE");
    clientVolume->unrefNoDelete();

    clientVolume->ref();
    hood = (SoDraggerKit*) findNode(clientVolume,"HOOD");
    clientVolume->unrefNoDelete();

    clientVolume->ref();
    sunRoof = (SoDraggerKit*) findNode(clientVolume,"SUN_ROOF");
    clientVolume->unrefNoDelete();

    clientVolume->ref();
    steeringWheelTransform = (SoTransform*) findNode(clientVolume,"STEERING_WHEEL_TRANSFORM");
    clientVolume->unrefNoDelete();

    clientVolume->ref();
    leftWheelRotation = (SoRotationXYZ*) findNode(clientVolume,"LEFT_WHEEL_ROTATION");
    clientVolume->unrefNoDelete();

    clientVolume->ref();
    rightWheelRotation = (SoRotationXYZ*) findNode(clientVolume,"RIGHT_WHEEL_ROTATION");
    clientVolume->unrefNoDelete();

    clientVolume->ref();
    leftIndicatorColor = (SoMaterial*) findNode(clientVolume,"LEFT_INDICATOR_COLOR");
    clientVolume->unrefNoDelete();

    clientVolume->ref();
    rightIndicatorColor = (SoMaterial*) findNode(clientVolume,"RIGHT_INDICATOR_COLOR");
    clientVolume->unrefNoDelete();

    return windowKit;
}

void SoChevyKit::setSheetMasterMode(SoNode* pipSheet, SbBool masterMode)
{


        // connect parts from pip to parts from window

        if(masterMode) // if context is master -> set callbacks
        {
			resetButton->setPressCallback(resetSceneCB, this);
            leftButton->setPressCallback(leftButtonCB, this);
            rightButton->setPressCallback(rightButtonCB, this);
            middleButton->setPressCallback(middleButtonCB, this);

            // connect the car light's intensity to the value of
            // light dial

            SoComposeVec3f *lightVec = new SoComposeVec3f();
            lightVec->x.connectFrom(&lightDial->value);
            lightVec->y.connectFrom(&lightDial->value);
            lightVec->z.connectFrom(&lightDial->value);

            lightColor->diffuseColor.connectFrom(&lightVec->vector);

			// connect the car color's transparency to the value
			// of the transparency slider

			bodyColor->transparency.connectFrom(&transparencySlider->value);

			// connect the car color to the selector color

			bodyColor->diffuseColor.connectFrom(&selectorColor->diffuseColor);

            // connect the steering wheel to the rotation of the front wheels
            SoDecomposeRotation *steeringEngine = new SoDecomposeRotation();
            steeringEngine->rotation.connectFrom(&steeringWheelTransform->rotation);
            rightWheelRotation->angle.connectFrom(&steeringEngine->angle);
            leftWheelRotation->angle.connectFrom(&steeringEngine->angle);

            // set up the indicator machines

            SoMFColor *colors = new SoMFColor();
            colors->set1Value(0, 0, 0, 0);
            colors->set1Value(1, 1, 0, 0);

            // left indicator

            leftIndicatorTimer = new SoTimeCounter();
            leftIndicatorTimer->frequency.setValue(3);
            leftIndicatorTimer->max = 0;

            SoSelectOne *selectLeftColor = new SoSelectOne(SoMFColor::getClassTypeId());
            selectLeftColor->input->connectFrom(colors);
            selectLeftColor->index.connectFrom(&leftIndicatorTimer->output);

            leftIndicatorColor->diffuseColor.connectFrom(selectLeftColor->output);

            // right indicator

            rightIndicatorTimer = new SoTimeCounter();
            rightIndicatorTimer->frequency.setValue(3);
            rightIndicatorTimer->max = 0;

            SoSelectOne *selectRightColor = new SoSelectOne(SoMFColor::getClassTypeId());
            selectRightColor->input->connectFrom(colors);
            selectRightColor->index.connectFrom(&rightIndicatorTimer->output);

            rightIndicatorColor->diffuseColor.connectFrom(selectRightColor->output);


        }
        else // if context is slave -> remove callbacks
        {
            rgbSelector->removeMoveCallback();
        }

}


/**
 * resets 
 *  - the rotation of the left door, the right handle and the hood,
 *  - the translation of the sunRoof
 *  - the position of the transparency slider
 *  - the position of the rgbSelector
 */
void SoChevyKit::resetScene()
{
	printf("reseting scene\n");

	SoTransform *doorTransform = (SoTransform *) leftDoor->getPart("constrainedTransform", true);
	doorTransform->rotation.setValue(SbVec3f(0,1,0), 0);

	SoTransform *handleTransform = (SoTransform *) rightHandle->getPart("constrainedTransform", true);
	handleTransform->rotation.setValue(SbVec3f(0,1,0), 0);

	SoTransform *hoodTransform = (SoTransform *) hood->getPart("constrainedTransform", true);
	hoodTransform->rotation.setValue(SbVec3f(0,1,0), 0);

	SoTransform *sunRoofTransform = (SoTransform *) sunRoof->getPart("constrainedTransform", true);
	sunRoofTransform->translation.setValue(0,-0.3,0);

}


