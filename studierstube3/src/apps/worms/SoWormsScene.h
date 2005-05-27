// ===========================================================================
//  (C) 2002 Vienna University of Technology
// ===========================================================================
//  NAME:       SoWormsScene.cpp
//  TYPE:       c++ header file
//  PROJECT:    Studierstube
//  CONTENT:    Main node for the worms game
//  VERSION:    0.1
// ===========================================================================
//  AUTHORS:    dw      Daniel Wagner
// ===========================================================================
//  HISTORY:
//
// ===========================================================================


#ifndef __SOWORMSSCENE_HEADERFILE__
#define __SOWORMSSCENE_HEADERFILE__


#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/misc/SoMD2NodeKit.h>

#include "SoTerrain.h"



class SoWormsScene : public SoSeparator
{
	SO_NODE_HEADER(SoWormsScene);

public:
	// Initializes this class.
	static void initClass();

	// Constructor
	SoWormsScene();

	void setupButtonCallbacks(SoNode* pipSheet, SbBool masterMode);

private:
	struct PlayerInfo
	{
		double			runSpeed, rotSpeed;
		double			md2DY;
		bool			isRunning;
		int				rotate;
	};

	// Methods
	virtual ~SoWormsScene();
	void createNodes();
	void correctPlayerPosition();
	void update();
	void onButtonCB(So3DButton* nButton);

	
	// Nodes
	//
	SoTerrain*		terrain;
	SoMD2NodeKit*	md2;
	PlayerInfo		playerInfo;

	double			frameTime, lastFrameTime;


	// Sensors and callbacks
	//
	static void timeCB(void *data, SoSensor* nSensor);
	static void playerPositionCB(void* data, SoSensor*);
	static void buttonCB(void* data, So3DButton* button);


	SoFieldSensor*	timeSensor;
	SoFieldSensor*	playerPositionSensor;


	// some simple utility methods
	//
	static double getCurrentTime();
	static SoNode* SoWormsScene::findNode(SoNode* root, SbString name);
};


#endif //__SOWORMSSCENE_HEADERFILE__

