/*
  nondsp.c - UNIX & MSDOS NON-DISPLAY, AND CHESSTOOL interface for Chess

  Revision: 1990-04-18

  Copyright (C) 1986, 1987, 1988, 1989, 1990 Free Software Foundation, Inc.
  Copyright (c) 1988, 1989, 1990  John Stanback

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

/// ep
///class ChessKit;
///#include "../ChessKit.h"

#include <ctype.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#ifdef GNU_MSDOS
#include <dos.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#else
#include <sys/param.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/ioctl.h>
void TerminateSearch (int), Die (int);
#endif /* GNU_MSDOS */

#include "gnuchess.h"

#define pxx " PNBRQK"
#define qxx " pnbrqk"
#define rxx "12345678"
#define cxx "abcdefgh"
#define printz printf
#define scanz scanf

static char mvstr[4][6];
static char* ColorStr[2] = {"White", "Black"};
static long evrate;
static int mycnt1, mycnt2;
static int ahead;

void
Initialize (void)
{
  mycnt1 = mycnt2 = 0;
#ifdef CHESSTOOL
  setlinebuf (stdout);
  /* setvbuf(stdout,NULL,_IOLBF,BUFSIZ); */
  printf ("Chess\n");
  if (Level == 0 && !TCflag)
    Level = 15;
#endif /* CHESSTOOL */
}

void
ExitChess (void)
{
  ListGame ();
  exit (0);
}

#ifndef GNU_MSDOS                           /* never called!!! */
void
Die (int sig)
{
  char s[80];
  ShowMessage ("Abort? ");
  scanz ("%s", s);
  if (strcmp (s, "yes") == 0)
    ExitChess ();
}
#endif /* GNU_MSDOS */

void
TerminateSearch (int sig)
{
#ifdef GNU_MSDOS
  sig++;                                /* shut up the compiler */
#endif /* GNU_MSDOS */
  flag.timeout = true;
  flag.bothsides = false;
}

void
algbr (short int f, short int t, short int flag)


/*
   Generate move strings in different formats.
*/

{
  int m3p;

  if (f != t)
    {
      /* algebraic notation */
      mvstr[0][0] = cxx[column (f)];
      mvstr[0][1] = rxx[row (f)];
      mvstr[0][2] = cxx[column (t)];
      mvstr[0][3] = rxx[row (t)];
      mvstr[0][4] = mvstr[3][0] = '\0';
      if ((mvstr[1][0] = pxx[board[f]]) == 'P')
        {
          if (mvstr[0][0] == mvstr[0][2])       /* pawn did not eat */
            {
              mvstr[2][0] = mvstr[1][0] = mvstr[0][2];  /* to column */
              mvstr[2][1] = mvstr[1][1] = mvstr[0][3];  /* to row */
              m3p = 2;
            }
          else
            /* pawn ate */
            {
              mvstr[2][0] = mvstr[1][0] = mvstr[0][0];  /* from column */
              mvstr[2][1] = mvstr[1][1] = mvstr[0][2];  /* to column */
              mvstr[2][2] = mvstr[0][3];
              m3p = 3;          /* to row */
            }
          mvstr[2][m3p] = mvstr[1][2] = '\0';
          if (flag & promote)
            {
              mvstr[0][4] = mvstr[1][2] = mvstr[2][m3p] = qxx[flag & pmask];
              mvstr[1][3] = mvstr[2][m3p + 1] = mvstr[0][5] = '\0';
            }
        }
      else
        /* not a pawn */
        {
          mvstr[2][0] = mvstr[1][0];
          mvstr[2][1] = mvstr[0][1];
          mvstr[2][2] = mvstr[1][1] = mvstr[0][2];      /* to column */
          mvstr[2][3] = mvstr[1][2] = mvstr[0][3];      /* to row */
          mvstr[2][4] = mvstr[1][3] = '\0';
          strcpy (mvstr[3], mvstr[2]);
          mvstr[3][1] = mvstr[0][0];
          if (flag & cstlmask)
            {
              if (t > f)
                {
                  strcpy (mvstr[1], "o-o");
                  strcpy (mvstr[2], "O-O");
                }
              else
                {
                  strcpy (mvstr[1], "o-o-o");
                  strcpy (mvstr[2], "O-O-O");
                }
            }
        }
    }
  else
    mvstr[0][0] = mvstr[1][0] = mvstr[2][0] = mvstr[3][0] = '\0';
}


int
VerifyMove (char *s, short int iop, unsigned short int *mv)

/*
   Compare the string 's' to the list of legal moves available for the
   opponent. If a match is found, make the move on the board.
*/

{
  static short pnt, tempb, tempc, tempsf, tempst, cnt;
  static struct leaf xnode;
  struct leaf *node;

  *mv = 0;
  if (iop == 2)
    {
      UnmakeMove (opponent, &xnode, &tempb, &tempc, &tempsf, &tempst);
      return (false);
    }
  cnt = 0;
  MoveList (opponent, 2);
  pnt = TrPnt[2];
  while (pnt < TrPnt[3])
    {
      node = &Tree[pnt++];
      algbr (node->f, node->t, (short) node->flags);
      if (strcmp (s, mvstr[0]) == 0 || strcmp (s, mvstr[1]) == 0 ||
          strcmp (s, mvstr[2]) == 0 || strcmp (s, mvstr[3]) == 0)
        {
          cnt++;
          xnode = *node;
        }
    }
  if (cnt == 1)
    {
      MakeMove (opponent, &xnode, &tempb, &tempc, &tempsf, &tempst, &INCscore);
      if (SqAtakd (PieceList[opponent][0], computer))
        {
          UnmakeMove (opponent, &xnode, &tempb, &tempc, &tempsf, &tempst);
          printz ("Illegal move\n");
          return (false);
        }
      else
        {
          if (iop == 1)
            return (true);
          UpdateDisplay (xnode.f, xnode.t, 0, (short) xnode.flags);
          if ((board[xnode.t] == pawn)
              || (xnode.flags & capture)
              || (xnode.flags & cstlmask))
            {
              Game50 = GameCnt;
              ZeroRPT ();
            }
          GameList[GameCnt].depth = GameList[GameCnt].score = 0;
          GameList[GameCnt].nodes = 0;
          ElapsedTime (1);
          GameList[GameCnt].time = (short) et;
          TimeControl.clock[opponent] -= et;
          --TimeControl.moves[opponent];
          *mv = (xnode.f << 8) | xnode.t;
          algbr (xnode.f, xnode.t, false);
          return (true);
        }
    }
#ifdef CHESSTOOL
  printz ("Illegal move\n");
#else
  if (cnt > 1)
    ShowMessage ("Ambiguous Move!");
#endif
  return (false);
}

void
help (void)
{
  ClrScreen ();
  printz ("CHESS command summary\n");
  printz ("----------------------------------------------------------------\n");
  printz ("g1f3      move from g1 to f3      quit      Exit Chess\n");
  printz ("Nf3       move knight to f3       beep      on/off\n");
  printz ("o-o       castle king side        easy      on/off\n");
  printz ("o-o-o     castle queen side       hash      on/off\n");
  printz ("bd        redraw board            reverse   board display\n");
  printz ("list      game to chess.lst       book      on/off\n");
  printz ("undo      undo last ply           remove    take back a move\n");
  printz ("edit      edit board              force     enter game moves\n");
  printz ("switch    sides with computer     both      computer match\n");
  printz ("white     computer plays white    black     computer plays black\n");
  printz ("depth     set search depth        clock     set time control\n");
  printz ("post      principle variation     hint      suggest a move\n");
  printz ("save      game to file            get       game from file\n");
  printz ("random    randomize play          new       start new game\n");
  printz ("----------------------------------------------------------------\n");
  printz ("Computer: %-12s Opponent:            %s\n",
          ColorStr[computer], ColorStr[opponent]);
  printz ("Depth:    %-12d Response time:       %ld sec\n",
          MaxSearchDepth, Level);
  printz ("Random:   %-12s Easy mode:           %s\n",
          (dither) ? "ON" : "OFF", (flag.easy) ? "ON" : "OFF");
  printz ("Beep:     %-12s Transposition table: %s\n",
          (flag.beep) ? "ON" : "OFF", (flag.hash) ? "ON" : "OFF");
  signal (SIGINT, TerminateSearch);
#ifndef GNU_MSDOS
  signal (SIGQUIT, TerminateSearch);
#endif /* GNU_MSDOS */
}

void
EditBoard (void)
/*
   Set up a board position. Pieces are entered by typing the piece followed
   by the location. For example, Nf3 will place a knight on square f3.
*/

{
  short a, r, c, sq, i;
  char s[80];

  ClrScreen ();
  UpdateDisplay (0, 0, 1, 0);
  printz (".   exit to main\n");
  printz ("#   clear board\n");
  printz ("c   change sides\n");
  printz ("enter piece & location: \n");

  a = white;
  do
    {
      scanz ("%s", s);
      if (s[0] == '#')
        for(sq = 0; sq < 64; sq++)
          {
            board[sq] = no_piece;
            color[sq] = neutral;
          }
      if (s[0] == 'c' || s[0] == 'C')
        a = otherside[a];
      c = s[1] - 'a';
      r = s[2] - '1';
      if ((c >= 0) && (c < 8) && (r >= 0) && (r < 8))
        {
          sq = locn (r, c);
          color[sq] = a;
          board[sq] = no_piece;
          for(i = no_piece; i <= king; i++)
            if ((s[0] == pxx[i]) || (s[0] == qxx[i]))
              {
                board[sq] = i;
                break;
              }
        }
  } while (s[0] != '.');
  for (sq = 0; sq < 64; sq++)
    Mvboard[sq] = (board[sq] != Stboard[sq]) ? 10 : 0;
  GameCnt = 0;
  Game50 = 1;
  ZeroRPT ();
  InitializeStats ();
  ClrScreen ();
  UpdateDisplay (0, 0, 1, 0);
}

void
SetupBoard (void)

/*
   Compatibility with Unix chess and the nchesstool.
   Set up a board position. Eight lines of eight characters are used
   to setup the board. a8-h8 is the first line.
   Black pieces are  represented  by  uppercase characters.
*/

{
  short r, c, sq, i;
  char ch;
  char s[80];

  NewGame ();

  gets (s);                     /* skip "setup" command */
  for (r = 7; r >= 0; r--)
    {
      gets (s);
      for (c = 0; c <= 7; c++)
        {
          ch = s[c];
          sq = locn (r, c);
          color[sq] = neutral;
          board[sq] = no_piece;
          for(i = no_piece; i <= king; i++)
            if (ch == pxx[i])
              {
                color[sq] = black;
                board[sq] = i;
                break;
              }
            else
              if (ch == qxx[i])
                {
                  color[sq] = white;
                  board[sq] = i;
                  break;
                }
        }
    }
  for (sq = 0; sq < 64; sq++)
    Mvboard[sq] = (board[sq] != Stboard[sq]) ? 10 : 0;
  InitializeStats ();
  ClrScreen ();
  UpdateDisplay (0, 0, 1, 0);
  printz ("Setup successful\n");
}

void
ShowDepth (char ch)
{
#ifdef GNU_MSDOS
  ch++;                                 /* shut up the compiler */
#endif /* GNU_MSDOS */
}

void
ShowResults (short int score, unsigned short int *bstline, char ch)
{
  register int i;
  if (flag.post)
    {
      fprintf (stderr, "%2d%c %6d %4ld %8ld  ", Sdepth, ch, score, et, NodeCnt);
      for (i = 1; bstline[i] > 0; i++)
        {
          if ((i > 1) && (i % 8 == 1))
            fprintf (stderr, "\n                          ");
          algbr ((short) (bstline[i] >> 8), (short) (bstline[i] & 0xFF), false);
          fprintf (stderr, "%5s ", mvstr[0]);
        }
      fprintf (stderr, "\n");
    }
}

void
SearchStartStuff (short int side)
{
  signal (SIGINT, TerminateSearch);
#ifndef GNU_MSDOS
  signal (SIGQUIT, TerminateSearch);
#endif /* GNU_MSDOS */
  if (flag.post)
    {
      fprintf (stderr, "\nMove# %d    Target= %ld    Clock: %ld\n",
               TCmoves - TimeControl.moves[side] + 1,
               ResponseTime, TimeControl.clock[side]);
    }
}

void
OutputMove (ChessKit *ck)
{
  printz ("%d. ... %s\n", ++mycnt1, mvstr[0]);
  if (root->flags & draw)
    {
      printz ("Draw\n");
      ExitChess ();
    }
  if (root->score == -9999)
    {
      printz ("%s\n", ColorStr[opponent]);
      ExitChess ();
    }
  if (root->score == 9998)
    {
      printz ("%s\n", ColorStr[computer]);
      ExitChess ();
    }
  if (flag.post)
    {
      fprintf (stderr, "Nodes= %ld  Eval= %ld  Rate= %ld  Seconds= %ld  ",
               NodeCnt, EvalNodes, evrate, et + ft);
      fprintf (stderr, "Hash/Coll= %ld/%ld\n",
               HashCnt, HashCol);
    }

  UpdateDisplay (root->f, root->t, 0, root->flags);
  fprintf (stderr, "My move is: %s\n", mvstr[0]);
  if (flag.beep)
    printz ("%c", 7);

  if (root->flags & draw)
    fprintf (stderr, "Drawn game!\n");
  else if (root->score == -9999)
    fprintf (stderr, "opponent mates!\n");
  else if (root->score == 9998)
    fprintf (stderr, "computer mates!\n");
  else if (root->score < -9000)
    fprintf (stderr, "opponent will soon mate!\n");
  else if (root->score > 9000)
    fprintf (stderr, "computer will soon mate!\n");

  /// ep
  // it's humans turn
  printf("gnu:Comm.wait\n");
//now   ck->semaComm->wait();
  ck->whiteTurn = !ck->whiteTurn.getValue(); // switch turn
  ck->enforceTurn(); // make sure only new player can move  
  strcpy(ck->lastMove,mvstr[0]);
  ck->gnuMove();
  printf("gnu:Comm.signal\n");
  ck->semaComm->signal();

}

void
ElapsedTime (short int iop)


/*
   Determine the time that has passed since the search was started. If the
   elapsed time exceeds the target (ResponseTime+ExtraTime) then set timeout
   to true which will terminate the search.
*/

{
  if (ahead)
    {
#ifndef GNU_MSDOS
      long nchar;
      ioctl(0, FIONREAD, &nchar);
      if (nchar)
#else
      if (kbhit())
#endif /* GNU_MSDOS */
        {
          flag.timeout = true;
          flag.bothsides = false;
        }
    }
  et = time ((long *) 0) - time0;
  if (et < 0)
    et = 0;
  ETnodes += 50;
  if (et > et0 || iop == 1)
    {
      if (et > ResponseTime + ExtraTime && Sdepth > 1)
        flag.timeout = true;
      et0 = et;
      if (iop == 1)
        {
          time0 = time ((long *) 0);
          et0 = 0;
        }
      if (et > 0)
        /* evrate used to be Nodes / cputime I dont` know why */
        evrate = NodeCnt / (et + ft);
      else
        evrate = 0;
      ETnodes = NodeCnt + 50;
    }
}

void
SetTimeControl (void)
{
  if (TCflag)
    {
      TimeControl.moves[white] = TimeControl.moves[black] = TCmoves;
      TimeControl.clock[white] = TimeControl.clock[black] = 60L * TCminutes;
    }
  else
    {
      TimeControl.moves[white] = TimeControl.moves[black] = 0;
      TimeControl.clock[white] = TimeControl.clock[black] = 0;
      Level = 60L * TCminutes;
    }
  et = 0;
  ElapsedTime (1);
}

void
ClrScreen (void)
{
#ifndef CHESSTOOL
  printz ("\n");
#endif
}

void
UpdateDisplay (short int f, short int t, short int redraw, short int isspec)
{
#ifndef CHESSTOOL
  short r, c, l;
  if (redraw)
    {
      printz ("\n");
      for (r = 7; r >= 0; r--)
        {
          for (c = 0; c <= 7; c++)
            {
              if (flag.reverse)
                l = locn (7 - r, 7 - c);
              else
                l = locn (r, c);
              if (color[l] == neutral)
                printz (" -");
              else if (color[l] == white)
                printz (" %c", qxx[board[l]]);
              else
                printz (" %c", pxx[board[l]]);
            }
          printz ("\n");
        }
      printz ("\n");
    }
#endif /* CHESSTOOL */
#ifdef GNU_MSDOS
  f++; t++; isspec++;                   /* shut up the compiler */
#endif /* GNU_MSDOS */
}

void
GetGame (void)
{
  FILE *fd;
  char fname[256];
  int c;
  short sq;
  unsigned short m;

  printz ("Enter file name: ");
  scanz ("%s", fname);
  if (fname[0] == '\0')
    strcpy (fname, "chess.000");
  if ((fd = fopen (fname, "r")) != NULL)
    {
      fscanf (fd, "%hd%hd%hd", &computer, &opponent, &Game50);
      fscanf (fd, "%hd%hd", &castld[white], &castld[black]);
      fscanf (fd, "%hd%hd", &TCflag, &OperatorTime);
      fscanf (fd, "%ld%ld%hd%hd",
              &TimeControl.clock[white], &TimeControl.clock[black],
              &TimeControl.moves[white], &TimeControl.moves[black]);
      for (sq = 0; sq < 64; sq++)
        {
          fscanf (fd, "%hu%hd", &m, &Mvboard[sq]);
          board[sq] = (m >> 8);
          color[sq] = (m & 0xFF);
          if (color[sq] == 0)
            color[sq] = neutral;
          else
            --color[sq];
        }
      GameCnt = 0;
      c = '?';
      while (c != EOF)
        {
          ++GameCnt;
          c = fscanf (fd, "%hu%hd%hd%ld%hd%hd%hd", &GameList[GameCnt].gmove,
                      &GameList[GameCnt].score, &GameList[GameCnt].depth,
                      &GameList[GameCnt].nodes, &GameList[GameCnt].time,
                      &GameList[GameCnt].piece, &GameList[GameCnt].color);
          if (GameList[GameCnt].color == 0)
            GameList[GameCnt].color = neutral;
          else
            --GameList[GameCnt].color;
        }
      GameCnt--;
      if (TimeControl.clock[white] > 0)
        TCflag = true;
      computer--;
      opponent--;
    }
  fclose (fd);
  InitializeStats ();
  UpdateDisplay (0, 0, 1, 0);
  Sdepth = 0;
}

void
SaveGame (void)
{
  FILE *fd;
  char fname[256];
  short sq, i, c;

  printz ("Enter file name: ");
  scanz ("%s", fname);

  if (fname[0] == '\0')
    strcpy (fname, "chess.000");
  if ((fd = fopen (fname, "w")) != NULL)
    {
      fprintf (fd, "%d %d %d\n", computer + 1, opponent + 1, Game50);
      fprintf (fd, "%d %d\n", castld[white], castld[black]);
      fprintf (fd, "%d %d\n", TCflag, OperatorTime);
      fprintf (fd, "%ld %ld %d %d\n",
               TimeControl.clock[white], TimeControl.clock[black],
               TimeControl.moves[white], TimeControl.moves[black]);
      for (sq = 0; sq < 64; sq++)
        {
          if (color[sq] == neutral)
            c = 0;
          else
            c = color[sq] + 1;
          fprintf (fd, "%d %d\n", 256 * board[sq] + c, Mvboard[sq]);
        }
      for (i = 1; i <= GameCnt; i++)
        {
          if (GameList[i].color == neutral)
            c = 0;
          else
            c = GameList[i].color + 1;
          fprintf (fd, "%d %d %d %ld %d %d %d\n",
                   GameList[i].gmove, GameList[i].score, GameList[i].depth,
                   GameList[i].nodes, GameList[i].time,
                   GameList[i].piece, c);
        }
      fclose (fd);
      printz ("Game saved on file: %s", fname);
    }
  else
    printz ("Could not open file: %s", fname);
}

void
ListGame (void)
{
  FILE *fd;
  short i, f, t;
  fd = fopen ("chess.lst", "w");
  fprintf (fd, "\n");
  fprintf (fd, "       score  depth   nodes  time         ");
  fprintf (fd, "       score  depth   nodes  time\n");
  for (i = 1; i <= GameCnt; i++)
    {
      f = GameList[i].gmove >> 8;
      t = (GameList[i].gmove & 0xFF);
      algbr (f, t, false);
      if ((i % 2) == 0)
        fprintf (fd, "         ");
      else
        fprintf (fd, "\n");
      fprintf (fd, "%5s  %5d     %2d %7ld %5d", mvstr[0],
               GameList[i].score, GameList[i].depth,
               GameList[i].nodes, GameList[i].time);
    }
  fprintf (fd, "\n\n");
  fclose (fd);
}

void
Undo (void)
/*
   Undo the most recent half-move.
*/

{
  short f, t;
  f = GameList[GameCnt].gmove >> 8;
  t = GameList[GameCnt].gmove & 0xFF;
  if (board[t] == king && distance (t, f) > 1)
    (void) castle (GameList[GameCnt].color, f, t, 2);
  else
    {
      /* Check for promotion: */
      if ((row (f) == 6 && row (t) == 7) || (row (f) == 1 && row (t) == 0))
        {
          int g, from = f;
          for (g = GameCnt - 1; g >= 0; g--)
            if (GameList[g].gmove & 0xFF == from)
              from = GameList[g].gmove >> 8;
          if ((row(from) == 1 && row(f) == 6)
             || (row(from) == 6 && row(f) == 1))
            board[t] = pawn;
        }

      board[f] = board[t];
      color[f] = color[t];
      board[t] = GameList[GameCnt].piece;
      color[t] = GameList[GameCnt].color;
      if (color[t] != neutral)
        Mvboard[t]--;
      Mvboard[f]--;
    }
  if (TCflag)
    ++TimeControl.moves[color[f]];
  GameCnt--;
  computer = otherside[computer];
  opponent = otherside[opponent];
  flag.mate = false;
  Sdepth = 0;
  UpdateDisplay (0, 0, 1, 0);
  InitializeStats ();
}

void
ShowMessage (char *s)
{
  fprintf (stderr, "%s\n", s);
}

void
ShowSidetomove (void)
{
}

void
PromptForMove (void)
{
#ifndef CHESSTOOL
  printz ("\nYour move is? \n");
#endif /* CHESSTOOL */
}


void
ShowCurrentMove (short int pnt, short int f, short int t)
{
#ifdef GNU_MSDOS
  f++; t++; pnt++;                      /* shut up the compiler */
#endif /* GNU_MSDOS */
}

void
ChangeAlphaWindow (void)
{
  printz ("window: ");
  scanz ("%hd", &Awindow);
}

void
ChangeBetaWindow (void)
{
  printz ("window: ");
  scanz ("%hd", &Bwindow);
}

void
GiveHint (void)
{
  algbr ((short) (hint >> 8), (short) (hint & 0xFF), false);
  fprintf (stderr, "Hint: %s\n", mvstr[0]);
}

void
SelectLevel (void)
{
  printz ("Enter #moves #minutes: ");
  scanz ("%hd %hd", &TCmoves, &TCminutes);
  printz ("Operator time= ");
  scanz ("%hd", &OperatorTime);
  TCflag = (TCmoves > 1);
  SetTimeControl ();
}

void
ChangeSearchDepth (void)
{
  printz ("depth= ");
  scanz ("%hd", &MaxSearchDepth);
}

void
SetContempt (void)
{
  printz ("contempt= ");
  scanz ("%hd", &contempt);
}

void
ChangeXwindow (void)
{
  printz ("xwndw= ");
  scanz ("%hd", &xwndw);
}

void
TestSpeed(void (*f) (short int side, short int ply))
{
  short i;
  long cnt, rate, t1, t2;

  t1 = time (0);
  for (i = 0; i < 10000; i++)
    {
      f (opponent, 2);
    }
  t2 = time (0);
  cnt = 10000L * (TrPnt[3] - TrPnt[2]);
  rate = cnt / (t2 - t1);
  printz ("Nodes= %ld Nodes/sec= %ld\n", cnt, rate);
}


void
InputCommand (ChessKit *ck)
/*
   Process the users command. If easy mode is OFF (the computer is thinking
   on opponents time) and the program is out of book, then make the 'hint'
   move on the board and call SelectMove() to find a response. The user
   terminates the search by entering ^C (quit siqnal) before entering a
   command. If the opponent does not make the hint move, then set Sdepth to
   zero.
*/

{
  int i;
  short ok, tmp;
  unsigned short mv;
  char s[80];

  ok = flag.quit = false;
  player = opponent;
  ft = 0;
  if (hint > 0 && !flag.easy && Book == NULL)
    {
      fflush (stdout);
      time0 = time ((long *) 0);
      algbr ((short) hint >> 8, (short) hint & 0xFF, false);
      strcpy (s, mvstr[0]);
      tmp = epsquare;
      if (flag.post)
        GiveHint ();
      if (VerifyMove (s, 1, &mv))
        {
          ahead = 1;
          SelectMove (computer, 2,ck);
          VerifyMove (mvstr[0], 2, &mv);
          if (Sdepth > 0)
            Sdepth--;
        }
      ft = time ((long *) 0) - time0;
      epsquare = tmp;
    }
  ahead = 0;
  while (!(ok || flag.quit))
    {
      UpdateDisplay (0, 0, 1, 0);
      PromptForMove ();
      /// ep
      /// i = scanz ("%s", s);
      printf("gnu:Move.wait (human shall perform move)\n");
      ck->semaMove->wait(); // gnu's turn
      printf("gnu:Comm.wait\n");
//now       ck->semaComm->wait(); // for copying lastMove
      strcpy(s,ck->lastMove);
      printf(s);
      printf("gnu:Comm.signal\n");
// now      ck->semaComm->signal();
      i=1; /// corresponding to succesful i=scanz()
      printz("gnu: human move was: %s, hhmmmm ...\n",s);

      if (i == EOF || s[0] == 0)
        ExitChess ();
      player = opponent;
      if (strcmp (s, "bd") == 0)
        {
          ClrScreen ();
          UpdateDisplay (0, 0, 1, 0);
        }
      else if (strcmp (s, "alg") == 0) /* noop */ ;
      else if ((strcmp (s, "quit") == 0) || (strcmp(s, "exit") == 0))
        flag.quit = true;
      else if (strcmp (s, "post") == 0)
        flag.post = !flag.post;
      else if ((strcmp (s, "set") == 0) || (strcmp(s, "edit") == 0))
        EditBoard ();
      else if (strcmp (s, "setup") == 0)
        SetupBoard ();
      else if (strcmp (s, "go") == 0)
        ok = true;
      else if (strcmp (s, "help") == 0)
        help ();
      else if (strcmp (s, "force") == 0)
        flag.force = !flag.force;
      else if (strcmp (s, "book") == 0)
        Book = NULL;
      else if (strcmp (s, "new") == 0)
        NewGame ();
      else if (strcmp (s, "list") == 0)
        ListGame ();
      else if (strcmp (s, "level") == 0 || strcmp (s, "clock") == 0)
        SelectLevel ();
      else if (strcmp (s, "hash") == 0)
        flag.hash = !flag.hash;
      else if (strcmp (s, "beep") == 0)
        flag.beep = !flag.beep;
      else if (strcmp (s, "Awindow") == 0)
        ChangeAlphaWindow ();
      else if (strcmp (s, "Bwindow") == 0)
        ChangeBetaWindow ();
      else if (strcmp (s, "rcptr") == 0)
        flag.rcptr = !flag.rcptr;
      else if (strcmp (s, "hint") == 0)
        GiveHint ();
      else if (strcmp (s, "both") == 0)
        {
          flag.bothsides = !flag.bothsides;
          Sdepth = 0;
          ElapsedTime (1);
          SelectMove (opponent, 1,ck);
          ok = true;
        }
      else if (strcmp (s, "reverse") == 0)
        {
          flag.reverse = !flag.reverse;
          ClrScreen ();
          UpdateDisplay (0, 0, 1, 0);
        }
      else if (strcmp (s, "switch") == 0)
        {
          computer = otherside[computer];
          opponent = otherside[opponent];
          flag.force = false;
          Sdepth = 0;
          ok = true;
        }
      else if (strcmp (s, "white") == 0 || strcmp (s, "first") == 0)
        {
          computer = white;
          opponent = black;
          ok = true;
          flag.force = false;
          Sdepth = 0;
        }
      else if (strcmp (s, "black") == 0)
        {
          computer = black;
          opponent = white;
          ok = true;
          flag.force = false;
          Sdepth = 0;
        }
      else if (strcmp (s, "undo") == 0 && GameCnt > 0)
        Undo ();
      else if (strcmp (s, "remove") == 0 && GameCnt > 1)
        {
          Undo ();
          Undo ();
        }
      else if (strcmp (s, "get") == 0)
        GetGame ();
      else if (strcmp (s, "save") == 0)
        SaveGame ();
      else if (strcmp (s, "depth") == 0)
        ChangeSearchDepth ();
      else if (strcmp (s, "random") == 0)
        dither = 6;
      else if (strcmp (s, "easy") == 0)
        flag.easy = !flag.easy;
      else if (strcmp (s, "contempt") == 0)
        SetContempt ();
      else if (strcmp (s, "xwndw") == 0)
        ChangeXwindow ();
      else if (strcmp (s, "test") == 0)
        {
          ShowMessage("Testing MoveList Speed");
          TestSpeed (MoveList);
          ShowMessage("Testing CaptureList Speed");
          TestSpeed (CaptureList);
        }
      else
        {
          ok = VerifyMove (s, 0, &mv);
          if (ok && mv != hint)
            {
              Sdepth = 0;
              ft = 0;
            }
          /// ep
          if (ok)
          {
             //printf("\ngnu: corr move !");
//now             printf("gnu:Comm.wait\n");
//now             ck->semaComm->wait(); 
             strcpy(ck->lastMove,"correct");
             printf("gnu:Comm.signal\n");
             ck->semaComm->signal();               

             printf("gnu:Move.wait (humans move was correct, waiting for signal to do my move)\n");
             ck->semaMove->wait();
          }
          else
          {
             printf("gnu: wrong move !\n");
             printf("gnu:Comm.wait\n");
             ck->semaComm->wait(); 
             strcpy(ck->lastMove,"wrong"); 
             printf("gnu:Comm.signal\n");
             ck->semaComm->signal();               
          }

        }
    }

  ElapsedTime (1);
  if (flag.force)
    {
      computer = opponent;
      opponent = otherside[computer];
    }
#ifdef CHESSTOOL
  printf ("%d. %s\n", ++mycnt2, s);
#endif /* CHESSTOOL */
  signal (SIGINT, TerminateSearch);
#ifndef GNU_MSDOS
  signal (SIGQUIT, TerminateSearch);
#endif /* GNU_MSDOS */
}
