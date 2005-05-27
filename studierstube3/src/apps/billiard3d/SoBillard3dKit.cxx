/* ======================================================================= */
/** source file for SoBillard3dKit Node.
 *
 * @author Team und Struppi
 *
 * $Id: SoBillard3dKit.cxx 3426 2004-07-14 12:11:27Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/engines/SoElapsedTime.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/actions/SoSearchAction.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/util/ivio.h>
#include <stbapi/workspace/SoContextKitSub.h>
#include <stbapi/event3d/Stb3DEventGenerator.h>
#include <stbapi/resource/SoPipSheetKit.h>
#include <stbapi/tracker/SoStationKit.h>

#include <assert.h>

#include "SoBillard3dKit.h"

//--initialization of kit structure in Inventor
SO_KIT_SOURCE(SoBillard3dKit);

//--initialization of context in Studierstube
CONTEXT_APPLICATION(SoBillard3dKit);


static const double FRICTION = 0.95;
static const double EPSILON  = 0.001;
static const double BOXSIZE  = 2;
static const double HOLESIZE = 0.4;
static const double BALLSIZE = 0.08;
static const double OUTSIDE = 100;
static const unsigned int NUM_BALLS = 15;

static unsigned int activeUser = 0;
static enum {ONE_PLAYER, TWO_PLAYER} gameMode = ONE_PLAYER;
//static enum {ONE_PLAYER, TWO_PLAYER} gameMode = TWO_PLAYER;
static unsigned long pressTime;

SoTimerSensor *timer;

SoTimerSensor *blinktimer;

static unsigned int isBlink = 0;

struct BallInfo ball[NUM_BALLS];

SoStationKit *queuepos[4];

SoCalculator *calcQueueVis;
SoSeparator *queueVis;

SoElapsedTime *timeSincePress;

//----------------------------------------------------------------------------
// static class initialization
//----------------------------------------------------------------------------

void SoBillard3dKit::initClass(void)
{
  SO_KIT_INIT_CLASS(SoBillard3dKit, SoContextKit, "SoContextKit");
}

//----------------------------------------------------------------------------
// virtual constructor
//----------------------------------------------------------------------------

SoContextKit* SoBillard3dKit::factory()
{
  return new SoBillard3dKit;
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoBillard3dKit::SoBillard3dKit()
{
  SO_KIT_CONSTRUCTOR(SoBillard3dKit);
  SO_KIT_INIT_INSTANCE();
}

//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------

SoBillard3dKit::~SoBillard3dKit()
{
	if( timer != NULL ) 
	{
		timer->unschedule();
		delete timer;
	}
	if( blinktimer != NULL )
	{
		blinktimer->unschedule();
		delete blinktimer;
	}
	destructor();
}

//----------------------------------------------------------------------------

//----------------------------------------------------------------------
// blink
// Callback function called by a timer once every second. 
// Changes the color of the 'white' ball according to the current player
//----------------------------------------------------------------------
void blink (void *data, SoSensor *) 
{
   SoMaterial *material = (SoMaterial *)ball[0].node->getChild(1);

   if (isBlink || ball[0].moving)
   {
		material->diffuseColor.setValue (1, 1, 1);
		isBlink = 0;
   }
   else
   {
	   if (activeUser == 0)
			material->diffuseColor.setValue (1, 0, 0);
	   else
			material->diffuseColor.setValue (0, 0, 1);

	   isBlink = 1;
   }
}

//----------------------------------------------------------------------
// moveBalls
// Callback function called by a timer 30 times per second. Moves
// balls according to current speed and diminuishes speed.
//----------------------------------------------------------------------
void moveBalls (void *data, SoSensor *) {
  for (int i = 0; i < NUM_BALLS; i++) {
    if (ball[i].moving) {
      SoTranslation *trans = (SoTranslation *)ball[i].node->getChild(0);
      trans->translation.setValue(trans->translation.getValue() + ball[i].dir);
      ball[i].dir *= FRICTION;
      if (ball[i].dir.length() < EPSILON) {
        ball[i].moving = 0;
        ball[i].dir = SbVec3f(0.0, 0.0, 0.0);
      }

      // handle collisions with borders
      float x, y, z;
      float vx, vy, vz;
      trans->translation.getValue().getValue(x, y, z);
      ball[i].dir.getValue(vx, vy, vz);
      if ((vx > 0) && (x >= (BOXSIZE / 2 - BALLSIZE)))
        ball[i].dir -= SbVec3f(2 * vx, 0, 0);
      if ((vx < 0) && (x <= -(BOXSIZE / 2 - BALLSIZE)))
        ball[i].dir -= SbVec3f(2 * vx, 0, 0);
      if ((vy > 0) && (y >= (BOXSIZE / 2 - BALLSIZE)))
        ball[i].dir -= SbVec3f(0, 2 * vy, 0);
      if ((vy < 0) && (y <= -(BOXSIZE / 2 - BALLSIZE)))
        ball[i].dir -= SbVec3f(0, 2 * vy, 0);
      if ((vz > 0) && (z >= (BOXSIZE / 2 - BALLSIZE)))
        ball[i].dir -= SbVec3f(0, 0, 2 * vz);
      if ((vz < 0) && (z <= -(BOXSIZE / 2 - BALLSIZE)))
        ball[i].dir -= SbVec3f(0, 0, 2 * vz);

	  // versenken
	  if (fabs(x) > (BOXSIZE/2-HOLESIZE) &&
		  fabs(y) > (BOXSIZE/2-HOLESIZE) &&
		  fabs(z) > (BOXSIZE/2-HOLESIZE))
	  {
        ball[i].moving = 0;
        ball[i].dir = SbVec3f(0.0, 0.0, 0.0);

		if (i==0)
			trans->translation.setValue(ball[0].initialPos.getValue());
		else
			trans->translation.setValue(SbVec3f(OUTSIDE, OUTSIDE, OUTSIDE));

		continue;
	  }

      // handle collisions with other balls
      double coll_dist = 4 * BALLSIZE * BALLSIZE;
      for (int j = 0; j < NUM_BALLS; j++) {
        if (j != i) {
          float x2, y2, z2;
          ((SoTranslation *)ball[j].node->getChild(0))->translation.getValue().getValue(x2, y2, z2);
          double dist;
          double distx = x2 - x;
          double disty = y2 - y;
          double distz = z2 - z;
          dist = distx * distx + disty * disty + distz * distz;
          if (dist <= coll_dist) {
            float vx2, vy2, vz2;
            ball[j].dir.getValue(vx2, vy2, vz2);
            while ( dist <= coll_dist ) {
              x -= vx;
              y -= vy;
              z -= vz;
              x2 -= vx2;
              y2 -= vy2;
              z2 -= vz2;
              distx = x2 - x;
              disty = y2 - y;
              distz = z2 - z;
              dist = distx * distx + disty * disty + distz * distz;
            }
            printf("Collision: %i and %i\n", i, j);
            double v1d = vx * distx + vy * disty + vz * distz;
            double v2d = vx2 * distx + vy2 * disty + vz2 * distz;
            vx -= distx * (v1d - v2d) / dist;
            vy -= disty * (v1d - v2d) / dist;
            vz -= distz * (v1d - v2d) / dist;
            ball[i].dir.setValue(vx, vy, vz);
            ((SoTranslation *)ball[i].node->getChild(0))->translation.setValue(x, y, z);
            vx2 -= distx * (v2d - v1d) / dist;
            vy2 -= disty * (v2d - v1d) / dist;
            vz2 -= distz * (v2d - v1d) / dist;
            ball[j].dir.setValue(vx2, vy2, vz2);
            ball[j].moving = 1;
            ((SoTranslation *)ball[j].node->getChild(0))->translation.setValue(x2, y2, z2);
          }
        }
      }
    }
  }
  if( !ball[0].moving )
    ((SoSwitch *)queueVis->getChild(2))->whichChild = SO_SWITCH_ALL;

}



//----------------------------------------------------------------------------
// Reads the pip sheet and sets the button callbacks
//----------------------------------------------------------------------------

SoNode* SoBillard3dKit::createPipSheet(int uid)
{
  // give your application a nice name
  comm->setContextAliasName(getName(),"Billiard3D");

  // load the geometry from the file into a separator
  SoSeparator *newPipSheet = readFile("billard3dsheet.iv",comm->workingDirectory);

  // reference, so that OIV will not remove it
  newPipSheet->ref();

  // do what you need to setup your connections with the sheet 
  // currently we do nothing with the pip sheet and its one button
  So3DButton *bNewGame = (So3DButton *)findNode(newPipSheet, "BUTTON_NEW_GAME");
  if(bNewGame) 
	bNewGame->addReleaseCallback(bNewGameCB, this);
  else
	fprintf(stderr, "BUTTON_NEW_GAME not found!\n");

  So3DButton *bGameMode = (So3DButton *)findNode(newPipSheet, "BUTTON_GAME_MODE");
  if(bGameMode) 
	bGameMode->addReleaseCallback(bGameModeCB, this);
  else
	fprintf(stderr, "BUTTON_GAME_MODE not found!\n");
  
    
  // unref without deleting to get the reference count correct
  newPipSheet->unrefNoDelete();
  return newPipSheet;
}

//----------------------------------------------------------------------------
// createWindowGeometry
// create window and attach scene graph to it. scene graph contains
// transforms that are connected to global fields updating them from
// the tracking stations.
//----------------------------------------------------------------------------

SoWindowKit* SoBillard3dKit::createWindowGeometry(int index)
{
  //--create a default window
  WindowCreate wc;
  SoWindowKit::defaultWindow(wc);
  SoWindowKit* windowKit = comm->createWindow(NULL, &wc, NULL, NULL);
  windowKit->size = SbVec3f(BOXSIZE/2, BOXSIZE/2, BOXSIZE/2);

  // read the scenegraph from the file "billard.iv" and add it to
  // the window client volume. this is where the applications geometry goes.
  SoSeparator* clientVolume = windowKit->getClientVolumeSeparator();
  SoSeparator* geometry = readFile("billard.iv",comm->workingDirectory);
  // show error if loading geometry went wrong
  assert( geometry );
  // add geometry to the child volume of the window
  clientVolume->addChild( geometry );

  // find balls: i -> arrayindex, j -> used for ball names
  for (int i = 0, j = 0; i < NUM_BALLS; i++, j++) {

    // there is no "Ball8", so continue with "Ball9"
    if (i == 8)
      j++;
    
    char sbuf[10];
    sprintf(sbuf, "Ball%i", j);
    if (!(ball[i].node = (SoSeparator *)SoNode::getByName(sbuf)))
      fprintf(stderr, "Ball %i not found!\n", i);
    else {
      float x, y, z;
      ((SoTranslation *)ball[i].node->getChild(0))->translation.getValue().getValue(x, y, z);
	  ball[i].initialPos = SbVec3f(x, y, z);
      ball[i].dir = SbVec3f(0, 0, 0);
      ball[i].moving = 0;
    }
  }

  // find queues
  if (!(queuepos[0] = (SoStationKit *)SoNode::getByName("QueuePos0_0")))
    fprintf(stderr, "QueuePos0_0 not found!\n");
  if (!(queuepos[1] = (SoStationKit *)SoNode::getByName("QueuePos0_1")))
    fprintf(stderr, "QueuePos0_1 not found!\n");
  if (!(queuepos[2] = (SoStationKit *)SoNode::getByName("QueuePos1_0")))
    fprintf(stderr, "QueuePos0_0 not found!\n");
  if (!(queuepos[3] = (SoStationKit *)SoNode::getByName("QueuePos1_1")))
    fprintf(stderr, "QueuePos0_1 not found!\n");

  if( !(queueVis = (SoSeparator *)SoNode::getByName("QueueVis")))
    fprintf(stderr, "QueueVis not found!\n");

  // initialize timer
  timer = new SoTimerSensor(moveBalls, NULL);
  timer->setInterval(1.0 / 30.0);
  timer->schedule();

  // initialize blink timer
  blinktimer = new SoTimerSensor(blink, NULL);
  blinktimer->setInterval(0.5f);
  blinktimer->schedule();


  timeSincePress = new SoElapsedTime;
  timeSincePress->on = false;
  timeSincePress->timeIn.setValue(0.0);
  timeSincePress->speed.setValue(2.0);

  // initialize engine for calculation of queuevis-positions
  calcQueueVis = new SoCalculator;

  // input A: position of white ball
  calcQueueVis->A.connectFrom( &((SoTranslation *)ball[0].node->getChild(0))->translation );

  // input B: position of queuepos2
  calcQueueVis->B.connectFrom( &queuepos[activeUser * 2]->position );

  // input C: position of queuepos3
  calcQueueVis->C.connectFrom( &queuepos[activeUser * 2 + 1]->position );


  // input a: time since last press
  if( !calcQueueVis->a.connectFrom( &timeSincePress->timeOut ) )
	  fprintf(stderr, "Could not connect\n");

  calcQueueVis->expression.set1Value(0, "ta=0-C[0]+B[0]");
  calcQueueVis->expression.set1Value(1, "tb=0-C[1]+B[1]");
  calcQueueVis->expression.set1Value(2, "tc=0-C[2]+B[2]");
  calcQueueVis->expression.set1Value(3, "td=a+1");
  calcQueueVis->expression.set1Value(4, "oA=vec3f(ta*td+A[0], tb*td+A[1], tc*td+A[2])");	
//  calcQueueVis->expression.set1Value(3, "oA=vec3f(ta+A[0], tb+A[1], tc+A[2])");	
//  calcQueueVis->expression.set1Value(0, "oA=vec3f(A[0]-C[0]+B[0],A[1]-C[1]+B[1], A[2]-C[2]+B[2])");	
  ((SoTranslation *)queueVis->getChild(0))->translation.connectFrom( &calcQueueVis->oA );

  ((SoSwitch *)queueVis->getChild(2))->whichChild = SO_SWITCH_ALL;

//  ((SoRotation *)queueVis->getChild(0))->rotation.setValue(0, 1, 0, 1.57);
//  if( ! ((SoRotation *)queueVis->getChild(0))->rotation.connectFrom( &queuepos[activeUser * 2 + 1]->orientation ) )
//	  fprintf(stderr, "ERROR CONNECTING\n");

  // finally return the new windowkit
  return windowKit;
}



//----------------------------------------------------------------------------
// windowEventCB
// handle clicking of pip-button
//----------------------------------------------------------------------------
SbBool SoBillard3dKit::windowEventCB(void* data,
                                  int message,
                                  SoWindowKit* window, 
                                  int userID,
                                  So3DEvent* event,
                                  SoPath* path) 
{
  // handle only pip-button
  if (message != WM_EVENT)
    return FALSE;
  //fprintf(stderr, "Button-Event: %i\n", event->getType());
  //if ( !(event->getButton(So3DEvent::BUTTON0)
  //       && (event->getType() == So3DEvent::ET_BUTTON0_EVENT ||
  //           event->getType() == So3DEvent::ET_MOVE_EVENT)))
  //  return FALSE;

  // do nothing while white ball is moving
  if (ball[0].moving == 1) return FALSE;

  // only active user may play
  if (userID != activeUser) {
	  return FALSE;
  }

  if (event->getType() != So3DEvent::ET_BUTTON0_EVENT) return FALSE;

  if (event->getButton(So3DEvent::BUTTON0) == 1)
  {
      fprintf (stderr, "started timer\n");
	  pressTime = event->getTime().getMsecValue();
      timeSincePress->timeIn.setValue(1.0);
      timeSincePress->on = true;
	  return FALSE;
  }

  fprintf (stderr, "stopped timer: %d\n", ((SoSFTime *)timeSincePress->timeOut[0])->getValue());
  timeSincePress->on = false;
  timeSincePress->timeIn.setValue(0.0);
  timeSincePress->reset.touch();

  float speed = (float)(event->getTime().getMsecValue() - pressTime) / 500.0;
  if (speed == 0) speed = 0.2f;
  if (speed > 5) speed = 5;

  fprintf (stderr, "speed: %f\n", speed);

  SbVec3f dir;

  if( activeUser == 0 )
    dir = (queuepos[1]->position.getValue() - queuepos[0]->position.getValue()) * speed;
  else
    dir = (queuepos[3]->position.getValue() - queuepos[2]->position.getValue()) * speed;

  ball[0].dir = dir;
  ball[0].moving = 1;

  ((SoSwitch *)queueVis->getChild(2))->whichChild = SO_SWITCH_NONE;

  if( gameMode != ONE_PLAYER ) {
    activeUser = 1 - activeUser;
    calcQueueVis->B.connectFrom( &queuepos[activeUser * 2]->position );
    calcQueueVis->C.connectFrom( &queuepos[activeUser * 2 + 1]->position );
  }
	  


   return TRUE;
}


// Callback for BUTTON_NEW_GAME
void SoBillard3dKit::bNewGameCB(void* data, So3DButton* button) {
  //--this is a static function so we don't know which instance
  //--thus pointer to context is passed in as <data> and we must cast it
  SoBillard3dKit* self = (SoBillard3dKit*)data;

  fprintf(stderr, "Button NewGame pressed\n"); 

  activeUser = 0;

  for(int i = 0; i < NUM_BALLS; i++) {
     float x, y, z;
	 ball[i].dir = SbVec3f(0, 0, 0);
	 ball[i].moving = 0;
     ball[i].initialPos.getValue(x, y, z);
     ((SoTranslation *)ball[i].node->getChild(0))->translation.setValue(x, y, z);
  }
}


// Callback for BUTTON_GAME_MODE
void SoBillard3dKit::bGameModeCB(void* data, So3DButton* button) {
  //--this is a static function so we don't know which instance
  //--thus pointer to context is passed in as <data> and we must cast it
  SoBillard3dKit* self = (SoBillard3dKit*)data;

  fprintf(stderr, "Button GameMode pressed\n"); 
  if( gameMode == ONE_PLAYER ) {
	gameMode = TWO_PLAYER;
    button->buttonColor.setValue(1.0, 0.0, 1.0);
  }
  else {
	gameMode = ONE_PLAYER;
    button->buttonColor.setValue(0.0, 0.0, 1.0);
  }
}
