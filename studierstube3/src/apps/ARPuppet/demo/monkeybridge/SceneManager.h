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
/* SceneManager.h contains the scene manager component
 *
 * @author Istvan Barakonyi
 *
 * $Id: SceneManager.h 4209 2005-05-19 09:06:39Z bara $
 * @file
 */
/* ======================================================================== */

#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

// Inventor classes
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFVec2f.h>
#include <Inventor/fields/SoSFVec4f.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFBool.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoMFVec2f.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoMFVec4f.h>
#include <Inventor/fields/SoMFRotation.h>
#include <Inventor/fields/SoMFTime.h>
#include <Inventor/fields/SoMFColor.h>
#include "Inventor/nodes/SoSeparator.h"
#include <Inventor/sensors/SoFieldSensor.h>

// STD library include
#include <string>

// own classes
#include "../../characters/SoPuppeteerKit.h"
// for the DLL export
#include "monkeybridge.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** SceneManager is a node responsible for the following tasks
    TODO: write documentation

@author Istvan Barakonyi
@ingroup ARPuppet
*/
class MONKEYBRIDGE_API SceneManager : public SoPuppeteerKit 
{
    SO_KIT_HEADER(SceneManager);

    SO_KIT_CATALOG_ENTRY_HEADER(tileLibrary);
    SO_KIT_CATALOG_ENTRY_HEADER(newTiles);

public:

    /** Tile type enum values for the 2nd component in the 2D vectors of SoMFVec2f field <em>playfieldInfo</em>
        These enum values are are not used directly in the scenegraph, only as a converted float value to remain easily scriptable.
      */
    enum TileType {
        /// default values
        INVALID=-2, WATER=-1,
        /// tiles from which characters start the game and which they have to reach to win the game
        STARTTILE, ENDTILE,
        /* initially unoccupied fix tiles (real objects) that can be occupied by the characters and built into their path:
           fixtile_12a, fixtile_22
         */
        FIXTILE0, FIXTILE1,
        /// decoration elements that cannot be occupied by any of the characters (e.g. volcano, lighthouse etc.)
        DECORATION0, DECORATION1,
        /* tiles placed by the players during the game: 
           tile_12a, tile_12b, tile_12c, tile_13, tile_22, tile_23a, tile_23b, tile_23c, tile_33
         */
        USERTILE0, USERTILE1, USERTILE2, USERTILE3, USERTILE4, USERTILE5, USERTILE6, USERTILE7, USERTILE8,
        MAXTILEINDEX=USERTILE8
    };

    // Avoiding "magic nubers" enables easier hotspot reference in the code
    enum HotspotIndex { HS_START=0, HS_CENTER, HS_END };

    /// Character walking direction enums
    enum Direction { NORTH=0, EAST, SOUTH, WEST };

    /// Tile information
    /*@{*/

    /* Array of offset vectors for tile hotspots --> for tile #i the j'th hotspot height offset is:
       hotspotOffset[i*5+j], where 0<=j<=4, 0 = north, 1 = east, 2 = south, 3 = west, 4 = center
     */    
    SoMFVec3f hotspotOffset;

    /*@}*/

    /// Playfield information
    /*@{*/

    /// (X,Y) = maximum number of fields in X and Y direction
    SoSFVec2f maxCoordinate;

    /// The size of a tile (defines an area of tileSize x tileSize meters)
    SoSFFloat tileSize;

    /// Scaling playfield parameters (connected to global game scale parameter)
    SoSFFloat globalScale;

    /*@}*/

    /// Position and orientation-related fields + information about the next tiles' type
    /*@{*/

    /// Input position of the cursor the moveable tiles are bound to
    SoMFVec3f tileToPlacePosition;

    /// Input orientation of the cursor the moveable tiles are bound to
    SoMFRotation tileToPlaceOrientation;

    /// Types of the current "next tile" candidates
    SoMFInt32 tileToPlaceType;

    /// Types of the current "next tile" candidates coming from a remote source
    SoMFInt32 tileToPlaceTypeRemote;

    /* The value of <em>tileToPlacePosition</em> snapped to grid if within the playfield, 
       otherwise echoes <em>tileToPlacePosition</em>
     */
    SoMFVec3f tileSnappedPosition;

    /// The value of <em>tileSnappedOrientation</em> snapped to grid, otherwise echoes <em>tileToPlaceOrientation</em>
    SoMFRotation tileSnappedOrientation;

    /// TEMPORARY variable storing the potential final information for the new tiles being moved:
    /// value format = (x_coord,y_coord,userID,type), WITHOUT encoded tile number and rotation information
    SoMFVec4f tileSnappedInfoTemp;
    
    /// Information about the most recently placed tile
    SoSFVec4f tileSnappedInfo;

    /// Information about the most recently placed remote tile.
    SoSFVec4f tileSnappedInfoRemote;

    /// If <em>useRemoteInfo</em> is TRUE, it means that information on tile#i is coming from a remote source, do not process it locally.
    SoMFBool useRemoteInfo;

    /* Maximum tile type index --> needs to be in accordance with the tile list in the director and 
       the tile types defined in the scene manager
     */
    SoSFInt32 maxTileTypeID;

    /* List of tile types to avoid (e.g. start tile, end tile, fixed tiles etc.) --> needs to be in 
       accordance with the tile types defined in the scene manager
     */
    SoMFInt32 avoidTileType;

    /// Defines colors to blend the tile texture with to identify to which user they belong to
    SoMFColor tileColor;

    /*@}*/
    
    /// Fields related to playfield occupancy and placing of new tiles
    /*@{*/

    /* Occupation information of the playfield, only tiles different from the type WATER need to be listed
       for each value = playfieldInfo(x,y) / (x,y) specifies a tile on the playfield
            - value[0] = userID + tileNum/10
            - value[1] = tile type + rotation/(2*M_PI)
     */
    SoMFVec2f playfieldInfo;

    /// Initial playfield occupation information
    SoMFVec4f playfieldInitInfo;

    /* Bonus objects on the playfield: [x,y,type,picked_up], picked_up = 1 --> bonus picked up, 0 = bonus not picked up yet
       TODO: add support for bonus objects and implement functionality
     */
    SoMFVec4f bonusObjectInfo;

    /* Indicates the (x,y) playfield coordinates of the decoration elements (e.g. lighthouse, volcano)
       It is connected to the avoidable object coordinate field of the bodi flock puppeteer
     */
    SoMFVec3f obstacleTilePosition;

    /* Initializes / resets the game:
       - reset playfield information to an initial value based on <em>playfieldInitInfo</em>
       - resets the "picked_up" component of all bonuses to 0 in <em>bonusObjectInfo</em>
       - sets all <em>wayCompleted</em> elements to FALSE
     */
    SoSFTrigger init;

    /*@}*/

    /// Information for game character motion
    /*@{*/    
    
    /// Coordinates indicating the previous playfield coordinates of the characters
    SoMFVec2f previousTileCoord;

    /// Coordinates indicating the current playfield coordinates of the characters
    SoMFVec2f currentTileCoord;

    /// Coordinates indicating the currently chosen next playfield coordinates of the characters
    SoMFVec2f nextTileCoord;

    /* The user ID of the player that has just started to move from the center hotspot to the end hotspot of the tile
       This field is connected from the director's <em>playerCompletedTile</em> field.
     */
    SoSFInt32 playerCompletedTile;

    /* The user ID of the player that has just started to move from the start hotspot to the center hotspot of the tile,
       therefore needs to know where the next tile will be to determine the direction to move into after reaching the center.
       This field is connected from the director's <em>seekNextTile</em> field.
     */
    SoSFInt32 seekNextTile;

    /* Indicates that the hotspot information for user#(hotspotSet.getValue()) has been set and ready to be used in the motion
       This field is connected to the director.
     */
    SoMFInt32 hotspotSet;

    /// Indicates whether user #i has reached his end tile. This field is connected to the game manager and to the director.
    SoMFBool targetReached;

    /* Indicates whether the value in <em>nextTileCoordinate</em> of user #i represents a valid tile.
       This field is connected to the director.
     */
    SoMFBool nextTileValid;

    /// Indicates whether the users have already placed their first tile
    SoMFBool firstTilePlaced;

    /*@}*/

    /// User information
    /*@{*/

    /// Number of player characters --> connected from the <em>characterNum</em> field of the choreographer
    SoSFInt32 playerNum;

    /* Input field storing the number of tiles each user may theoretically choose from --> provides a hint to the scene 
       manager how to associate the tiles to users among all the <em>tileTotalNum</em> pieces of tiles
     */
    SoMFInt32 tileNum;

    /// Maximum number of tiles the users are allowed to place in front of their character
    SoMFInt32 tileToPlaceNumMax;

    /// The current number of tiles that the users are actually allowed to place.
    /// (dynamically manipulated by the director based on character behavior i.e. how many tiles users are ahead of their character)
    SoMFInt32 tileToPlaceNum;

    /// Output field indicating the total number of current tiles of all users ( always set to tileToPlacePosition.getNum() )
    SoSFInt32 tileTotalNum;

    /// Number of seconds to elapse before fixing the tile's position
    SoMFTime timeOut;

    /// This field indicates whether users are currently allowed to place tiles (TRUE) or not (FALSE)
    SoMFBool enabled;

    /// Indicates whether currently placed user tiles are over valid playfield spots
    SoMFBool isOverValidCoord;

    /* Indicates whether to show the "tile placed" cursor for a certain user or not. It is set to TRUE for a certain user if the 
       user has just placed a new tile on the playfield, the <em>timeOut</em> has passed and the tile got fixed. It will be set back
       to FALSE after <em>tilePlacedCursorTimeOut</em> seconds have passed
     */
    SoMFBool tilePlacedCursorOn;

    /// Number of seconds to show the "tile placed" cursor before blending out
    SoMFTime tilePlacedCursorTimeOut;

    /* If TRUE, it indicates that a serious error has happened during initialization and the game cannot continue, 
       e.g. no start or end tile has been defined for one of the players. If it's FALSE, the initialization was successful.
     */
    SoSFInt32 initError;

    /// Indicates whether to turn the visible representation of the physical tiles on for debugging or only render into the Z-buffer
    SoSFBool showPhysicalTiles;

    /// Specifies a filename for the texture of the debug output of the tiles
    SoSFString physicalTileTexture;

    /*@}*/

    /// Constructor
    SceneManager();

SoINTERNAL public:

    /// Inventor class initialization
    static void initClass();

protected:

    /// Indicating whether the scene manager component has been initialized
    bool initialized;

    /// Objects for internal scene graph
    SoSeparator *oldTiles;
    SoSeparator *physicalTiles;

    /// Field sensors
    SoFieldSensor *tileToPlacePositionSensor;
    SoFieldSensor *tileToPlaceOrientationSensor;
    SoFieldSensor *tileToPlaceTypeRemoteSensor;
    SoFieldSensor *tileSnappedInfoRemoteSensor;
    SoFieldSensor *playerCompletedTileSensor;
    SoFieldSensor *seekNextTileSensor;
    SoFieldSensor *initSensor;

    /// List containing timers for placing tiles and "tile placed" cursors
    SbPList timerList;
    SbPList tilePlacedTimerList;

    /// Destructor
    virtual ~SceneManager();

    // Setting up field connections, etc.
    SbBool setUpConnections(SbBool onOff, SbBool doItAlways);

    /// Static callback functions for field sensors
    static void tilePoseCB(void *data,SoSensor *sensor);
    static void tileToPlaceTypeRemoteCB(void *data,SoSensor *sensor);
    static void tileSnappedInfoRemoteCB(void *data,SoSensor *sensor);
    static void playerCompletedTileCB(void *data,SoSensor *sensor);
    static void seekNextTileCB(void *data,SoSensor *sensor);
    static void initCB(void *data,SoSensor *sensor);
    
    /// Static callback functions for alarm sensors
    static void timeOutCB(void *data,SoSensor *sensor);
    static void tilePlacedCursorCB(void *data,SoSensor *sensor);

    /// Virtual functions inherited from SoPuppeteerKit
    void positionChanged(SoNode *node);
    void orientationChanged(SoNode *node);

    /// Initialize scene manager, called by initCB
    void initialize();

    // initialize timer list
    void initTimerLists();

    // place tile on playfield
    void placeTileOnField(int tileID,bool remoteInfo);

    /// Calculate user ID from tile ID
    void getUserIdFromTileNum(int tileID,int &userID,int &userTileID);

    /// Utility function to convert (x,y) coordinates to a single index for the field <em>playfieldInfo</em>
    int getPlayfieldInfoIndex(int x,int y);
    int getPlayfieldInfoIndex(SbVec2f coord);

    /// Public functions to retrieve the relative position and orientation on the playfield of the tile defined by <em>tileInfo</em>
    void getTilePositionOnField(SbVec2f tileInfo,SbVec3f &position);
    void getTilePositionOnField(SbVec4f tileInfo,SbVec3f &position);
    void getTileOrientationOnField(SbVec4f tileInfo,SbRotation &orientation);

    /// Generate new type ID for new tile based on the given user ID
    int generateTileType(int userID);

    /// Add fix tile to the scene manager's scene graph
    void addOldTile(SbVec4f info);

    /// Add new tile to the scene manager's scene graph
    void addNewTile(int tileID);

    /// Fix current tile as old tile and generate a new tile in place of it
    void replaceTile(int tileID);

    /* Calculates the correct nextTileCoordinate values for the users.
       Rules:         
           if there is a tile providing the character with a straight path: go straight
           else check each direction and go into the direction where a valid neighbor tile is found
     */
    void setNextTile(int userID);

    /// Sets end, start hotspot coordinates for the characters when instructed
    void setHotspots(int userID);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SCENEMANAGER_H_
