 /* ======================================================================= */
/** header file for SoTableTopKit Node.
  *
  * @author Christiane Ulbricht
  *
  * $Id: SoTableTopKit.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOSAMPLEKIT_H_
#define _SOSAMPLEKIT_H_

#include <stbapi/widgets/So3DButton.h>
#include <stbapi/workspace/SoContextKit.h>
#include "tabletop.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include <string>

using namespace std;

    FILE *logi;
    const char* logFileName = "..\\..\\src\\apps\\tabletop\\log.txt";
    const char* configFileName = "..\\..\\src\\apps\\tabletop\\config\\config.txt";

    const int defNrOfPlayers = 2;
    const static int defNrOfCatapultsPerPlayer = 1;
    const int defNrOfWindmillsPerPlayer = 1;
    const int defWindmillIntensity = 3;
    const float defWindmillAngle = M_PI/8.0; //0.3926
    const float defBalloonSpeed = 0.2;
    const float defFieldLength= 0.5;
    const int defMaxNrOfBalloons = 4;
    const int defShootToWin = 4;

    //terrainLevelOfDetail: number of squares per 2*fieldLength
    //has to be a power of 2, + 1
    const int defTerrainLevelOfDetail = 5;
    const float defTerrainRange = 0.25;
    const float defParabelHeight = 0.3;
    const float defParabelShape = 3.0; //the smaller the flatter
    const float defParabelWidth = 0.4; //the smaller the wider
    const float defBulletSpeed = 0.025; //the higher the faster
    const float defCatapultArmAngle = 1.4;
    const float defCatapultSpeed = 2.0; //the higher the faster
    const float defFollowSpeed = 0.5;
    const int defNrOfChests = 2;

    float balloonSize;

    int terrainLevelOfDetail; 
    float terrainRange;
    
    int startingWindmillIntensity;
    float startingWindmillAngle;
    float startingParabelHeight;
    float startingParabelShape;
    float startingParabelWidth;
    float startingBulletSpeed;
    float startingCatapultArmAngle;
    float startingCatapultSpeed;

    string gameType;
    string gameStatus;
    string movementType;
    string showTerrain;
    string showAnimation;
    string showFlowVis;
    string newBalloons;
    int activePlayer;
    int nrOfPlayers;
    int nrOfCatapultsPerPlayer;
    int nrOfWindmillsPerPlayer;
    float balloonSpeed;
    float fieldLength;
    int maxNrOfBalloons;
    int shootToWin;
    float followSpeed;
    int nrOfChests;

    SoSeparator** players;
    SoSeparator* root2;
    SoSeparator* menu;
    SoSeparator* chests; 
    SoSeparator* gameWon; 
    SoGroup* stations;
    SoSeparator* crosses;

    float* windmillIntensity;
    float* windmillAngle;
    float* parabelHeight;
    float* parabelShape; 
    float* parabelWidth;
    float* bulletSpeed;
    float* catapultArmAngle;
    float* catapultSpeed;
    string** catapultStatus;
    float** catapultTime;
    int** nrOfArmMoves;

    int** nrOfBullets; //nrOfBullets of player x at catapult y
    bool** armUp;
    float** formerAngle;
    float** angle;
    
    float** yValues;

    int* pointsOfPlayer;
    int* playerHasShot;
    int* nrOfBalloons;
    int* nrOfFallingBalloons;
    float* intervalLength;

    SbVec3f** balloonColors;
    SbVec3f** balloonPositions;
    SbVec3f** initCatapultPositions;
    SbVec3f** initWindmillPositions;

    SbTime myBaseTime;

/** SoTableTopKit contains the implementation of the Tabletop Game
 *
 * @author Christiane UIbricht
 */
class TABLETOP_API SoTableTopKit
:
    public SoContextKit
{
    SO_KIT_HEADER(SoTableTopKit);

public:
    /** static class initialization */
    static void initClass();
    /** constructor */
    SoTableTopKit();
    /** override this to create new context of a specific type */     
    SoContextKit* factory();                 
    /** destructor */
    ~SoTableTopKit(); 

private:
    /** create new pip sheet
     * @param uid user assigned to sheet 
     */ 
    SoNode* createPipSheet( int uid );

    /**
     * addSpecialEventCB adds special EventCallback to the pieces of the player
     * @param nrOfThePlayer number of the player
     */
    static void addSpecialEventCB(int nrOfThePlayer);

    /**
     * removeSpecialEventCB removes special EventCallback from the pieces of the player
     * @param nrOfThePlayer number of the player
     */
    static void removeSpecialEventCB(int nrOfThePlayer);

    /**
     * addDefaultEventCB adds default EventCallback to the pieces of the player
     * @param nrOfThePlayer number of the player
     */
    static void addDefaultEventCB(int nrOfThePlayer);

    /**
     * removeDefaultEventCB removes default EventCallback from the pieces of the player
     * @param nrOfThePlayer number of the player
     */
    static void removeDefaultEventCB(int nrOfThePlayer);
    
    /**
     * setRotation sets the rotation of transform to the value of the rotation of the stationKit
     * @param transform the SoTransform node
     * @param stationKit the stationKit
     */
    static void setRotation(SoTransform* transform, SoStationKit* stationKit);

    /**
     * setTranslation sets the translation of transform to the values of x,y and z
     * @param transform the SoTransform node
     * @param x the value of x
     * @param y the value of y
     * @param z the value of z
     */
    static void setTranslation(SoTransform* transform, float x, float y, float z);
    
    /**
     * myDefaultEventCB sets the transform node of a piece to the value of the according stationKit
     * @param userData the transform node of the piece
     * @param stationId the id of the station
     * @param stationKit the according SoStationKit node
     * @param action the SoHandle3DEventAction node
     */
    static void myDefaultEventCB( void *userData, int stationId, SoStationKit *stationKit, SoHandle3DEventAction *action);

    /**
     * mySpecialEventCB sets the transform node of a piece to the value of the according stationKit
     * or not, if the marker is outside of the circle
     * @param userData the piece
     * @param stationId the id of the station
     * @param stationKit the according SoStationKit node
     * @param action the SoHandle3DEventAction node
     */
    static void mySpecialEventCB( void *userData, int stationId, SoStationKit *stationKit, SoHandle3DEventAction *action);

    /**
     * playSound plays a wave file
     * @param fileName the name of the file
     */
    static void playSound(string fileName);

    /**
     * sqr returns the square of x
     * @param x the number
     * @return the square of x
     */
    static float sqr(float x);

    /**
     * signum returns 1 if x is greater or equal than 0 and -1 if not
     * @param x the number
     * @return the value of the signum function of x
     */
    static float signum(float x);

    /**
     * distanceBetweenTwoPoints returns the distance between point1 and point2
     * @param point1 the position of point1
     * @param point2 the position of point2
     * @return the distance between point1 and point2
     */
    static float distanceBetweenTwoPoints(SbVec3f point1, SbVec3f point2);

    /**
     * setLastPosToNewPos sets the value of the lastPos node to the actual value of 
     * the transform node of the piece
     * @param nrOfThePlayer number of the player
     */
    static void setLastPosToNewPos(int nrOfThePlayer);

    /**
     * changeStyleOfLastPos set the drawStyle value of the lastPos node to posStyle
     * @param nrOfThePlayer number of the player
     * @param posStyle the desired style
     */
    static void changeStyleOfLastPos(int nrOfThePlayer, int posStyle);

    /**
     * checkPositions returns true if all markers are at the correct positions
     * @param nrOfThePlayer number of the player
     * @return true if all markers are at the correct positions, false otherwise
     */
    static bool checkPositions(int nrOfThePlayer);

    /**
     * checkMenu checks if the menuMarker has intersected a button and returns the 
     * name of the button
     * @param vpReg needed for calculating the intersection
     * @return the name of the button, or an empty string, if no button was intersected
     */
    static string checkMenu(SbViewportRegion vpReg); 

    /**
     * checkChest schecks if a catapult has intersected a chest. if yes, the parabel 
     * of the bullets of this catapult is enlarged
     */
    static void checkChests();

    /**
     * rotateWindmillArrows rotates the arrows of the windmills if the areas of wind 
     * of two or more windmills are intersecting
     */
    static void rotateWindmillArrows();

    /**
     * switchLastPosGeometry alters the geometry of the lastPos node to whichChild (circle or square)
     * @param nrOfThePlayer number of the player
     * @param whichChild number of the child that should become visible
     */
    static void switchLastPosGeometry(int nrOfThePlayer, int whichChild);

    /**
     * switchWindmills switches the rotor node of the windmills of one player on or off
     * @param nrOfThePlayer number of the player
     * @param on true if rotor should switch to on, false otherwise
     */
    static void switchWindmills(int nrOfThePlayer, bool on); 

    /**
     * switchWindmillsOfAllPlayers calls switchWindmills for all players
     * @param on true if rotor should switch to on, false otherwise
     */
    static void switchWindmillsOfAllPlayers(bool on); 

    /**
     * angleBetweenVectors returns the angle between two lines
     * @param x1 x value of line 1
     * @param z1 z value of line 1
     * @param x2 x value of line 2
     * @param z2 z value of line 2
     * @return the angle between the two lines
     */
    static float angleBetweenVectors(float x1, float z1, float x2, float z2);

    /**
     * if a balloon touches another balloon, the second balloon is moved out of the way
     * @param nrOfThePlayer number of the player
     * @param nrOfTheBalloon number of the balloon of player nrOfThePlayer, that is testet for collisions
     * @param distanceBalloon distance between the balloon nrOfTheBalloon and the windmill
     * @param xWindmill value of x of the position of the windmill
     * @param zWindmill value of z of the position of the windmill
     * @param posOfTheBalloon contains the position of the balloon that is tested
     */
    static void collisionDetectionBalloons(int nrOfThePlayer, int nrOfTheBalloon, float distanceBalloon, float xWindmill, float zWindmill, SbVec3f posOfTheBalloon);

    /**
     * positionOfTheBalloons calculates the positions of the balloons of one player
     * @param nrOfThePlayer number of the player
     */
    static void positionOfTheBalloons(int nrOfThePlayer); 

    /**
     * positionOfTheFallingBalloons calculates the position of the falling balloon 
     * of one player and removes them from the scene graph if they reached the ground
     * @param nrOfThePlayer number of the player
     */
    static void positionOfTheFallingBalloons(int nrOfThePlayer); 

    /**
     * positionOfTheCatapults calculates the position of the arm of the catapult 
     * and the bullets of one player
     * @param nrOfThePlayer number of the player
     * @param count the value of the counter of the timerCallback routine
     * @param vpReg needed for calculating the intersections
     */
    static void positionOfTheCatapults(int nrOfThePlayer,double count, SbViewportRegion vpReg);

    /**
     * movePieces calculates position and orientation of the pieces in followUp mode
     * and returns true if all pieces stand still
     * @return true if all pieces stand still, false otherwise
     */
    static bool movePieces();

    /**
     * createMenu creates the menu node, containing the menu marker and the button
     * @param coneFile the geometry of the menu marker
     * @param nextFile the geometry of the button
     * @return the menu node
     */
    static SoSeparator* createMenu(SoSeparator* coneFile, SoSeparator* nextFile); 
    
    /**
     * addCross adds a cross to the crosses separator
     * @param crossFile the geometry of the cross
     */
    static void addCross(SoSeparator* crossFile);

    /**
     * createChests creates the chests node
     * @param chestFile the geometry of the chest
     * @return the chest node
     */
    static SoSeparator* createChests(SoSeparator* chestFile); 

    /**
     * isGameWon returns true if the player has won the game
     * @param nrOfThePlayer number of the player
     * @return true if the player has won the game, false otherwise
     */
    static boolean isGameWon(int nrOfThePlayer);

    /**
     * addGameWonText adds the gameWon node to the scene graph
     */
    static void addGameWonText();

    /**
     * animationStopped returns true if the animations started in execute status have stopped
     * @return true if the animations have stopped, false otherwise
     */
    static boolean animationStopped();

    /**
     * myTimerCB handels the state of the application
     * @param data is not used
     * @param sensor contains the time value
     */
    static void myTimerCB(void *data, SoSensor *sensor); 

    /**
     * initVariables initializes the variables
     */
    static void initVariables();

    /**
     * readConfigFile reads the configFile
     */
    static void readConfigFile();

    /**
     * createWindowGeometry creates the scene graph and returns an according windowKit
     * @param index
     * @return the root node of the scene graph
     */
    SoWindowKit* createWindowGeometry( int index ); 

    /**
     * stopTraversal stops the traversal of the station callbacks after the station node
     * has been reached
     * @param 
     * @return 
     */
    static SoCallbackAction::Response stopTraversal(void *, SoCallbackAction *, const SoNode *node);

    /**
     * getYValueOfTerrain returns the interpolated value of y at the position (x,z)
     * @param x the value of x
     * @param z the value of z
     * @return the value of y at the position (x,z)
     */
    static float getYValueOfTerrain(float x, float z);
 
    /**
     * sub returns the difference between num and sub, or, if this difference is
     * negative, the value of num - sub + (terrainLevelOfDetail-1)
     * @param num the value of num
     * @param sub the value of sub
     * @return the difference between num and sub, or, if this difference is
     * negative, the value of num - sub + (terrainLevelOfDetail-1)
     */
    static int sub(int num, int sub);

    /**
     * add returns the sum of num and add, or, if this sum is greater than 
     * (terrainLevelOfDetail-1), the value of num + add - (terrainLevelOfDetail-1)
     * @param num the value of num
     * @param add the value of add
     * @return the sum of num and add, or, if this sum is greater than 
     * (terrainLevelOfDetail-1), the value of num + add - (terrainLevelOfDetail-1)
     */
    static int add(int num, int add);

    /**
     * getRandom returns a random number between 0 and range
     * @param range the max value of the random number
     * @return a random number between 0 and range
     */
    static float getRandom(float range);

    /**
     * calculateYValues calculates the values of y of the terrain
     */
    static void calculateYValues();    

    /**
     * createTerrain creates the terrain node
     * @return the terrain node
     */
    static SoSeparator* createTerrain();
}; //class SoTableTopKit

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOTABLETOPKIT_H_
