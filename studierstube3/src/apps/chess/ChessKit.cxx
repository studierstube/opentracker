 /* =======================================================================
  * (C) 2001 Vienna University of Technology
  * =======================================================================
  * PROJECT: Studierstube
  * ======================================================================= */
/** ChessKit.h containts the implementation of ChessKit
  * ChessKit is the main class for the Studierstube chess applet.
  * Study this example if you are interested in developing your own
  * multi-user applications.
  *
  * @see ChessManKit implementation of chess playing figures
  *
  * @author  Dieter Schmalstieg, Florian Ledermann
  *
  * $Id: ChessKit.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <assert.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/interaction/SoDragKit.h>
#include <stbapi/util/ivio.h>
#include <stbapi/util/math2.h>
#include <stbapi/misc/ivmacros.h>
#include <stbapi/resource/SoUserKit.h>
#include <stbapi/resource/SoPipSheetKit.h>
#include <stbapi/window3d/SoWindowKit.h>

#include "ChessKit.h"
#include "ChessManKit.h"
#include "gnuchess/gnuchess.h"

int gnu_main (int argc, char **argv,ChessKit *ck);

THREADFUNC_RETURN_TYPE
gnucall(void *data)
{
    ChessKit *ck = (ChessKit *)data;
    int argc;
    char *argv[5];

    argc=3;
    argv[1]="20";
    argv[2]="2";
	
    gnu_main(argc,&(argv[0]),ck);

	return THREADFUNC_NULL;
}

//--initialization of kit structure in Inventor
SO_KIT_SOURCE(ChessKit);

//--initialization of context in Studierstube
// CONTEXT_APPLICATION(ChessKit);

 /* ----------------------------------------------------------------------- */
/** static class initialization
  * this is automatically called when the first instance is loaded          */
 /* ----------------------------------------------------------------------- */

void
ChessKit::initClass(void)
{
    SO_KIT_INIT_CLASS(ChessKit, SoContextKit, "SoContextKit");
    ChessManKit::initClass();

}

 /* ----------------------------------------------------------------------- */
/** virtual constructor - called to generically create ChessKit instances   */
 /* ----------------------------------------------------------------------- */

SoContextKit*
ChessKit::factory()
{
    return new ChessKit;
}

 /* ----------------------------------------------------------------------- */
/** constructor - initialize node                                           */
 /* ----------------------------------------------------------------------- */

ChessKit::ChessKit()
{
    SO_KIT_CONSTRUCTOR(ChessKit);
    SO_NODE_ADD_FIELD(whiteTurn, (TRUE));
    SO_KIT_INIT_INSTANCE();

    Semaphore::initClass();
    semaComm = new Semaphore;
    semaMove = new Semaphore;

//    fprintf(stderr,"chesskit: movewait\n");
    semaMove->wait();
    semaComm->wait();
    createThread(gnucall, this, 0);	
}

 /* ----------------------------------------------------------------------- */
/** destructor - don't forget to call SoContextKit destruction routine!     */
 /* ----------------------------------------------------------------------- */

ChessKit::~ChessKit()
{
    // destructor();
}

 /* ----------------------------------------------------------------------- */
/** Read pip sheet from file and set up the button callbacks                */
 /* ----------------------------------------------------------------------- */

/*
SoNode*
ChessKit::createPipSheet(int uid)
{
    comm->setContextAliasName(getName(),"Chess");
    SoSeparator *newPipSheet = readFile("sheet.iv",comm->workingDirectory);
    return newPipSheet;
}
*/

void ChessKit::checkPipConnections(int uid, SoNode* pipSheetContents){
    //comm->setContextAliasName(getName(),"Chess");
}

 /* ----------------------------------------------------------------------- */
/** create window and attach scene graph to it.                             */
 /* ----------------------------------------------------------------------- */


SbBool ChessKit::checkWindowGeometry(){
    //windowKit->size = SbVec3f(500.0,500.0,500.0);
//--build scene and place it window's client area
	SoWindowKit* myWindow = getWindow(0);
	
    if (myWindow == NULL)
	{
        myWindow = new SoWindowKit();
        myWindow->size = SbVec3f(1.0,1.0,1.0);
	}

    SoSeparator* clientVolume = myWindow->getClientVolume();

	// we get the board from the file directly
	SoSeparator* board = (SoSeparator*)findNode(clientVolume, "BOARD");
		
	whitePieces = (So3DSeparator*) board->getChild(0);
	blackPieces = (So3DSeparator*) board->getChild(1);

    init(TRUE); // reset and initialize the game board (master AND slave) 

	return TRUE;
    
}

 /* ----------------------------------------------------------------------- */
/** setup pip sheet */
 /* ----------------------------------------------------------------------- */

void ChessKit::checkPipMasterMode(SoNode* pipSheet, SbBool masterMode) {
    So3DButton* b = (So3DButton*)findNode(pipSheet,"NEWGAME_BUTTON");
    b->addPressCallback(resetGame,this);
	printf("ChessKit: setting reset button callback\n");
}

 /* ----------------------------------------------------------------------- */
/** callback for a new game */
 /* ----------------------------------------------------------------------- */

void
ChessKit::resetGame(void* data, So3DButton* button)
{
	printf("ChessKit: reset callback triggered\n");
    ChessKit* self = (ChessKit*)data;
    if(!self->isMaster) return; // do only if we are master 
    self->init(); // call reset routine
}

 /* ----------------------------------------------------------------------- */
/** game reset routine */ 
 /* ----------------------------------------------------------------------- */

void
ChessKit::init(SbBool forceInit)
{


	if(!isMaster&&!forceInit) return; // if context is master -> set callbacks

    printf("ChessKit: setting chessmen...\n");

    for(int i=0; i<16; i++) // for all chess men
    {
        ChessManKit* p = (ChessManKit*) whitePieces->getChild(i); // get white chess man
        p->reset(); // reset default position and color
        p->setChessKit(this); // set pointer to chess kit
        p = (ChessManKit*) blackPieces->getChild(i); // get white chess man
        p->reset(); // reset default position and color
        p->setChessKit(this); // set pointer to chess kit
    }
    // setup station filters properly
//    whitePieces->stations.setValue( -1 );
    whitePieces->stations.setValue(0);
    whitePieces->blocked = FALSE;
//    blackPieces->stations.setValue( -1 );
    blackPieces->stations.setValue(0);
    blackPieces->blocked = FALSE;
    whiteTurn = TRUE; // white starts
    enforceTurn(); // make sure only white can move
}
 /* ----------------------------------------------------------------------- */
/** game reset routine */ 
 /* ----------------------------------------------------------------------- */
void 
ChessKit::resetAllColors(SoSeparator *root)
{

    ChessManKit* cm;
    So3DSeparator *col;
    for(int i=0; i<16; i++) // for all chess men
    {
        // get white chess man and reset color
        col = (So3DSeparator *)findNode(root,"WHITE_PIECES");
        cm=(ChessManKit *)col->getChild(i); 
        cm->resetColor(); 
        // get black chess man and reset color
        col = (So3DSeparator *)findNode(root,"BLACK_PIECES");
        cm=(ChessManKit *)col->getChild(i); 
        cm->resetColor();
    }

}

 /* ----------------------------------------------------------------------- */
/** search for a particular chess man */
 /* ----------------------------------------------------------------------- */

ChessManKit*
ChessKit::findChessMan(int desiredPosX, int desiredPosY)
{
    for(int i=0; i<16; i++)
    {
        AGETCHILD(p,ChessManKit,whitePieces,i);
        if(p->currentPosition.getValue()[0]==float(desiredPosX) 
        && p->currentPosition.getValue()[1]==float(desiredPosY)) return p;
        AVGETCHILD(p,ChessManKit,blackPieces,i);
        if(p->currentPosition.getValue()[0]==float(desiredPosX) 
        && p->currentPosition.getValue()[1]==float(desiredPosY)) return p;
	}
    return NULL; // if field is empty return null
}

 /* ----------------------------------------------------------------------- */
/** determine wether chessMan may move to desired posistion */
// this procedure of the chess kit is called by a chess man when
// it wants to move to a specific position. it computes whether the chess man
// may move there, and if another chessman is beaten. by its return value,
// it signals the chess man whether it may move to the desired position
// besides, a beaten figure is removed, and the turn to move is passed on.
 /* ----------------------------------------------------------------------- */

SbBool
ChessKit::moveEval(ChessManKit* chessMan, int desiredPosX, int desiredPosY)
{
    ChessManKit* p = findChessMan(desiredPosX, desiredPosY);
    if(p == chessMan) return FALSE; // same old pos -> no move, abort
    if(p != NULL) // if not free space
    {
        if(p->isWhite.getValue() == chessMan->isWhite.getValue()) 
            return FALSE; // cannot beat own chessmen, abort
        // else must be a beating, so...
        p->translation = SbVec3f(1000,1000,1000); // move far away
    }
    whiteTurn = !whiteTurn.getValue(); // switch turn
    enforceTurn(); // make sure only new player can move
    
    return TRUE; // indicate user may move to that position
}

 /* ----------------------------------------------------------------------- */
/** set event masks so only the right player can make a move. 
  * get white players's pen (user index 0) and determine the pen's tracker 
  * station id then set the station mask of the event separator to filter
  * out events from all stations except for white's pen if white player is
  * supposed to move, or to block all events if it is black player's turn.
  * Do the opposite for black player and black chess men */
 /* ----------------------------------------------------------------------- */

void
ChessKit::enforceTurn()
{
	// TODO: fix this for the new api!
	/*
    // modify scene graph so other user cannot move chess man
    SoPenKit* pk;
    if(SoUserKit::getUserNumber()>1)
    {
        // get white player's pen
        pk = SoUserKit::getUserKitSequentially(0)->getPen();
        // if it is white's turn, allow white's pen, else allow nothing
////////////////////        whitePieces->stations.set1Value(0, (whiteTurn.getValue() ? pk->station.getValue() : -1));

        // same prodedure for black
        pk = SoUserKit::getUserKitSequentially(1)->getPen();
///////////////////        blackPieces->stations.set1Value(0, (!whiteTurn.getValue() ? pk->station.getValue() : -1));
    }
    else // someone is playing with him/herself, cannot do anything useful
        printf("warning - only 1 user playing chess...\n");
		*/
}

void
ChessKit::gnuMove()
{
    ChessManKit* cm;

    // calculate coordinates from string lastMove
    // old position
    int ox=(lastMove[0]-1)-96;
    int oy=(8-lastMove[1])+48;
    // new position
    int nx=(lastMove[2]-1)-96;
    int ny=(8-lastMove[3])+48;

    // check if chessman is hidden
    cm=findChessMan(nx,ny);
    if (cm!=NULL)
    {
      // move chessman far away
      cm->currentPosition = SbVec2f(float(200),float(200));
      cm->translation = SbVec3f(0,200,200);
    }

    // find chessman
    cm=findChessMan(ox,oy);
    // new translation
    float stbnx=cm->positionToBoard(nx);
    float stbny=cm->positionToBoard(ny);
    // move chessman
    cm->currentPosition = SbVec2f(float(nx),float(ny));
    cm->translation = SbVec3f(stbnx,chessboardplane,stbny);
}

/*
TO DO: different raypen implementation!

void ChessKit::cbraypen( void *userData, int stationId,  SoStationKit *sk3, SoHandle3DEventAction *action)
{

    SbString value;
    const char *str;
    float x3,y3,z3;
    float x4,y4,z4;
    float dx,dy,dz;

    ChessManKit *cm;

    SoSeparator *root   = (SoSeparator  *) userData;
    SoSeparator *raysep = (SoSeparator  *) root->getChild(0);
    SoStationKit *sk4   = (SoStationKit *) raysep->getChild(1);

    
    //SoSeparator *laser  = (SoSeparator  *) raysep->getChild(2)->getChild(0)->getChild(0);
    SoSeparator *laser = (SoSeparator  *)findNode(root,"LASER_BEAM");
    // find node containing	text explaining
    // possible moves of this chessman
    SoSwitch *txtsep = (SoSwitch *)findNode(root,"RULE_TEXT");
    // find translation of text
    SoTranslation *txttrans = (SoTranslation *)findNode(txtsep,"TEXT_POINT");
    // find textfields
    SoText3 *txt1 = (SoText3 *)findNode(txtsep,"TXTLINE1");
    SoText3 *txt2 = (SoText3 *)findNode(txtsep,"TXTLINE2");
    SoText3 *txt3 = (SoText3 *)findNode(txtsep,"TXTLINE3");
    SoText3 *txt4 = (SoText3 *)findNode(txtsep,"TXTLINE4");
    SoText3 *txt5 = (SoText3 *)findNode(txtsep,"TXTLINE5");

    // calculate position of station3
    sk3->currentPosition.get1(0,value);
    str=value.getString();
    sscanf(str,"%f %f %f",&x3,&y3,&z3);
    SbVec3f *r3 = new SbVec3f(x3,y3,z3);

    // calculate position of station4
    sk4->currentPosition.get1(0,value);
    str=value.getString();
    sscanf(str,"%f %f %f",&x4,&y4,&z4);
    SbVec3f *r4 = new SbVec3f(x4,y4,z4);

    // calculate orientation of pointer



    // set orientation and position of laser 
    // to point along laser-input-device.
    // one point of "laser" beam
    SoTranslation *trans = (SoTranslation *)findNode(laser,"LASER_POINT");
    x3=8;
    trans->translation.setValue(x3,y3,z3);//   *r3;
    SoRotation *rotx = (SoRotation *)findNode(laser,"LASER_ROTX");
    SoRotation *roty = (SoRotation *)findNode(laser,"LASER_ROTY");

    dx=x4-x3;
    dy=y4-y3;
    dz=z4-z3;
    dx=dz/20.0;
    float alpha = -atan(dy/dz);
    float beta  = atan(dx/dz);
    rotx->rotation.setValue(SbVec3f(1,0,0),alpha);
    roty->rotation.setValue(SbVec3f(0,1,0),beta); 

    // apply RayPickAction
    SoRayPickAction *pick = new SoRayPickAction(SbViewportRegion());
    pick->setRay(SbVec3f(x3,y3,z3),SbVec3f(dx,dy,dz));
    pick->setPickAll(TRUE);
    pick->apply(root);
    // look for intersection with chessman
    SoPickedPoint *ppt=pick->getPickedPoint(1);
    bool intersect =FALSE;
    if (ppt!=NULL) // something intersects ...
    {
        SoPath *pa = ppt->getPath();
        SoNode *last = pa->getTail();
        // ... is it a chessman ?
        if (last->isOfType(ChessManKit::getClassTypeId()))
        {
            // show explanation
            txtsep->whichChild=0;
            cm = (ChessManKit*)last;
            intersect=TRUE;
            // unhighlight all chessman ...
            resetAllColors(root);
            // ... and highlight selected chessman
            cm->setPieceColor(SbColor(1,0,0));
            // calculate position of chessman
            
            float qx3=cm->currentPosition.getValue()[0];
            float qy3=cm->currentPosition.getValue()[1];
            qx3=cm->positionToBoard((int) qx3);
            qy3=cm->positionToBoard((int) qy3);
            txttrans->translation=SbVec3f(chessboardplane,qx3,qy3);
            // which kind of chessman ?
            SbString kindstr;
            last->getField("kind")->get(kindstr);
            const char *ks=kindstr.getString();
            
            if (strcmp(ks,"ROOK")==0)
            {
                txt1->string="ROOK";
                txt2->string="The rook moves in a straight";
                txt3->string="line, horizontally or vert-";
                txt4->string="ically. It may not jump";
                txt5->string="over other pieces.";
            }
            else if (strcmp(ks,"BISHOP")==0)
            {
                txt1->string="BISHOP";
                txt2->string="The bishop moves in a";
                txt3->string="straight diagonal line.";
                txt4->string="The bishop may not";
                txt5->string="jump over other pieces.";
            }
            else if (strcmp(ks,"QUEEN")==0)
            {
                txt1->string="QUEEN";
                txt2->string="The queen may move in any";
                txt3->string="straight line, horizontal,";
                txt4->string="vertical, or diagonal.";
                txt5->string="";
            }
            else if (strcmp(ks,"KNIGHT")==0)
            {
                txt1->string="KNIGHT";
                txt2->string="It makes a move that consists of";
                txt3->string="first one step in a horizontal or";
                txt4->string="vertical direction, and then a step";
                txt5->string="diagonally in an outward direction.";
            }
            else if (strcmp(ks,"PAWN")==0)
            {
                txt1->string="PAWN";
                txt2->string="The pawn moves one square";
                txt3->string="straight forward.";
                txt4->string="";
                txt5->string="";
            }
            else if (strcmp(ks,"KING")==0)
            {
                txt1->string="KING";
                txt2->string="The king moves one square";
                txt3->string="in any direction, horizontally,";
                txt4->string="vertically, or diagonally. ";
                txt5->string="";
            }

            else {
			    txtsep->whichChild=-1;
            }

            //fprintf(stderr,kindstr.getString());
            //fprintf(stderr,"\n");
        }
    }
    if (intersect==FALSE)
    {
        // remove explanation
        txtsep->whichChild=-1;
        // unhighlight all chessman
        resetAllColors(root);
    }
}
*/

 /* ----------------------------------------------------------------------- */
//eof

