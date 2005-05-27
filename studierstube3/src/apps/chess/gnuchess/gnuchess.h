/*
  gnuchess.h - Header file for GNU CHESS

  Revision: 1990-04-18

  Copyright (C) 1986, 1987, 1988, 1989, 1990 Free Software Foundation, Inc.

  This file is part of CHESS.

  CHESS is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY.  No author or distributor accepts responsibility to anyone for
  the consequences of using it or for whether it serves any particular
  purpose or works at all, unless he says so in writing.  Refer to the CHESS
  General Public License for full details.

  Everyone is granted permission to copy, modify and redistribute CHESS, but
  only under the conditions described in the CHESS General Public License.
  A copy of this license is supposed to have been given to you along with
  CHESS so you can know your rights and responsibilities.  It should be in a
  file named COPYING.  Among other things, the copyright notice and this
  notice must be preserved on all copies.
*/

// ep
#ifndef _GNUCHESS_H_
#define _GNUCHESS_H_

// ep
#include "../ChessKit.h"

#include <stdio.h>
/* <stdio.h */

// ep rauskommentiert
//#if !defined(__STDC__) || !defined(GNU_MSDOS)
//#define const
//#endif


// ep rauskommentiert
//#ifndef __GNUC__
//#define inline
//#endif

/*
  ttblsz must be a power of 2.
  Setting ttblsz 0 removes the transposition tables.
*/
#ifdef GNU_MSDOS
#define ttblsz (1 << 9)
#else
#define ttblsz (1 << 16)
#define huge
#endif /* MSODS */

#define maxdepth 30
#define white 0
#define black 1
#define neutral 2
#define no_piece 0
#define pawn 1
#define knight 2
#define bishop 3
#define rook 4
#define queen 5
#define king 6
#define bpawn 7
#define pmask 0x0007
#define promote 0x0008
#define cstlmask 0x0010
#define epmask 0x0020
#define exact 0x0040
#define pwnthrt 0x0080
#define check 0x0100
#define capture 0x0200
#define draw 0x0400
#define maxdepth 30
#define false 0
#define true 1
/* #define absv(x) ((x) < 0 ? -(x) : (x)) */

struct leaf
{
  short f, t, score, reply;
  unsigned short flags;
};
struct GameRec
{
  unsigned short gmove;
  short score, depth, time, piece, color;
  long nodes;
};
struct TimeControlRec
{
  short moves[2];
  long clock[2];
};
struct BookEntry
{
  struct BookEntry *next;
  unsigned short *mv;
};

struct flags
{
  short mate;           /* the game is over */
  short post;           /* show principle variation */
  short quit;           /* quit/exit gnuchess */
  short reverse;        /* reverse board display */
  short bothsides;      /* computer plays both sides */
  short hash;           /* enable/disable transposition table */
  short force;          /* enter moves */
  short easy;           /* disable thinking on opponents time */
  short beep;           /* enable/disable beep */
  short timeout;        /* time to make a move */
  short rcptr;          /* enable/disable recapture heuristics */
};

extern struct leaf Tree[2000], *root;
extern short TrPnt[maxdepth];
extern short board[64], color[64];
extern short PieceList[2][16], PawnCnt[2][8];
extern short castld[2], Mvboard[64];
extern short svalue[64];
extern struct flags flag;
extern short opponent, computer, Awindow, Bwindow, INCscore;
extern short dither, player;
extern short xwndw, epsquare, contempt;
extern long ResponseTime, ExtraTime, Level, et, et0, time0, ft;
extern long NodeCnt, ETnodes, EvalNodes, HashCnt, HashCol;
extern struct GameRec GameList[512];
extern short GameCnt, Game50;
extern short Sdepth, MaxSearchDepth;
extern struct BookEntry *Book;
extern struct TimeControlRec TimeControl;
extern short TCflag, TCmoves, TCminutes, OperatorTime;
extern const short otherside[3];
extern const short Stboard[64];
extern const short Stcolor[64];
extern unsigned short hint, PrVar[maxdepth];

#define row(a) ((a) >> 3)
#define column(a) ((a) & 7)
#define locn(a,b) (((a) << 3) | b)
#ifdef __ZTC__
#define distance(a,b) (distdata[((a) << 6) + (b)])
extern short *distdata;
#else
#define distance(a,b) distdata[a][b]
extern short distdata[64][64];
#endif


/* gnuchess.c external functions */
extern void NewGame (void);
/* book.c */
extern int parse (FILE * fd, unsigned short int *mv, short int side);
extern void GetOpenings (void);
extern void OpeningBook (unsigned short int *hint);
/* search.c */
extern void repetition (short int *cnt);
extern void SelectMove (short int side, short int iop,ChessKit *ck);
extern int search (short int side,
                   short int ply,
                   short int depth,
                   short int alpha,
                   short int beta,
                   unsigned short int *bstline,
                   short int *rpt);
/* tran.c */
#if ttblsz
extern int ProbeTTable (short int side,
                        short int depth,
                        short int *alpha,
                        short int *beta,
                        short int *score);
extern void PutInTTable (short int side,
                         short int score,
                         short int depth,
                         short int alpha,
                         short int beta,
                         unsigned short int mv);
extern void ZeroTTable (void);
extern void ZeroRPT (void);
#ifdef HASHFILE
extern int ProbeFTable (short int side,
                        short int depth,
                        short int *alpha,
                        short int *beta,
                        short int *score);
extern void PutInFTable (short int side,
                         short int score,
                         short int depth,
                         short int alpha,
                         short int beta,
                         short unsigned int f,
                         short unsigned int t);
#endif /* HASHFILE */
#endif /* ttblsz */
/* move.c */
extern void Initialize_moves (void);
extern void MoveList (short int side, short int ply);
extern void CaptureList (short int side, short int ply);
extern int castle (short int side, short int kf, short int kt, short int iop);
extern void MakeMove (short int side,
                      struct leaf * node,
                      short int *tempb,
                      short int *tempc,
                      short int *tempsf,
                      short int *tempst,
                      short int *INCscore);
extern void UnmakeMove (short int side,
                        struct leaf * node,
                        short int *tempb,
                        short int *tempc,
                        short int *tempsf,
                        short int *tempst);
extern void InitializeStats (void);
/* eval.c */
extern int SqAtakd (short int sq, short int side);
extern int evaluate (short int side,
                     short int ply,
                     short int alpha,
                     short int beta,
                     short int INCscore,
                     short int *slk,
                     short int *InChk);
extern void ScoreLoneKing (short int side, short int *score);
extern void ScorePosition (short int side, short int *score);
extern void ExaminePosition (void);
extern void UpdateWeights (void);

/* *dsp.c external functions */
extern void Initialize (void);
extern void InputCommand (ChessKit *ck);
extern void ExitChess (void);
extern void ClrScreen (void);
extern void SetTimeControl (void);
extern void SelectLevel (void);
extern void UpdateDisplay (short int f,
                           short int t,
                           short int flag,
                           short int iscastle);
extern void ElapsedTime (short int iop);
extern void ShowSidetomove (void);
extern void SearchStartStuff (short int side);
extern void ShowDepth (char ch);
extern void ShowResults (short int score,
                         unsigned short int *bstline,
                         char ch);
extern void algbr (short int f, short int t, short int flag);
extern void OutputMove (ChessKit *ck);
extern void ShowCurrentMove (short int pnt, short int f, short int t);
extern void ListGame (void);
extern void ShowMessage (char *s);
extern void ClrScreen (void);
extern void gotoXY (short int x, short int y);
extern void ClrEoln (void);
extern void DrawPiece (short int sq);
extern void UpdateClocks (void);

#endif // _GNUCHESS_H_
