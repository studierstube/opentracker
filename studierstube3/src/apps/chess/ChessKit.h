 /* =======================================================================
  * (C) 2001 Vienna University of Technology
  * =======================================================================
  * PROJECT: Studierstube
  * ======================================================================= */
/** ChessKit.h containts the definition of ChessKit.
  * ChessKit is the main class for the Studierstube chess applet.
  * Study this example if you are interested in developing your own
  * multi-user applications.
  *
  * @see ChessManKit implementation of chess playing figures
  *
  * @author Dieter Schmalstieg
  *
  * $Id: ChessKit.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _CHESSKIT_H_
#define _CHESSKIT_H_

#include <assert.h>
#include "chess.h"
#include <stbapi/stbapi.h>
#include <stbapi/interaction/So3DSeparator.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/workspace/SoContextKit.h>
#include <stbapi/tracker/SoStationKit.h> 
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoText3.h> 
#include <Inventor/nodes/SoText2.h> 
#include <Inventor/nodes/SoMatrixTransform.h>
#include <Inventor/nodes/SoSwitch.h> 
#include <Inventor/SoPickedPoint.h>
#include "../../stbapi/util/zpthread.h" 

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class ChessManKit;

/** ChessKit - main class for chess applet.
  * ChessKit is the main class for the 2-player chess applet for the
  * Studierstube framework. It is intended as tutorial code for non-trivial
  * multi-user interaction in the Studierstube framework. In particular,
  * it demonstrates the use of 3D events and 3D-aware nodes using the
  * example of chess men (ChessManKit).
  * - This applet shows a chess board and lets two users play.
  * - User can drag and drop chess man with the pen. They will automatically
  * - snap to the next playfield center.
  * - The pip only has a restart game button.
  * - Multiple simultaneous games are possible.
  * - There is no chess engine, so game progress such as chess mate etc.
  *   must be determined by the human users.
  * - However, formal rules of the game are enforced. Users can only
  *   move chess men when its their turn. They can never move the opponent's
  *   chess men. Illegal moves (such as beating one's own figure) are
  *   disallowed (the chess man simply jumps back to its initial position.
  *   Note that some of the more complex legal (e.g. Rochade) and illegal
  *   moves are not detected to keep the code simple.
  *
  * @author Dieter Schmalstieg
  */





class CHESS_API ChessKit
:
    public SoContextKit
{
    SO_KIT_HEADER(ChessKit);

public:

/// true if it white is to turn
    SoSFBool whiteTurn;

/// static class initialization
    static void initClass();

/// constructor - initializes node
    ChessKit();

/// creates new context of a ChessKit type
    SoContextKit* factory();

/// destructor - does nothing
    ~ChessKit();

/** callback triggered to reset a game
  * @param data pointer to ChessKit instance needed by static member
  * @param button pointer to the button that was pressed on pip */
    static void resetGame(void* data, So3DButton* button);

/** determine wether chessMan may move to desiredPos(X/Y)
  * this procedure of the chess kit is called by a chess man when
  * it wants to move to a specific position. it computes whether the chess man
  * may move there, and if another chessman is beaten. by its return value,
  * it signals the chess man whether it may move to the desired position
  * besides, a beaten figure is removed, and the turn to move is passed on.
  * @param chessMan the chess man to examine
  * @param desiredPosX x-coordinate of target position
  * @param desiredPosY y-coordinate of target position
  * @return true if this is a valid move */
    SbBool moveEval(ChessManKit* chessMan, int desiredPosX, int desiredPosY);

/// called after every move to make sure only right player can move
    void enforceTurn();

/** search and return chess man at a given position
  * @param desiredPosX x-coordinate of queried position
  * @param desiredPosY y-coordinate of queried position
  * @return pointer to found chess man or null if none found */
    ChessManKit* findChessMan(int desiredPosX, int desiredPosY);

    char lastMove[80];
    Semaphore* semaComm;
    Semaphore* semaMove;

    void gnuMove();

    static void cbraypen( void *userData, int stationId,  SoStationKit *stationKit, SoHandle3DEventAction *action);

private:



    /** create new pip sheet
      * @param uid id of user assigned to sheet
      * @param pipSheetContents A node containing the pipSheets content.
      * @return root node of created pip sheet */
    //SoNode* createPipSheet(int uid);
	virtual void checkPipConnections( int uid, SoNode * pipSheetContents );

    /** 
    Create window and content. Basically reads the contents of geometry/app_geom.iv.
    @param index index of window 
    */
    SbBool checkWindowGeometry();

    /** set master/slave mode for one user's pip sheet
      * @param pipSheet pointer to a particular pip sheet
      * @param masterMode true if master, false if slave mode */
	void checkPipMasterMode(SoNode* pipSheet, SbBool masterMode);

/// init function resets the game
    virtual void init(SbBool forceInit=FALSE);

/// resets colors of all chessmen
    static void resetAllColors(SoSeparator *root);
    
/// pointer to root node of all white pieces
    So3DSeparator* whitePieces;

/// pointer to root node of all black pieces
    So3DSeparator* blackPieces;

}; //class ChessKit

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_CHESSKIT_H_

