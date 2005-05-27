 /* ========================================================================
  * (C) 2002 Vienna University of Technology
  * ========================================================================
  * PROJECT: Tabletop Game
  * ======================================================================== */ 
/** contains the implementation of the SoTableTopKit node
  *
  * @author  Christiane Ulbricht
  *
  * $Id: SoTableTopKit.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

/** source file for SoTableTopKit Node.*/


#include <Inventor/nodes/SoText3.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoFile.h>
#include <Inventor/nodes/SoRotor.h>
#include <Inventor/nodes/SoShuttle.h>
#include <Inventor/nodes/SoPendulum.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoWriteAction.h>
#include <stbapi/util/ivio.h>
#include <stbapi/resource/SoPipSheetKit.h>
#include <stbapi/tracker/SoStationKit.h>
#include <stbapi/workspace/SoContextKitSub.h>
#include <stbapi/event3d/Stb3DEventGenerator.h>

#include "SoTableTopKit.h"
#include <Math.h>
#include <time.h>

#include "ReadConfigFile.h"
#include "Convert.h"

//--initialization of kit structure in Inventor
SO_KIT_SOURCE(SoTableTopKit);

//--initialization of context in Studierstube
CONTEXT_APPLICATION(SoTableTopKit);

//----------------------------------------------------------------------------
// static class initialization
//----------------------------------------------------------------------------

void SoTableTopKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoTableTopKit, SoContextKit, "SoContextKit");
}

//----------------------------------------------------------------------------
// virtual constructor
//----------------------------------------------------------------------------

SoContextKit* SoTableTopKit::factory()
{
    return new SoTableTopKit;
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoTableTopKit::SoTableTopKit()
{
    SO_KIT_CONSTRUCTOR(SoTableTopKit);
    SO_KIT_INIT_INSTANCE();
}

//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------

SoTableTopKit::~SoTableTopKit()
{

    fclose(logi);
    delete []players;

    delete []parabelHeight;
    delete []parabelShape; 
    delete []parabelWidth;
    delete []bulletSpeed;
    delete []catapultArmAngle;
    delete []catapultSpeed;

    delete []pointsOfPlayer;
    delete []playerHasShot;
    delete []nrOfBalloons;
    delete []nrOfFallingBalloons;
    delete []intervalLength;

    delete []windmillIntensity;
    delete []windmillAngle;

    for (int i=0;i<nrOfPlayers;i++)
    {
        delete []nrOfBullets[i];
        delete []armUp[i];
        delete []formerAngle[i];        
        delete []angle[i];        
        
        delete []balloonColors[i];
        delete []balloonPositions[i];

        delete []catapultStatus[i];
        delete []catapultTime[i];
        delete []nrOfArmMoves[i];

        delete []yValues[i];
    }

    delete []nrOfBullets;
    delete []armUp;
    delete []formerAngle;
    delete []angle;

    delete []balloonColors;
    delete []balloonPositions;
    delete []initCatapultPositions;
    delete []initWindmillPositions;

    delete []catapultStatus;
    delete []catapultTime;
    delete []nrOfArmMoves;

    delete []yValues;

    destructor();
}

//----------------------------------------------------------------------------
// Reads the pip sheet and sets the button callbacks
//----------------------------------------------------------------------------

SoNode* SoTableTopKit::createPipSheet(int uid)
{
//--read pip sheet from file "samplesheet.iv"

    // give your application a nice name
    comm->setContextAliasName(getName(),"VRLUSample");

    // load the geometry from the file into a separator
    SoSeparator *newPipSheet = readFile("samplesheet.iv",comm->workingDirectory);

    // reference, so that OIV will not remove it
    newPipSheet->ref();

    // do what you need to setup your connections with the sheet 
    // currently we do nothing with the pip sheet and its one button
    
    // unref without deleting to get the reference count correct
    newPipSheet->unrefNoDelete();
    return newPipSheet;
}

//----------------------------------------------------------------------------
// adds special EventCallback to the pieces
//----------------------------------------------------------------------------
void 
SoTableTopKit::addSpecialEventCB(int nrOfThePlayer) 
{
    int offset = nrOfCatapultsPerPlayer+nrOfWindmillsPerPlayer;

    for (int i=0;i<nrOfCatapultsPerPlayer+nrOfWindmillsPerPlayer;i++) 
    {
        SoStationKit* station = (SoStationKit*) stations->getChild(i+offset*nrOfThePlayer);
        SoSeparator* separator = (SoSeparator*) players[nrOfThePlayer]->getChild(i);
        station->addEventCallback(mySpecialEventCB,separator);
    }
}

//----------------------------------------------------------------------------
// removes special EventCallback from the pieces
//----------------------------------------------------------------------------
void 
SoTableTopKit::removeSpecialEventCB(int nrOfThePlayer) 
{
    int offset = nrOfCatapultsPerPlayer+nrOfWindmillsPerPlayer;

    for (int i=0;i<nrOfCatapultsPerPlayer+nrOfWindmillsPerPlayer;i++) 
    {
        SoStationKit* station = (SoStationKit*) stations->getChild(i+offset*nrOfThePlayer);
        station->removeEventCallback(mySpecialEventCB);
    }
}

//----------------------------------------------------------------------------
// adds default EventCallback to the pieces
//----------------------------------------------------------------------------
void 
SoTableTopKit::addDefaultEventCB(int nrOfThePlayer) 
{
    int offset = nrOfCatapultsPerPlayer+nrOfWindmillsPerPlayer;

    for (int i=0;i<nrOfCatapultsPerPlayer+nrOfWindmillsPerPlayer;i++) 
    {
        SoStationKit* station = (SoStationKit*) stations->getChild(i+offset*nrOfThePlayer);
        SoSeparator* separator = (SoSeparator*) players[nrOfThePlayer]->getChild(i);
        SoSeparator* geometry = (SoSeparator*) separator->getChild(0);
        SoTransform* transform = (SoTransform*) geometry->getChild(0);

        station->addEventCallback(myDefaultEventCB,transform);
    }
}

//----------------------------------------------------------------------------
// removes default EventCallback from the pieces
//----------------------------------------------------------------------------
void 
SoTableTopKit::removeDefaultEventCB(int nrOfThePlayer) 
{
    int offset = nrOfCatapultsPerPlayer+nrOfWindmillsPerPlayer;

    for (int i=0;i<nrOfCatapultsPerPlayer+nrOfWindmillsPerPlayer;i++) 
    {
        SoStationKit* station = (SoStationKit*) stations->getChild(i+offset*nrOfThePlayer);
        station->removeEventCallback(myDefaultEventCB);
    }
}

//----------------------------------------------------------------------------
// sets the rotation of transform to the value of the rotation of the stationKit 
//----------------------------------------------------------------------------
void
SoTableTopKit::setRotation(SoTransform* transform, SoStationKit* stationKit)
{
    SbVec3f tempVec;
    float a,b,c,s;

    const class SbRotation* currentOr = stationKit->currentOrientation.getValues(0);
    currentOr->getValue(tempVec,s);    
    tempVec.getValue(a,b,c);

    if (b > -0.9) 
    {
        s = 2.0*M_PI-s;
        b = -1.0;
    }
    tempVec.setValue(0.0, b, 0.0);
    transform->rotation.setValue(tempVec,s);
}

//----------------------------------------------------------------------------
// sets the translation of transform to the values of x,y and z
//----------------------------------------------------------------------------
void
SoTableTopKit::setTranslation(SoTransform* transform, float x, float y, float z)
{
    transform->translation.setValue(x,y,z);
}


//----------------------------------------------------------------------------
// sets the transform node of a piece to the value of the according stationKit
//----------------------------------------------------------------------------
void 
SoTableTopKit::myDefaultEventCB( void *userData, int stationId, 
                             SoStationKit *stationKit, SoHandle3DEventAction *action)
{
    float x,y,z;

    SoTransform* transform = (SoTransform*) userData;

    const class SbVec3f* currentPos = stationKit->currentPosition.getValues(0);
    currentPos->getValue(x,y,z);

    (showTerrain == "Yes") ? y = getYValueOfTerrain(x,z) : y = 0.0;

    setTranslation(transform, x, y, z);
    
    setRotation(transform,stationKit);
}

//----------------------------------------------------------------------------
// sets the transform node of a piece to the value of the according stationKit
// or not, if the marker is outside of the circle
//----------------------------------------------------------------------------
void 
SoTableTopKit::mySpecialEventCB( void *userData, int stationId, 
                             SoStationKit *stationKit, SoHandle3DEventAction *action)
{
    if (gameStatus != "EXECUTE") 
    {
        float xNew,yNew,zNew;
        
        SbVec3f tempVec1,tempVec2;
        bool dontChangePosition = FALSE;
        float distance = 0;

        SoSeparator* piece = (SoSeparator*) userData;
        SoSeparator* geometry = (SoSeparator*) piece->getChild(0);
        SoTransform* transform = (SoTransform*) geometry->getChild(0);

        SoSeparator* lastPos = (SoSeparator*) piece->getChild(1);
        SoTransform* lastPosTransform = (SoTransform*) lastPos->getChild(1);
        SoSwitch* lastPosSwitch = (SoSwitch*) lastPos->getChild(2);

        //lastPos
        tempVec1 = lastPosTransform->translation.getValue();

        const class SbVec3f* currentPos = stationKit->currentPosition.getValues(0);

        currentPos->getValue(xNew,yNew,zNew);

        //position of the marker
        tempVec2.setValue(xNew,yNew,zNew);

        dontChangePosition = (distanceBetweenTwoPoints(tempVec1,tempVec2) >= 0.18);

        if (dontChangePosition) 
        {
            if (lastPosSwitch->whichChild.getValue() == 1) lastPosSwitch->whichChild = 0;
        }
        else 
        {
            (showTerrain == "Yes") ? yNew = getYValueOfTerrain(xNew,zNew) : yNew = 0.0;

            setTranslation(transform, xNew, yNew, zNew);

            setRotation(transform, stationKit);

            if (lastPosSwitch->whichChild.getValue() == 0) lastPosSwitch->whichChild = 1;
        }
    }
}

//----------------------------------------------------------------------------
// plays a wave file
//----------------------------------------------------------------------------
void
SoTableTopKit::playSound(string fileName)
{
    PlaySound(Convert::stringToChar(fileName), NULL, (SND_FILENAME|SND_NOWAIT|SND_NODEFAULT|SND_ASYNC));
}

//----------------------------------------------------------------------------
// returns the square of x
//----------------------------------------------------------------------------
float 
SoTableTopKit::sqr(float x)
{
    return x*x;
}

//----------------------------------------------------------------------------
// returns 1 if x is greater or equal than 0 and -1 if not
//----------------------------------------------------------------------------
float 
SoTableTopKit::signum(float x)
{
    float signum;
    (x >= 0.0) ? signum = 1.0 : signum = -1.0;
    return signum;
}

//----------------------------------------------------------------------------
// returns the distance between point1 and point2
//----------------------------------------------------------------------------
float 
SoTableTopKit::distanceBetweenTwoPoints(SbVec3f point1, SbVec3f point2)
{
    float x1,y1,z1,x2,y2,z2;

    point1.getValue(x1,y1,z1);
    point2.getValue(x2,y2,z2);

    return sqrt(sqr(x1-x2)+sqr(y1-y2)+sqr(z1-z2));
}

//----------------------------------------------------------------------------
// sets the value of the lastPos node to the actual value of the transform
// node of the piece
//----------------------------------------------------------------------------
void 
SoTableTopKit::setLastPosToNewPos(int nrOfThePlayer) 
{
    SbVec3f tempVec; 
    for (int i=0;i<nrOfCatapultsPerPlayer+nrOfWindmillsPerPlayer;i++) 
    {
        SoSeparator* separator = (SoSeparator*) players[nrOfThePlayer]->getChild(i);
        SoSeparator* geometry = (SoSeparator*) separator->getChild(0);
        SoTransform* translation = (SoTransform*) geometry->getChild(0);
        SoSeparator* lastPos = (SoSeparator*) separator->getChild(1);
        SoTransform* lastPosTrans = (SoTransform*) lastPos->getChild(1);

        tempVec = translation->translation.getValue();
        lastPosTrans->translation.setValue(tempVec);
    }
}

//----------------------------------------------------------------------------
// set the drawStyle value of the lastPos node to posStyle
//----------------------------------------------------------------------------
void 
SoTableTopKit::changeStyleOfLastPos(int nrOfThePlayer, int posStyle) 
{
    SoSeparator* firstPiece = (SoSeparator*) players[nrOfThePlayer]->getChild(0);
    SoSeparator* posSep = (SoSeparator*) firstPiece->getChild(1);
    SoDrawStyle* posDraw = (SoDrawStyle*) posSep->getChild(0);
    posDraw->style.setValue(posStyle);
}

//----------------------------------------------------------------------------
// returns true if all markers are at the correct positions
//----------------------------------------------------------------------------
bool 
SoTableTopKit::checkPositions(int nrOfThePlayer)
{
    float distance;
    bool allMarkersAreCorrect =  TRUE;

    for (int i=0;i<nrOfCatapultsPerPlayer+nrOfWindmillsPerPlayer;i++) 
    {
        SoSeparator* separator = (SoSeparator*) players[nrOfThePlayer]->getChild(i);
        SoSeparator* geometry = (SoSeparator*) separator->getChild(0);
        SoTransform* trans = (SoTransform*) geometry->getChild(0);
        SbVec3f fig = trans->translation.getValue();

        SoSeparator* posSeparator = (SoSeparator*) separator->getChild(1);
        SoTransform* postTrans = (SoTransform*) posSeparator->getChild(1);
        SbVec3f pos = postTrans->translation.getValue();

        SoSwitch* posSwitch = (SoSwitch*) posSeparator->getChild(2);

        distance = distanceBetweenTwoPoints(fig, pos);

        if (distance < 0.1) 
            posSwitch->whichChild = 1;
        else 
        {
            allMarkersAreCorrect = FALSE;
            posSwitch->whichChild = 0;
        }
    }
    return allMarkersAreCorrect;
}

//----------------------------------------------------------------------------
// checks if the menuMarker has intersected a button and returns the 
// name of the button
//----------------------------------------------------------------------------
string 
SoTableTopKit::checkMenu(SbViewportRegion vpReg) 
{
    string buttonName = "";

    SoSeparator* menuMarker = (SoSeparator*) menu->getChild(0);
    
    SoGetBoundingBoxAction bboxActionMenuMarker (vpReg);
    bboxActionMenuMarker.apply(menuMarker);
    SbBox3f menuMarkerBox = bboxActionMenuMarker.getBoundingBox();

    SoSeparator* buttonSep = (SoSeparator*) menu->getChild(1);
    SoSwitch* buttonGeometrySwitch = (SoSwitch*) buttonSep->getChild(2);
    
    SoGetBoundingBoxAction bboxButton (vpReg);
    bboxButton.apply(buttonSep);
    SbBox3f buttonBox = bboxButton.getBoundingBox();
    
    if (menuMarkerBox.intersect(buttonBox)) 
    {
        if (buttonGeometrySwitch->whichChild.getValue() == 0) 
        {
            buttonGeometrySwitch->whichChild=1;
            playSound("..\\..\\src\\apps\\tabletop\\sound\\key1.wav");
            if (gameStatus == "MOVE") buttonName = "ChangePlayer";
        }
    }
    else 
    {
        if (buttonGeometrySwitch->whichChild.getValue() == 1) 
        {
            buttonGeometrySwitch->whichChild=0;
            playSound("..\\..\\src\\apps\\tabletop\\sound\\key1.wav");
        }

    }

    return buttonName;
}

//----------------------------------------------------------------------------
// checks if a catapult has intersected a chest. if yes, the parabel of the
// bullets of this catapult is enlarged
//----------------------------------------------------------------------------
void
SoTableTopKit::checkChests() 
{
    int i,j,k;
    float distance;
    bool allMarkersAreCorrect =  TRUE;

    for (i=0;i<nrOfPlayers;i++) 
    {
        if (gameStatus == "ACTION" || i == activePlayer) 
        {
            SoSeparator* player = players[i];
            for (j=0;j<nrOfCatapultsPerPlayer;j++) 
            {
                SoSeparator* catapultSeparator = (SoSeparator*) player->getChild(j);
                SoSeparator* catapultGeometry = (SoSeparator*) catapultSeparator->getChild(0);
                SoTransform* catapultTrans = (SoTransform*) catapultGeometry->getChild(0);
                SbVec3f cat = catapultTrans->translation.getValue();

                for (k=nrOfChests-1;k>=0;k--) 
                {
                    SoSeparator* chestSep = (SoSeparator*) chests->getChild(k);
                    SoTransform* chestTrans = (SoTransform*) chestSep->getChild(0);
                    SbVec3f chest = chestTrans->translation.getValue();

                    distance = distanceBetweenTwoPoints(cat, chest);

                    if (distance < 0.05) 
                    {
                        chests->removeChild(chestSep);
                        nrOfChests--;
                        parabelHeight[i] += 0.01;
                        parabelWidth[i] -= 0.01;
                        parabelShape[i] -= 1.0;
                        intervalLength[i] = sqrt(parabelHeight[i]/parabelWidth[i])/parabelShape[i];
                    }
                }
            }
        }
    }
}

//----------------------------------------------------------------------------
// rotates the arrows of the windmills if the areas of wind of two or more
// windmills are intersecting
//----------------------------------------------------------------------------
void
SoTableTopKit::rotateWindmillArrows() 
{
    int i,j,k,m,n;
    float xWindmillPos,yWindmillPos,zWindmillPos;
    float xTempWindmillPos,yTempWindmillPos,zTempWindmillPos;
    float xArrowPos,yArrowPos,zArrowPos,xArrowPosCalc,zArrowPosCalc;
    float sWindmillRot,sTempWindmillRot,sArrow;
    float distance, windmillArrowAngle,newAngle;
    SbVec3f tempVec;

    for (i=0;i<nrOfPlayers;i++) 
    {
        SoSeparator* player = players[i];
        for (j=0;j<nrOfWindmillsPerPlayer;j++) 
        {
            SoSeparator* windmillSeparator = (SoSeparator*) player->getChild(nrOfCatapultsPerPlayer+j);
            SoSeparator* windmillGeometry = (SoSeparator*) windmillSeparator->getChild(0);
            SoSeparator* flowVis = (SoSeparator*) windmillGeometry->getChild(3);

            SoTransform* windmillTransform = (SoTransform*) windmillGeometry->getChild(0);
            tempVec = windmillTransform->translation.getValue();
            tempVec.getValue(xWindmillPos,yWindmillPos,zWindmillPos);

            windmillTransform->rotation.getValue(tempVec,sWindmillRot);

            //flowVis
            for (n=0;n<3*(int)(windmillIntensity[i]*10);n++) 
            {
                SoSeparator* arrowSep = (SoSeparator*) flowVis->getChild(n);
                SoTransform* arrowTrans = (SoTransform*) arrowSep->getChild(0);
                tempVec = arrowTrans->translation.getValue();
                tempVec.getValue(xArrowPos,yArrowPos,zArrowPos);

                SoTransform* arrowWindmillTrans = (SoTransform*) arrowSep->getChild(1);
                arrowWindmillTrans->rotation.getValue(tempVec,sArrow);                    

                tempVec.setValue(0.0,0.0,0.0);
                arrowWindmillTrans->rotation.setValue(tempVec,0.0);     

                for (k=0;k<nrOfPlayers;k++) 
                {
                    SoSeparator* tempPlayer = players[k];
                    for (m=0;m<nrOfWindmillsPerPlayer;m++) 
                    {
                        if (!((i==k) && (m==j)))
                        {
                            SoSeparator* tempWindmillSeparator = (SoSeparator*) tempPlayer->getChild(nrOfCatapultsPerPlayer+m);
                            SoSeparator* tempWindmillGeometry = (SoSeparator*) tempWindmillSeparator->getChild(0);

                            SoTransform* tempWindmillTransform = (SoTransform*) tempWindmillGeometry->getChild(0);
                            tempVec = tempWindmillTransform->translation.getValue();
                            tempVec.getValue(xTempWindmillPos,yTempWindmillPos,zTempWindmillPos);
                            tempWindmillTransform->rotation.getValue(tempVec,sTempWindmillRot);

                            //to get the correct position of the arrow, rotate it
                            xArrowPosCalc = xArrowPos*cos(sWindmillRot) - zArrowPos*sin(sWindmillRot);
                            zArrowPosCalc = xArrowPos*sin(sWindmillRot) + zArrowPos*cos(sWindmillRot);

                            //and translate it to windmillPos 
                            xArrowPosCalc += xWindmillPos;
                            zArrowPosCalc += zWindmillPos;

                            distance = sqrt(sqr(xArrowPosCalc - xTempWindmillPos)+sqr(zArrowPosCalc - zTempWindmillPos));

                            //distance is smaller than windmillIntensity of the opponent
                            tempVec.setValue(0.0,-1.0,0.0);
                            if (distance < windmillIntensity[k]) 
                            {
                                windmillArrowAngle = angleBetweenVectors(xArrowPosCalc-xTempWindmillPos, zArrowPosCalc-zTempWindmillPos, -sin(sTempWindmillRot),cos(sTempWindmillRot));
                                if (windmillArrowAngle < windmillAngle[k]) 
                                {
                                    newAngle = angleBetweenVectors(xArrowPosCalc-xWindmillPos, zArrowPosCalc-zWindmillPos, xArrowPosCalc-xTempWindmillPos,zArrowPosCalc-zTempWindmillPos);
                                    if (xArrowPosCalc-xWindmillPos < xArrowPosCalc-xTempWindmillPos)
                                        newAngle *= -1.0;
                                    newAngle = newAngle/2.0;
                                    arrowWindmillTrans->rotation.setValue(tempVec,newAngle);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


//----------------------------------------------------------------------------
// alters the geometry of the lastPos node to whichChild (circle or square)
//----------------------------------------------------------------------------
void
SoTableTopKit::switchLastPosGeometry(int nrOfThePlayer, int whichChild) 
{
    for (int i=0;i<nrOfCatapultsPerPlayer+nrOfWindmillsPerPlayer;i++) 
    {
        SoSeparator* separator = (SoSeparator*) players[nrOfThePlayer]->getChild(i);
        SoSeparator* lastPos = (SoSeparator*) separator->getChild(1);
        SoSwitch* lastPosGeometrySwitch = (SoSwitch*) lastPos->getChild(3);
    
        lastPosGeometrySwitch->whichChild = whichChild;
    }
}

//----------------------------------------------------------------------------
// switches the rotor node of the windmills of one player on or off
//----------------------------------------------------------------------------
void
SoTableTopKit::switchWindmills(int nrOfThePlayer, bool on) 
{
    for (int i =0;i<nrOfWindmillsPerPlayer;i++)
    {
        SoSeparator* windmillSep = (SoSeparator*) players[nrOfThePlayer]->getChild(nrOfCatapultsPerPlayer+i);
        SoSeparator* windmillGeometry = (SoSeparator*) windmillSep->getChild(0);
        SoSeparator* windmillContent = (SoSeparator*) windmillGeometry->getChild(1);

        SoSeparator* top = (SoSeparator*) windmillContent->getChild(0);
        SoRotor* windmillRotor = (SoRotor*) top->getChild(1);

        windmillRotor->on = on;
    }
}

//----------------------------------------------------------------------------
// calls switchWindmills for all players
//----------------------------------------------------------------------------
void
SoTableTopKit::switchWindmillsOfAllPlayers(bool on) 
{
    for (int i=0;i<nrOfPlayers;i++) switchWindmills(i, on);
}

//----------------------------------------------------------------------------
// returns the angle between two lines
//----------------------------------------------------------------------------
float
SoTableTopKit::angleBetweenVectors(float x1, float z1, float x2, float z2)
{
    float vec1ByVec2,distanceVec1,distanceVec2,angle,calc;
    vec1ByVec2 = x1*x2 + z1*z2;
    distanceVec1 = sqrt(sqr(x1)+sqr(z1));
    distanceVec2 = sqrt(sqr(x2)+sqr(z2));
    calc = vec1ByVec2/(distanceVec1*distanceVec2);
    if (calc>1.0) calc = 1.0;
    angle = acos(calc);
    
    return angle;
}


//----------------------------------------------------------------------------
// if a balloon touches another balloon, the second balloon is moved out 
// of the way
//----------------------------------------------------------------------------
void
SoTableTopKit::collisionDetectionBalloons(int nrOfThePlayer, int nrOfTheBalloon, float distanceBalloon, float xWindmill, float zWindmill, SbVec3f posOfTheBalloon)
{
    float xOld,yOld,zOld;
    float xM,yM,zM;
    float distanceBalloonM, distanceBalloons;
    float deltaX, deltaZ;
    
    for (int i=0;i<nrOfPlayers;i++) 
    {
        SoSeparator* balloons = (SoSeparator*) players[i]->getChild(nrOfCatapultsPerPlayer+nrOfWindmillsPerPlayer);
        for (int m = nrOfBalloons[i]-1;m>=0;m--)
        {
            //avoid intersecting balloon with itself
            if (!(m==nrOfTheBalloon && i==nrOfThePlayer))  
            {
                SoSeparator* balloonM = (SoSeparator*) balloons->getChild(m);
                SoTransform* balloonTransM = (SoTransform*) balloonM->getChild(1);
                SbVec3f bTrM = balloonTransM->translation.getValue();
                bTrM.getValue(xM,yM,zM);

                distanceBalloonM = sqrt(sqr(xM-xWindmill)+sqr(zM-zWindmill));
                if (distanceBalloonM > distanceBalloon) {
                    distanceBalloons = distanceBetweenTwoPoints(bTrM,posOfTheBalloon);

                    if (distanceBalloons <= (0.07*balloonSize))
                    {
                        posOfTheBalloon.getValue(xOld,yOld,zOld);
                        deltaX = (xM - xOld)/10.0;
                        deltaZ = (zM - zOld)/10.0;
                        balloonTransM->translation.setValue(xM+deltaX,yM,zM+deltaZ);
                        collisionDetectionBalloons(nrOfThePlayer, m, distanceBalloonM, xWindmill, zWindmill, bTrM);
                    }
                }
            }
        }
    }
}

//----------------------------------------------------------------------------
// calculates the positions of the balloons of one player
//----------------------------------------------------------------------------
void 
SoTableTopKit::positionOfTheBalloons(int nrOfThePlayer) 
{
    int j,k;
    float xOld,yOld,zOld,xCalc,zCalc,xNew,zNew,deltaX,deltaZ,x,y,z,s, xDirWindmill, zDirWindmill;
    float distanceBalloon, balloonWindmillAngle;

    SoSeparator* balloons = (SoSeparator*) players[nrOfThePlayer]->getChild(nrOfCatapultsPerPlayer+nrOfWindmillsPerPlayer);

    /* position of the balloons */
    for (j = nrOfBalloons[nrOfThePlayer]-1;j>=0;j--)
    {
        SoSeparator* balloon = (SoSeparator*) balloons->getChild(j);
        SoTransform* balloonTrans = (SoTransform*) balloon->getChild(1);

        SbVec3f bTr = balloonTrans->translation.getValue();

        bTr.getValue(xOld,yOld,zOld);
        xCalc=xOld;
        zCalc=zOld;

        xNew =xOld;
        zNew =zOld;

        for (k=0;k<nrOfWindmillsPerPlayer;k++)
        {
            SoSeparator* windmillSeparator = (SoSeparator*) players[nrOfThePlayer]->getChild(nrOfCatapultsPerPlayer+k);
            SoSeparator* windmillGeometry = (SoSeparator*) windmillSeparator->getChild(0);
            SoTransform* windmillTransform = (SoTransform*) windmillGeometry->getChild(0);

            SbVec3f tempVec = windmillTransform->translation.getValue();
            tempVec.getValue(x,y,z);
            windmillTransform->rotation.getValue(tempVec,s);

            //translate balloon so that windmill is placed at origin and curPosWindmill can be ignored
            xCalc -= x;
            zCalc -= z;

            distanceBalloon = sqrt(sqr(xCalc)+sqr(zCalc));

            if (distanceBalloon < windmillIntensity[nrOfThePlayer])
            {
                //vector of the direction of the windmill
                xDirWindmill = -sin(s);
                zDirWindmill = cos(s);

                //angle between the vector of the balloon (xCalc,zCalc) and the vector of the dir of the windmill
                balloonWindmillAngle = angleBetweenVectors(xCalc,zCalc,xDirWindmill,zDirWindmill);

                if (balloonWindmillAngle < windmillAngle[nrOfThePlayer]) 
                {
                    float maxWert = 0.005;
                    deltaX = (xCalc*windmillIntensity[nrOfThePlayer]/(100.0*balloonWindmillAngle*distanceBalloon));
                    if (fabs(deltaX) > maxWert) deltaX = maxWert*signum(deltaX);

                    deltaZ = (zCalc*windmillIntensity[nrOfThePlayer]/(100.0*balloonWindmillAngle*distanceBalloon));
                    if (fabs(deltaZ) > maxWert) deltaZ = maxWert*signum(deltaZ);
                    xNew = xOld + deltaX;
                    zNew = zOld + deltaZ;

                    collisionDetectionBalloons(nrOfThePlayer, j, distanceBalloon, x, z, bTr);
                    if (gameType != "StrategyGame")
                    {
                        SoMaterial* mat = (SoMaterial*) balloon->getChild(0);
                        mat->transparency = 0.0;
                    }
                }
                else
                {
                    SoMaterial* mat = (SoMaterial*) balloon->getChild(0);
                    mat->transparency = 0.5;
                }            
            }
            else
            {
                SoMaterial* mat = (SoMaterial*) balloon->getChild(0);
                mat->transparency = 0.5;
            }
        } //for nrOfWindmillsPerPlayer
        if (xNew > fieldLength) xNew = fieldLength;
        if (xNew < -fieldLength) xNew = -fieldLength;

        if (zNew > fieldLength) zNew = fieldLength;
        if (zNew < -fieldLength) zNew = -fieldLength;

        balloonTrans->translation.setValue(xNew,yOld,zNew);
    } //for nrOfBalloons
}

//----------------------------------------------------------------------------
// calculates the position of the falling balloon of one player and removes
// them from the scene graph if they reached the ground
//----------------------------------------------------------------------------
void 
SoTableTopKit::positionOfTheFallingBalloons(int nrOfThePlayer) 
{
    int j;
    float xOld,yOld,zOld,yNew;

    SoSeparator* balloons = (SoSeparator*) players[nrOfThePlayer]->getChild(nrOfCatapultsPerPlayer+nrOfWindmillsPerPlayer);
    SoSeparator* fallingBalloons = (SoSeparator*) players[nrOfThePlayer]->getChild(nrOfCatapultsPerPlayer+nrOfWindmillsPerPlayer+1);

    /* position of the falling balloons */
    for (j = nrOfFallingBalloons[nrOfThePlayer]-1;j>=0;j--)
    {
        SoSeparator* fallingBalloon = (SoSeparator*) fallingBalloons->getChild(j);
        SoTransform* fallingBalloonTrans = (SoTransform*) fallingBalloon->getChild(1);

        SbVec3f bTr = fallingBalloonTrans->translation.getValue();

        bTr.getValue(xOld,yOld,zOld);
        yNew = yOld-0.005;
        if (yNew > 0.0)
        {
            fallingBalloonTrans->translation.setValue(xOld,yNew,zOld);

            bTr = fallingBalloonTrans->scaleFactor.getValue();
            bTr.getValue(xOld,yOld,zOld);
            fallingBalloonTrans->scaleFactor.setValue(xOld-0.05,yOld-0.05,zOld-0.05);
        }
        else 
        {
            if (newBalloons == "Yes") 
            {
                /* move fallingBalloon from fallingBalloons to balloons */
                int balloonNr = Convert::sbNameToInt(fallingBalloon->getName());

                SoTransform* trans = (SoTransform*) fallingBalloon->getChild(1);
                trans->translation.setValue(balloonPositions[nrOfThePlayer][balloonNr]);
                trans->scaleFactor.setValue(1.0,1.0,1.0);

                balloons->addChild(fallingBalloon);
                nrOfBalloons[nrOfThePlayer]++;
            }

            fallingBalloons->removeChild(fallingBalloon);
            nrOfFallingBalloons[nrOfThePlayer]--;
        }
    }
}

//----------------------------------------------------------------------------
// calculates the position of the arm of the catapult and the bullets 
// of one player
//----------------------------------------------------------------------------
void 
SoTableTopKit::positionOfTheCatapults(int nrOfThePlayer, double count, SbViewportRegion vpReg)
{
    int j,k,m,n;
    float xOld,yOld,zOld,zCalc,yNew,zNew,x,y,z;

    SoSeparator* points = (SoSeparator*) players[nrOfThePlayer]->getChild(nrOfCatapultsPerPlayer+nrOfWindmillsPerPlayer+2);

    for (j=0;j<nrOfCatapultsPerPlayer;j++)
    {
        SoSeparator* catapultSeparator = (SoSeparator*) players[nrOfThePlayer]->getChild(j);
        SoSeparator* catapultGeometry = (SoSeparator*) catapultSeparator->getChild(0);
        SoTransform* catapultTransform = (SoTransform*) catapultGeometry->getChild(0);
        SoSeparator* catapultData = (SoSeparator*) catapultGeometry->getChild(1);

        SoSeparator* arm = (SoSeparator*) catapultData->getChild(1);
        SoRotationXYZ* armRot = (SoRotationXYZ*) arm->getChild(1);

        SoSeparator* bulletCatapult = (SoSeparator*) catapultData->getChild(2);
        SoDrawStyle* bulletCatStyle = (SoDrawStyle*) bulletCatapult->getChild(0);
        SoRotationXYZ* bulletCatapultRot = (SoRotationXYZ*) bulletCatapult->getChild(2);
        SoFile* bulletGeometry = (SoFile*) bulletCatapult->getChild(4);

        SoSeparator* bullets = (SoSeparator*) catapultData->getChild(3);

        float bulletZ, bulletY, armLength;
        armLength=0.055;


        if (catapultStatus[nrOfThePlayer][j] != "STOPPED")
        {
            angle[nrOfThePlayer][j] = catapultArmAngle[nrOfThePlayer] *(1-fabs(cos((count-catapultTime[nrOfThePlayer][j])*catapultSpeed[nrOfThePlayer]+M_PI)));
    
            /* position of the catapult and the catapult bullet */
            if (armUp[nrOfThePlayer][j]) 
            {
                //arm starts to go down
                if (formerAngle[nrOfThePlayer][j] > angle[nrOfThePlayer][j]) 
                {
                    if (gameStatus != "ACTION" && gameStatus != "WAITFORGAMEWON")
                        playSound("..\\..\\src\\apps\\tabletop\\sound\\hitball.wav");

                    armUp[nrOfThePlayer][j] = FALSE;
                    /* arm of the catapult */
                    armRot->angle = angle[nrOfThePlayer][j];

                    /* catapult bullet */
                    bulletCatStyle->style.setValue(SoDrawStyle::INVISIBLE);

                    /* new bullet */
                    nrOfBullets[nrOfThePlayer][j]++;
                    bulletY = armLength*sin(formerAngle[nrOfThePlayer][j]) + 0.005;
                    bulletZ = (armLength/2.0)-armLength*cos(formerAngle[nrOfThePlayer][j]) - 0.005;

                    SoSeparator* sep = new SoSeparator;

                    SoTransform* bulletTransFix = new SoTransform;
                    bulletTransFix->translation.setValue(0.0,bulletY,bulletZ);

                    SoTransform* bulletTrans = new SoTransform;
                    bulletTrans->translation.setValue(0.0,0.0,0.0);

                    sep->addChild(bulletTransFix);
                    sep->addChild(bulletTrans);
                    sep->addChild(bulletGeometry);
                    bullets->addChild(sep);
                }
                //arm goes up
                else                    
                {
                    /* arm of the catapult */
                    armRot->angle = angle[nrOfThePlayer][j];
                    
                    /* bullet */
                    bulletCatapultRot->angle = angle[nrOfThePlayer][j];
                }
            }
            else 
            {
                //arm starts to go up
                if (formerAngle[nrOfThePlayer][j] < angle[nrOfThePlayer][j]) 
                {
                    if (catapultStatus[nrOfThePlayer][j] == "SHOULDSTOP" && nrOfArmMoves[nrOfThePlayer][j] == 1)
                    {
                        catapultStatus[nrOfThePlayer][j] = "STOPPED";
                    }
                    armUp[nrOfThePlayer][j] = TRUE;

                    /* bullet */
                    bulletCatStyle->style.setValue(SoDrawStyle::FILLED);
                    bulletCatapultRot->angle=angle[nrOfThePlayer][j];
                        
                    nrOfArmMoves[nrOfThePlayer][j]--;
                }
                //arm goes down
                else                    
                {
                    armRot->angle = angle[nrOfThePlayer][j];
                }
            } // if (armUp[i][j]) 

            formerAngle[nrOfThePlayer][j]= angle[nrOfThePlayer][j];
        } // if (catapultStatus[i][j] != "STOPPED")


        /* position of the bullets */
        for (k = nrOfBullets[nrOfThePlayer][j]-1;k>=0;k--)
        {
            SoSeparator* bullet = (SoSeparator*) bullets->getChild(k);
            SoTransform* bulletTrans = (SoTransform*) bullet->getChild(1);

            SbVec3f tempVec = bulletTrans->translation.getValue();
            tempVec.getValue(xOld,yOld,zOld);

            /* parabel */
            zNew = zOld+bulletSpeed[nrOfThePlayer];
            zCalc = (zNew - intervalLength[nrOfThePlayer])*parabelShape[nrOfThePlayer];
            yNew = -1.0*parabelWidth[nrOfThePlayer]*(zCalc*zCalc) + parabelHeight[nrOfThePlayer];

            //if the bullet meets the ground
            if (yNew < -armLength-0.005)
            {
                bullets->removeChild(bullet);
                nrOfBullets[nrOfThePlayer][j]--;
            }
            //else check for intersections with balloons
            else
            {
                bulletTrans->translation.setValue(0.0,yNew,zNew);
        
                for (m = 0;m<nrOfPlayers;m++)
                {
                    SoSeparator* tempPlayer = players[m];
                    SoSeparator* balloonsOfTempPlayer = (SoSeparator*) tempPlayer->getChild(nrOfCatapultsPerPlayer+nrOfWindmillsPerPlayer);
                    SoSeparator* fallingBalloonsOfTempPlayer = (SoSeparator*) tempPlayer->getChild(nrOfCatapultsPerPlayer+nrOfWindmillsPerPlayer+1);

                    for (n = nrOfBalloons[m]-1;n>=0;n--)
                    {
                        SoSeparator* balloon = (SoSeparator*) balloonsOfTempPlayer->getChild(n);
                        SoGetBoundingBoxAction bboxActionBalloon (vpReg);
                        bboxActionBalloon.apply(balloon);
                        SbBox3f balloonBox = bboxActionBalloon.getBoundingBox();

                        float s,a,b,c;

                        SbVec3f tempVec = catapultTransform->translation.getValue();
                        tempVec.getValue(x,y,z);
                        catapultTransform->rotation.getValue(tempVec,s);
                        tempVec.getValue(a,b,c);

                        SoSeparator* temp = new SoSeparator;
                        SoTransform* tempTrans = new SoTransform;
                        tempTrans->translation.setValue(x,y,z);
                        tempTrans->rotation.setValue(tempVec,s);
                        temp->addChild(tempTrans);
                        temp->addChild(bullet);

                        SoGetBoundingBoxAction bboxActionBullet (vpReg);
                        bboxActionBullet.apply(temp);
                        SbBox3f bulletBox = bboxActionBullet.getBoundingBox();

                        //bullet hit balloon
                        if (balloonBox.intersect(bulletBox))
                        {
                            /* set the points */
                            nrOfBalloons[m]--;
                            nrOfFallingBalloons[m]++;
                            if ((nrOfPlayers == 1) || (nrOfThePlayer != m)) 
                            {
                                pointsOfPlayer[nrOfThePlayer]++;
                                playerHasShot[nrOfThePlayer]++;
                            }
                            else pointsOfPlayer[nrOfThePlayer]--;

                            SoText3* pointsText = (SoText3*) points->getChild(2);
                            pointsText->string = pointsOfPlayer[nrOfThePlayer];
        
                            /* move balloon from balloons to fallingBalloons */
                            SoTransform* balloonTrans = (SoTransform*) balloon->getChild(1);
                            balloonTrans->scaleFactor.setValue(0.95,0.95,0.95);

                            fallingBalloonsOfTempPlayer->addChild(balloon);
                            balloonsOfTempPlayer->removeChild(balloon);

                            if (!isGameWon(nrOfThePlayer))
                                playSound("..\\..\\src\\apps\\tabletop\\sound\\bigblow.wav");
                        }
                    } //for all balloons                        
                } //for all players
            } //else
        }//for each bullet
    } //end for (each catapult) for (j=0;j<nrOfCatapultsPerPlayer;j++)
}

//----------------------------------------------------------------------------
// calculates position and orientation of the pieces in followUp mode
// and returns true if all pieces stand still
//----------------------------------------------------------------------------
bool 
SoTableTopKit::movePieces() 
{
    int i,j;
    SbVec3f tempVec1,tempVec2;
    bool dontChangePosition;
    float distance;
    float xOld,yOld,zOld,xNew,yNew,zNew;
    int offset = nrOfCatapultsPerPlayer+nrOfWindmillsPerPlayer;
    bool allPiecesStandStill = TRUE;

    for (i=0;i<nrOfPlayers;i++) 
    {
        SoSeparator* player = players[i];

        for (j=0;j<nrOfCatapultsPerPlayer+nrOfWindmillsPerPlayer;j++) 
        {
            dontChangePosition = FALSE;
            distance = 0.0;

            SoSeparator* piece = (SoSeparator*) player->getChild(j);
            SoSeparator* geometry = (SoSeparator*) piece->getChild(0);
            SoTransform* transform = (SoTransform*) geometry->getChild(0);

            SoStationKit* stationKit = (SoStationKit*) stations->getChild(j+offset*i);
            const class SbVec3f* currentPos = stationKit->currentPosition.getValues(0);

            //position of the marker
            currentPos->getValue(xNew,yNew,zNew);
            (showTerrain == "Yes") ? yNew = getYValueOfTerrain(xNew,zNew) : yNew = 0.0;
            tempVec2.setValue(xNew,yNew,zNew);
            
            //old position of the catapult
            tempVec1 = transform->translation.getValue();
            tempVec1.getValue(xOld,yOld,zOld);

            //distance between the marker and the old position of the catapult
            distance = distanceBetweenTwoPoints(tempVec1,tempVec2);
            
            //set position of cross to the position of the marker
            SoSeparator* cross = (SoSeparator*) crosses->getChild(j+offset*i);
            SoTransform* crossTrans = (SoTransform*) cross->getChild(1);
            crossTrans->translation.setValue(xNew,yNew,zNew);
          
            //make cross visible or invisible, depending on its distance from the piece
            SoDrawStyle* crossStyle = (SoDrawStyle*) cross->getChild(0);
            if (distance >= 0.08) 
                crossStyle->style.setValue(SoDrawStyle::FILLED);
            else
                crossStyle->style.setValue(SoDrawStyle::INVISIBLE);

            if (distance >= 0.005 && followSpeed < distance) 
            {
                //new position of the catapult is a small step in the direction of the marker
                xNew = xOld+(xNew-xOld)*(followSpeed/distance); 
                zNew = zOld+(zNew-zOld)*(followSpeed/distance);
                (showTerrain == "Yes") ? yNew = getYValueOfTerrain(xNew,zNew) : yNew = 0.0;
                
                allPiecesStandStill = FALSE;
            }
            else
            {
                xNew = xOld; yNew = yOld; zNew = zOld;
            }

            //in strategy mode, check if the piece is inside or outside of the circle
            SoSwitch* lastPosSwitch = NULL;
            if ((gameStatus != "ACTION")&&(gameStatus != "INIT")
                &&(gameStatus != "WAITFORGAMEWON")&&(gameStatus != "GAMEWON")) 
            {
                tempVec2.setValue(xNew,yNew,zNew);

                SoSeparator* lastPos = (SoSeparator*) piece->getChild(1);
                SoTransform* lastPosTransform = (SoTransform*) lastPos->getChild(1);
                lastPosSwitch = (SoSwitch*) lastPos->getChild(2);
            
                tempVec1 = lastPosTransform->translation.getValue();
                dontChangePosition = (distanceBetweenTwoPoints(tempVec1,tempVec2) >= 0.18);
            
                //set the circle to green again
                if ((!dontChangePosition) && (lastPosSwitch->whichChild.getValue() == 0)) lastPosSwitch->whichChild = 1; 
            }

            if (dontChangePosition)
            {
                if (lastPosSwitch->whichChild.getValue() == 1) lastPosSwitch->whichChild = 0;
                allPiecesStandStill = TRUE;
            }
            else 
            {
                setTranslation(transform, xNew,yNew,zNew);
                setRotation(transform,stationKit);
            }
        } //for j
    }//for i
    return allPiecesStandStill;
}

//----------------------------------------------------------------------------
// creates the menu node, containing the menu marker and the button
//----------------------------------------------------------------------------
SoSeparator* 
SoTableTopKit::createMenu(SoSeparator* coneFile, SoSeparator* nextFile) 
{
    float x,y,z;

    SoSeparator* menuSeparator = new SoSeparator;

    SoSeparator* menuMarker = new SoSeparator;
    SoDrawStyle* menuMarkerStyle = new SoDrawStyle;
    if (gameStatus == "ACTION") 
	    menuMarkerStyle->style.setValue(SoDrawStyle::INVISIBLE);
    else
	    menuMarkerStyle->style.setValue(SoDrawStyle::FILLED);

    SoTransform* menuMarkerTrans = new SoTransform;

    menuMarker->addChild(menuMarkerStyle);
    menuMarker->addChild(menuMarkerTrans);
    menuMarker->addChild(coneFile);

    menuSeparator->addChild(menuMarker);

    SoSeparator* chPlayerButtonSep = new SoSeparator;

    SoSwitch* chPlayerButtonTransSwitch = new SoSwitch;
    chPlayerButtonTransSwitch->whichChild=0;

    x = 2.0*fieldLength/5.0;
    z = 4.0*fieldLength/5.0;
    SoTransform* chPlayerTrans1 = new SoTransform;
    (showTerrain == "Yes") ? y = getYValueOfTerrain(x, -z) : y = 0.0; 
    chPlayerTrans1->translation.setValue(x, y, -z);
    chPlayerTrans1->rotation.setValue(SbVec3f(0.0,1.0,0.0),M_PI/2.0);

    SoTransform* chPlayerTrans2 = new SoTransform;
    (showTerrain == "Yes") ? y = getYValueOfTerrain(-x, -z) : y = 0.0; 
    chPlayerTrans2->translation.setValue(-x, y, -z);
    chPlayerTrans2->rotation.setValue(SbVec3f(0.0,1.0,0.0),M_PI/2.0);

    chPlayerButtonTransSwitch->addChild(chPlayerTrans1);
    chPlayerButtonTransSwitch->addChild(chPlayerTrans2);

    SoSwitch* chPlayerButtonGeometrySwitch = new SoSwitch;
    chPlayerButtonGeometrySwitch->whichChild=0;

    SoScale* chPlayerScale1 = new SoScale;
    
    SoScale* chPlayerScale2 = new SoScale;
    chPlayerScale2->scaleFactor.setValue(1.0,0.85,1.0);

    chPlayerButtonGeometrySwitch->addChild(chPlayerScale1);
    chPlayerButtonGeometrySwitch->addChild(chPlayerScale2);

    chPlayerButtonSep->addChild(menuMarkerStyle);
    chPlayerButtonSep->addChild(chPlayerButtonTransSwitch);
    chPlayerButtonSep->addChild(chPlayerButtonGeometrySwitch);
    chPlayerButtonSep->addChild(nextFile);

    menuSeparator->addChild(chPlayerButtonSep);

    return menuSeparator;
}

//----------------------------------------------------------------------------
// adds a cross to the crosses separator
//----------------------------------------------------------------------------
void
SoTableTopKit::addCross(SoSeparator* crossFile)
{
    SoSeparator* crossSep = new SoSeparator;

    SoDrawStyle* crossStyle = new SoDrawStyle;
    crossStyle->style.setValue(SoDrawStyle::INVISIBLE);

    SoTransform* crossTrans = new SoTransform;

    crossSep->addChild(crossStyle);
    crossSep->addChild(crossTrans);
    crossSep->addChild(crossFile);
    crosses->addChild(crossSep);
}

//----------------------------------------------------------------------------
// creates the chests node
//----------------------------------------------------------------------------
SoSeparator* 
SoTableTopKit::createChests(SoSeparator* chestFile) 
{
    float x,y,z;
    SoSeparator* chestSeparator = new SoSeparator;

    SoRotor* rotorChest = new SoRotor;
    rotorChest->rotation.setValue(0.0, 1.0, 0.0, 0.0); //  #y-axis
    rotorChest->speed = 0.05;
    
    SoPendulum* pendulum = new SoPendulum;
    SbVec3f* vec = new SbVec3f(0.0, 0.0, 1.0);
    pendulum->rotation0.setValue(*vec, -0.2); 
    pendulum->rotation1.setValue(*vec, 0.2);

    if (showAnimation == "Yes") 
    {
        chestFile->insertChild(rotorChest,0);
        chestFile->insertChild(pendulum,1);
    }

    float reducedFieldLength = (2.0*fieldLength/3.0);
    for (int i=0;i<nrOfChests;i++) 
    {
        SoSeparator* chestSep = new SoSeparator; 
        chestSeparator->addChild(chestSep);

        SoTransform* transformChest = new SoTransform;
        transformChest->scaleFactor.setValue(0.25,0.25,0.25);
        vec->setValue(0.0,1.0,0.0);
        transformChest->rotation.setValue(*vec, (float)i*(M_PI/nrOfChests));

        if (i<nrOfChests/2) 
            x = (float)(rand()%(int)(reducedFieldLength*10))/10.0;
        else 
            x = (float)(rand()%(int)(reducedFieldLength*10.0))/10.0-reducedFieldLength;
        z = (float)(-rand()%(int)(reducedFieldLength*20.0))/10.0+reducedFieldLength;
        (showTerrain == "Yes") ? y = getYValueOfTerrain(x,z) : y = 0.0;
        transformChest->translation.setValue(x,y,z);
        
        chestSep->addChild(transformChest);
        chestSep->addChild(chestFile);
    }
    return chestSeparator;
}

//----------------------------------------------------------------------------
// returns true if the player has won the game
//----------------------------------------------------------------------------
boolean 
SoTableTopKit::isGameWon(int nrOfThePlayer) 
{
    return (playerHasShot[nrOfThePlayer]==shootToWin || nrOfBalloons[(nrOfThePlayer+1) % nrOfPlayers] == 0);
}

//----------------------------------------------------------------------------
// adds the gameWon node to the scene graph
//----------------------------------------------------------------------------
void
SoTableTopKit::addGameWonText()
{ 
    playSound("..\\..\\src\\apps\\tabletop\\sound\\bigblow_and_finale.wav");

    //gameWon
    gameWon = new SoSeparator;
    root2->addChild(gameWon);

    SoMaterial* gameWonMat = new SoMaterial;
    gameWonMat->diffuseColor.setValue(balloonColors[activePlayer][0]);
    gameWon->addChild(gameWonMat);

    SoRotor* rotor = new SoRotor;
    rotor->rotation.setValue(0.0, 1.0, 0.0, 0.0); //  #y-axis
    rotor->speed = 0.2;
    rotor->on = TRUE;
    gameWon->addChild(rotor);

    SoShuttle* shuttle = new SoShuttle;
    shuttle->translation0.setValue(0.0, 0.3,0.0);
    shuttle->translation1.setValue(0.0,0.5,0.0);
    gameWon->addChild(shuttle);

    SoScale* scale = new SoScale;
	scale->scaleFactor.setValue(0.005, 0.005, 0.005);
    gameWon->addChild(scale);

    SoText3* gameWonText = new SoText3;
    gameWonText->parts = SoText3::ALL;
    gameWonText->justification = SoText3::CENTER;
    if (activePlayer == 0)
        gameWonText->string.set1Value(0,"GELB");
    else 
        gameWonText->string.set1Value(0,"BLAU");
    gameWonText->string.set1Value(1,"GEWINNT"); 
    gameWon->addChild(gameWonText);
}

//----------------------------------------------------------------------------
// returns true if the animations started in execute status have stopped
//----------------------------------------------------------------------------
boolean
SoTableTopKit::animationStopped()
{
    int i;
    bool stopped = TRUE;
    stopped = (catapultStatus[activePlayer][0] == "STOPPED");

    //check if there are flying bullets left
    i=0;
    while (stopped && i<nrOfCatapultsPerPlayer) 
    {
        stopped &= (nrOfBullets[activePlayer][i] == 0);
        i++;
    }
    //check if there are falling balloons left
    i=0;
    while (stopped && i<nrOfPlayers) 
    {
        stopped &= (nrOfFallingBalloons[i] == 0);
        i++;
    }
    return stopped;
}


//----------------------------------------------------------------------------
// handels the state of the application
//----------------------------------------------------------------------------
void 
SoTableTopKit::myTimerCB(void *data, SoSensor *sensor) 
{    
    int i;

    SbViewportRegion vpReg;

    SbTime time = ((SoTimerSensor *)sensor)->getBaseTime();
    double count = (time.getValue()-myBaseTime.getValue());

    bool allPiecesStandStill = TRUE;
    if (movementType == "FollowUp" && gameStatus != "EXECUTE") 
        allPiecesStandStill = movePieces();

    string buttonName = "";

    if (gameType == "StrategyGame")
        buttonName = checkMenu(vpReg); 

    if (gameStatus == "ACTION" || gameStatus == "MOVE")
    {
        checkChests(); 
        if (showFlowVis == "Yes") rotateWindmillArrows();
    }

    if (gameStatus == "INIT") 
    {
        bool allMarkersAreCorrect = TRUE;
        
        //at this state checkPositions checks if the pieces are located at initPos
        for (i=0;i<nrOfPlayers;i++)
            allMarkersAreCorrect &= checkPositions(i); 

        if (allMarkersAreCorrect) 
        {
            playSound("..\\..\\src\\apps\\tabletop\\sound\\arthur.wav");

            for (i=0;i<nrOfPlayers;i++)
                for (int j=0;j<nrOfCatapultsPerPlayer+nrOfWindmillsPerPlayer;j++)
                {
                    SoSeparator* piece = (SoSeparator*) players[i]->getChild(j);
                    piece->removeChild(1);
                }
            
            if (gameType == "StrategyGame")
            {
                gameStatus = "MOVE";

                //set activePlayer
                activePlayer = 0;
        
                //make lastPos-fields (on initPos position) visible
                changeStyleOfLastPos(activePlayer,SoDrawStyle::FILLED);

                for (i=0;i<nrOfPlayers;i++) setLastPosToNewPos(i);

                if (movementType != "FollowUp") 
                {
                    removeDefaultEventCB(activePlayer);
                    addSpecialEventCB(activePlayer);
                }
            }
            else
            {
                gameStatus = "ACTION";
                switchWindmillsOfAllPlayers(TRUE);
            }

        }
    }
    else if (gameStatus == "MOVE") 
    {
        if (buttonName == "ChangePlayer") 
        { 
            gameStatus = "WAITFORSTANDSTILL";
            for (i=0;i<nrOfCatapultsPerPlayer;i++) 
            {
                catapultStatus[activePlayer][i] = "SHOULDSTOP";
                nrOfArmMoves[activePlayer][i] = 1;
            }
        }
    }
    else if (gameStatus == "GOTOLASTPOS") 
    {
        //checkMenu is not called, so the changePlayerButton cannot be pressed
        bool allMarkersAreCorrect = TRUE;

        allMarkersAreCorrect &= checkPositions(activePlayer);
        if (allMarkersAreCorrect) 
        {
            gameStatus = "MOVE";
            switchLastPosGeometry(activePlayer,0);
            if (movementType != "FollowUp") 
            {
                removeDefaultEventCB(activePlayer);
                addSpecialEventCB(activePlayer);
            }
        }
    }
    else if (gameStatus == "WAITFORSTANDSTILL") 
    {
        if (allPiecesStandStill) 
        {
            gameStatus = "EXECUTE";
            switchWindmills(activePlayer, TRUE);
            removeSpecialEventCB(activePlayer);
        }
    }
    else if (gameStatus == "EXECUTE") 
    {
        if (!animationStopped()) 
        {
            positionOfTheBalloons(activePlayer);        

            for (i=0;i<nrOfPlayers;i++) positionOfTheFallingBalloons(i);

            positionOfTheCatapults(activePlayer, count, vpReg);
        }
        else 
        {
            bool allMarkersAreCorrect = TRUE;

            switchWindmills(activePlayer, FALSE);

            addSpecialEventCB(activePlayer);

            //make lastPos-fields of "old" player invisible
            changeStyleOfLastPos(activePlayer,SoDrawStyle::INVISIBLE);

            if (isGameWon(activePlayer)) 
            {
                gameStatus = "GAMEWON";
                addGameWonText();
            }
            else 
            {
                setLastPosToNewPos(activePlayer);

                if (movementType != "FollowUp") 
                {
                    removeSpecialEventCB(activePlayer);
                    addDefaultEventCB(activePlayer);
                }

                //change active player
                activePlayer = (activePlayer+1) % nrOfPlayers;
                
                allMarkersAreCorrect &= checkPositions(activePlayer);
                if (!allMarkersAreCorrect) 
                {
                    gameStatus = "GOTOLASTPOS";
                    switchLastPosGeometry(activePlayer,1);
                }
                else 
                {
                    gameStatus = "MOVE";
                    if (movementType != "FollowUp") 
                    {
                        removeDefaultEventCB(activePlayer);
                        addSpecialEventCB(activePlayer);
                    }
                }

                //make lastPos-fields of "new" player visible
                changeStyleOfLastPos(activePlayer,SoDrawStyle::FILLED);

                //change the position of the chPlayerButton
                SoSeparator* chPlayerButtonSep = (SoSeparator*) menu->getChild(1);
                SoSwitch* chPlayerButtonTransSwitch = (SoSwitch*) chPlayerButtonSep->getChild(1);
                chPlayerButtonTransSwitch->whichChild = activePlayer;
            }
        }
    }
    else if (gameStatus == "WAITFORGAMEWON") 
    {
        if (!animationStopped()) 
        {
            for (i=0;i<nrOfPlayers;i++)
            {
                positionOfTheBalloons(i);  
                
                positionOfTheFallingBalloons(i);

                positionOfTheCatapults(i,count, vpReg);
            }
        }
        else
        {
            gameStatus = "GAMEWON";
        }

    }
    else if (gameStatus == "GAMEWON") 
    {
    }
    else if (gameStatus == "ACTION") 
    {
        bool gameIsWon = FALSE;
        
        int nrOfThePlayer = -1;
        while (!gameIsWon && nrOfThePlayer<nrOfPlayers) 
        {
            nrOfThePlayer++;
            gameIsWon = isGameWon(nrOfThePlayer);
        }

        if (gameIsWon) 
        {
            switchWindmillsOfAllPlayers(FALSE);
            gameStatus = "WAITFORGAMEWON";
            activePlayer = nrOfThePlayer; 

            addGameWonText();
            for (i=0;i<nrOfPlayers;i++)
                for (int j=0;j<nrOfCatapultsPerPlayer;j++) 
                {
                    catapultStatus[i][j] = "SHOULDSTOP";
                    nrOfArmMoves[i][j] = 1;
                }
        }
        else 
        {
            for (i=0;i<nrOfPlayers;i++)
            {
                positionOfTheBalloons(i);  
                
                positionOfTheFallingBalloons(i);

                positionOfTheCatapults(i,count, vpReg);
            }
        }
    }
}

//----------------------------------------------------------------------------
// initializes the variables
//----------------------------------------------------------------------------
void 
SoTableTopKit::initVariables() 
{
    int i,j;
    float balloonDistance = fieldLength/(maxNrOfBalloons-1.0);

    players = new SoSeparator*[nrOfPlayers];
    
    windmillIntensity = new float[nrOfPlayers];
    windmillAngle = new float[nrOfPlayers];
    parabelHeight = new float[nrOfPlayers];
    parabelShape = new float[nrOfPlayers];
    parabelWidth = new float[nrOfPlayers];
    bulletSpeed = new float[nrOfPlayers];
    catapultArmAngle = new float[nrOfPlayers];
    catapultSpeed = new float[nrOfPlayers];
    pointsOfPlayer = new int[nrOfPlayers];
    playerHasShot = new int[nrOfPlayers];

    intervalLength = new float[nrOfPlayers];
    nrOfBalloons = new int[nrOfPlayers];
    nrOfFallingBalloons = new int[nrOfPlayers];

    nrOfBullets = new int*[nrOfPlayers];
    armUp = new bool*[nrOfPlayers];
    formerAngle = new float*[nrOfPlayers];
    angle = new float*[nrOfPlayers];

    catapultStatus = new string*[nrOfPlayers];
    catapultTime = new float*[nrOfPlayers];
    nrOfArmMoves = new int*[nrOfPlayers];

    /* initialization */
    for (i=0;i<nrOfPlayers;i++)
    {    
        windmillIntensity[i] = (float)startingWindmillIntensity/10.0;
        windmillAngle[i] = startingWindmillAngle;
        parabelHeight[i] = startingParabelHeight;
        parabelShape[i] = startingParabelShape;
        parabelWidth[i] = startingParabelWidth;
        bulletSpeed[i] = startingBulletSpeed;
        catapultArmAngle[i] = startingCatapultArmAngle;
        catapultSpeed[i] = startingCatapultSpeed;
        
        intervalLength[i] = sqrt(parabelHeight[i]/parabelWidth[i])/parabelShape[i];
        nrOfBalloons[i] = maxNrOfBalloons;
        nrOfFallingBalloons[i] = 0;
        pointsOfPlayer[i] = 0;
        playerHasShot[i] = 0;

        nrOfBullets[i] = new int[nrOfCatapultsPerPlayer];
        armUp[i] = new bool[nrOfCatapultsPerPlayer];
        formerAngle[i] = new float[nrOfCatapultsPerPlayer];
        angle[i] = new float[nrOfCatapultsPerPlayer];
        
        catapultStatus[i] = new string[nrOfCatapultsPerPlayer];
        catapultTime[i] = new float[nrOfCatapultsPerPlayer];
        nrOfArmMoves[i] = new int[nrOfCatapultsPerPlayer];

        for (j=0;j<nrOfCatapultsPerPlayer;j++)
        {
            nrOfBullets[i][j] = 0;
            armUp[i][j] = TRUE;
            formerAngle[i][j] = 0.0;        
            angle[i][j] = 0.0;        
            catapultStatus[i][j] = "MOVES";
            catapultTime[i][j] = 0.0;
            nrOfArmMoves[i][j] = 1;
        }
    }

    //calculate the colors and the positions of the balloons
    balloonColors = new SbVec3f*[nrOfPlayers];
    balloonPositions = new SbVec3f*[nrOfPlayers];
    float offset = 1.0/(float)maxNrOfBalloons;
    float x,y,z, position;
    for (i = 0; i < nrOfPlayers; i++)
    {
        balloonColors[i] = new SbVec3f[maxNrOfBalloons];
        balloonPositions[i] = new SbVec3f[maxNrOfBalloons];
        for (j = 0; j < maxNrOfBalloons; j++)
        {
            SbVec3f* vecColors;
            if (i==0)  //first player
                vecColors = new SbVec3f(1.0,1.0-(float)j*offset,0.0);
            else
                vecColors = new SbVec3f(0.0,(float)j*offset,1.0);
            balloonColors[i][j] = *vecColors;

            SbVec3f* vecPositions;
            position = fieldLength/2.0;
            x = i*position-position+i*position;
            (maxNrOfBalloons > 1) ? z = j*balloonDistance-fieldLength/2.0 : z = 0.0;
            if (showTerrain == "Yes")
                y = getYValueOfTerrain(x,z) + 0.05;
            else y = 0.1;
            vecPositions = new SbVec3f(x,y,z);

            balloonPositions[i][j] = *vecPositions;

        }
    }
    
    //calculate the intial positions of the catapults and windmills
    initCatapultPositions = new SbVec3f*[nrOfPlayers];
    initWindmillPositions = new SbVec3f*[nrOfPlayers];
    float catapultDistance = fieldLength/(nrOfCatapultsPerPlayer-1.0);
    float windmillDistance = fieldLength/(nrOfWindmillsPerPlayer-1.0);
    for (i = 0; i < nrOfPlayers; i++)
    {
        initCatapultPositions[i] = new SbVec3f[nrOfCatapultsPerPlayer];
        for (j = 0; j < nrOfCatapultsPerPlayer; j++)
        {
            SbVec3f* vecPositions;
            position = fieldLength/3.0;
            x = i*position-position+i*position;
            (nrOfCatapultsPerPlayer > 1) ? z = j*catapultDistance-fieldLength/2.0 : z = 0.0;
            (showTerrain == "Yes") ? y = 0.01 + getYValueOfTerrain(x,z) : y = 0.0;
            vecPositions = new SbVec3f(x,y,z);
            initCatapultPositions[i][j] = *vecPositions;

        }
        initWindmillPositions[i] = new SbVec3f[nrOfWindmillsPerPlayer];
        for (j = 0; j < nrOfWindmillsPerPlayer; j++)
        {
            SbVec3f* vecPositions;
            position = 2.0*fieldLength/3.0;
            x = i*position-position+i*position;
            (nrOfWindmillsPerPlayer > 1) ? z = j*windmillDistance-fieldLength/2.0 : z = 0.0;
            (showTerrain == "Yes") ? y = 0.01 + getYValueOfTerrain(x,z) : y = 0.0;
            vecPositions = new SbVec3f(x,y,z);
            initWindmillPositions[i][j] = *vecPositions;

        }
    }
}

//----------------------------------------------------------------------------
// reads the configFile
//----------------------------------------------------------------------------
void 
SoTableTopKit::readConfigFile() 
{

    ReadConfigFile configFile = ReadConfigFile(configFileName);

    gameType = configFile.getValueOrDefault("gameType","ActionGame");
    movementType = configFile.getValueOrDefault("movementType","Default");

    nrOfPlayers = Convert::stringToInt(configFile.getValueOrDefault("nrOfPlayers", Convert::intToString(defNrOfPlayers)));
    nrOfCatapultsPerPlayer = Convert::stringToInt(configFile.getValueOrDefault("nrOfCatapultsPerPlayer",Convert::intToString(defNrOfCatapultsPerPlayer)));
    nrOfWindmillsPerPlayer = Convert::stringToInt(configFile.getValueOrDefault("nrOfWindmillsPerPlayer",Convert::intToString(defNrOfWindmillsPerPlayer)));
    balloonSpeed = Convert::stringToFloat(configFile.getValueOrDefault("balloonSpeed",Convert::floatToString(defBalloonSpeed)));
    followSpeed = Convert::stringToFloat(configFile.getValueOrDefault("followSpeed",Convert::floatToString(defFollowSpeed)));
    fieldLength = Convert::stringToFloat(configFile.getValueOrDefault("fieldLength",Convert::floatToString(defFieldLength)));
    nrOfChests = Convert::stringToInt(configFile.getValueOrDefault("nrOfChests", Convert::intToString(defNrOfChests)));
    maxNrOfBalloons = Convert::stringToInt(configFile.getValueOrDefault("maxNrOfBalloons",Convert::intToString(defMaxNrOfBalloons)));
    shootToWin = Convert::stringToInt(configFile.getValueOrDefault("shootToWin",Convert::intToString(defShootToWin)));
    showTerrain = configFile.getValueOrDefault("showTerrain","No");
    showAnimation = configFile.getValueOrDefault("showAnimation","No");
    showFlowVis = configFile.getValueOrDefault("showFlowVis","No");
    newBalloons = configFile.getValueOrDefault("newBalloons","No");

    terrainLevelOfDetail = Convert::stringToInt(configFile.getValueOrDefault("terrainLevelOfDetail",Convert::intToString(defTerrainLevelOfDetail)));
    terrainRange = Convert::stringToFloat(configFile.getValueOrDefault("terrainRange",Convert::floatToString(defTerrainRange)));
    
    startingWindmillIntensity = Convert::stringToInt(configFile.getValueOrDefault("startingWindmillIntensity",Convert::intToString(defWindmillIntensity)));
    startingWindmillAngle = Convert::stringToFloat(configFile.getValueOrDefault("startingWindmillAngle",Convert::floatToString(defWindmillAngle)));
    startingParabelHeight = Convert::stringToFloat(configFile.getValueOrDefault("startingParabelHeight",Convert::floatToString(defParabelHeight)));
    startingParabelShape = Convert::stringToFloat(configFile.getValueOrDefault("startingParabelShape",Convert::floatToString(defParabelShape)));
    startingParabelWidth = Convert::stringToFloat(configFile.getValueOrDefault("startingParabelWidth",Convert::floatToString(defParabelWidth)));
    startingBulletSpeed = Convert::stringToFloat(configFile.getValueOrDefault("startingBulletSpeed",Convert::floatToString(defBulletSpeed)));
    startingCatapultArmAngle = Convert::stringToFloat(configFile.getValueOrDefault("startingCatapultArmAngle",Convert::floatToString(defCatapultArmAngle)));
    startingCatapultSpeed = Convert::stringToFloat(configFile.getValueOrDefault("startingCatapultSpeed",Convert::floatToString(defCatapultSpeed)));

    gameStatus = "INIT";
}


//----------------------------------------------------------------------------
// creates the scene graph
//----------------------------------------------------------------------------

SoWindowKit* 
SoTableTopKit::createWindowGeometry(int index)
{
    int i,j,k;   
    
    //initiate random number generator
    time_t currentTime;
    time(&currentTime);
    srand(currentTime);
    
    //generate new log file
    logi=fopen(logFileName,"w");
    fprintf(logi,"LogFile\n");
    fprintf(logi,"%d\n",currentTime);
    fclose(logi);
    logi=fopen(logFileName,"a");

    //read config file
    readConfigFile();

    //create terrain to generate the height field, needed for initVariables()
    SoSeparator* terrainSeparator = createTerrain();
    
    initVariables();

    playSound("..\\..\\src\\apps\\tabletop\\sound\\getready.wav");

    //create scene graph
    WindowCreate wc;
    SoWindowKit::defaultWindow(wc);
    SoWindowKit* windowKit = comm->createWindow(NULL, &wc, NULL, NULL);
//    windowKit->size = SbVec3f(10,10,10);
    windowKit->size = SbVec3f(100,100,100);

    SoSeparator* clientVolume = windowKit->getClientVolumeSeparator();

    SoSeparator* root = new SoSeparator;

    root2 = new SoSeparator;
    root->addChild(root2);  

    //create stations
    SoGroup* stationSep = new SoGroup;
 
    SoTransform* transform2 = new SoTransform;
    transform2->rotation.setValue(SbVec3f(0.0,1.0,0.0),1.5707);
    stationSep->addChild(transform2);
    stations = new SoGroup;
    stationSep->addChild(stations);
    root->addChild(stationSep);

    //create an additional light source
    SoDirectionalLight* light = new SoDirectionalLight;
    light->intensity = 3.0;
    root2->addChild(light);

    //add terrain
    root2->addChild(terrainSeparator);

    //create crossesSeparator
    SoSeparator* crossFile;
    if (movementType == "FollowUp") {
        crosses = new SoSeparator;
        crossFile = readFile("grafik\\cross.iv",comm->workingDirectory);
        if( crossFile == NULL )
        {
            printf("tabletop : couldn't load cross !!\n");
            exit(0);
        }
        root2->addChild(crosses);
    }
    
    //read the geometry files
    SoSeparator* balloonFile = readFile("grafik\\balloon.iv",comm->workingDirectory);
    if( balloonFile == NULL )
    {
        printf("tabletop : couldn't load balloonGeometry !!\n");
        exit(0);
    }
    balloonSize = 1.0;
    SoTransform* balloonTrans = new SoTransform;
    balloonTrans->scaleFactor.setValue(balloonSize,balloonSize,balloonSize);
    balloonFile->insertChild(balloonTrans,0);
    
    SoSeparator* catapultFile = readFile("grafik\\catapult.iv",comm->workingDirectory);
    if( catapultFile == NULL )
    {
        printf("tabletop : couldn't load catapult !!\n");
        exit(0);
    }
            
    SoSeparator* initPosFile = readFile("grafik\\initPos.iv",comm->workingDirectory);
    if( initPosFile == NULL )
    {
        printf("tabletop : couldn't load initPos !!\n");
        exit(0);
    }

    SoSeparator* kFile = readFile("grafik\\k.iv",comm->workingDirectory);
    if( kFile == NULL )
    {
        printf("tabletop : couldn't load k !!\n");
        exit(0);
    }

    SoSeparator* wFile = readFile("grafik\\w.iv",comm->workingDirectory);
    if( wFile == NULL )
    {
        printf("tabletop : couldn't load w !!\n");
        exit(0);
    }

    SoSeparator* oneFile = readFile("grafik\\one.iv",comm->workingDirectory);
    if( oneFile == NULL )
    {
        printf("tabletop : couldn't load one !!\n");
        exit(0);
    }
                
    SoSeparator* twoFile = readFile("grafik\\two.iv",comm->workingDirectory);
    if( twoFile == NULL )
    {
        printf("tabletop : couldn't load two !!\n");
        exit(0);
    }

    SoSeparator* arrow = readFile("grafik\\arrow.iv",comm->workingDirectory);
    if( arrow == NULL )
    {
        printf("tabletop : couldn't load arrow !!\n");
        exit(0);
    }

    SoSeparator* chestFile = readFile("grafik\\chest.iv",comm->workingDirectory);
    if( chestFile == NULL )
    {
        printf("tabletop : couldn't load chestTop !!\n");
        exit(0);
    }

    SoSeparator* coneFile = readFile("grafik\\cone.iv",comm->workingDirectory);
    if( coneFile == NULL )
    {
	    printf("tabletop : couldn't load cone !!\n");
        exit(0);
    }

    SoSeparator* nextFile = readFile("grafik\\next.iv",comm->workingDirectory);
    if( nextFile == NULL )
    {
	    printf("tabletop : couldn't load next !!\n");
        exit(0);
    }

    SoSeparator* circleFile = readFile("grafik\\circle.iv",comm->workingDirectory);
    if( circleFile == NULL )
    {
        printf("tabletop : couldn't load circle !!\n");
        exit(0);
    }

    SoScale* circleScale = new SoScale;
    //make the circle appear higher, so that it can be seen in the terrain
    if(showTerrain == "Yes") 
        circleScale->scaleFactor.setValue(1.5,4.0,1.5);
    else circleScale->scaleFactor.setValue(1.5,0.1,1.5);
    circleFile->insertChild(circleScale,0);

    //add menu, so that it is drawn before the transparent balloons are drawn
    SoStationKit* menuStation = NULL;
    if (gameType == "StrategyGame")
    {
        //create menu
        menu = createMenu(coneFile, nextFile);
        root2->addChild(menu);

        SoSeparator* menuMarker = (SoSeparator*) menu->getChild(0);
        SoTransform* menuMarkerTrans = (SoTransform*) menuMarker->getChild(1);

        //add menuMarker to stations
        menuStation = new SoStationKit;
        int stationNr = nrOfPlayers*(nrOfCatapultsPerPlayer+nrOfWindmillsPerPlayer);
        menuStation->stations.setValue(stationNr);
        menuStation->addEventCallback(myDefaultEventCB,menuMarkerTrans);
        menuStation->transformUpdate = FALSE;
    }

    //add chests, so that they are drawn before the transparent balloons are drawn
    chests = createChests(chestFile); 
    root2->addChild(chests);

    //all initPos-fields have the same style, one can turn on and off all fields with one command
    SoDrawStyle* initPosStyle = new SoDrawStyle;
    initPosStyle->style.setValue(SoDrawStyle::FILLED);

    for (i=0;i<nrOfPlayers;i++)
    {
        SoSeparator* player = new SoSeparator;

        //all lastPos-fields have the same style for one player
        //one can turn on and off all fields of one player with one command
        SoDrawStyle* lastPosStyle = new SoDrawStyle;
        lastPosStyle->style.setValue(SoDrawStyle::INVISIBLE);

        for (j=0;j<nrOfCatapultsPerPlayer;j++)
        {
            SoSeparator* separator = new SoSeparator;
            SoSeparator* geometry = new SoSeparator;
            SoSeparator* initPos = new SoSeparator;
            SoSeparator* lastPos = new SoSeparator;

            //geometry
            SoTransform* transform = new SoTransform;
            transform->translation.setValue(0.0, 0.0, 0.0);
            transform->rotation.setValue(0.0, 0.0, 0.0, 0.0);
            geometry->addChild(transform);

            SoSeparator* catapultContentFile = readFile("grafik\\catapultContent.iv",comm->workingDirectory);
            if( catapultContentFile == NULL )
            {
                printf("tabletop : couldn't load catapultContent !!\n");
                exit(0);
            }

            //color coding
            SoMaterial* catapultMat = new SoMaterial;
            catapultMat->diffuseColor.setValue(balloonColors[i][0]);   
            
            SoSeparator* catContentSep = (SoSeparator*) catapultContentFile->getChild(0);
            catContentSep->insertChild(catapultMat,3);

            catapultContentFile->insertChild(catapultFile,0);

            geometry->addChild(catapultContentFile);

            //add a number to the catapult geometry
            SoSeparator* number = new SoSeparator;
            
            SoTransform* numberTransform = new SoTransform;
            numberTransform->translation.setValue(0.0, -0.005, -0.05);
            numberTransform->rotation.setValue(SbVec3f(0.0,1.0,0.0),M_PI);

            SoMaterial* numberMat = new SoMaterial;
            numberMat->diffuseColor.setValue(balloonColors[i][0]);            
            
            number->addChild(numberTransform);
            number->addChild(numberMat);
            if (j == 0) number->addChild(oneFile);
            else number->addChild(twoFile);

            geometry->addChild(number);    
            separator->addChild(geometry);

            
            //add station
            SoStationKit* station = new SoStationKit;
            int stationNr = i*(nrOfCatapultsPerPlayer+nrOfWindmillsPerPlayer)+j;
            station->stations.setValue(stationNr);
            if (movementType != "FollowUp") station->addEventCallback(myDefaultEventCB,transform);
            station->transformUpdate = FALSE;
            stations->addChild(station);

            //initPos
            SoTranslation* initPosCatapultTrans = new SoTranslation;
            initPosCatapultTrans->translation.setValue(initCatapultPositions[i][j]);

            SoSwitch* initPosCatapultSwitch = new SoSwitch;
            initPosCatapultSwitch->whichChild = 0;

            SoMaterial* initPosCatapultMat1 = new SoMaterial;
            initPosCatapultMat1->diffuseColor.setValue(1.0, 0.0, 0.0);

            SoMaterial* initPosCatapultMat2 = new SoMaterial;
            initPosCatapultMat2->diffuseColor.setValue(0.0, 1.0, 0.0);

            //add a label to the initPos field, k for katapult and the number of the catapult
            SoSeparator* initPosNumber = new SoSeparator;
            SoRotation* initPosNumberTransform = new SoRotation;
            if (i == 0) 
                initPosNumberTransform->rotation.setValue(SbVec3f(0,1,0),3*M_PI/2.0);
            else 
                initPosNumberTransform->rotation.setValue(SbVec3f(0,1,0),M_PI/2.0);

            SoMaterial* initPosNumberMat = new SoMaterial;
            initPosNumberMat->diffuseColor.setValue(balloonColors[i][0]);            
        
            initPosNumber->addChild(initPosNumberTransform);
            initPosNumber->addChild(initPosNumberMat);
            initPosNumber->addChild(kFile);
            if (j == 0) initPosNumber->addChild(oneFile);
            else initPosNumber->addChild(twoFile);
        
            initPosCatapultSwitch->addChild(initPosCatapultMat1);
            initPosCatapultSwitch->addChild(initPosCatapultMat2);

            initPos->addChild(initPosStyle);
            initPos->addChild(initPosCatapultTrans);
            initPos->addChild(initPosCatapultSwitch);
            initPos->addChild(initPosFile);
            initPos->addChild(initPosNumber);

            separator->addChild(initPos);

            //if it is a strategy game, init-fields and lastPos-fields are created
            if (gameType == "StrategyGame")
            {
                //lastPos
                SoTranslation* lastPosCatapultTrans = new SoTranslation;
                lastPosCatapultTrans->translation.setValue(initCatapultPositions[i][j]);

                SoSwitch* lastPosCatapultSwitch = new SoSwitch;
                lastPosCatapultSwitch->whichChild = 1;

                SoMaterial* lastPosCatapultMat1 = new SoMaterial;
                lastPosCatapultMat1->diffuseColor.setValue(1.0, 0.0, 0.0);
                lastPosCatapultMat1->transparency = 0.8;

                SoMaterial* lastPosCatapultMat2 = new SoMaterial;
                lastPosCatapultMat2->diffuseColor.setValue(0.0, 1.0, 0.0);
                lastPosCatapultMat2->transparency = 0.8;

                SoSwitch* lastPosGeoSwitch = new SoSwitch;
                lastPosGeoSwitch->whichChild = 0;
                lastPosGeoSwitch->addChild(circleFile);
                lastPosGeoSwitch->addChild(initPosFile);

                lastPosCatapultSwitch->addChild(lastPosCatapultMat1);
                lastPosCatapultSwitch->addChild(lastPosCatapultMat2);

                lastPos->addChild(lastPosStyle);
                lastPos->addChild(lastPosCatapultTrans);
                lastPos->addChild(lastPosCatapultSwitch);
                lastPos->addChild(lastPosGeoSwitch);
                separator->addChild(lastPos);
            }

            //add catapult to player
            player->addChild(separator);

            //add cross if movementType is followUp
            if (movementType == "FollowUp") addCross(crossFile);
        }
        
        for (j=0;j<nrOfWindmillsPerPlayer;j++)
        {
            SoSeparator* separator = new SoSeparator;
            SoSeparator* geometry = new SoSeparator;
            SoSeparator* initPos = new SoSeparator;
            SoSeparator* lastPos = new SoSeparator;

            //geometry
            SoTransform* transform = new SoTransform;
            transform->translation.setValue(0.0, 0.0, 0.0);
            transform->rotation.setValue(0.0, 0.0, 0.0, 0.0);
            geometry->addChild(transform);

            SoSeparator* windmillContentFile = readFile("grafik\\windmillContent.iv",comm->workingDirectory);
            if( windmillContentFile == NULL )
            {
                printf("tabletop : couldn't load windmillContent !!\n");
                exit(0);
            }
            //color coding
            SoMaterial* windmillMat = new SoMaterial;
            windmillMat->diffuseColor.setValue(balloonColors[i][0]); 
            windmillContentFile->insertChild(windmillMat,1);
            geometry->addChild(windmillContentFile);

            //add a number to the windmill geometry
            SoSeparator* number = new SoSeparator;
            
            SoTransform* numberTransform = new SoTransform;
            numberTransform->translation.setValue(0.0, -0.005, -0.05);
            numberTransform->rotation.setValue(SbVec3f(0.0,1.0,0.0),M_PI);

            SoMaterial* numberMat = new SoMaterial;
            numberMat->diffuseColor.setValue(balloonColors[i][0]);            
            
            number->addChild(numberTransform);
            number->addChild(numberMat);
            if (j == 0) number->addChild(oneFile);
            else number->addChild(twoFile);
            geometry->addChild(number);

            //add station
            SoStationKit* station = new SoStationKit;
            int stationNr = i*(nrOfCatapultsPerPlayer+nrOfWindmillsPerPlayer)+nrOfCatapultsPerPlayer+j;
            station->stations.setValue(stationNr);
            if (movementType != "FollowUp") station->addEventCallback(myDefaultEventCB,transform);
            stations->addChild(station);

            //add flow visualisation
            if (showFlowVis == "Yes") 
            {
                float xArrow,zArrow;
                SoSeparator* flowVis = new SoSeparator;

                SoMaterial* flowVisMat = new SoMaterial;
                flowVisMat->diffuseColor.setValue(balloonColors[i][0]);            

                for (k=1;k<(int)(windmillIntensity[i]*10)+1;k++) 
                {
                    //add first arrow
                    SoSeparator* flowVisArrowSep1 = new SoSeparator;

                    SoTransform* flowVisArrowTrans1 = new SoTransform;

                    SbVec3f* vec = new SbVec3f(0.0,1.0,0.0);
                    xArrow = (float)k*sin(windmillAngle[i])*0.1;
                    zArrow = (float)k*cos(windmillAngle[i])*0.1;
                    flowVisArrowTrans1->translation.setValue(xArrow,0.12,zArrow);
                    flowVisArrowTrans1->rotation.setValue(*vec,windmillAngle[i]);

                    SoTransform* flowVisWindmillTrans1 = new SoTransform;
                    flowVisWindmillTrans1->translation.setValue(0.0,0.0,0.0);
                    flowVisWindmillTrans1->rotation.setValue(*vec,0.0);

                    flowVisArrowSep1->addChild(flowVisArrowTrans1);
                    flowVisArrowSep1->addChild(flowVisWindmillTrans1);
                    flowVisArrowSep1->addChild(flowVisMat);
                    flowVisArrowSep1->addChild(arrow);

                    flowVis->addChild(flowVisArrowSep1);

                    //add second arrow
                    SoSeparator* flowVisArrowSep2 = new SoSeparator;

                    SoTransform* flowVisArrowTrans2 = new SoTransform;

                    flowVisArrowTrans2->translation.setValue(0.0,0.12,(float)k*0.1);

                    SoTransform* flowVisWindmillTrans2 = new SoTransform;
                    flowVisWindmillTrans2->translation.setValue(0.0,0.0,0.0);
                    flowVisWindmillTrans2->rotation.setValue(*vec,0.0);

                    flowVisArrowSep2->addChild(flowVisArrowTrans2);
                    flowVisArrowSep2->addChild(flowVisWindmillTrans2);
                    flowVisArrowSep2->addChild(flowVisMat);
                    flowVisArrowSep2->addChild(arrow);

                    flowVis->addChild(flowVisArrowSep2);

                    //add third arrow
                    SoSeparator* flowVisArrowSep3 = new SoSeparator;

                    SoTransform* flowVisArrowTrans3 = new SoTransform;

                    flowVisArrowTrans3->translation.setValue(-xArrow,0.12,zArrow);
                    flowVisArrowTrans3->rotation.setValue(*vec,-windmillAngle[i]);

                    SoTransform* flowVisWindmillTrans3 = new SoTransform;
                    flowVisWindmillTrans3->translation.setValue(0.0,0.0,0.0);
                    flowVisWindmillTrans3->rotation.setValue(*vec,0.0);

                    flowVisArrowSep3->addChild(flowVisArrowTrans3);
                    flowVisArrowSep3->addChild(flowVisWindmillTrans3);
                    flowVisArrowSep3->addChild(flowVisMat);
                    flowVisArrowSep3->addChild(arrow);

                    flowVis->addChild(flowVisArrowSep3);
                }
                geometry->addChild(flowVis);
            }
            separator->addChild(geometry);

            //initPos
            SoTranslation* initPosWindmillTrans = new SoTranslation;
            initPosWindmillTrans->translation.setValue(initWindmillPositions[i][j]);

            SoSwitch* initPosWindmillSwitch = new SoSwitch;
            initPosWindmillSwitch->whichChild = 0;

            SoMaterial* initPosWindmillMat1 = new SoMaterial;
            initPosWindmillMat1->diffuseColor.setValue(1.0, 0.0, 0.0);

            SoMaterial* initPosWindmillMat2 = new SoMaterial;
            initPosWindmillMat2->diffuseColor.setValue(0.0, 1.0, 0.0);

            //add a label to the initPos field, w for windmill and the number of the windmill
            SoSeparator* initPosNumber = new SoSeparator;
    
            SoRotation* initPosNumberTransform = new SoRotation;
            if (i == 0) 
                initPosNumberTransform->rotation.setValue(SbVec3f(0,1,0),3*M_PI/2.0);
            else 
                initPosNumberTransform->rotation.setValue(SbVec3f(0,1,0),M_PI/2.0);

            SoMaterial* initPosNumberMat = new SoMaterial;
            initPosNumberMat->diffuseColor.setValue(balloonColors[i][0]);            
        
            initPosNumber->addChild(initPosNumberTransform);
            initPosNumber->addChild(initPosNumberMat);
            initPosNumber->addChild(wFile);
            if (j == 0) initPosNumber->addChild(oneFile);
            else initPosNumber->addChild(twoFile);

            initPosWindmillSwitch->addChild(initPosWindmillMat1);
            initPosWindmillSwitch->addChild(initPosWindmillMat2);

            initPos->addChild(initPosStyle);
            initPos->addChild(initPosWindmillTrans);
            initPos->addChild(initPosWindmillSwitch);
            initPos->addChild(initPosFile);
            initPos->addChild(initPosNumber);
            separator->addChild(initPos);

            //if it is a strategy game, init-fields and lastPos-fields are created
            if (gameType == "StrategyGame")
            {
                //lastPos
                SoTranslation* lastPosWindmillTrans = new SoTranslation;
                lastPosWindmillTrans->translation.setValue(initWindmillPositions[i][j]);

                SoSwitch* lastPosWindmillSwitch = new SoSwitch;
                lastPosWindmillSwitch->whichChild = 1;

                SoMaterial* lastPosWindmillMat1 = new SoMaterial;
                lastPosWindmillMat1->diffuseColor.setValue(1.0, 0.0, 0.0);
                lastPosWindmillMat1->transparency = 0.8;

                SoMaterial* lastPosWindmillMat2 = new SoMaterial;
                lastPosWindmillMat2->diffuseColor.setValue(0.0, 1.0, 0.0);
                lastPosWindmillMat2->transparency = 0.8;

                lastPosWindmillSwitch->addChild(lastPosWindmillMat1);
                lastPosWindmillSwitch->addChild(lastPosWindmillMat2);

                SoSwitch* lastPosGeoSwitch = new SoSwitch;
                lastPosGeoSwitch->whichChild = 0;
                lastPosGeoSwitch->addChild(circleFile);
                lastPosGeoSwitch->addChild(initPosFile);

                lastPos->addChild(lastPosStyle);
                lastPos->addChild(lastPosWindmillTrans);
                lastPos->addChild(lastPosWindmillSwitch);
                lastPos->addChild(lastPosGeoSwitch);
                separator->addChild(lastPos);
            }

            //add windmill to player
            player->addChild(separator);

            //add cross if movementType is followUp
            if (movementType == "FollowUp") addCross(crossFile);
        }

        SoSeparator* balloons = new SoSeparator;
        player->addChild(balloons); 

        //add balloons
        for (j = 0;j<maxNrOfBalloons;j++)
        {
            SoSeparator* balloonSep = new SoSeparator;
            balloonSep->setName(Convert::stringToChar(Convert::intToString(j)));
        
            SoMaterial* mat = new SoMaterial;
            mat->transparency = 0.5;
            mat->diffuseColor.setValue(balloonColors[i][j]);

            SoTransform* trans = new SoTransform;
            trans->translation.setValue(balloonPositions[i][j]);
            
            SoShuttle* shuttle = new SoShuttle;
            float x,y,z,rand1, rand2;
            balloonPositions[i][j].getValue(x,y,z);

            //to avoid that a balloon does not move
            rand1 = (float)(rand()%5)/100.0;
            rand2 = (float)(rand()%5)/100.0;
            if (rand1 + rand2 <= 0.01) rand1 = 0.02;

            shuttle->translation0.setValue(0.0,y-rand1,0.0);
            shuttle->translation1.setValue(0.0,y+rand2,0.0);
            shuttle->speed = 0.5;

            balloonSep->addChild(mat);
            balloonSep->addChild(trans);
            if (showAnimation == "Yes") balloonSep->addChild(shuttle);
            balloonSep->addChild(balloonFile);
            balloons->addChild(balloonSep);
        }

        //add the separator for the falling balloons
        SoSeparator* fallingBalloons = new SoSeparator;
        player->addChild(fallingBalloons); 

        //add the points
        SoSeparator* points = new SoSeparator;
        
        SoMaterial* pointsMat = new SoMaterial;
        pointsMat->diffuseColor.setValue(balloonColors[i][0]);

        SoTransform* pointsTransform = new SoTransform;
        float expandFL = fieldLength*1.1;
        pointsTransform->translation.setValue(i*expandFL-expandFL+i*expandFL,0.2,0.0);
        pointsTransform->scaleFactor.setValue(0.0075,0.0075,0.0075);
        
        SoText3* pointsText = new SoText3;
        pointsText->parts = SoText3::ALL;
        pointsText->string = "0";
        points->addChild(pointsMat);
        points->addChild(pointsTransform);
        points->addChild(pointsText);
        player->addChild(points);

        //add player to root
        root2->addChild(player);
        
        players[i] = player;
        
        switchWindmills(i,FALSE);
    }
    
    //add the station for the menu marker
    if (gameType == "StrategyGame")
    {
        stations->addChild(menuStation);
    }

    clientVolume->addChild( root );
    
    //activate the timer callback
    SoTimerSensor *mySensor = new SoTimerSensor;

    mySensor->setFunction(myTimerCB);
    mySensor->setData(root);
    mySensor->schedule();
    myBaseTime.setValue(mySensor->getBaseTime().getValue());

    //stop traversial at root2
    SoCallbackAction cbAction;
    cbAction.addPreCallback(SoSeparator::getClassTypeId(),stopTraversal, NULL);
    cbAction.apply(root);

    // finally return the new windowkit
    return windowKit;
}

//----------------------------------------------------------------------------
// stops the traversal of the station callbacks after the station node
// has been reached
//----------------------------------------------------------------------------
SoCallbackAction::Response
SoTableTopKit::stopTraversal(void *, SoCallbackAction *, const SoNode *node)
{
    return SoCallbackAction::ABORT;
}

//----------------------------------------------------------------------------
//getYValueOfTerrain returns the interpolated value of y at the position (x,z)
//----------------------------------------------------------------------------

float 
SoTableTopKit::getYValueOfTerrain(float x, float z) 
{
    float yValue;
 
    float xYValue = ((x+fieldLength)*(terrainLevelOfDetail-1))/(2.0*fieldLength);
    float zYValue = ((z+fieldLength)*(terrainLevelOfDetail-1))/(2.0*fieldLength);

    int i = (int)xYValue, j= (int)zYValue;
    if (i>=0 && i<terrainLevelOfDetail-1 && j>=0 && j<terrainLevelOfDetail-1) 
    {
        float xDiff = xYValue - i;
        float zDiff = zYValue - j;

        float diff = yValues[i][j]-yValues[i+1][j];
        float yFront = yValues[i][j] - xDiff*diff;

        diff = yValues[i][j+1]-yValues[i+1][j+1];

        float yBack = yValues[i][j+1] - xDiff*diff;        

        diff = yFront - yBack;
        yValue = 0.001 + yFront - zDiff*diff;    
    }
    else yValue = 0.0;
    return yValue;
}

//----------------------------------------------------------------------------
// returns the difference between num and sub, or, if this difference is
// negative, the value of num - sub + (terrainLevelOfDetail-1)
//----------------------------------------------------------------------------
int 
SoTableTopKit::sub(int num, int sub) 
{
    int result = num - sub;
    if (result < 0 ) result = result + terrainLevelOfDetail-1;
    return result;
}

//----------------------------------------------------------------------------
// returns the sum of num and add, or, if this sum is
// greater than (terrainLevelOfDetail-1), the value of 
// num + add - (terrainLevelOfDetail-1)
//----------------------------------------------------------------------------
int 
SoTableTopKit::add(int num, int add) 
{
    int result = num + add;
    if (result > terrainLevelOfDetail-1) result = result - (terrainLevelOfDetail-1);
    return result;
}

//----------------------------------------------------------------------------
// returns a random number between 0 and range
//----------------------------------------------------------------------------
float 
SoTableTopKit::getRandom(float range) 
{
    return (((float)rand())/32767.0)*range - range/2.0;
}

//----------------------------------------------------------------------------
// calculates the values of y of the terrain
//----------------------------------------------------------------------------
void 
SoTableTopKit::calculateYValues() 
{
    int s = terrainLevelOfDetail/2;
    int i,j,jStart; 
    float yValue;
    
    if (showTerrain == "No") 
    {
        terrainLevelOfDetail= 1 + 1;
    }
    
    yValues = new float*[terrainLevelOfDetail];

    for (i=0;i<terrainLevelOfDetail;i++) yValues[i] = new float[terrainLevelOfDetail];

    yValues[0][0] = 0.0; 
    yValues[0][terrainLevelOfDetail-1] = 0.0; 
    yValues[terrainLevelOfDetail-1][0] = 0.0; 
    yValues[terrainLevelOfDetail-1][terrainLevelOfDetail-1] = 0.0; 

    //do not calculate a terrain
    if (showTerrain == "No") return;

    while (s > 0)
    {
        //Square
        for (i=s; i<terrainLevelOfDetail; i=i+2*s)
            for (j=s; j<terrainLevelOfDetail; j=j+2*s) 
            {
                yValue = getRandom(terrainRange) + ( yValues[i-s][j-s] 
                        + yValues[i+s][j-s] + yValues[i-s][j+s] 
                        + yValues[i+s][j+s] ) / 4.0;
                if (yValue < 0.0) yValues[i][j] = 0.0;
                else yValues[i][j] = yValue;
            }

        //Diamond
        for (i=0; i<terrainLevelOfDetail; i=i+s)
        {
            if ((i/s)%2 == 0) jStart = s;
            else jStart = 0;
            
            for (j=jStart; j<terrainLevelOfDetail; j = j+2*s) 
            {
                yValue = getRandom(terrainRange) + ( yValues[sub(i,s)][j] 
                        + yValues[i][sub(j,s)] + yValues[add(i,s)][j] 
                        + yValues[i][add(j,s)] ) / 4.0;
                if (yValue < 0.0) yValues[i][j] = 0.0;
                else yValues[i][j] = yValue;
            }
        }

        s /= 2;
        terrainRange /= 2.0;
    }
}

//----------------------------------------------------------------------------
// creates the terrain node
//----------------------------------------------------------------------------
SoSeparator* 
SoTableTopKit::createTerrain() 
{
    int i,j;

    SoSeparator* separator = new SoSeparator;

    SoMaterial* material = new SoMaterial; 
    material->diffuseColor.setValue(0.4,0.6,0.3);
    material->transparency = 0.5;
    separator->addChild(material);

    SoShapeHints* shapeHints = new SoShapeHints;
    //shapeHints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
    shapeHints->creaseAngle.setValue( 3.0 );
    separator->addChild(shapeHints);

    calculateYValues();
    SoCoordinate3 *coord = new SoCoordinate3;

    SbVec3f* coordinates;
    coordinates = new SbVec3f[terrainLevelOfDetail*terrainLevelOfDetail];
    for (i=0;i<terrainLevelOfDetail;i++) 
    {
        for (j=0;j<terrainLevelOfDetail;j++) 
        {
            SbVec3f* vector;
            //x and y values have to be scaled and translated
            vector = new SbVec3f(float(i)*(2*fieldLength/(terrainLevelOfDetail-1))-fieldLength, yValues[i][j], float(j)*(2*fieldLength/(terrainLevelOfDetail-1))-fieldLength);
            coordinates[j+i*terrainLevelOfDetail] = *vector;
        }
    }

    coord->point.setValues(0, terrainLevelOfDetail*terrainLevelOfDetail, coordinates);
    separator->addChild(coord);

    SoIndexedFaceSet* terrain = new SoIndexedFaceSet;

    int* coordIndex;
    coordIndex = new int[5*(terrainLevelOfDetail-1)*(terrainLevelOfDetail-1)];
    for (i=0;i<terrainLevelOfDetail-1;i++) 
    {
        for (j=0;j<5*(terrainLevelOfDetail-1);j=j+5) 
        {
            int offset = j+i*5*(terrainLevelOfDetail-1);
            int offset2 = (j/5)*terrainLevelOfDetail+i;
            coordIndex[offset] = offset2;
            coordIndex[offset+1] = offset2+1;
            coordIndex[offset+2] = offset2+terrainLevelOfDetail+1;
            coordIndex[offset+3] = offset2+terrainLevelOfDetail;
            coordIndex[offset+4] = -1;
        }
    }

    terrain->coordIndex.setValues(0, 5*(terrainLevelOfDetail-1)*(terrainLevelOfDetail-1), coordIndex);

    separator->addChild(terrain);
    return separator;
}

