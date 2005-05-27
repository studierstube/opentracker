 /* =======================================================================
  * (C) 2001 Vienna University of Technology
  * =======================================================================
  * PROJECT: Studierstube
  * ======================================================================= */
/** ChessManKit.h containts the implementation of ChessManKit
  * ChessManKit is the widget class for the Studierstube chess applet.
  * Study this example if you are interested in developing your own
  * multi-user applications.
  *
  * @see ChessKit implementation of chess main applet (inkluding chess board)
  *
  * @author  Dieter Schmalstieg, Florian Ledermann
  *
  * $Id: ChessManKit.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <assert.h>
#include <stbapi/util/math2.h>
#include <stbapi/misc/ivmacros.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoTransform.h>

#include "ChessManKit.h"

SO_KIT_SOURCE(ChessManKit);

 /* ----------------------------------------------------------------------- */
/** static class initialization
  * this is automatically called when the first instance is loaded          */
 /* ----------------------------------------------------------------------- */

void
ChessManKit::initClass(void)
{
    SO_KIT_INIT_CLASS(ChessManKit, SoDragKit, "SoDragKit");
}

 /* ----------------------------------------------------------------------- */
/** constructor - initialize node
  * also set up callbacks for picking up and dropping chess man             */
 /* ----------------------------------------------------------------------- */

ChessManKit::ChessManKit()
{
    SO_KIT_CONSTRUCTOR(ChessManKit);
    //-----------------------name-|-type-------|nuldef|par |rite   |public
    SO_NODE_DEFINE_ENUM_VALUE(ChessManType, PAWN);
    SO_NODE_DEFINE_ENUM_VALUE(ChessManType, ROOK);
    SO_NODE_DEFINE_ENUM_VALUE(ChessManType, BISHOP);
    SO_NODE_DEFINE_ENUM_VALUE(ChessManType, KNIGHT);
    SO_NODE_DEFINE_ENUM_VALUE(ChessManType, KING);
    SO_NODE_DEFINE_ENUM_VALUE(ChessManType, QUEEN);
    SO_NODE_SET_SF_ENUM_TYPE(kind, ChessManType);
    SO_NODE_ADD_FIELD(tileLength, (0.0625));
    SO_NODE_ADD_FIELD(defaultPosition, (0,0));
    SO_NODE_ADD_FIELD(currentPosition, (0,0));
    SO_NODE_ADD_FIELD(kind,            (PAWN));
    SO_NODE_ADD_FIELD(isWhite,         (TRUE));
    SO_KIT_INIT_INSTANCE();

//--set callbacks to own handling methods
    setPressCallback(cbPickupPiece,this);
    setReleaseCallback(cbDropPiece,this);
}

 /* ----------------------------------------------------------------------- */
/** destructor - does nothing                                               */
 /* ----------------------------------------------------------------------- */

ChessManKit::~ChessManKit()
{
    // nil
}

 /* ----------------------------------------------------------------------- */
/** set a new color for chess man                                           */
 /* ----------------------------------------------------------------------- */

void
ChessManKit::setPieceColor(SbColor color)
{
    SoMaterial* mat = (SoMaterial*)getPart("material",TRUE);
    assert(mat);
    mat->diffuseColor = color;
}

 /* ----------------------------------------------------------------------- */
/** callback highlights chess man when user picks it up                     */
 /* ----------------------------------------------------------------------- */

void
ChessManKit::cbPickupPiece(void* data, SoDragKit* piece)
{
    // if slave mode then do not react
    if(piece->getBase3DMode() == Base3D::BASE3D_SLAVEMODE) return;
    ((ChessManKit*)piece)->setPieceColor(SbColor(1,0,0));
}

 /* ----------------------------------------------------------------------- */
/** callback calls drop function when user drops the chess man              */
 /* ----------------------------------------------------------------------- */

void
ChessManKit::cbDropPiece(void* data, SoDragKit* piece)
{
    // if slave mode then do not react
    if(piece->getBase3DMode() == Base3D::BASE3D_SLAVEMODE) return;
    ((ChessManKit*)piece)->drop();
}

 /* ----------------------------------------------------------------------- */
/** compute quantized position of a coordinate (and its logical position)   */
 /* ----------------------------------------------------------------------- */

void
ChessManKit::placePiece(float original, float& quantized, short& boardPos)
// original ... position in stb-coordinates
// quantized ...relative position to center of chessboard in stb-coordinates
// boardPos  ... chess coordinates 0..7
{
    quantized = floor(original/tileLength.getValue()) + 4.0; //4.0
    boardPos = int(clamp(quantized,0.0,7.0)); //clamp to [0..7]
    quantized = positionToBoard(boardPos); // compute quantized world coord.
}

 /* ----------------------------------------------------------------------- */
/** return true if given move is a valid move for that chess man            */
 /* ----------------------------------------------------------------------- */

SbBool
ChessManKit::validMove(int nx, int ny)
{
    int ox = (int)currentPosition.getValue()[0]; // old x
    int oy = (int)currentPosition.getValue()[1]; // old y
    int dx = m2abs(ox-nx);                       // delta x
    int dy = m2abs(oy-ny);                       // delta y
    if(dx==0 && dy==0) return FALSE; // disallow null moves

    switch(kind.getValue())
    {
    case PAWN: return 
        (dx==0 && dy==1) ||                     // normal move
        (dx==0 && dy==2 && (oy==1 || oy==6)) || // first pawn move
        (dx==1 && dy==1);                       // attack move
    case ROOK:   return dx==0 || dy==0; // either x *or* y movement
    case BISHOP: return dx==dy; // only diagonal
    case KNIGHT: return (dx==1 && dy==2) || (dx==2 && dy==1); //only "jumps"
    case KING:   return (dx<2 && dy<2); // only one field
    case QUEEN:  return (dx==dy || dx==0 || dy==0); // x, y, or diagonal
    default:     assert(FALSE);
    }
    return FALSE; // dummy return value
}

 /* ----------------------------------------------------------------------- */
/** drop a piece
  * first compute quantized and logical position from drop position.
  * then, verify that this is a valid move internally (validMove of
  * ChessManKit) and from the board (- of ChessKit).
  * If the latter returns true, it also signals to the chess board that
  * the move is final.
  * Finally, write back position - either the new one for a valid move,
  * or the old position before the user's move operation if the move
  * is invalid. In this case the user may (must) move again, as turns
  * are not changed.                                                        */
 /* ----------------------------------------------------------------------- */

void
ChessManKit::drop()
{

//--compute quantized new position
    SbVec3f v = translation.getValue();
    SbVec2f quant;
    SbVec2s bpos;
    placePiece(v[0],quant[0],bpos[0]);
    placePiece(v[2],quant[1],bpos[1]);



//--verify new position
    // SbBool valid, eval;
    // valid = validMove(bpos[0],bpos[1]); // check internal valid move
    // call board moveEval to (a) determine whether this move is possible,
    // and (b) make this move final if it is possible
    // if(valid) eval = ck->moveEval(this, bpos[0],bpos[1]);

    int ox = (int)currentPosition.getValue()[0]; // old x
    int oy = (int)currentPosition.getValue()[1]; // old y

//    ck->semaComm->wait();
    strcpy(ck->lastMove,"    ");
    ck->lastMove[0]=(1+ox)+96;
    ck->lastMove[1]=(8-oy)+48;
    ck->lastMove[2]=(1+bpos[0])+96;
    ck->lastMove[3]=(8-bpos[1])+48;
    ck->semaComm->signal();

    fprintf(stderr,"stb: semamove signal");
    ck->semaMove->signal();
    fprintf(stderr,"stb: semacomm wait");
    ck->semaComm->wait();

//--write back quantized new position
    if (strncmp(ck->lastMove,"correct",7)==0) // a valid move -> move to new (quantized) position
    {

        
        // check if chessman is hidden
        ChessManKit *cm=ck->findChessMan(bpos[0],bpos[1]);
        if (cm!=NULL)
        {
          // move chessman far away
          cm->currentPosition = SbVec2f(float(200),float(200));
          cm->translation = SbVec3f(0,200,200);
        }
        currentPosition = SbVec2f(float(bpos[0]),float(bpos[1]));
        translation = SbVec3f(quant[0],chessboardplane,quant[1]);
        fprintf(stderr,"stb: semamove signal\n");
        ck->semaMove->signal();
        fprintf(stderr,"stb: semacomm wait\n");
        ck->semaComm->wait();
    
    }
    else // invalid move -> move back
    {
        float x = positionToBoard((int)currentPosition.getValue()[0]);
        float y = positionToBoard((int)currentPosition.getValue()[1]);
        translation = SbVec3f(x, chessboardplane, y);
    }
    resetColor(); // unhighlight
    rotation = SbRotation(0,0,0,1); // reset chess man to upright pose
}

 /* ----------------------------------------------------------------------- */
/** reset color to either white or black                                    */
 /* ----------------------------------------------------------------------- */

void
ChessManKit::resetColor()
{
//    const SbColor whiteColor(0.8,0.6,0.6);
//      const SbColor blackColor(0.2,0.2,0.1);
      const SbColor whiteColor(0.9,0.9,0.9);
      const SbColor blackColor(0.92,0.51,0.0);
    setPieceColor(isWhite.getValue() ? whiteColor : blackColor);
}

 /* ----------------------------------------------------------------------- */
/** convert logical position to world position                              */
 /* ----------------------------------------------------------------------- */

float
ChessManKit::positionToBoard(int pos)
// relative position to center of chessboard in stb-coordinates
{
    return pos*tileLength.getValue() - 3.5*tileLength.getValue(); //+0.5-4.0
}

 /* ----------------------------------------------------------------------- */
/** reset a chess man's position and color                                  */
 /* ----------------------------------------------------------------------- */

void
ChessManKit::reset()
{
    float x = positionToBoard((int)defaultPosition.getValue()[0]);
    float y = positionToBoard((int)defaultPosition.getValue()[1]);
    translation = SbVec3f(x,chessboardplane,y);
    currentPosition = defaultPosition.getValue();
    resetColor();
}

 /* ----------------------------------------------------------------------- */
/** set internal pointer to related chess board                             */
 /* ----------------------------------------------------------------------- */

void
ChessManKit::setChessKit(ChessKit* aChessKit)
{
    ck = aChessKit;
}

//----------------------------------------------------------------------------
//eof

