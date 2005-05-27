/* =======================================================================
 * (C) 2005 Vienna University of Technology
 * =======================================================================
 * PROJECT: iOrb
 * ======================================================================= */
/** SoOptionBallWidget.cxx
 *
 * @author  EMC
 *
 * $Id: SoOptionBallWidget.cxx,v 1.2 2005/01/26 23:16:52 chris Exp $
 * @file                                                                   */
/* ======================================================================= */

#include "globals.h"

#include <Inventor/SoPickedPoint.h> 
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoText2.h>
#include <stbapi/widgets/SoLabelKit.h>
#include <Inventor/details/SoDetail.h> 
#include <Inventor/details/SoCubeDetail.h> 
#include <Inventor/actions/SoRayPickAction.h> 
#include <Inventor/nodes/SoLabel.h> 
#include <Inventor/nodes/SoFont.h> 

#include "SoOptionBallWidget.h"

SO_KIT_SOURCE(SoOptionBallWidget);

const int optionCount[] = {4, 6, 8, 10, 12, 20};

void SoOptionBallWidget::initClass(void) 
{
	if (SoType::fromName("SoOptionBallWidget").isBad())
	{
		SO_KIT_INIT_CLASS(SoOptionBallWidget, SoMenuWidget, "SoMenuWidget");
	}
}

SoOptionBallWidget::SoOptionBallWidget(void):
	pickerSwitch(NULL),
	pickerPart(NULL),
	pickerTransform(NULL),
	ballTransform(NULL),
	labelMat(NULL),
	labelFont(NULL)
{
	SO_KIT_CONSTRUCTOR(SoOptionBallWidget);

	startRotation = SbRotation::identity();

	SO_NODE_DEFINE_ENUM_VALUE(TypeMode, OPTION_4);
	SO_NODE_DEFINE_ENUM_VALUE(TypeMode, OPTION_6);
	SO_NODE_DEFINE_ENUM_VALUE(TypeMode, OPTION_8);
	SO_NODE_DEFINE_ENUM_VALUE(TypeMode, OPTION_10);
	SO_NODE_DEFINE_ENUM_VALUE(TypeMode, OPTION_12);
	SO_NODE_DEFINE_ENUM_VALUE(TypeMode, OPTION_20);
	SO_NODE_SET_SF_ENUM_TYPE(type, TypeMode);

	SO_KIT_ADD_FIELD(isMoving, (FALSE));
	SO_KIT_ADD_FIELD(type, (OPTION_6));
	SO_KIT_ADD_FIELD(value, (""));
	SO_KIT_ADD_FIELD(selected, (""));
	SO_KIT_ADD_FIELD(selectedIndex, (-1));
	SO_KIT_ADD_FIELD(ballRadius, (0.1f));
	SO_KIT_ADD_FIELD(labelOffset, (0.05f));

	SO_KIT_INIT_INSTANCE();

	// HACK: change this to something more general for other types than OPTION_6

	labelPos = new SbVec3f[optionCount[OPTION_6]];

    setLabelPos();

	setUpConnections(TRUE, TRUE);
}

void SoOptionBallWidget::setLabelPos()
{
	// The part indices are ordered from 0 to 5 as [ front, back, left, right, top, bottom ].
	float r = ballRadius.getValue() + labelOffset.getValue();
	labelPos[0] = SbVec3f(0, 0, r);
	labelPos[1] = SbVec3f(0, 0, -r);
	labelPos[2] = SbVec3f(-r, 0, 0);
	labelPos[3] = SbVec3f(r, 0, 0);
	labelPos[4] = SbVec3f(0, r, 0);
	labelPos[5] = SbVec3f(0, -r, 0);
}

SoOptionBallWidget::~SoOptionBallWidget(void)
{
	destroyGeometry();

	DELAPTR(labelPos);
}


void SoOptionBallWidget::createGeometry()
{
	destroyGeometry();

	createBallPart();
	createPickerPart();

	isMoving.setValue(TRUE);
	onRotationInChanged(startRotation);
	isMoving.setValue(FALSE);
}

void SoOptionBallWidget::createBallPart()
{
	SoSeparator* geom = getGeometryNode();

	SoSeparator* ballPart = new SoSeparator();

    ballTransform = new SoTransform();
	ballTransform->ref();
	ballTransform->translation.setValue(0, 0, 0);
	ballTransform->rotation.setValue(SbRotation::identity());

	ballPart->addChild(ballTransform);

	SoSeparator* ballVis = new SoSeparator();

	// ball itself here
    SoMaterial* ballMat = new SoMaterial();
	ballMat->diffuseColor.setValue(0.2f, 1, 0.2f);
	ballVis->addChild(ballMat);

    SoSphere* ball = new SoSphere();
	ball->radius.setValue(ballRadius.getValue());
	ballVis->addChild(ball);

	ballPart->addChild(ballVis);

	// labels here
	DELAPTR(labelMat);
	DELAPTR(labelFont);
	labelMat = new SoMaterial*[optionCount[type.getValue()]];
	labelFont = new SoFont*[optionCount[type.getValue()]];

	for (int i = 0; i < optionCount[type.getValue()]; i++)
	{
		labelMat[i] = NULL;
		labelFont[i] = NULL;

		if (i >= value.getNum()) continue;

		SoSeparator* labelPart = new SoSeparator();

		SoTransform* transform = new SoTransform();
		transform->translation.setValue(labelPos[i]);

		// label font
		SoFont* font = new SoFont();
        font->name = fontName.getValue();
		font->size = fontSize.getValue();
		font->ref();
		labelFont[i] = font;

		SoText2* text = new SoText2();
		text->justification.setValue(SoText2::CENTER);
		text->string.setValue(value[i]);

		SoMaterial* mat = new SoMaterial();
		mat->diffuseColor.setValue(0.2f,0.2f,0.2f);
		mat->ref();
		labelMat[i] = mat;
		
		labelPart->addChild(font);
		labelPart->addChild(mat);
		labelPart->addChild(transform);
		labelPart->addChild(text);

		ballPart->addChild(labelPart);
	}

	geom->addChild(ballPart);
}

void SoOptionBallWidget::createPickerPart()
{
	SoSeparator* geom = getGeometryNode();

	pickerSwitch = new SoSwitch();
	pickerSwitch->ref();
	pickerSwitch->whichChild.setValue(SO_SWITCH_NONE);

    pickerPart = new SoSeparator();
    pickerPart->ref();

	pickerTransform = new SoTransform();
	pickerTransform->ref();
	pickerTransform->translation.setValue(0, 0, 0);
	pickerTransform->rotation.setValue(SbRotation::identity());

	pickerPart->addChild(pickerTransform);

	SoCube* cube = new SoCube();
    cube->width.setValue(0.1f);
	cube->height.setValue(0.1f);
	cube->depth.setValue(0.1f);
	
	pickerPart->addChild(cube);

	pickerSwitch->addChild(pickerPart);

	geom->addChild(pickerSwitch);
}

void SoOptionBallWidget::destroyGeometry()
{
	// decrease reference count

	// ball part
	UNREF(ballTransform);

	if (labelMat != NULL)
	{
		for (int i = 0; i < optionCount[type.getValue()]; i++)
	        UNREF(labelMat[i]);
		DELAPTR(labelMat);
	}
	if (labelFont != NULL)
	{
		for (int i = 0; i < optionCount[type.getValue()]; i++)
	        UNREF(labelFont[i]);
		DELAPTR(labelFont);
	}
    
	// picker part
	UNREF(pickerSwitch);
	UNREF(pickerPart);
	UNREF(pickerTransform);

	getGeometryNode()->removeAllChildren();
}

SbBool SoOptionBallWidget::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	if (!doItAlways && connectionsSetUp == onOff)
		return onOff;

	if (onOff)
	{
		// We connect AFTER base class.
		SoMenuWidget::setUpConnections(onOff, doItAlways);

		createGeometry();

		SET_SENSOR(typeSensor, type, SoOptionBallWidget::typeChanged);
		SET_SENSOR(ballRadiusSensor, ballRadius, SoOptionBallWidget::ballRadiusChanged);
		SET_SENSOR(labelOffsetSensor, labelOffset, SoOptionBallWidget::labelOffsetChanged);
	}
	else
	{
		destroyGeometry();

		// We disconnect BEFORE base class.
		SoMenuWidget::setUpConnections(onOff, doItAlways);
	}
	return !(connectionsSetUp = onOff);
}


void SoOptionBallWidget::onClickSelect()
{
	if (isMoving.getValue() == FALSE)
	{
		startRotation = rotation.getValue();
		isMoving.setValue(TRUE);
	} 
	else 
	{ 
		isMoving.setValue(FALSE); 
	}
}

void SoOptionBallWidget::typeChanged(void *data, SoSensor *sensor)
{
	assert(data != NULL);
	assert(((SoNode*)data)->isOfType(SoOptionBallWidget::getClassTypeId()));

	SoOptionBallWidget *self = (SoOptionBallWidget*)data;

	// Set cube geometry
	self->type.setValue(OPTION_6);

    self->destroyGeometry();
	self->createGeometry();
}

void SoOptionBallWidget::ballRadiusChanged(void *data, SoSensor *sensor)
{
	assert(data != NULL);
	assert(((SoNode*)data)->isOfType(SoOptionBallWidget::getClassTypeId()));

	SoOptionBallWidget *self = (SoOptionBallWidget*)data;
	self->setLabelPos();
	self->destroyGeometry();
	self->createGeometry();
}

void SoOptionBallWidget::labelOffsetChanged(void *data, SoSensor *sensor)
{
	assert(data != NULL);
	assert(((SoNode*)data)->isOfType(SoOptionBallWidget::getClassTypeId()));

	SoOptionBallWidget *self = (SoOptionBallWidget*)data;
	self->setLabelPos();
	self->destroyGeometry();
	self->createGeometry();
}

void SoOptionBallWidget::onClickConfirm()
{
	// User clicks to confirm selection
	setStartWidget();
}

void SoOptionBallWidget::onPrimaryRotationChanged(float angleIncrement)
{
	// React on user input
}


void SoOptionBallWidget::onSecondaryRotationChanged(float angleIncrement)
{
	// React on user input
}

void SoOptionBallWidget::onRotationInChanged(const SbRotation& rot)
{
	rotation.setValue(rot);

	// change the ball's rotation but only when in moving state
	if ((pickerTransform != NULL) && (ballTransform != NULL) && (isMoving.getValue() == TRUE))
	{
		SbRotation remainder = SbRotation::identity();

		// undo the previously saved rotation and do the full rotation
		remainder = startRotation.inverse() * rot;
		ballTransform->rotation.setValue(ballTransform->rotation.getValue() *  remainder);
		pickerTransform->rotation.setValue(ballTransform->rotation.getValue());

		// update saved rotation
		startRotation = rot;

		// start the fun (raypicking)

		SbViewportRegion vpr;
        SoRayPickAction rpa(vpr);

		// from front
		SbVec3f start = SbVec3f(0, 0, 10);
		SbVec3f direction = SbVec3f(0, 0, -1);
        rpa.setRay(start, direction, 0, 100);
		rpa.setPickAll(FALSE);

		rpa.apply(pickerPart);

		SoPickedPoint* pp = rpa.getPickedPoint();
		if (pp != NULL)
		{
			if (pp->getDetail()->isOfType(SoCubeDetail::getClassTypeId()))
			{
				// picked object was our cube (hopefully)
                const SoCubeDetail* cube_detail = (SoCubeDetail*)pp->getDetail();

				selectedIndex = cube_detail->getPart();

				if (selectedIndex.getValue() < value.getNum())
				{
                    selected.setValue(value[selectedIndex.getValue()]);
				}

				if (labelMat != NULL)
				{
					for (int i = 0; i < optionCount[type.getValue()]; i++)
					{
						if (labelMat[i] == NULL) continue;

						if (i == cube_detail->getPart())
						{
							labelMat[i]->diffuseColor.setValue(1, 0, 0);
							labelFont[i]->size.setValue(fontSize.getValue()*1.5f);
						} else {
							labelMat[i]->diffuseColor.setValue(0.5f, 0.5f, 0.5f);
							labelFont[i]->size.setValue(fontSize.getValue());
						}
					}
				}
			}
		}
	}
}
