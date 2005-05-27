 /* =======================================================================
  * (C) 2001 Vienna University of Technology
  * =======================================================================
  * PROJECT: Studierstube
  * ======================================================================= */
/** ChessManKit.h contains the definition of ChessManKit.
  * ChessManKit is the widget class for the Studierstube chess applet.
  * Study this example if you are interested in developing your own
  * multi-user applications.
  *
  * @see ChessKit implementation of chess main applet (including chess board)
  *
  * @author  Dieter Schmalstieg
  *
  * $Id: ChessManKit.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _CHESSMANKIT_H_
#define _CHESSMANKIT_H_

#include <Inventor/SbColor.h>
#include <Inventor/fields/SoSFVec2f.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>
#include <stbapi/interaction/SoDragKit.h>
#include "ChessKit.h"
#include "chess.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif


#define chessboardplane 0.0

/** Widget class for chess applet.
  * ChessManKit implements a generic chess figure as a 3D-aware widget.
  * It maintains its own properties like current and default position,
  * white or black etc. It known what kind of chess man it is (kind, pawn
  * etc.) and allows only valid moves for that chess man type.
  * It allows a user to move it, then will automatically adjust its position
  * to the next play field's center when the player drops it.
  *
  * @author Dieter Schmalstieg
  */
class CHESS_API ChessManKit
:
    public SoDragKit
{
    SO_KIT_HEADER(ChessManKit);

public:
/// determines which kind of chess man the node represents
    enum ChessManType
    {
        PAWN   = 0, ///< enum value for pawn
        ROOK   = 1, ///< enum value for root
        BISHOP = 2, ///< enum value for bishop
        KNIGHT = 3, ///< enum value for knight
        KING   = 4, ///< enum value for king
        QUEEN  = 5  ///< enum value for queen
    };

/// what kind of chess man this node represents
    SoSFEnum kind;

/// default position for this chess man
    SoSFVec2f defaultPosition;

/// current position for this chess man
    SoSFVec2f currentPosition;

/// field is true if node is a white chess man, false if black
    SoSFBool isWhite;

/// field stores the length of one play tile of the chess board
    SoSFFloat tileLength;

/// static class initialization
    static void initClass();

/// default constructor
    ChessManKit();

/// destructor
    ~ChessManKit();

/// call this to reset chess man to default position/color
    void reset();

/** callback executed when user picks up chess man
  * @param data note used
  * @param piece pointer to chess man instance */
    static void cbPickupPiece(void* data, SoDragKit* piece);

/** callback executed when user drops piece, checks for valid move
  * @param data note used
  * @param piece pointer to chess man instance */
    static void cbDropPiece(void* data, SoDragKit* piece);

/** set internal pointer to chess board owning this piece
  * @param aChessKit pointer to the chess board */
    void setChessKit(ChessKit* aChessKit);

/** convert single logical board coordinate to world coordinate
  * @param pos logical (integer) board coordinate
  * @return world coordinate */
    float positionToBoard(int pos);

/** set the chess man to a given color
  * @param color the new color */
    void setPieceColor(SbColor color);

/** compute single quantized coordinate after movement
  * @param original coordinate that the user has input
  * @param quantized coordinate of next play field's center
  * @param boardPos integer (logical) value of that play field */
    void placePiece(float original, float& quantized, short& boardPos);

    /// reset chess man's color to either white or black
    void resetColor();

private:

/** check for internal validity of movement
  * @param nx logical target x-coordinate
  * @param ny logical target y-coordinate
  * @return true if it is a valid move */
    SbBool validMove(int nx, int ny);

/// trigger (and check) movement after user has dropped figure
    void drop();

/// pointer to containing chess kit
    ChessKit* ck;

}; //class ChessManKit

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_CHESSMANKIT_H_

