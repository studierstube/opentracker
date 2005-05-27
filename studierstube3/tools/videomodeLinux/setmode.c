/* ===========================================================================
    Copyright (c) 2000
    Vienna University of Technology
    
    This software is provided AS IS, without any express or implied
    warranty.  In no event will the authors or the Vienna University of
    Technology be held liable for any damages or loss of profit arising
    from the use or non-fitness for a particular purpose of this software.
   ===========================================================================
    NAME:       modeon
    TYPE:       c code
    PROJECT:    XF86 videmode switch for iGlasses
    CONTENT:    Switch the 640x480 @ 60Hz mode on
   ===========================================================================
    AUTHORS:    jp      Jan Prikryl
   ===========================================================================
    HISTORY:

    31-Oct-00 11:42:19  jp      last modification
    01-Jan-00  1:21:46  jp      created
=========================================================================== */

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>

#define SETMODE_ID_STRING	"XF86 setmode 0.1"
#define BUFFER_LENGTH		1024

static struct option longOpts[] =
{
  {"help",    no_argument,       NULL, 'h'},
  {"output",  required_argument, NULL, 'o'},
  {"verbose", no_argument,       NULL, 'v'},
  {"version", no_argument,       NULL, 'V'},
  {0, 0, 0, 0}
};

static char * shortOpts = "ho:vV";

void
printVersion(void)
{
  fputs(SETMODE_ID_STRING, stdout);
  fputs("\n\nCopyright (C) 2000 Free Software Foundation, Inc.\n"
	"This program is distributed in the hope that it will be useful,\n"
	"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
	"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
	"GNU General Public License for more details.\n\n", stdout);
}

void
printHelp(void)
{
  fputs("Usage: xf86vidmode [options] file\n\n"
	"Options:\n"
	"  -h, --help            print this help.\n"
	"  -o, --output=<FILE>   save current videomode to FILE.\n"
	"  -v, --verbose         be verbose.\n"
	"  -V, --version         display program version and exit.\n\n" 
	"Report bugs to <prikryl@acm.org>\n", stdout);
}

int
readModeInfo(
	const char * modeFileName,
	XF86VidModeModeInfo * mode
	)
{
  FILE * modeFile;
  char   buffer[BUFFER_LENGTH];
  int    nc;
  double vSync;

  modeFile = fopen(modeFileName, "r");
  if (!modeFile)
  {
    perror("ERROR: readModeInfo()");
    return 1;
  }

  do {
    fgets(buffer, BUFFER_LENGTH, modeFile);
  } while (buffer[0] == '#');

  /* Modeline for 1280x1024 may look for example like
     "135 1280 1312 1416 1664 1024 1027 1030 1064" ... */

  nc = sscanf(buffer,
	      "%lf %hu %hu %hu %hu %hu %hu %hu %hu %u",
	      &vSync,
	      &mode->hdisplay, &mode->hsyncstart,
	      &mode->hsyncend, &mode->htotal,
	      &mode->vdisplay, &mode->vsyncstart,
	      &mode->vsyncend, &mode->vtotal,
	      &mode->flags);

  mode->hskew    = 0;
  mode->dotclock = (unsigned int) 1000.0 * vSync;
  mode->privsize = 0;
  mode->private  = NULL;

  fclose(modeFile);

  if (nc != 10) 
  {
    fprintf(stderr, "ERROR: readModeInfo() read only %d items\n", nc);
    return 1;
  }

  return 0;
}

int
writeModeInfo(
	const char * modeFileName,
	const unsigned int videoclock,
	const XF86VidModeModeLine * mode
	)
{
  FILE * modeFile;
  float  vSync;

  modeFile = fopen(modeFileName, "w");
  if (!modeFile)
  {
    perror("writeModeInfo()");
    return 1;
  }

  fputs("# "SETMODE_ID_STRING"\n# automatically saved mode info\n",
	modeFile);
	
  vSync = ((float) videoclock) / 1000.0;

  fprintf(modeFile,
	  "%f %hu %hu %hu %hu %hu %hu %hu %hu %u\n",
	  vSync,
	  mode->hdisplay, mode->hsyncstart, mode->hsyncend, mode->htotal,
	  mode->vdisplay, mode->vsyncstart, mode->vsyncend, mode->vtotal,
	  mode->flags);

  fclose(modeFile);

  return 0;
}

int
main(
	unsigned int argc,
	char ** argv
	)
{
  Display      * display = XOpenDisplay(NULL);
  Status         status;
  int            screen  = DefaultScreen(display);
  int            ret;
  int            x, y;
  unsigned int   videoclock;
  char         * outFile = NULL;
  char           flgVerbose = 0;

  XF86VidModeModeLine  line;
  XF86VidModeModeInfo  modeInfo;

  /* Parse command line parameters */

  while (1)
  {
    int c = getopt_long(argc, argv, shortOpts, longOpts, NULL);

    if (c == -1) break;

    switch (c)
    {
      case 'h':
	printHelp();
	exit(0);
	break;
      case 'o':
	outFile = optarg;
	break;
      case 'v':
	flgVerbose = 1;
	break;
      case 'V':
	printVersion();
	exit(0);
	break;
      default:
	fprintf(stderr,
		"WARNING: getopt() returned unknown character 0%o\n",
		c);
    }
  }

  if (optind >= argc)
  {
    fprintf(stderr,
	    "ERROR: missing input file\n");
    exit(1);
  }

  /* Get the original mode */

  XF86VidModeGetModeLine(display, screen, &videoclock, &line);

  if (flgVerbose)
  {
    printf(SETMODE_ID_STRING"\n\ncurrent clock: %u\n", videoclock);
    printf("current mode : %d, %d, %d, %d,\n",
	   line.hdisplay, line.hsyncstart, line.hsyncend, line.htotal);
    printf("               %d,\n",
	   line.hskew);
    printf("               %d, %d, %d, %d\n",
	   line.vdisplay, line.vsyncstart, line.vsyncend, line.vtotal);
    printf("               %d\n",
	   line.flags);
    printf("               %d\n",
	   line.privsize);
  }

  /* Write it to disk */

  if (outFile)
  {
    if (writeModeInfo(outFile, videoclock, &line))
      exit(1);

    if (flgVerbose)
      printf("saved current mode to `%s'\n", outFile);
  }

  /* Read in the new mode */

  if (flgVerbose)
    printf("reading new videomode from `%s'\n", argv[optind]);

  if (readModeInfo(argv[optind], &modeInfo))
    exit(1);

  if (flgVerbose)
  {
    printf("new clock ...: %u\n", modeInfo.dotclock);
    printf("new mode ....: %d, %d, %d, %d,\n",
	   modeInfo.hdisplay, modeInfo.hsyncstart, 
	   modeInfo.hsyncend, modeInfo.htotal);
    printf("               %d,\n",
	   modeInfo.hskew);
    printf("               %d, %d, %d, %d\n",
	   modeInfo.vdisplay, modeInfo.vsyncstart,
	   modeInfo.vsyncend, modeInfo.vtotal);
    printf("               %d\n",
	   modeInfo.flags);

  }

  /* Validate the new mode */

  status = XF86VidModeValidateModeLine(display, screen, &modeInfo);

  if ( status > 1000 )
  {
    status--;
    fputs("ERROR: New mode refused (", stderr);
    switch (status) {
      case XF86VidModeBadClock:
	fputs("bad clock", stderr);
	break;
      case XF86VidModeBadHTimings:
	fputs("bad horizontal timings", stderr);
	break;
      case XF86VidModeBadVTimings:
	fputs("bad vertical timings", stderr);
	break;
      case XF86VidModeModeUnsuitable:
	fputs("mode not suitable", stderr);
	break;
      case XF86VidModeExtensionDisabled:
	fputs("extension disabled", stderr);
	break;
      case XF86VidModeClientNotLocal:
	fputs("client not local", stderr);
	break;
      case XF86VidModeZoomLocked:
	fputs("zoom locked", stderr);
	break;
      default:
	fputs("unknown error", stderr);
    }
    fputs(").\n", stderr);
    exit(1);
  }

  if (flgVerbose)
    printf("mode line validated with status %d\n", status);

  /* Switch to the given mode */

  if (flgVerbose)
    printf("switching to the new videomode\n");

  XF86VidModeLockModeSwitch(display, screen, 0);
  XSync(display, False);

  if (flgVerbose)
    printf("mode locked\n");

  XF86VidModeSetViewPort(display, screen, 1, 1);
  XSync(display, False);

  if (flgVerbose)
    printf("viewport set\n");

  XF86VidModeSwitchToMode(display, screen, &modeInfo);
  XSync(display, False);

  if (flgVerbose)
  {
    printf("running at specified mode\n", x, y);
  }

  /* This shall theoretically not be necessary. In reality the server needs
     usually several cycles until the viewport really moves to [1,1] */

  while (1)
  {
    XF86VidModeGetViewPort(display, screen, &x, &y);

    if (flgVerbose)
    {
      printf("viewport reported at [%d,%d]\n", x, y);
    }

    if (x <= 1 && y <= 1) break;
    if (x > 1) x >>= 1;
    if (y > 1) y >>= 1;

    if (flgVerbose)
    {
      printf("requesting viewport at [%d,%d]\n", x, y);
    }

    XF86VidModeSetViewPort(display, screen, x, y);
    XSync(display, False);
    usleep(100);
  }

  if (flgVerbose)
  {
    XF86VidModeGetViewPort(display, screen, &x, &y);
    printf("final viewport at [%d,%d]\n", x, y);
  }

  return 0;
}
