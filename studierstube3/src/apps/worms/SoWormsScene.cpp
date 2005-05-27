// ===========================================================================
//  (C) 2002 Vienna University of Technology
// ===========================================================================
//  NAME:       SoWormsScene.cpp
//  TYPE:       c++ source code
//  PROJECT:    Studierstube
//  CONTENT:    Main node for the worms game
//  VERSION:    0.1
// ===========================================================================
//  AUTHORS:    dw      Daniel Wagner
// ===========================================================================
//  HISTORY:
//
// ===========================================================================


#include "SoWormsScene.h"
#include <assert.h>

#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/fields/SoSFTime.h>


#define STR_PLAYERLEFT		"PLAYER_LEFT"
#define STR_PLAYERRIGHT		"PLAYER_RIGHT"
#define STR_PLAYERFORWARD	"PLAYER_FORWARD"

static char*				playerButtonNames[] =  { STR_PLAYERLEFT,
													 STR_PLAYERRIGHT,
													 STR_PLAYERFORWARD };

SO_NODE_SOURCE(SoWormsScene);


// This initializes the SoWormsScene class.
//
void
SoWormsScene::initClass()
{
	// Initialize type id variables.
	SO_NODE_INIT_CLASS(SoWormsScene, SoSeparator, "Separator");
}


void
SoWormsScene::timeCB(void* data, SoSensor* )
{
	assert(data);
	SoWormsScene* self = (SoWormsScene*)data;

	self->update();
}


void
SoWormsScene::playerPositionCB(void* data, SoSensor* nSensor)
{
	assert(data);
	SoWormsScene* self = (SoWormsScene*)data;
	SoFieldSensor* sensor = (SoFieldSensor*)nSensor;

	sensor->detach();
	self->correctPlayerPosition();
	sensor->attach(&self->md2->position);
}


void
SoWormsScene::buttonCB(void* data, So3DButton* button)
{
    SoWormsScene* self = (SoWormsScene*)data;

	self->onButtonCB(button);
}



// Constructor
//
SoWormsScene::SoWormsScene() : terrain(NULL), md2(NULL),
							   timeSensor(NULL), playerPositionSensor(NULL)
{
	SO_NODE_CONSTRUCTOR(SoWormsScene);

	// init player data
	//
	playerInfo.md2DY = 0.025;
	playerInfo.isRunning = false;
	playerInfo.runSpeed = 0.1;
	playerInfo.rotSpeed = 1.0;
	playerInfo.rotate = 0;

	createNodes();

	timeSensor = new SoFieldSensor(timeCB, this);
	timeSensor->attach(SoDB::getGlobalField("realTime"));

	if(md2)
	{
		playerPositionSensor = new SoFieldSensor(playerPositionCB, this);
		playerPositionSensor->attach(&md2->position);

		md2->position.setValue(0.4, 1, -0.4);
	}

}


// Destructor
//
SoWormsScene::~SoWormsScene()
{
}


void
SoWormsScene::createNodes()
{
	// terrain
	//
	terrain = new SoTerrain;
	terrain->textureName = "../apps/worms/data/TerrainTexSmall.jpg";
	terrain->heightMapName = "../apps/worms/data/Heightmap.raw";
	terrain->position = SbVec3f(0, -0.4, 0);
	addChild(terrain);


	// player characters
	//
	double md2Scale = 0.7;		// --> 0.3

	md2 = new SoMD2NodeKit;
	md2->meshname.setValue("../apps/worms/data/Ryla.md2");
	md2->skinname.setValue("../apps/worms/data/Ryla.jpg");
	//md2->rotation.setValue(0, 1, 0, 1.2);
	md2->scaleFactor.setValue(md2Scale, md2Scale, md2Scale);
	md2->animname.setValue("run");
	md2->animtime.setValue(1.5);
	md2->setHeading(4);
	terrain->addChild(md2);
}


void
SoWormsScene::update()
{
	double curTime = getCurrentTime();

	frameTime = curTime - lastFrameTime;
	if(frameTime>1)							// prevent time-out errors...
		frameTime = 0;

	if(playerInfo.rotate)
	{
		double	angle = frameTime * playerInfo.rotSpeed * playerInfo.rotate;
		md2->setHeading(md2->getHeading() + angle);
	}

	if(playerInfo.isRunning)
	{
		double	distance = frameTime * playerInfo.runSpeed,
				dir = md2->getHeading();
		SbVec3f	vec(cos(dir), 0, -sin(dir));

		md2->position.setValue(md2->position.getValue() + (float)distance * vec);

		if(md2->animname.getValue()!="run")
		{
			md2->animname = "run";
			md2->animtime = 1.5;
		}
	}
	else
	{
		if(md2->animname.getValue()!="stand")
		{
			md2->animname = "stand";
			md2->animtime = 5.0;
		}
	}


	lastFrameTime = curTime;
}


void
SoWormsScene::setupButtonCallbacks(SoNode* pipSheet, SbBool masterMode)
{
	printf("INFO: setting up button callbacks for SoWormsScene\n");

	int			numButtons = sizeof(playerButtonNames) / sizeof(char*);
	So3DButton* b;

	if(masterMode)
	{
		// connect buttons with our callback
		//
		for(int i=0; i<numButtons ; i++)
		{
			if(b = (So3DButton*)SoWormsScene::findNode(pipSheet,playerButtonNames[i]))
			{
				b->addPressCallback(buttonCB,this);
				b->addReleaseCallback(buttonCB,this);
			}
			else
				printf("WARNING: Button '%s' not found on PIP\n", playerButtonNames[i]);
		}
	}
	else
	{
		// release our callback from the buttons
		//
		for(int i=0; i<numButtons; i++)
		{
			if(b = (So3DButton*)SoWormsScene::findNode(pipSheet,playerButtonNames[i]))
			{
				b->removePressCallback(buttonCB);
				b->removeReleaseCallback(buttonCB);
			}
		}
	}
}


void
SoWormsScene::onButtonCB(So3DButton* nButton)
{
	SbString	str = nButton->nodeId.getValue();
	bool		down = nButton->pressed.getValue() != 0;

	if(str==STR_PLAYERFORWARD)
	{
		printf("INFO: button-forward callback: '%s'\n", down ? "DOWN" : "UP");
		playerInfo.isRunning =  down;
	}

	if(str==STR_PLAYERLEFT)
	{
		printf("INFO: button-left callback: '%s'\n", down ? "DOWN" : "UP");
		playerInfo.rotate = down ? 1 : 0;
	}

	if(str==STR_PLAYERRIGHT)
	{
		printf("INFO: button-right callback: '%s'\n", down ? "DOWN" : "UP");
		playerInfo.rotate = down ? -1 : 0;
	}

}


void
SoWormsScene::correctPlayerPosition()
{
	if(!terrain || !md2)
		return;

	SbVec3f	p = md2->position.getValue();
	double	height;

	terrain->getHeight(p[0], p[2], height);

	p[1] = height + playerInfo.md2DY;
	md2->position.setValue(p);
}


double
SoWormsScene::getCurrentTime()
{
	SoSFTime*		timeField = (SoSFTime*)SoDB::getGlobalField("realTime");
	const SbTime&	time = timeField->getValue();

	return time.getValue();
}


// SoWormsScene::findNode raises no assert (in contrast to findNode)
//
SoNode*
SoWormsScene::findNode(SoNode* root, SbString name)
{
    SoSearchAction sa;
	sa.setSearchingAll(TRUE);
    sa.setName(name);
    sa.apply(root);
    SoFullPath* path = (SoFullPath*)sa.getPath();
    if(path==NULL)
		return NULL;
    return path->getTail();
}
